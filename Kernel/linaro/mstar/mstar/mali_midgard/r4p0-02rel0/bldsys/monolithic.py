# This confidential and proprietary software may be used only as
# authorised by a licensing agreement from ARM Limited
# (C) COPYRIGHT 2010-2013 ARM Limited
# ALL RIGHTS RESERVED
# The entire notice above must be reproduced on all authorised
# copies and copies may only be made to the extent permitted
# by a licensing agreement from ARM Limited.

import os, sys, re

import SCons.Defaults
import SCons.Node
import SCons.Script
from patch import Patch

# ------------------------------------------------------------
#		Monolithic build support code
# ------------------------------------------------------------
#
# Very Simple Overview
#
# Every library build declaration is 'intercepted' and a record
# is taken of all the object files that go into the library. This occurs
# every time SCons is run irrespective of whether the libraries have already
# been built. The libraries are still built exactly as before. Every program
# build declaration is 'intercepted' and a record is taken of the object files
# and libraries it requires. Once all the sconscripts have been processed, the
# program SCons nodes are altered such that the program links in the object files
# in the libraries rather than the libraries themselves. Linking of shared and
# external libraries is un-altered.
#
#------------

# The absolute path of the project - where all the module folders are to be found.
# This is set appropriately in post_script_processing()
project_directory = ''

# Flag that indicates if we are doing an Android build. Set automatically.
build_android = False

# Manual switch to control whether Android builds use static libraries or not.
android_use_static_libs = True

# Prefix used on generated Mock files.
MOCK_FILE_PREFIX = "Mock"

# X11 library path - only used when building for ARM Linux with winsys = x11
X11_LIBPATH = '#x11/thirdparty/lib'

BIFL_LIBPATH = '#clcc/tests/internal/unit_tests/third_party/libs'

ANDROID_RELEASE_TARGETS = ['compiler', 'clcc']

ANDROID_RELEASE_HEADER = """
ifeq ($(TARGET_BOARD_PLATFORM),exynos5)
_should_build_mali_from_src := true
ifdef HAVE_ARM_MALI6XX_SRC
ifeq ($(HAVE_ARM_MALI6XX_SRC),false)
_should_build_mali_from_src := false
endif
endif

ifeq ($(_should_build_mali_from_src),true)
"""

ANDROID_RELEASE_FOOTER = """
endif # _should_build_mali_from_src == true)

_should_build_mali_from_src :=

endif # $(TARGET_BOARD_PLATFORM) == exynos5
"""

# ------------------------------

def _fatal_error( str ):
	'''
	Print a fatal message and stop the whole build.
	'''
	sys.stderr.write("\nFATAL ERROR in monolithic.py:\n%s\n\n" % str)
	exit(1)

# ------------------------------

def _get_file_base( abs_file ):
	'''
	Returns the filename minus its path and any/all extensions.
	'''

	file_path, file_name = os.path.split( abs_file )

	# Some files have multiple extensions. Remove them all.
	while "." in file_name:
		file_name, file_ext = os.path.splitext( file_name )

	return ( file_name )

# ------------------------------

def _get_env_var( env, var_name ):
	'''
	Returns a duplicate of the list of the specified variable in the specified
	environment or an empty list if its not defined.

	Note that the environment variable is not expanded.
	'''
	
	if var_name in env and len(env[var_name]) > 0:
		return list(env[var_name])	# Return a duplicate, not a reference.
	else:
		return []

# ------------------------------

class _Details( object ):
	
	"""
	Base class for tracking key details of objects, libraries and programs

	dst_node                Reference to the SCons.Node.FS.File of target object/library/program
	env                     Reference to its environment
	is_cpp                  Boolean (only used in Android builds)

	os                      OS the object/library/program is to be built for. Ref to env['os']
	arch                    Arch the object/library/program is to be built for. Ref to env['arch']

	All of the remainder are cloned from the correspond env variable after all the sconscripts have
	been processed. Later they might be slightly processed (eg expanding environment variables and shortening
	paths).
	
	slibs                   duplicate of env['LIBS'] or an empty list if LIBS undefined. 
	
	The last ones are only used in Android builds. 

	defines                 duplicate of env['CPPDEFINES'] dictionary (always defined)
	include_paths           duplicate of env['CPPPATH'] list (always defined)
	os_include_paths        duplicate of env['OS_CPPPATH'] list or an empty list if OS_CPPPATH undefined
	ccflags                 list of contents of env['CCFLAGS'] or an empty list if CCFLAGS undefined
	
	"""

	def __init__( self, env, node ):

		self.dst_node = node
		self.env = env
		self.is_cpp = False
		self.os = env['os']
		self.arch = env['arch']
		self.slibs = []
		self.defines = dict()
		self.include_paths = []
		self.os_include_paths = []
		self.ccflags = []

	def _copy_key_env_details( self ):

		'''
		Called for every object,library & program (except libmali) once all the sconscripts have been processed.
		We don't want to take copies in the constructor as the env might be changed before the
		actual build occurs. We duplicate the originals as some might be changed during
		subsequent processing.
		'''
		env = self.env
	
		self.slibs = _get_env_var( env, 'LIBS' )
		
		# The remainder is only needed in Android builds

		self.defines = dict( env['CPPDEFINES'] )
		self.include_paths = _get_env_var( env, 'CPPPATH' )
		self.os_include_paths = _get_env_var( env, 'OS_CPPPATH' )
	
		# 'CCFLAGS', 'SHCCFLAGS' & 'CXXFLAGS' are a little different as they can either be
		# a list, a single string with space separated flags or an instance of CLVar which
		# also converts to a single string with space separated flags. We want to store them
		# as a list of individual string flags.
		if 'CCFLAGS' in env and len(env['CCFLAGS']) > 0:
			if isinstance(env['CCFLAGS'], list):
				# Assume its a list of strings - duplicate it
				self.ccflags = list(env['CCFLAGS'])
			else:
				self.ccflags = str(env['CCFLAGS']).split()

		if 'SHCCFLAGS' in env and len(env['SHCCFLAGS']) > 0:
			if isinstance(env['SHCCFLAGS'], list):
				# Assume its a list of strings - duplicate it
				shccflags = list(env['SHCCFLAGS'])
			else:
				shccflags = str(env['SHCCFLAGS']).split()

			# And append them to self.ccflags taking care to ignore $CCFLAGS
			for flag in shccflags:
				if not flag in self.ccflags and flag != '$CCFLAGS':
					self.ccflags.append( flag )

		if 'CXXFLAGS' in env and len(env['CXXFLAGS']) > 0:
			if isinstance(env['CXXFLAGS'], list):
				# Assume its a list of strings - duplicate it
				cxxflags = list(env['CXXFLAGS'])
			else:
				cxxflags = str(env['CXXFLAGS']).split()

			# And append them to self.ccflags taking care to ignore $CCFLAGS 
			for flag in cxxflags:
				if not flag in self.ccflags and flag != '$CCFLAGS':
					self.ccflags.append( flag )

# ------------------------------


# Class to track the object files that go into the libraries
class LibDetails( _Details ):

	'''
	This class extends _Details class and adds the following instance variables:

	object_nodes[]            List of nodes of the object files in this library (SCons.Node.FS.File or SCons.Node.FS.Entry)
	is_shared                 Boolean
	is_mock                   Boolean - library contains mock objects
	is_public_static          Boolean - is library going in STATIC_LIB_PATH
	is_empty                  Boolean - true if has no objects (only used on static libs). Empty static libraries are
	                          always excluded from libmali and aren't added to Android.mk on Android builds.

	libname                   Library filename minus the prefix and suffix
	custom_prefix             Custom prefix name for private static libraries (only used in Android builds)

	The remainder are set after processing the sconscripts

	external_libs             List of strings of external libraries required for this library
	internal_static_libs      List of internal static LibDetails required for this library
	internal_shared_libs      List of internal shared LibDetails required for this library
	internal_lib_objects      List of Nodes of objects (not libraries) specified in LIBS (rarely occurs)
	source_files              Set of all the 'C' source files in this library (filename with project dir relative path)

	excluded                  Boolean, whether this library be excluded from libmali and/or the Android make file
	'''

	def __init__(self, env, node, filename, source_obj_nodes, shared ):

		_Details.__init__(self, env, node)

		self.is_shared = shared
		self.is_mock = False
		self.is_empty = False
		self.object_nodes = []
		self.external_libs = []
		self.internal_static_libs = []
		self.internal_shared_libs = []
		self.internal_lib_objects = []
		self.source_files = set()
		self.is_public_static = False
		self.excluded = False
		self.custom_prefix = ''

		# Work out the base library name (the file name minus the lib prefix and suffix)

		if shared:
			prefix, suffix = _get_shared_lib_prefix_and_suffix( env )
		else:
			prefix, suffix = _get_static_lib_prefix_and_suffix( env )

		if len( prefix ) > 0 and filename.startswith(prefix):
			filename = filename[ len( prefix ) : ]
		if len( suffix ) > 0 and filename.endswith(suffix):
			filename = filename[ : - len( suffix ) ]
		self.libname = filename

		#print "Lib :"+ str(node) + " is " + filename
		abs_file = env.File(node).abspath

		# Check the target library matches its env - sanity check
		if not abs_file.startswith( env['BUILD_DIR_PATH'] ):
			print "WARNING - LibDetails - Target library not in BUILD_DIR_PATH"
			print "Library path: " + abs_file
			print "BUILD_DIR_PATH: " + env['BUILD_DIR_PATH']

		# All objects in source_obj_nodes[] must already have been declared, so they will be in
		# declared_object_nodes[]
		for obj in source_obj_nodes:
			obj_details = declared_object_nodes[obj]
			# Quick sanity check
			if obj_details.os != self.os or obj_details.arch != self.arch:
				print "WARNING - LibDetails - object in this library is not same os/arch:"
				print "Library: " + abs_file
				print "Object: " + obj_details.env.File(obj).abspath

			self.is_mock |= obj_details.is_mock

		# Record the object nodes
		self.object_nodes = list(source_obj_nodes)

		if shared :
			# We expect all shared libraries to be in SHARED_LIB_PATH
			if not abs_file.startswith( env['SHARED_LIB_PATH'] ):
				print "WARNING - LibDetails - shared library not in SHARED_LIB_PATH:"
				print env.File(node).abspath
		else:
			if abs_file.startswith( env['STATIC_LIB_PATH'] ):
				# Most libraries in STATIC_LIB_PATH are public (ie used by other module libraries
				# and test programs), but there are some exceptions (eg libraries that are specific to
				# tests of a single module)
				self.is_public_static = True
			else:
				# Static libraries not going in STATIC_LIB_PATH. Must be private
				# to a module (typically these are mocks for another module's library).
				lib_path, lib_name = os.path.split( abs_file )

				# Strip off the build variant from the path
				short_lib_path = _shorten_abs_build_path_name( env, lib_path )

				# Replace all directory separators with underscores leaving us
				# with a unique prefix for the library.
				self.custom_prefix = short_lib_path.replace( os.sep,'_' ) + "__"

			if not self.object_nodes:
				# No objects so its an empty static library. 
				self.is_empty = True

		#print "\n-------\n"

# ------------------------------

# Class to track the object files
class ObjectDetails( _Details ):

	'''
	This class extends _Details class and adds the following instance variables:

	src_node        SCons.Node.FS.File of the source file
	src_file        Real source file (string)        
	basename        Object filename with no path or extension - used to match mock files
	is_shared       Boolean
	is_mock         Boolean
	'''

	def __init__(self, env, src_node, tgt_node, shared ):

		_Details.__init__(self, env, tgt_node)

		self.src_node = src_node
		self.is_shared = shared

		abs_obj = env.File( tgt_node ).abspath
		abs_src = env.File( src_node ).abspath

		# Initially we set src_file to the absolute path. It gets changed to
		# be relative to the project directory once we've processed all the
		# sconscripts.
		self.src_file = abs_src

		# Check the target object matches its env - sanity check
		if not abs_obj.startswith( env['BUILD_DIR_PATH'] ):
			print "WARNING - ObjectDetails - Target object not in BUILD_DIR_PATH"
			print "Object path: " + abs_obj
			print "BUILD_DIR_PATH: " + env['BUILD_DIR_PATH']

		# Most source files are mirrored into the build tree but they don't have to be so
		# we don't check them.

		# Is it a mock file
		src_path, src_name = os.path.split( abs_src )
		self.is_mock = src_name.startswith( MOCK_FILE_PREFIX )

		# Is it a C++ source file
		src_base, src_ext = os.path.splitext( src_name )
		if len( src_ext ) > 2 and src_ext.upper() in ['.CPP', '.C++']:
			self.is_cpp = True

		# Is it the cmock file
		if src_base == 'cmock':
			self.is_mock = True;

		# Record the filename of the object minus path and prefix.
		# Note that the object file doesn't have to have the same name as the source
		self.basename = _get_file_base( abs_obj )


# ------------------------------

def _shorten_abs_build_path_name( env, build_file ):
	'''
	Returns the target file minus the build path.
	'''

	if build_file.startswith( env['BUILD_DIR_PATH'] ):
		return build_file.replace( env['BUILD_DIR_PATH'] + os.sep,'', 1 )
	else:
		return build_file

# ------------------------------

def _shorten_abs_path_name( abs_file ):
	'''
	Returns the target file minus the project path.
	'''
	global project_directory

	if abs_file.startswith( project_directory ):
		return abs_file.replace( project_directory + os.sep,'', 1 )
	else:
		return abs_file

# ------------------------------

# Dictionary of the generated shared and static libraries, each is an instance of LibDetails.
# These record the details of the make up of the library.
# The key is the absolute path of the library.
declared_libs = dict()

# List of unique generated static library file names (no path, strings)
static_lib_names = []

# List of unique generated shared library file names (no path, strings).
shared_lib_names = []

def _record_declared_library( source, target, env, shared=False ):
	'''
	Private function. Tracks which objects go into which libraries but
	makes no changes. Called by the shared and static library build
	emitter methods (below)
	'''
	global declared_libs
	global static_lib_names
	global shared_lib_names

	for tg in target:
		# tg is always a SCons.Node.FS.File

		# Each library can only have one build declaration, so we can use the absolute
		# path of the target library as the key.
		declared_lib_key = env.File( tg ).abspath

		lib_path, lib_name = os.path.split( declared_lib_key )

		# Record the library name
		if shared:
			if lib_name not in shared_lib_names:
				shared_lib_names.append( lib_name )
		else:
			if lib_name not in static_lib_names:
				static_lib_names.append( lib_name )

		# Each library can only be declared once.
		if declared_lib_key not in declared_libs:

			declared_libs[ declared_lib_key ] = LibDetails(
						env,
						tg,		# node
						lib_name,	# filename (no path)
						source,		# list of object nodes
						shared		# shared library flag
						)
		else:
			# This library has already been declared!
			_fatal_error( "_record_declared_library - library has already been declared: %s" % declared_lib_key )

	# No change
	return target, source


def static_library_emitter_method( source, target, env ):
	'''
	Tracks which objects go into which libraries but
	makes no changes.
	'''
	return _record_declared_library( source, target, env, False )


def shared_library_emitter_method( source, target, env ):
	'''
	Tracks which objects go into which libraries but
	makes no changes.
	'''
	return _record_declared_library( source, target, env, True )


# ------------------------------

def list_declared_libs(env):
	'''
	Debug function that lists all the static and shared libraries that
	have been declared.
	'''
	global declared_libs
	global static_lib_names
	global shared_lib_names

	print "\nDeclared Libs: " + str(len(declared_libs))
	for lib_key in declared_libs:
		print _shorten_abs_build_path_name(declared_libs[lib_key].env, lib_key)
	print "\n"

	print "\nStatic Library names: " + str(len(static_lib_names))
	for lib in static_lib_names:
		print lib

	print "\nShared Library names: " + str(len(shared_lib_names))
	for lib in shared_lib_names:
		print lib
	print "\n"


# ------------------------------

# Dictionary of the generated shared and static objects, each is an instance of ObjectDetails.
# These record the details of the object files. The key is the object file SCons node
declared_object_nodes = dict()

def _record_declared_object( target, source, env, shared=False ):
	'''
	Private function. Tracks object files but makes no changes.
	'''
	global declared_object_nodes

	for (idx, tg) in enumerate(target):
		# tg is always a SCons.Node.FS.File - its the object node
		#
		# Some modules declare the same object multiple times. We assume the last
		# declaration is the one to keep.

		# Use the target Scons node as the key
		declared_object_nodes[ tg ] = ObjectDetails(
					env,
					source[idx],	# source file node
					tg,		# object file node
					shared		# shared object flag
					)
	# No change
	return target, source


def shared_object_emitter_method( target, source, env ):
	'''
	Tracks makeup of shared objects.
	'''

	# Replicate built-in emitter method
	for tgt in target:
		tgt.attributes.shared = 1

	# Record the details, but make no changes
	return _record_declared_object( target, source, env, True )


def static_object_emitter_method( target, source, env ):
	'''
	Tracks makeup of static objects.
	'''

	# Replicate built-in emitter method
	for tgt in target:
		tgt.attributes.shared = None

	# Record the details, but make no changes
	return _record_declared_object( target, source, env, False )

# ------------------------------

def list_declared_objects(env):
	'''
	Debug function that lists all the static and shared objects that
	have been declared.
	'''
	global declared_object_nodes

	print "\nDeclared Objects: " + str(len(declared_object_nodes))
	for obj_node in declared_object_nodes:
		obj_details = declared_object_nodes[ obj_node ]
		print str(obj_details.src_node) + " -> " + str(obj_details.dst_node)
	print "\n"


# ------------------------------
#
# Expand the specified dollar environment variable. The function is recursive so the
# var argument can be a list, a SCons node or a string.
#
# Returns the expanded version of the variable as a list of strings (or Nodes).
# Strings which aren't valid environment variable syntax or aren't known to the
# environment are returned un-altered.
#
# In all cases a single list (not nested) is returned. There is no attempt to
# ensure the uniqueness of the strings in the returned list.
#
# In SCons, environment variables are used both with and without a $ prefix.
# This function uses the $ prefix to distinguish environment variables from
# plain strings. Hence the var argument must always have a $ prefix, otherwise
# it will just assume its a plain string and return it un-expanded.
#
# eg:	var			returns
#	------			-------
#	'fred'			['fred']
#	'stdlib_$platform' 	['stdlib_dummy']
#	'$LIBS'			['rt','tpi','base','gles','base','egl']
#
#
# NOTE: - Duplicates functionality of env.subst  .....
# The main problem with subst() is that it expands everything. It converts file nodes to their absolute string path.
# It returns a single long string with the expanded results separated by spaces. So it won't work if any of the vars
# have a space in their definition. So its quite a bit different to _expand_env_variable() and due to the spaces problem
# it might not ever be possible to get it to return anything useful. However bear in mind that it is intended to return
# a string to pass to the command line, and all we ever do is expand in order to pass values on the command line, so
# the space problem shouldn't occur. Note that it also always expects the var to have $ prefix.
# Note that subst does not expand paths with a # prefix (neither does _expand_env_variable ).
#
# Summary: The major difference between env.subst() and _expand_env_variable() is that _expand_env_variable() does not
# expand nodes and the code that uses _expand_env_variable() uses these nodes. It should be possible to wrap env.subst()
# such that the returned data matches _expand_env_variable() however it might not be worth the effort.
#
# ---

def _expand_env_variable( env, var ):

	# Returns a list of strings
	ans = []

	# Look for ${..} and $... environment variables (which are later
	# called brace_var and plain_var
	env_re = re.compile( '(\$\{([_0-9a-zA-Z]+)\}|\$([_0-9a-zA-Z]+))' )

	if isinstance( var, list ) :
		#print "[IN] _expand_env_variable - is list so process each in turn"
		# Its a list, need to process each in turn - do it recursively
		for item in var:
			if isinstance( item, str ) and "$" not in item :
				# Simple plain string - no need to use recursion
				#print "_expand_env_variable - simple case, append: " + item
				ans.append( item )
			else:
				#print "_expand_env_variable - have to process this element recursively"
				ans.extend( _expand_env_variable( env, item ) )
	elif isinstance( var, str ) :
		#print "[IN] _expand_env_variable - is str: " + var
		# Get a list of matching tuples
		matching = env_re.findall( var )

		#print "_expand_env_variable - has $ match(es)"
		for match_tuple in matching:
			# Expand the tuple
			dollar_match, brace_var, plain_var = match_tuple
			# The regex will only match one of the two groups, so
			# one of the two (brace_var and plain_var) will always be an
			# empty string
			evar = brace_var + plain_var

			#print "_expand_env_variable - consider: "+ evar

			if evar in env :
				# Its a known environment variable
				val = env[evar]

				#print "_expand_env_variable - val: ",
				#print val

				if len(dollar_match) < len(var):
					# We only matched part of the string so we
					# must be dealing with an environment variable
					# that expands to a string which will replace
					# the part we matched. This means the environment
					# variable CAN NOT expand to a list. If it does
					# then its being misused.

					if isinstance( val, str ) :
						if "$" not in val :
							# This is the easy case - just replace
							# the matched part of the source string
							var = var.replace( dollar_match, val, 1 )
						else:
							# Nested environment variable. Use recursion to expand
							# it as far as possible.
							# Remember we get back a list.
							#print "Nested environment variable - process recursively"
							val_list = _expand_env_variable( env, val )

							# The list must have just one element and it
							# must be a string. Its been fully expanded so
							# we don't need to try to expand it any further
							if len(val_list) == 1 :
								val = val_list[0]
								if isinstance( val, str ) :
									var = var.replace( dollar_match, val, 1 )
								else:
									print "Misuse of environment variable"
									# Leave the source string un-altered.
							else:
								print "Misuse of environment variable"
								# Leave the source string un-altered.
					else:
						# Misuse of environment variable.
						print "Attempt to use environment variable that is not a string in a string context."
						# Leave the source string un-altered.
				else:
					# The environment variable will replace the
					# whole string. Clear the source string now.
					var = ''

					if isinstance( val, str ) and "$" not in val :
						#print "_expand_env_variable - val is str: "+ val
						ans.append( val )
					else:
						#print "_expand_env_variable - process recursively ..."
						ans.extend( _expand_env_variable( env, val ) )

		# The default case if we didn't find any environment variables in the string
		if var:
			ans.append( var )

	else:
		# Its probably a node of some sort - leave it alone
		#print "[IN] is a node ",
		#print type( var )
		ans = [ var ]

	#print "[OUT] _expand_env_variable - returning ans: ",
	#print ans
	return ans

# ------------------------------

def _get_env_var_str( env, string ):
	'''
	Expand the specified environment variable string. The var
	argument must not have a $ prefix. Intended to be used with
	variables that expand to a single string.
	'''

	if string != "" and string in env:
		# Note that subst() only expands strings that have a
		# "$" prefix - and it always returns a single string.
		return env.subst( "$" + string )
	return ""

# ------------------------------

def _get_program_prefix_and_suffix( env ):
	return _get_env_var_str(env,'PROGPREFIX'), _get_env_var_str(env,'PROGSUFFIX')

def _get_static_lib_prefix_and_suffix( env ):
	return _get_env_var_str(env,'LIBPREFIX'), _get_env_var_str(env,'LIBSUFFIX')

def _get_shared_lib_prefix_and_suffix( env ):
	return _get_env_var_str(env,'SHLIBPREFIX'), _get_env_var_str(env,'SHLIBSUFFIX')

def _full_static_lib_filename( env, lib_name ):
	'''
	Add static library prefix and suffix to supplied library name
	'''
	prefix, suffix = _get_static_lib_prefix_and_suffix( env )
	if lib_name.startswith(prefix):
		# Assume the library already has the standard prefix
		prefix = ''
	return prefix + lib_name + suffix

def _full_shared_lib_filename( env, lib_name ):
	'''
	Add shared library prefix and suffix to supplied library name
	'''
	prefix, suffix = _get_shared_lib_prefix_and_suffix( env )
	if lib_name.startswith(prefix):
		# Assume the library already has the standard prefix
		prefix = ''
	return prefix + lib_name + suffix

def _bare_program_name( env, prog_name ):
	'''
	Strip program prefix and suffix from supplied program filename
	'''
	prefix, suffix = _get_program_prefix_and_suffix( env )

	if len( prefix ) > 0 and prog_name.startswith(prefix):
		prog_name = prog_name[ len( prefix ) : ]
	if len( suffix ) > 0 and prog_name.endswith(suffix):
		prog_name = prog_name[ : - len( suffix ) ]

	return prog_name

# ------------------------------

# Some module API files do not have names that closely match
# the generated mock files. This table records the mappings.
SOURCE_API_TO_MOCK_MAPPING = [
                        ('mali_egl_platform_dummy',          'Mockmali_egl_platform'),
                        ('mali_egl_winsys_fbdev',             'Mockmali_egl_platform'),
                        ('mali_egl_winsys_dummy',             'Mockmali_egl_platform'),
                        ('mali_gles_vertex_state',          'Mockmali_gles_vertex_module_api'),
                        ('mali_cstate_tracker_builder', 'Mockmali_cstate_tracker'),
                        ('mali_cstate_tracker_map',         'Mockmali_cstate_tracker'),
                        ('mali_cobj_common',                    'Mockmali_cobj_instance'),
                        ('mali_gles_surface_pixel_format', 'Mockmali_gles_surface'),
                        ('mali_gles_sync_object',       'Mockmali_gles_sync_interface'),
                        ('mali_gles_texture_egl_image', 'Mockmali_gles_egl_image_interface'),
                        ('mali_egl_image',              'Mockmali_egl_interop'),
                        ('mali_gles_context',           'Mockmali_gles_context_interface'),
                        ('mali_gles_sync_module_api',   'Mockmali_gles_sync_interface'),
                        ('mali_egl_pbuffer_surface',    'Mockmali_egl_interop'),
                        ('mali_egl_sync',               'Mockmali_egl_interop'),
                        ('mali_egl_thread_state',       'Mockmali_egl_interop'),
			('mali_egl_context',            'Mockmali_egl_interop'),
			('mali_egl_display',            'Mockmali_egl_interop'),
			('mali_egl_winsys_common',      'Mockmali_egl_winsys'),
			]

def _remove_mocked_object_files( object_nodes ):
	'''
	Scans through the supplied complete list of object nodes going into a
	program and removes those which are being mocked. (Typically object_nodes[]
	will include a Mock object file that shares the same basename as the file
	its mocking.)
	'''

	# Note that all objects in object_nodes[] must already be unique.

	# The resulting filtered list
	filtered_object_nodes = []

	# Get the names of the objects (minus suffix)
	existing_names = [ declared_object_nodes[x].basename  for x in object_nodes ]

	# Get names of all the special API files that have names that don't match their mocks
	special_api_names = [ x[ 0 ] for x in SOURCE_API_TO_MOCK_MAPPING ]

	# To keep things simple, we process object_nodes[] in two passes.

	# Copy all the mock files to filtered_object_nodes[] - they should
	# all be unique so we won't check they are.
	for obj in object_nodes :
		if declared_object_nodes[ obj ].is_mock:
			filtered_object_nodes.append( obj )

	# Now process the non-mock files, only copy the ones that are not
	# being mocked
	for obj in object_nodes :
		obj_details = declared_object_nodes[ obj ]
		if not obj_details.is_mock:
			# Check if there already is a mock file with the same base name.
			obj_file = obj_details.basename
			mock = MOCK_FILE_PREFIX + obj_file
                        if mock not in existing_names:
                                # No there is no mock version of the file.
                                # Some API 'c' files don't have the same name as the
                                # files that mock them.
                                if obj_file in special_api_names:
                                        # The mock file has a different name
                                        idx = special_api_names.index(obj_file)
                                        mock = SOURCE_API_TO_MOCK_MAPPING[idx][1]
                                        if mock not in existing_names:
                                                # not mocked - add it
                                                filtered_object_nodes.append( obj )
                                else:
                                        # Not a special case api file - we can add it to the list
                                        filtered_object_nodes.append( obj )

	return filtered_object_nodes

# ------------------------------

def _add_objects( old_object_nodes, new_object_nodes ):
	'''
	Add new_object_nodes[] to old_object_nodes[] taking care to ensure there are
	no entries with same source file node. (Each object file is derived from a source
	file and we mustn't link in multiple objects compiled from the same source file.)
	
	Some modules are building non-static versions of source files and linking both
	those and the normal version of the file into the same executable. This works
	when one of the objects is in an archive, but the monolithic mechanism
	uses the objects that go in the archives not the archives themselves. Hence
	the monolithic system has to take extra care to avoid linking in objects that
	are compiled from the same source file. The first found is assumed to be the correct one
	(as the order should be based on the order of paths in LIBPATH).
	'''
	existing_source_nodes = [ declared_object_nodes[x].src_node for x in old_object_nodes ]

	for obj in new_object_nodes :

		src_node = declared_object_nodes[obj].src_node

		if src_node not in existing_source_nodes:
			# This one is new, add it to the old list. Note that we know it doesn't
			# already exist in old_object_nodes[] because if it did then its src node
			# would already be in existing_source_nodes[]
			old_object_nodes.append( obj )
			# and add the source node to the local copy of existing source nodes
			existing_source_nodes.append( src_node )

# ------------------------------

# Class to track the libraries and objects that go into the program executables
class ProgDetails( _Details ):

	'''
	This class extends _Details class and adds the following instance variables:

	objects                   List of object nodes supplied to the program emitter method
	prog_filename             The filename (string with no path, but retains any prefix & suffix)

	The remainder are set after processing the sconscripts

	source_files              Set of all the source files that will be linked into this executable
	                          (filename with project dir relative path)

	external_libs             List of external shared libraries required for this program (strings)
	internal_static_libs      List of internal static LibDetails required for this program
	internal_shared_libs      List of internal shared LibDetails required for this program

	android_linked_objects    List of objects that will be linked into this executable
	android_static_libs       List of android static libs this program needs to link against (external to Mali) (strings)
	excluded                  Boolean, whether this program be excluded from the Android make file
	is_test                   Boolean, whether this program is a test executable (only used in Android builds)
	'''

	def __init__(self, env, node, objects, prog_filename ):

		_Details.__init__(self, env, node)

		self.prog_filename = prog_filename
		self.objects = list(objects)            # Clone supplied objects

		# These get set after all the sconscripts have been processed
		self.source_files = []
		self.internal_static_libs = []
		self.internal_shared_libs = []
		self.android_static_libs = []
		self.android_linked_objects = []
		self.external_libs = []
		self.excluded = False
		self.is_test = False

		abs_file = env.File(node).abspath

		# Check the target program matches its env - sanity check
		if not abs_file.startswith( env['BUILD_DIR_PATH'] ):
			print "WARNING - ProgDetails - Target program in wrong build tree"
			print "Program path: " + abs_file
			print "BUILD_DIR_PATH: " + env['BUILD_DIR_PATH']


# ------------------------------
# Dictionary of the generated program executables, each is an instance of ProgDetails.
# These record the details of the object files and libraries that make up the executable.
# The key is the full path of the executable.
programs = dict()


# ------------------------------

def program_build_emitter( source, target, env ):
	'''
	Tracks which objects and libraries go into which
	program executables but	makes no changes.
	'''
	global programs

	for tg in target:

		# Each program can only have one build declaration, so we can use the absolute
		# path of the target program as the key.
		prog_key = env.File( tg ).abspath

		if prog_key not in programs:

			# The key is the absolute filename - split it.
			prog_path, prog_name = os.path.split( prog_key )

			programs[ prog_key ] =  ProgDetails(
						env,
						tg,     # target program node
						source, # list of source object nodes
						prog_name
						)
		else:
			# This library has already been declared!
			_fatal_error( "program_build_emitter - program has already been declared: %s" % prog_key )
	# No change
	return target, source

# ------------------------------

_mono_debug = False	# Debug
_mono_debug2 = False	# Debug lower level functions

def _find_library_details( env, lib_name, libs_paths ):
	'''
	Return the LibDetails for the specified library - searching
	the specified library paths until its found. Returns None if
	its not found.

	Note lib_name must not include the path or the library
	suffix but it is permitted to already have the standard
	library prefix.
	'''
	global declared_libs
	global static_lib_names
	global shared_lib_names

	if _mono_debug2:
		print "_find_library_details: " + lib_name
		#print "libs_paths: ",
		#print libs_paths

	# First we need to see if any static or shared library with
	# this name has been declared.

	# Work out what the filenames would be for a static and shared
	# library with this lib_name.
	static_lib_name = _full_static_lib_filename( env, lib_name )
	shared_lib_name = _full_shared_lib_filename( env, lib_name )

	if _mono_debug2:
		print "Full lib names: " + static_lib_name + " and " + shared_lib_name

	# Now see if either library has been declared
	if static_lib_name not in static_lib_names \
	and shared_lib_name not in shared_lib_names:
		# Unknown library. Assume its an external library
		if _mono_debug2:
			print "Not known"
		return None

	# We now know there should be a library in declared_libs[] that matches.

	# For each path in libs_paths find the first for which a library
	# with the specified name matches a known library in
	# declared_libs[]. Note that the library might not yet have been
	# created so we don't check the file exists.

	"""
	if _mono_debug2:
		print "Checking LIBPATH: ",
		print libs_paths
	"""

	for path in libs_paths:
		if not isinstance( path, str ):
			print "Path is not a STRING: ",
			print type( path )
			# Hopefully we can convert it to a string
			path = str(path)
		if path[0] == '#':
			# its a project relative path - convert it to a full path
			path = env['BUILD_DIR_PATH'] + path[1:]

		found_lib_name = ''

		# See if there is a matching shared or static library in this path
		abs_shared_lib_name = os.path.join(path, shared_lib_name)
		abs_static_lib_name = os.path.join(path, static_lib_name)

		# Nobody should declare a shared and static library with
		# the same base name in the same directory. If it occurs
		# then this code will choose the shared one.
		if abs_shared_lib_name in declared_libs:
			found_lib_name = abs_shared_lib_name
		elif abs_static_lib_name in declared_libs:
			found_lib_name = abs_static_lib_name

		if found_lib_name:
			if _mono_debug2:
				print "Found"
			return declared_libs[found_lib_name]

	# We haven't found the library - the build will almost certainly fail.
	_fatal_error( "_find_library_details - failed to find details for known library: %s in paths %s" % (lib_name, (libs_paths)))
	return None


# ------------------------------
def _get_libs( env, slibs = None ):

	'''
	From the LIBs in the specified environment (which is mainly just strings),
	OR the slibs argument if its supplied, get a list of libraries that the library/program
	requires. Return a list of the internal static libraries (instances of LibDetails), a
	list of the internal shared libraries (instances of LibDetails), a list of external
	libraries (strings) and a list of object nodes (if any) found in LIBS/slibs.
	(Some scripts add object file nodes to LIBS)
	
	IMPORTANT: This function is only ever called before libmali has been declared,
	so if the string 'mali' or 'libmali' is in the list of LIBS (or slibs), then it
	is assumed to be an external library and the string is returned in filtered_external_libs[]
	'''

	def _add_lib_details( details ):
		'''
		Append reference to the specified library details to
		returned_internal_shared_libs[] or returned_internal_static_libs[]
		as appropriate, but ignore static libraries that are known to be empty.
		'''

		if details.is_shared:
			if details not in returned_internal_shared_libs:
				returned_internal_shared_libs.append( details )
		else:
			if not details.is_empty and details not in returned_internal_static_libs:
				returned_internal_static_libs.append( details )

	global declared_libs

	# List of the external libs (strings) remaining after we've removed the internal ones.
	filtered_external_libs = []

	# List of LibDetails of the internal static libraries
	returned_internal_static_libs = []

	# List of LibDetails of the internal shared libraries
	returned_internal_shared_libs = []

	# List of object nodes found in LIBS/slibs
	returned_internal_objects = []

	# Make a local copy of the environment so we can make a temporary change
	clone_env = env.Clone()

	# If there is an slibs (which there usually will be) then it is usually
	# more up to date than env['LIBS'].
	if slibs:
		# Overwrite clone_env['LIBS'] so we can recursively expand it.
		clone_env['LIBS'] = slibs

	# Note that not all environments will have a LIBS defined.
	if 'LIBS' in clone_env:
		# A list of libraries has been specified. Identify the internal ones.

		# Recursively expand LIBS as far as possible into a list of library
		# strings and library node lists. Duplicate entries are permissable.
		supplied_libs = _expand_env_variable( clone_env, '$LIBS' )

		# Remove any empty strings from the list
		while '' in supplied_libs:
			supplied_libs.remove('')

		# We usually also need to use the LIBPATH
		libs_paths = None
		if 'LIBPATH' in clone_env:
			# Expand LIBPATH into a list of library paths
			libs_paths = _expand_env_variable( clone_env,'$LIBPATH' )

		if _mono_debug2:
			print "libs_paths:"
			print libs_paths
			print "LIBS:"

		for lib in supplied_libs:
			if isinstance( lib, SCons.Node.NodeList ) and len(lib) == 1:
				# We should be able to find it in declared_libs[]
				abs_lib = lib[0].abspath

				if _mono_debug2:
					print _shorten_abs_build_path_name(clone_env, abs_lib),
					print "\tNODE"

				if abs_lib in declared_libs:
					_add_lib_details( declared_libs[ abs_lib ] )
				else:
					# check if its an object file
					if lib[0] in declared_object_nodes:
						if _mono_debug2:
							print "Object: ",
							print lib[0]
						if lib[0] not in returned_internal_objects:
							returned_internal_objects.append( lib[0] )
					else:
						# The build will almost certainly fail
						_fatal_error( "_get_libs - failed to find details for: %s" % abs_lib )
			elif isinstance( lib, str ):
				# Most of the entries will be strings. These require more effort.
				# We assume they are all libraries.
				if _mono_debug2:
					print lib,
					if len(lib) < 7:
						print "\t",
				details = None

				if libs_paths and len( libs_paths ) > 0 and lib != '':
					# We need to search libs_paths for a match
					details = _find_library_details( clone_env, lib, libs_paths )

				if details:
					# Its a generated library.
					if _mono_debug2:
						print "\tInternal"
					_add_lib_details( details )
				else:
					# Assume its an external library - just add it to the list of libraries
					# taking care to ensure its unique. (Its inefficient to link the same
					# library multiple times).
					if _mono_debug2:
						print "\tExternal"
					if lib not in filtered_external_libs :
						filtered_external_libs.append( lib )
			else:
				# Never seen any other type.
				print lib,
				print "UN-EXPECTED TYPE ",
				print type( lib )
	
	return returned_internal_static_libs, returned_internal_shared_libs, filtered_external_libs, returned_internal_objects

# ------------------------------

def _print_details( details ):
	'''
	Private debug function to print details of specified library or program.
	Currently only called before any Android changes have been made to the records.
	'''
	if _mono_debug: # and False:

		env = details.env

		print "\n------"
		print _shorten_abs_build_path_name( env, env.File(details.dst_node).abspath ) + "\n"

		if hasattr( details, "custom_prefix") and details.custom_prefix != '':
			print "Custom library prefix: " + details.custom_prefix

		if len(details.source_files) > 0:
			print "Source files: "
			for src in details.source_files:
				print "\t" + src

		cppdefines = details.defines

		if len(cppdefines) > 0:
			print "CPPDEFINE: "
			for defy in cppdefines:
				print "\tDEFINE " + defy,
				val = cppdefines[defy]
				if not val is None:
					print "=",
					if isinstance(val, str) and len(val) == 0 :
						# Special case to define an empty string
						print "\"\""
					else:
						print val

		if len(details.ccflags) > 0:
			print "CCFLAGS: "
			for flag in details.ccflags:
				print "\t" + flag

		if len(details.include_paths) > 0:
			print "CPPPATH: "
			for inc_path in details.include_paths:
				print "\t" + str(inc_path)

		if len(details.os_include_paths) > 0:
			print "OS_CPPPATH: "
			for inc_path in details.os_include_paths:
				print "\t" + str(inc_path)

		if details.slibs:
			print "slibs: (" + str(len(details.slibs)) + ")"
			for lib in details.slibs:
				print "\t" + str(lib)

		if 'LIBS' in details.env:
			print "LIBS: (" + str(len(details.env['LIBS'])) + ")"
			for lib in details.env['LIBS']:
				print "\t" + str(lib)

		print "OS: " + details.env['os']
		print "Arch: " + details.env['arch']

		print "\n"


# ------------------------------

def _expand_and_add_cpppaths( env, dst_list, src_list ):
	'''
	Private function - Expand the CPP paths in src_list,
	shorten them so they are relative to the project directory
	and add them to dst_list.
	
	IMPORTANT dst_list & src_list must be different lists
	'''
	if src_list is dst_list:
		_fatal_error( "_expand_and_add_cpppaths - src_list and dst_list must be different" )

	for path in src_list:
		expanded_path = _expand_env_variable( env, path )
		path_str = str( expanded_path[0] )

		# Ignore path that specifies the project directory
		if path_str != project_directory:

			# Remove any leading '#' and the project directory
			if path_str[0] == '#':
				path_str = path_str[1:]
			else:
				# Only want project directory relative path
				path_str = _shorten_abs_path_name( path_str )

			if path_str not in dst_list:
				dst_list.append( path_str )

# ------------------------------

def _expand_and_add_cppdefines( env, dst_dict, src_dict ):
	'''
	Private function - Expand the CPP defines in src_dict
	and add them to dst_dict. If a key in src_dict already
	exists in dst_dict then it gets set to the value in
	src_dict.

	IMPORTANT dst_dict & src_dict must be different dictionaries.
	'''
	if src_dict is dst_dict:
		_fatal_error( "_expand_and_add_cppdefines - src_dict and dst_dict must be different" )

	for define in src_dict:
		val = src_dict[define]
		if val != '':
			expanded_val = _expand_env_variable( env, val )
			dst_dict[define] = expanded_val[0]
		else:
			dst_dict[define] = val


# ------------------------------

def _expand_env_details( details ):
	'''
	Private function - Expand the CPPPATH and CPPDEFINES
	of the specified library/program.
	'''

	env = details.env

	expanded_inc_paths = []
	_expand_and_add_cpppaths( env, expanded_inc_paths, details.include_paths )
	details.include_paths = expanded_inc_paths

	expanded_defines = dict()
	_expand_and_add_cppdefines( env, expanded_defines, details.defines )
	details.defines = expanded_defines


# ------------------------------

# List of static libraries we don't want to go in the shared mali library
# Note that this list might be altered indirectly by post_script_processing()

LIBMALI_EXCLUDED_STATIC_LIBS=[ 'cstate_nostatic',
			'cframe_dump',
			'gles_unit',
			'opencl_context_unit',
			'opencl_dispatch_unit',
			'opencl_marshal_unit',
			'opencl_objects_unit',
			'opencl_simultaneous_device_usage_unit',
			'opencl_platform_unit',
			'opencl_plugin_unit',
			'opencl_plugin_device_unit',
			'opencl_plugin_memory_unit',
			'opencl_simultaneous_cpu_gpu_usage_unit',
			'opencl_system_integration_unit',
			'opencl_test_common_unit',
			'opencl_performance_unit',
			'opencl_qualifiers_unit',
			'opencl_thread_and_reentrance_safety_unit',
			'opencl_regression_unit',
			'opencl_gl_interop_unit',
			'opencl_egl_image_unit',
			'bifs',
			'ccfrontend',
			'cbthirdparty',
			'cbopenclutil',
			'osutestcommon',
			'kern_testing',
			'base_memleak_runner',
			'basebenchmarking',
			'base_test_helpers',
			'tpi','tpi_egl','utf']

def _build_shared_mali_library( env ):
	'''
	Private function - declares the build rule for the shared mali library

	We only have a complete picture of all the object files of all the modules after
	sconstruct has processed all the sconscripts and we've processed all the library
	and program records. So only now can we declare how to build the shared mali library.
	Note that it will not contain any mocked object files. To keep it simple we use the
	details recorded in the static libraries. Rather than hard code the full list of
	libraries here, we will look at what static libraries have been declared and ignore
	some specific ones. We also ignore any which are marked as excluded. If build targets
	are specified on the build line, then these also effect the contents of libmali.

	Note that env is always the target environment
	
	Returns a reference to the new libmali record in declared_libs[]
	'''

	global declared_libs

	build_os = env['os']
	build_arch = env['arch']

	if _mono_debug:
		print "\n=========================\n\n"
		print "_build_shared_mali_library"

	# Create a list of static libraries from the libraries in declared_libs[] that are public static
	# and that are for the target and are not filtered out.
	
	static_libs = []	 	# (a list of the absolute paths of the static libraries)
	for lib in declared_libs:
		if declared_libs[lib].is_public_static \
		and not declared_libs[lib].is_empty \
		and declared_libs[lib].os == build_os \
		and declared_libs[lib].arch == build_arch \
		and declared_libs[lib].libname not in LIBMALI_EXCLUDED_STATIC_LIBS \
		and not declared_libs[lib].excluded \
		and (env['android_release'] == '0' or declared_libs[lib].libname not in ANDROID_RELEASE_TARGETS):

			static_libs.append( lib )

	if _mono_debug:
		print "\nList of static libs used to create shared mali lib:"
		for lib in static_libs:
			print _shorten_abs_build_path_name( declared_libs[lib].env, lib )

	# NOTE: If we used sets instead of lists then the order of the objects and external
	# libraries would change on each run, which would cause SCons to re-build the library.
	object_nodes = []
	external_libs = []
	static_libs_details = []

	# Now build a list of all the object nodes and external libraries of the public static libraries
	for lib in static_libs:
		lib_details = declared_libs[ lib ]
		
		object_nodes.extend( x for x in lib_details.object_nodes if x not in object_nodes )

		# And also record the external libraries of each static library
		external_libs.extend( x for x in lib_details.external_libs if x not in external_libs )

		# record the details of each static library
		static_libs_details.append(lib_details)

	'''
	if _mono_debug:
		print "\nComplete list of object nodes for shared mali library:"
		for node in object_nodes:
			print node
	'''

	# Also need to link against some other OS specific libraries
	os_external_libs = []

	if build_os == 'linux' or build_os == 'android':
		os_external_libs.extend( ['m','dl'] )
		if env['backend'] == 'user' and env['no_mali'] == '0':
			os_external_libs.append( 'stdc++' )

	external_libs.extend( x for x in os_external_libs if x not in external_libs )
	external_libs = _expand_env_variable( env, external_libs )

	# And for hardware builds we need to include the model
	if env['backend'] == 'user' and env['no_mali'] == '0' and 'MidgardModel' not in external_libs:
		external_libs.append('MidgardModel')

	'''
	if _mono_debug:
		print "\nComplete list of external libraries for shared mali library: "
		print external_libs
	'''

	# Create a suitable LIBPATH
	lib_path = ['$STATIC_LIB_PATH']
	if build_os == 'linux' and env['winsys'] == 'x11' and X11_LIBPATH not in lib_path:
		lib_path.append(X11_LIBPATH)

	if build_os == 'linux':
		if env['winsys'] == 'directfb' or env['winsys'] == 'wayland' or env['winsys'] == 'gm' or env['winsys'] == 'multi':
			lib_path.append('$SHARED_LIB_PATH')

	if env['cl']:
		lib_path.append(BIFL_LIBPATH)
		
	malienv = env.Clone()
	mappath = os.path.join(malienv.GetLaunchDir(), "bldsys")
	mapfile = os.path.join(mappath, "libmali.map")
	malienv['_LIBFLAGS'] = malienv['_LIBFLAGS'] + (' -Wl,--Bsymbolic -Wl,--version-script='+mapfile)
	
	# Declare the shared mali library - note that this causes libmali to be added to declared_libs[]
	lib_mali_node = malienv.SharedLibrary( target = '$SHARED_LIB_PATH/mali', source = object_nodes, LIBS = external_libs, LIBPATH = lib_path )[0]

	# The shared mali library isn't dependent on the static libraries, however SCons (for
	# reasons unknown) thinks it is but does not ensure that they are built in time.
	# So we explicitly set the shared mali library to be dependent on the static libraries.
	lib_nodes = [ declared_libs[x].dst_node for x in static_libs ]
	env.Depends( lib_mali_node, lib_nodes )

	# Set libmali as a build target.
	# Note that if libmali is the only build target specified, then the build system
	# will behave as if no target were specified and build cl,egl,&gles. However
	# if libs_install directory is set then only libmali will be copied to it,
	# (this is due to the way SCons handles the Install function).
	env.LibTarget('libmali', lib_mali_node)

	# The call to SharedLibrary will have added a record for the mali library to
	# declared_libs[]. Create a reference to it.
	libmali_details = declared_libs[ env.File( lib_mali_node ).abspath ]

	# Sanity check
	if libmali_details.dst_node != lib_mali_node:
		_fatal_error( "_build_shared_mali_library - libmali entry in declared_libs[] has incorrect node!" )

	# Record the list of required external libraries
	libmali_details.external_libs = external_libs

	# Record the list of details of the static libraries
	libmali_details.internal_static_libs = static_libs_details

	# -------

	if build_android:

		# Process the CPPPATH and CPPDEFINES of the library
		_expand_env_details( libmali_details )
		if "MALI_UNIT_IMPOSTERS" in libmali_details.defines:
			# This shouldn't happen.
			_fatal_error( "_build_shared_mali_library - imposters found in libmali" )

	# -------

	if _mono_debug:
		_print_details( libmali_details )

	return libmali_details


# ------------------------------

def _filter_out_mocked_libs( lib_details_list ):
	'''
	Create a new list of LibDetails derived from lib_details_list that
	doesn't include any library for which there is also a mocking library
	'''

	# Build a list of filtered_lib_details and initially populate it with libraries that mock
	filtered_lib_details = [ x for x in lib_details_list if x.is_mock ]

	# Now add all the non-mocking libraries for which there is no mock library
	for lib_details in lib_details_list:
		if not lib_details.is_mock:
			if lib_details.libname not in [ x.libname for x in filtered_lib_details ]:
				filtered_lib_details.append( lib_details )
			elif _mono_debug:
				print "Filtered out mocked library",
				print _shorten_abs_build_path_name( lib_details.env, lib_details.env.File(lib_details.dst_node).abspath )

	return filtered_lib_details

# ------------------------------

def _update_library_record( abs_lib ):
	'''
	Private function - Updates the specified library record with the
	external libraries it requires, the nodes of the internal libraries
	it requires and the nodes of any objects specified in LIBS.
	
	Note, this function does not process the libMali record.
	'''

	global declared_libs

	lib_details = declared_libs[ abs_lib ]
	lib_env = lib_details.env

	"""
	if _mono_debug:
		print "_update_library_record - ",
		print _shorten_abs_build_path_name( lib_env, lib_env.File(lib_details.dst_node).abspath )
	"""

	# Get the libraries and objects this library requires to be linked with.
	# Note that if the strings 'libmali' or 'mali' are in lib_details.slibs then it
	# will be assumed to be an external library as libmali has not been declared yet.
	internal_static_libs, internal_shared_libs, external_libs, internal_lib_objects = _get_libs( lib_env, lib_details.slibs )

	# No libraries have object nodes in LIBS yet. Handy debug line
	#if internal_lib_objects: print _shorten_abs_build_path_name( lib_env, lib_env.File(lib_details.dst_node).abspath ) + " has objects in LIBS"

	# Update the library record
	lib_details.external_libs = list(external_libs)
	lib_details.internal_static_libs = list(internal_static_libs)
	lib_details.internal_shared_libs = list(internal_shared_libs)
	lib_details.internal_lib_objects = list(internal_lib_objects)

	# ----------

	if build_android:

		# Process the CPPPATH and CPPDEFINES of the library
		_expand_env_details( lib_details )


# ------------------------------

def _process_objects():
	'''
	Private function - Updates all the object records.
	Expands the object's CPPPATH and updates the src_file
	field to be relative to the project directory
	'''

	global declared_object_nodes

	for obj_node in declared_object_nodes:

		obj_details = declared_object_nodes[ obj_node ]
		obj_env = obj_details.env

		# Object record needs a local copy of the key env vars.
		obj_details._copy_key_env_details()

		# Process the CPPPATH and shorten them so they are relative to the project directory
		# Note that some objects are built with custom CPPPATHs.
		expanded_include_paths = []
		
		_expand_and_add_cpppaths( obj_env, expanded_include_paths, obj_details.include_paths )
		
		# Update the record
		obj_details.include_paths = expanded_include_paths

		# Note that we don't need to process the OS_CPPPATHs.

		# Figure out the real source file
		if obj_details.src_node.is_derived():
			# The absoulte path of generated source files
			# will already be correct.
			src_file = obj_details.src_file
		else:
			# Normal source files will be mirrored into the
			# build tree by SCons. We want the real source file.
			src_file = obj_env.File(obj_details.src_node.srcnode()).abspath

		# Shorten the path so its project relative
		src_file = _shorten_abs_path_name( src_file )

		# And update the record
		obj_details.src_file = src_file


# ------------------------------

_static_lib_count = 1    # Debug counter
_static_lib_trigger = [] # list of libraries to debug (order numbers), empty for none

def _process_static_libraries():
	'''
	Private function - Updates each static library record with the
	external libraries it requires, the nodes of the internal libraries
	it requires and the nodes of any objects specified in LIBS
	'''
	global _static_lib_count, _static_lib_trigger, _mono_debug

	# Process all the declared libraries
	for abs_lib in declared_libs:

		lib_details = declared_libs[ abs_lib ]

		# Only interested in static libraries
		if not lib_details.is_shared:

			# Debug
			if _static_lib_count in _static_lib_trigger:
				_mono_debug = True

			if _mono_debug:
				lib_env = lib_details.env
				print "\n=========================\n\n"
				print "%d _process_static_libraries: " % _static_lib_count
				print _shorten_abs_build_path_name( lib_env, abs_lib )
				print abs_lib + "\n"
				#print(lib_env.Dump())

			_update_library_record( abs_lib )

			# Clear redundant info that the static library will never link with. (Static libraries
			# are archives of objects.) But leave external shared libraries un-changed as they
			# will be inherited by libmali (which is created from the contents of the static libraries).
			lib_details.internal_static_libs = []
			lib_details.internal_shared_libs = []
			lib_details.internal_lib_objects = []

			if _mono_debug:
				_print_details( lib_details )

			# Debug
			_static_lib_count += 1
			if len( _static_lib_trigger ) > 0:
				# debugging specific static libraries - switch debug off
				_mono_debug = False


# ------------------------------

_shared_lib_count = 1    # Debug counter
_shared_lib_trigger = [] # list of libraries to debug (order numbers), empty for none

def _process_shared_libraries():
	'''
	Private function - Fixes the dependencies of the shared libraries
	'''
	global _shared_lib_count, _mono_debug

	# SCons doesn't seem able to associate the library strings specified in LIBS
	# with the nodes of the actual libraries. So for shared libraries we tell it the
	# dependencies here and change LIBS to use the static library nodes instead of strings.

	# Process all the declared libraries
	for abs_lib in declared_libs:

		lib_details = declared_libs[ abs_lib ]

		# Only interested in shared libraries
		if lib_details.is_shared:

			lib_node = lib_details.dst_node
			lib_env = lib_details.env

			# Debug
			if _shared_lib_count in _shared_lib_trigger:
				_mono_debug = True

			if _mono_debug:
				print "\n=========================\n\n"
				print "%d _process_shared_libraries: " % _shared_lib_count
				print _shorten_abs_build_path_name( lib_env, abs_lib ) + "\n"
				#print(lib_env.Dump())

			_update_library_record( abs_lib )

			# Internal objects nodes to link in (that were specified in LIBS)
			# These will be very rare. (They never occur at the time of writing)
			lib_object_nodes = list(lib_details.internal_lib_objects)

			# Required internal static and shared libraries (instances of LibDetails)
			required_static_lib_details = list(lib_details.internal_static_libs)
			required_shared_lib_details = list(lib_details.internal_shared_libs)

			# Now remove any static or shared libraries for which there is also a mock library.
			filtered_static_lib_details = _filter_out_mocked_libs( required_static_lib_details )
			filtered_shared_lib_details = _filter_out_mocked_libs( required_shared_lib_details )

			# Fix the shared library's dependencies.

			# Set the library as dependent on all internally generated object Nodes.
			# SCons hasn't setup any of the dependencies yet.
			lib_env.Depends( lib_node, lib_object_nodes )

			#"""
			if _mono_debug:
				print "Dependencies: "
				print "\tLib Objects: "		# Objects specified in LIBS
				for node in lib_object_nodes:
					obj_env = declared_object_nodes[ node ].env
					print "\t\t" + _shorten_abs_build_path_name( obj_env, obj_env.File(node).abspath )
			#"""

			# and all the internally generated libraries
			lib_nodes = [ x.dst_node for x in filtered_static_lib_details ]
			lib_env.Depends( lib_node, lib_nodes )
			lib_nodes = [ x.dst_node for x in filtered_shared_lib_details ]
			lib_env.Depends( lib_node, lib_nodes )

			# Build a list to replace env['LIBS'].
			# Start with the external libraries (strings)
			retained_libs = list(lib_details.external_libs)

			# Append the nodes of the static libraries to retained_libs[]
			for lib in filtered_static_lib_details:
				if lib.dst_node not in retained_libs:
					retained_libs.append( lib.dst_node )

			# Append the nodes of any objects specfied in LIBS to retained_libs[]
			for obj in lib_object_nodes:
				if obj not in retained_libs:
					retained_libs.append( obj )

			# Append the base name of the shared libraries to retained_libs[]
			# Note we can't use the nodes of the shared libraries otherwise the
			# shared library being created will try to link to them at run
			# time using their full path (in the build tree).
			for lib in filtered_shared_lib_details:
				if lib.libname not in retained_libs:
					retained_libs.append( lib.libname )

			# Set LIBS to the retained libraries - so now it will specify the external
			# library strings, the internal shared library strings and the internal static
			# library nodes.
			lib_env['LIBS'] = list(retained_libs)

			if _mono_debug:
				_print_details( declared_libs[ abs_lib ] )

			# Debug switch
			_shared_lib_count += 1
			if len(_shared_lib_trigger) > 0:
				# debugging specific shared libraries - switch debug off
				_mono_debug = False

# ------------------------------

# Set of shared libraries that are wrappers for libmali
LIBMALI_WRAPPER_LIBS = set(['EGL', 'GLESv1_CM', 'GLESv2'])

# ------------------------------

_prog_count = 1    # Debug counter
_prog_trigger = [] # list of programs to debug (order numbers), set to empty list for none

def _process_programs():
	'''
	Private method.

	Changes every program node such that instead of specifying static libraries to link in,
	it directly links in the object nodes that are in those static libraries.
	'''

	global programs, _mono_debug, _prog_count
	_mono_debug3 = False	# Flag, print source object details

	# First get each program to take a copy of its key env vars
	for prog_key in programs:
		programs[ prog_key ]._copy_key_env_details()

	#print "Program details\n---------------"

	for prog_key in programs:
		prog_record = programs[ prog_key ]
		prog_node = prog_record.dst_node
		prog_env = prog_record.env

		## Debug switch
		if _prog_count in _prog_trigger:
			# Doing single prog debug, switch debug on
			_mono_debug = True

		if _mono_debug:
			print "\n------------\n" + str( _prog_count ),
			print prog_key
			#print(prog_env.Dump())
			print

		# If there is a LIBPATH then make sure its a list
		if 'LIBPATH' in prog_env:
			lib_path = prog_env['LIBPATH']
			if not isinstance( lib_path, list ):
				# Make it a list
				prog_env['LIBPATH'] = [lib_path]

		# Are we going to add the objects of the static libraries to the program.
		if not build_android or \
		not android_use_static_libs or \
		prog_env['os'] != 'android':
			add_static_lib_objects = True
		else:
			add_static_lib_objects = False

		# Get the libraries this program requires
		# Note that if the strings 'libmali' or 'mali' are in prog_record.slibs then it
		# will be assumed to be an external library as libmali has not been declared yet.
		static_lib_details, shared_lib_details, external_libs, lib_object_nodes = _get_libs( prog_env, prog_record.slibs )

		# Very few programs put object nodes in LIBS. Handy debug line.
		if _mono_debug and lib_object_nodes: print _shorten_abs_build_path_name( prog_env, prog_key) + " has objects in LIBS"

		if external_libs and "mali" in external_libs:
			# Any program directly linking libmali is assumed to be a test, but note that
			# only tests that do not use gles or egl should directly link libmali
			prog_record.is_test = True

		# Record the required external libraries
		prog_record.external_libs = external_libs

		# List of required external and shared internal libraries (strings). Start with just the externals.
		retained_libs = list( external_libs )
		if _mono_debug and retained_libs: print "Links with external libraries " + str(retained_libs)

		# Set the program as dependent on all internally generated object Nodes.
		# SCons hasn't setup any of the dependencies yet.
		prog_env.Depends( prog_node, lib_object_nodes )

		# Now remove any static or shared libraries for which there is also a mock library.
		filtered_static_lib_details = _filter_out_mocked_libs( static_lib_details )
		filtered_shared_lib_details = _filter_out_mocked_libs( shared_lib_details )

		# Set the program as dependent on the remaining internally generated static and shared libraries
		lib_nodes = [ x.dst_node for x in filtered_static_lib_details ]
		prog_env.Depends( prog_node, lib_nodes )

		lib_nodes = [ x.dst_node for x in filtered_shared_lib_details ]
		prog_env.Depends( prog_node, lib_nodes )

		# Build up a list of object files that will be linked into the program

		# List of the objects to link - start with the objects of the program
		expanded_source_objs = list( prog_record.objects )

		# Now add all object nodes in lib_object_nodes (usually there aren't any)
		for obj in lib_object_nodes:
			_add_objects( expanded_source_objs, [ obj ] )

		# process the libraries in filtered_static_lib_details

		if filtered_static_lib_details and _mono_debug:
			print "Link objects from static libraries:"

		for lib in filtered_static_lib_details:
			if add_static_lib_objects:
				# Append the nodes of the objects in the static library.
				_add_objects( expanded_source_objs, lib.object_nodes )

			if lib.libname in ['tpi','tpi_egl','utf']:
				# Only test programs link with the tpi & utf libraries
				prog_record.is_test = True

			if _mono_debug:
				print "\t" + _shorten_abs_build_path_name( lib.env, lib.env.File(lib.dst_node).abspath )

		# process the libraries in filtered_shared_lib_details
		for lib in filtered_shared_lib_details:
			# Append the shared library base filename to the list of retained libraries.
			# Note we can't use the nodes of the shared libraries otherwise the program will
			# try to link to them at run time using their full path (in the build tree).
			if lib.libname not in retained_libs:
				retained_libs.append( lib.libname )
				if _mono_debug: print "Links with internal shared library " + lib.libname

			if lib.libname in LIBMALI_WRAPPER_LIBS:
				# Only test programs link with the the mali wrapper libraries
				prog_record.is_test = True

		# Update the program record
		prog_record.internal_static_libs = filtered_static_lib_details
		prog_record.internal_shared_libs = filtered_shared_lib_details

		# Process the list removing objects for which there is also a mock object
		new_source_objs = _remove_mocked_object_files(expanded_source_objs)

		# Set LIBS to just the retained libraries (external and internal shared libraries)
		# Note that all programs that share the same environment will use this updated LIBS.
		prog_env['LIBS'] = list(retained_libs)

		# If this is an X11 build then add the path of the X11 libs to LIBPATH
		if prog_env['os'] == 'linux' and prog_env['winsys'] == 'x11':
			if 'LIBPATH' in prog_env:
				lib_path = prog_env['LIBPATH']
				if X11_LIBPATH not in lib_path:
					lib_path.append(X11_LIBPATH)
				prog_env['LIBPATH'] = lib_path
			else:
				prog_env['LIBPATH'] = [X11_LIBPATH]

		if _mono_debug3 and new_source_objs:
			print "Final list of objects going into the program: "
			for src_obj in new_source_objs:
				print "\t" + _shorten_abs_build_path_name( prog_env, prog_env.File(src_obj).abspath )

		# Add the complete set of object nodes
		# The add_source() function ensures there are no duplicates
		for src_obj in new_source_objs:
			if src_obj not in prog_node.sources:
				prog_node.add_source( [src_obj] )

		# Ensure the program is dependent on the complete set of source objects.
		# This might be over-kill but it does no harm.
		prog_env.Depends( prog_node, new_source_objs )

		# And add the complete set of source objects to the executor
		# of the program node. It also ensures there are no duplicates.
		prog_node.get_executor(create=0).add_sources( new_source_objs )

		# --------

		if build_android:

			# Record which objects nodes are going into this program
			prog_record.android_linked_objects = new_source_objs
			progname = prog_record.prog_filename

			if prog_record.internal_shared_libs:
				# Get the list of shared library names (note that this list is parallel to
				# prog_record.internal_shared_libs )
				shared_libnames = [ x.libname for x in prog_record.internal_shared_libs ]

				# Find out which (if any) mali wrapper shared libs it links with, use a set.
				shared_libnames_set = set(shared_libnames)
				wrapper_libs = shared_libnames_set.intersection( LIBMALI_WRAPPER_LIBS )

				if wrapper_libs:
					# Tests that link with any of the mali wrapper libraries need to be
					# changed to link against libmali instead. Remember that libmali
					# hasn't been declared yet so we have to treat it as an external libary
					# for now.
					if 'mali' not in prog_record.external_libs:
						prog_record.external_libs.append( 'mali' )

					# And the program should no longer link with the wrapper libraries.
					# Build a list of the indexes of the wrapper libraries to remove.
					indexes=[]
					for wrapper_lib in wrapper_libs:
						indexes.append( shared_libnames.index(wrapper_lib) )

					# sort them highest to lowest index
					indexes.sort(reverse=True)

					# Now delete them.
					for idx in indexes:
						del prog_record.internal_shared_libs[idx]

			# Process the CPPPATH and CPPDEFINES of the program
			_expand_env_details( prog_record )

		# --------

		if _mono_debug:
			_print_details( prog_record )

		# --------
		# Debug switch
		_prog_count += 1
		if len(_prog_trigger) > 0:
			# debugging specific programs - switch debug off
			_mono_debug = False
			_mono_debug3 = False

# ------------------------------

def _update_libmali_excludes(env):
	'''
	Private method.

	The tpi and utf	are normally excluded from libmali but for some
	builds we want them to be included.
	'''

	if env['debug'] == '1' or env['gles_qa'] == '1':
		LIBMALI_EXCLUDED_STATIC_LIBS.remove('utf')
		LIBMALI_EXCLUDED_STATIC_LIBS.remove('tpi')

# ------------------------------

def _append_os_include_paths( details, new_inc_paths ):
	'''
	Append the supplied list of OS include path strings to the specified
	library/program record, ensuring each path is unique.
	'''
	for path in new_inc_paths:
		path_str = str(path)
		if path_str not in details.os_include_paths:
			details.os_include_paths.append( path_str )

# ------------------------------

def _replace_include_path( details, orig_inc_path, new_inc_path ):
	'''
	Replace the specified include path with the specified new
	include path in the specified library/program record.
	
	If neither the original or new include paths are in the
	list then the new path is added to the end of the list.
	
	If the new path is already in the list and occurs later than
	the specified original, then it is removed and the original
	gets replaced by the new path.
	
	If the new path is already in the list and occurs earlier than
	the specified original, then the original path gets removed.
	'''

	# NOTE: This function is currently not used but it might be useful in the future.

	if not orig_inc_path or not new_inc_path or orig_inc_path == new_inc_path:
		# Miss usage
		_fatal_error( "_replace_include_path - incorrect usage!" )

	orig_idx = -1
	new_idx = -1

	# first get the indices of the two paths if they are already in the list
	if orig_inc_path in details.include_paths:
		orig_idx = details.include_paths.index(orig_inc_path)

	if new_inc_path in details.include_paths:
		new_idx = details.include_paths.index(new_inc_path)

	if new_idx > 0:
		# New path is already in the list.
		if orig_idx > 0:
			# Original path is also in the list
			if new_idx > orig_idx:
				# Remove the existing copy of the new path as it occurs later in the list
				del details.include_paths[new_idx]
				# Replace the original with the new
				details.include_paths[orig_idx] = new_inc_path
			else:
				# Existing copy of the new path occurs earlier in the list. Simply remove
				# the original path
				del details.include_paths[orig_idx]

		# If the original path is not in the list but the new path is, then do nothing.

	elif orig_idx > 0:
		# The new path is not already in the list, but the original path is. This is
		# the expected normal usage. Replace the original path with the new.
		details.include_paths[orig_idx] = new_inc_path
	else:
		# Neither path is in the list. Add the new path
		details.include_paths.append( new_inc_path )

# ------------------------------

def _add_android_env_details( details, include_paths, os_include_paths, defines, ccflags  ):
	'''
	Private function - add the OS_CPPPATHs, CPPPATHs, CPPDEFINES and CCFLAGS in the specified
	record to the lists/dictionary referenced.
	
	details:                ref to either an instance of ObjectDetails, LibDetails or ProgDetails.
	include_paths           ref to list of CPPPATH
	os_include_paths        ref to list of OS_CPPPATH
	defines                 ref to dictionary of CPPDEFINES
	ccflags                 ref to list of 'CCFLAGS'
	'''
	# Add CPPPATH
	for path in details.include_paths:
		path_str = str(path)
		if path_str not in include_paths:
			include_paths.append( path_str )

	# Add OS_CPPPATH
	for path in details.os_include_paths:
		path_str = str(path)
		if path_str not in os_include_paths:
			os_include_paths.append( path_str )
	
	# Add CPPDEFINES
	defines.update( details.defines )

	# Add CCFLAGS but don't overwrite existing ones.
	for flag in details.ccflags:
		if flag not in ccflags:
			ccflags.append( flag )

# ------------------------------

def _add_android_build_details( details, object_nodes, delete_static_def ):
	'''
	Private function - Collect extra details of the specified library/program
	required for Android builds and add them to the record.
	
	details:                ref to either an instance of LibDetails or ProgDetails.
	object_nodes:           complete list of the object nodes going in this library/program
	
	Note: Only used for shared libraries (including libmali) and programs 
	'''
	env = details.env

	# Flag to record if any of the source files are C++
	is_cpp = False

	# Record the source files in this library/program (set of the file names with project relative path)
	source_files = set()

	# All libraries and programs are likely to need these Android include paths
	_append_os_include_paths( details, ["bionic/libm/arm"] )
	_append_os_include_paths( details, ["frameworks/native"] )

	# Record the OS_CPPPATHs, CPPPATHs, CPPDEFINES and CCFLAGS of all the objects and static libraries that
	# make up this library/program. Start with the library's own CCFLAGS, OS_CPPPATH and CPPPATH but only
	# add its CPPDEFINES at the end

	all_cppdefines = dict()
	all_ccflags = details.ccflags

	# First add CPPPATHs, OS_CPPPATHs, CPPDEFINES and CCFLAGS of all the objects that have
	# a different environment to the library/program

	for obj_node in object_nodes:
		obj_details = declared_object_nodes[ obj_node ]

		# add the source file to source_files[]
		source_files.add( obj_details.src_file )

		# record if any source file is C++
		is_cpp |= obj_details.is_cpp

		if not obj_details.env is env:
			_add_android_env_details( obj_details,
						 details.include_paths,
						 details.os_include_paths,
						 all_cppdefines,
						 all_ccflags )

	if not isinstance(details, ProgDetails ) or not android_use_static_libs:

		# This library/program will be built from the objects/source files in
		# the static libraries it is supposed to link against, so it needs to
		# inherit the key environment variables from those static libraries.
		# Add CPPPATHs, OS_CPPPATHs, CPPDEFINES and CCFLAGS of all the static libraries
		# that have a different environment to the library/program

		for lib_details in details.internal_static_libs:
			if not lib_details.env is env:
				_add_android_env_details( lib_details,
							 details.include_paths,
							 details.os_include_paths,
							 all_cppdefines,
							 all_ccflags )

	# Add the CPPDEFINES of the library/program itself
	# We do this after processing the objects and static libraries because the
	# library/program defines might alter the object defines and we assume the
	# library/program defines have a higher precedence.
	all_cppdefines.update( details.defines )

	if delete_static_def and 'STATIC' in all_cppdefines:
		# Remove the re-definition of STATIC
		del(all_cppdefines[ 'STATIC' ])

	# Update the library/program record with refs to the updated data
	details.source_files = source_files
	details.defines = all_cppdefines
	details.is_cpp = is_cpp
	details.ccflags = all_ccflags

# ------------------------------

def _add_android_object_build_details( details, object_nodes ):
	'''
	Private function - Collect extra details of the specified static library
	required for Android builds and add them to the record.
	
	details:                ref to an instance of LibDetails
	object_nodes:           complete list of the object nodes going in this library
	
	Note: Only used for static libraries
	'''

	env = details.env
	
	# Record the source files in this library (set of the file names with project relative path)
	source_files = set()

	# Record the OS_CPPPATHs, CPPPATHs, CPPDEFINES and CCFLAGS of all the objects going into
	# this library.	Start with the library's own CCFLAGS, OS_CPPPATH and CPPPATH but only add
	# its CPPDEFINES at the end

	all_cppdefines = dict()
	all_ccflags = details.ccflags

	# First add CPPPATHs, OS_CPPPATHs, CPPDEFINES and CCFLAGS of all the objects
	# that do not have the same environment as the library.

	for obj_node in object_nodes:
		obj_details = declared_object_nodes[ obj_node ]

		# add the source file to source_files[]
		source_files.add( obj_details.src_file )

		# If the object has a different environment to the library
		if not obj_details.env is env:
			_add_android_env_details( obj_details,
						 details.include_paths,
						 details.os_include_paths,
						 all_cppdefines,
						 all_ccflags )

	# Add the CPPDEFINES of the library itself. We do this after processing the
	# objects because the library defines might alter the object defines and we
	# assume the library defines have a higher precedence.
	all_cppdefines.update( details.defines )

	# Update the library record with refs to the updated data
	if android_use_static_libs:
		details.source_files = source_files
	details.defines = all_cppdefines
	details.ccflags = all_ccflags

# ------------------------------

def _disable_compiler_optimisation( details ):
	'''
	Private function - Check the details of the specified library/program
	and if it doesn't already have a 'C' compiler optimisation setting, then
	add a flag to turn compiler optimisation off.
	
	details:    ref to either an instance of LibDetails or ProgDetails.
	
	Note: Used for all shared & static libraries (including libmali) and programs,
	      but only on Android builds
	'''

	# Debug build - want to turn off compiler optimisation
	add_optimise_option = True

	for flag in details.ccflags:
		if flag.startswith('-O'):
			# Already has an optimisation setting - don't change it
			add_optimise_option = False
			break

	if add_optimise_option:
		details.ccflags.append( "-O0")

# ------------------------------

def _add_android_build_data_to_static_libs( env ):
	'''
	Note that env is always the target env
	'''

	build_os = env['os']
	build_arch = env['arch']

	for abs_lib in declared_libs:
		lib_details = declared_libs[ abs_lib ]
		is_cpp = False
		if not lib_details.is_shared \
		and lib_details.os == build_os \
		and lib_details.arch == build_arch:

			for obj_node in lib_details.object_nodes:
				obj_details = declared_object_nodes[ obj_node ]
				is_cpp |= obj_details.is_cpp
			# All libraries and programs are likely to need these Android include paths
			if is_cpp:
				_append_os_include_paths( lib_details, ["bionic"] )
				_append_os_include_paths( lib_details, ["external/stlport/stlport"] )
        
			_append_os_include_paths( lib_details, ["bionic/libm/arm"] )
			_append_os_include_paths( lib_details, ["frameworks/native"] )

			_add_android_object_build_details( lib_details, lib_details.object_nodes )

			if env['debug'] == '1':
				# Debug build - want to turn off compiler optimisation
				_disable_compiler_optimisation( lib_details )

# ------------------------------

def _add_android_build_data_to_shared_libs( env ):
	'''
	Note that env is always the target env
	'''

	common_external_android_libs = ["utils","cutils","binder","log","stlport"]

	graphic_external_android_libs = ["ui", "hardware"]

	build_os = env['os']
	build_arch = env['arch']

	for abs_lib in declared_libs:
		lib_details = declared_libs[ abs_lib ]
		if lib_details.is_shared \
		and lib_details.os == build_os \
		and lib_details.arch == build_arch:

			_add_android_build_details(
				lib_details,
				lib_details.object_nodes + lib_details.internal_lib_objects,
				delete_static_def=True )

			extra_external_libs = []

			libname = lib_details.libname

			# There is special handling for the shared egl and gles libraries
			if libname in ['EGL','GLESv1_CM','GLESv2' ]:
				# We need to link against some extra Android libraries
				extra_external_libs.extend( [ x for x in common_external_android_libs if x not in extra_external_libs ] )
				extra_external_libs.extend( [ x for x in graphic_external_android_libs if x not in extra_external_libs ] )
		
			if libname == 'mali':
				# We need to link against some extra Android libraries
				extra_external_libs.extend( [ x for x in common_external_android_libs if x not in extra_external_libs ] )

			if extra_external_libs:
				lib_details.external_libs.extend( [ x for x in extra_external_libs if x not in lib_details.external_libs ] )
		
			if env['debug'] == '1':
				# Debug build - want to turn off compiler optimisation
				_disable_compiler_optimisation( lib_details )

# ------------------------------

def _add_android_build_data_to_programs( env ):
	'''
	Note that env is always the target env
	'''

	build_os = env['os']
	build_arch = env['arch']

	# Almost all programs re-define STATIC as '' which means that all functions
	# and data declared as STATIC will be global. This causes problems on Android
	# when we choose to build the programs without using static libraries, as all
	# source files have to built with the same CPPDEFINES and this causes duplicate
	# symbols for a lot of tests. Fortunately only a small number of tests really
	# need to define STATIC as ''.
	# Any program whose name starts with one of the stings in the following list
	# can redefine STATIC. Other programs that re-define STATIC will have the
	# re-definition deleted.

	nostatic_progs = [
		'mali_cpom_',
		'mali_cstate_',
		'mali_gles_',
		'mali_cdeps_' ]

	for prog_key in programs:

		prog_record = programs[ prog_key ]

		if prog_record.os == build_os \
		and prog_record.arch == build_arch:
			delete_static_def = True

			# Is this one of the programs that must be built with STATIC undefined
			filename = _bare_program_name(prog_record.env, prog_record.prog_filename)
			
			for prog in nostatic_progs:
				if filename.startswith( prog ):
					# Yes, then we mustn't delete the STATIC re-definition
					delete_static_def = False
					break

			_add_android_build_details( prog_record, prog_record.android_linked_objects, delete_static_def )

			if android_use_static_libs:
				# Need to output all the internal static libs the program is using. Append their names
				# to android_static_libs[]
				prog_record.android_static_libs.extend([ 'lib' + x.custom_prefix + x.libname for x in prog_record.internal_static_libs ])

			if prog_record.is_cpp:
				# This program uses C++
				# Add on the Android C++ include paths
				_append_os_include_paths( prog_record, ["external/stlport/stlport"] )
				_append_os_include_paths( prog_record, ["bionic"] )
				# and link against the external C++ library
				prog_record.external_libs.append('stlport')

			# If programs link in libegl_winsys then they will also need libhardware
			static_libnames = [ x.libname for x in prog_record.internal_static_libs ]
			if 'egl_winsys' in static_libnames:
				if not 'hardware' in prog_record.external_libs:
					prog_record.external_libs.append('hardware')
				if not 'ui' in prog_record.external_libs:
					prog_record.external_libs.append('ui')
			if 'tpi_egl' in static_libnames:
				if not 'utils' in prog_record.external_libs:
					prog_record.external_libs.append('utils')
				if not 'gui' in prog_record.external_libs:
					prog_record.external_libs.append('gui')
			if env['debug'] == '1':
				# Debug build - want to turn off compiler optimisation
				_disable_compiler_optimisation( prog_record )

# ------------------------------------------------------------------------------------------------------

def _update_libmali_users( libmali_details ):
	'''
	Update all shared libraries and programs that link with libmali
	
	Any library or program that links with libmali will currently consider libmali to be an
	external library (because libmali didn't exist when they were processed). This function
	corrects their lists of external and internal shared libraries and sets the libaries/programs
	to be dependent on libmali.

	NOTE that we don't need to alter the LIBS of the libraries/programs as we always specify shared
	libraries in LIBS as strings, not their nodes so the string 'libmali' or 'mali' should already
	occur in the LIBS and doesn't need to be changed.
	'''

	def _check_and_correct_external_libs( details ):
		links_libmali = False
		if 'libmali' in details.external_libs:
			# Remove 'libmali' external_libs
			details.external_libs.remove( 'libmali' )
			links_libmali = True

		if 'mali' in details.external_libs:
			# Remove 'mali' from external_libs
			details.external_libs.remove( 'mali' )
			links_libmali = True
		return links_libmali


	libmali_node = libmali_details.dst_node

	# track if found and libmali users
	found_libmali_user = False

	# First the libraries
	for abs_lib in declared_libs:

		lib_details = declared_libs[ abs_lib ]

		if _check_and_correct_external_libs( lib_details ):
			# Set the library to depend on libmali
			lib_details.env.Depends( lib_details.dst_node, libmali_node )
			# Add reference to libmali's details to the list of internal shared libraries
			lib_details.internal_shared_libs.append(libmali_details)
			# Flag that we've found a libmali user
			found_libmali_user = True

	# Now the programs
	for prog_key in programs:
		prog_record = programs[ prog_key ]

		if _check_and_correct_external_libs( prog_record ):
			# Set the program to depend on libmali
			prog_record.env.Depends( prog_record.dst_node, libmali_node )
			# Add reference to libmali's details to the list of internal shared libraries
			prog_record.internal_shared_libs.append(libmali_details)
			# Flag that we've found a libmali user
			found_libmali_user = True

	if found_libmali_user:
		# libmali should not be excluded from the Android make file
		libmali_details.excluded = False
	else:
		# nothing links with libmali so it should be excluded from the Android make file
		libmali_details.excluded = True

# ------------------------------------------------------------------------------------------------------

def _write_android_include_path_details( fout, details, quiet=False ):
	'''
	Return and write out (if quiet==False) the C pre-processor include paths
	(if there are any) that are recorded in the supplied
	details, to the specified open Android.mk file.
	'''

	inc_paths = details.include_paths
	os_include_paths = details.os_include_paths
	includes = []

	if len(inc_paths) > 0 or len(os_include_paths) > 0:
		# Include paths - one per line
		if not quiet:
			fout.write("\nLOCAL_C_INCLUDES := ")
			if details.env['android_release'] == '1':
				fout.write("$(MALI_COMMON_C_INCLUDES)");
				if details.libname in ANDROID_RELEASE_TARGETS:
					fout.write(" -g0")
				fout.write("\n");
				return

		# Android include paths come first (if there are any). They
		# should always be absolute or relative to the android root
		# directory.
		for inc_path in os_include_paths:
			if not quiet:
				fout.write( " \\\n\t%s" % inc_path )
			if inc_path not in includes:
				includes.append(inc_path)

		for inc_path in inc_paths:
			# Work out if we need to prefix the path with $(LOCAL_PATH) 
			if not os.path.isabs(inc_path):
				# Remove any paths to the build directory when 'unit' == 0
				# The assumption is that the build directory doesn't stay around after 
				# the Android.mk file has been generated
				if not ( details.env['unit'] == '0' and inc_path.startswith( 'build' + os.sep ) ):
					# It is relative. Now check if it exists. 
					# Note: For clcc/third_party and android_prebuilts they don't yet exist but 
					# will do after the host build has been executed, so we prefix them with $(LOCAL_PATH)
					if ( os.path.exists(inc_path) or 
						inc_path.startswith( 'clcc' + os.sep + 'third_party' ) or 
						inc_path.startswith( 'android_prebuilt' ) ):
						# Any relative path reachable from the current
						# directory needs to be prefixed with $(LOCAL_PATH)
						inc_path = os.path.join( "$(LOCAL_PATH)", inc_path )

					if not quiet:
						fout.write( " \\\n\t%s" % inc_path )
					if inc_path not in includes:
						includes.append(inc_path)

		if not quiet:
			fout.write( "\n" )

		return includes

# ------------------------------

def _write_android_src_file_details( fout, details ):
	'''
	Write out the source files (if there are any) that are recorded
	in the supplied details	to the specified open Android.mk file.
	'''

	sources = details.source_files

	if len(sources) > 0:
		fout.write("\nLOCAL_SRC_FILES := ")
		for src in sorted( sources ):
			# Strip "build/<hash>/" from any paths to clcc sources
			# Required for source files generated during the SCons build phase; strip conditionally
			# to prevent unit test builds being upset.
			if (os.sep + 'clcc' + os.sep) in src:
				match = re.match( r'build/.*?/(\S+)', src )
				if match:
					src = match.group( 1 )
			fout.write( " \\\n\t%s" % src )
		fout.write( "\n" )

# ------------------------------

def _write_android_cflags_details( fout, details, quiet=False ):
	'''
	Return and write out (if quiet==False) the C pre-processor flags and defines
	(if there are any) that are recorded in the supplied
	details, to the specified open Android.mk file.
	'''

	ccflags = details.ccflags
	cppdefines = details.defines
	flags = []

	# Filter out compiler flags that are not available or appropriate on Android
	for flag in ['-pthread','-Werror','-fPIC','-fno-rtti']:
		while flag in ccflags:
			ccflags.remove( flag )
	
	if len(cppdefines) > 0 or len(ccflags) > 0 :
		# C, preprocessor flags - one per line
		if not quiet:
			fout.write("\nLOCAL_CFLAGS :=")
			if details.env['android_release'] == '1':
				fout.write(" $(MALI_COMMON_CFLAGS)\n")
				return []

		if len(ccflags) > 0:
			for flag in ccflags:
				if not quiet:
					fout.write( " \\\n\t%s" % flag )
				if flag not in flags:
					flags.append(flag)

		if len(cppdefines) > 0:
			for defy in cppdefines:
				val = str(cppdefines[defy])
				item = "-D%s" % defy
				if not val is None:
					item += "="
					if isinstance(val, str) and len(val) == 0 :
						# Special case to define an empty string
						item += "\"\""
					else:
						item += val

				if item not in flags:
					flags.append(item)

				if not quiet:
					fout.write( " \\\n\t%s" % item )

		# Add Khronos headers support and Android version definition
		extra_flags = ["-D__unix__", "-DPLATFORM_SDK_VERSION=$(PLATFORM_SDK_VERSION)"]
		for flag in extra_flags:
			if  flag not in flags:
				flags.append(flag)

			if not quiet:
				fout.write(" \\\n\t%s" % flag )

		if not quiet:
			fout.write( "\n" )

		return flags


# ------------------------------

def _write_android_shared_lib_details( fout, details ):
	'''
	Output details of the shared libraries that are required
	to build this library/program
	'''

	# lists of required external and internal library names (strings) with no prefix
	external_ld_libs = []
	external_libs = []
	internal_shared_libs = [ x.libname for x in details.internal_shared_libs ]

	# Filter out external libraries that are not available on Android
	for lib in details.external_libs:
		if lib in [ 'pthread','rt','z','m' ] :
			# These ones are specified in LOCAL_LDLIBS
			external_ld_libs.append( lib )
		elif lib not in [ 'MidgardModel', 'LLVM' ]:
			external_libs.append( lib )

		if lib == 'LLVM' and 'stlport' not in details.external_libs:
			external_libs.append( 'stlport' )
 
	if internal_shared_libs or external_libs:

		if _mono_debug:
			fout.write("\n# Num Internal shared libs: %d" % len(internal_shared_libs) )
			fout.write("\n# Num External shared libs: %d" % len(external_libs) )

		fout.write("\nLOCAL_SHARED_LIBRARIES := ")
		
		for lib in external_libs:
			fout.write( " \\\n\tlib%s" % lib )

		for lib in internal_shared_libs:
			if lib == 'mali':
				# libmali is called libGLES_mali on Android
				lib = 'GLES_mali'
			fout.write( " \\\n\tlib%s" % lib )
		fout.write( "\n" )

	if external_ld_libs:

		if _mono_debug:
			fout.write("\n# Num External ld libs: %d" % len(external_ld_libs) )

		fout.write("\nLOCAL_LDLIBS += ")

		for lib in external_ld_libs:
			fout.write( " \\\n\t-l%s" % lib )
		fout.write( "\n" )

# ------------------------------

def _write_android_static_lib_details( fout, details ):
	'''
	Output details of the static libraries that are required
	to build this program.
	'''
	if 'LLVM' in details.external_libs:
		details.android_static_libs += details.env['LLVM_INDIVIDUAL_LIBS']

	if details.android_static_libs:

		fout.write("\nLOCAL_GROUP_STATIC_LIBRARIES := true\n")

		# Duplicate the static libs
		static_libs = list(details.android_static_libs)

		if _mono_debug:
			fout.write("\n# Num Static libs: %d" % len(static_libs) )
		fout.write( "\nLOCAL_STATIC_LIBRARIES :=")
		for libname in static_libs:
			fout.write( " \\\n\t%s" % libname )
		fout.write( "\n" )

# ------------------------------

def _write_android_lib_details( fout, details ):
	'''
	Output details of the specified library to build
	'''

	extra_lines = []
	libname = 'lib' + details.custom_prefix + details.libname

	# Don't output build details for the following shared libraries
	# as their content is really in libmali.
	if libname in ['libEGL','libGLESv1_CM','libGLESv2' ]:
		return

	if libname == 'libmali':
		# On Android we need to change its name to...
		libname = 'libGLES_mali'

	# ----
	# Now write the details

	fout.write("\n\n#--------\n# %s\n" % libname)

	if _mono_debug:
		fout.write("#\n# %s\n" %  details.env.File(details.dst_node).abspath )

	fout.write("\ninclude $(CLEAR_VARS)\n")
	fout.write("LOCAL_PRELINK_MODULE := false\n")

	_write_android_include_path_details( fout, details )

	_write_android_src_file_details( fout, details )

	_write_android_cflags_details( fout, details )
   
	# Add in the llvm/clang static libs to libGLES_mali
	if libname == 'libGLES_mali' and (details.env['cl'] == '1' or details.env['android_release'] == '1'):
		fout.write("\nLOCAL_GROUP_STATIC_LIBRARIES := true\n")
		fout.write( "\nLOCAL_STATIC_LIBRARIES := " )
		if details.env['cl'] == '1':
			for lib in details.env['LLVM_INDIVIDUAL_LIBS']:
				fout.write( "\\\n\t%s" % lib )
		if details.env['android_release'] == '1':
			for lib in ANDROID_RELEASE_TARGETS:
				fout.write( "\\\n\tlib%s" % lib )

		fout.write( "\n" )
        
	if details.is_shared:
		# Only shared libraries link with other libraries
		_write_android_shared_lib_details( fout, details )

	fout.write( "\nLOCAL_MODULE := %s\n" % libname )
	fout.write( "\nLOCAL_MODULE_TAGS := eng optional\n" )
	fout.write( "\nLOCAL_ARM_MODE := arm\n" )
	if details.env['android_module_owner'] != 'none':
		fout.write( "\nLOCAL_MODULE_OWNER := %s\n" % details.env['android_module_owner'])
	fout.write( "\nLOCAL_PROPRIETARY_MODULE := true\n");
	if libname == "libGLES_mali":
		if details.env['mstar'] == '1':
			fout.write( "\nLOCAL_MODULE_PATH := $(TARGET_OUT_SHARED_LIBRARIES)/egl\n" )
		else:
			fout.write( "\nLOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR_SHARED_LIBRARIES)/egl\n" )
		if details.env['cl'] == '1' and not details.env['android_release'] == '1':
			if details.env['mstar'] == '1':
				fout.write( "\nLOCAL_POST_INSTALL_CMD := $(hide) cd $(TARGET_OUT_SHARED_LIBRARIES); \\\n")
				fout.write( "\tln -sf /lib/egl/libGLES_mali.so libOpenCL.so.1.1; \\\n")
			else:
				fout.write( "\nLOCAL_POST_INSTALL_CMD := $(hide) cd $(TARGET_OUT_VENDOR_SHARED_LIBRARIES); \\\n")
				fout.write( "\tln -sf /vendor/lib/egl/libGLES_mali.so libOpenCL.so.1.1; \\\n")
			fout.write( "\tln -sf libOpenCL.so.1.1 libOpenCL.so.1; \\\n")
			fout.write( "\tln -sf libOpenCL.so.1 libOpenCL.so\n")
	if details.env['instr'] == "1" or details.env['instr'] == "2" :
		if libname == "libmali_cinstr_plugin":
			fout.write( "\nLOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR_SHARED_LIBRARIES)\n" )

	for addition in extra_lines:
		fout.write( "\n%s\n" % addition )

	if details.is_shared:
		fout.write( "\ninclude $(BUILD_SHARED_LIBRARY)\n")
	else:
		fout.write( "\ninclude $(BUILD_STATIC_LIBRARY)\n")

# ------------------------------

def _write_android_prog_details( fout, details ):
	'''
	Output details of the specified program to build
	'''
	progname = details.prog_filename

	fout.write("\n\n#--------\n# %s\n" % progname)

	if _mono_debug:
		fout.write("#\n# %s\n" %  details.env.File(details.dst_node).abspath )
		if details.is_test:
			fout.write("#\n# Test program\n")

	fout.write("\ninclude $(CLEAR_VARS)\n")
	fout.write("LOCAL_PRELINK_MODULE := false\n")

	_write_android_include_path_details( fout, details )

	_write_android_src_file_details( fout, details )

	_write_android_cflags_details( fout, details )

	_write_android_static_lib_details( fout, details )

	_write_android_shared_lib_details( fout, details )

	fout.write( "\nLOCAL_MODULE := %s\n" % progname )
	fout.write( "\nLOCAL_MODULE_TAGS := eng optional tests\n" )
	fout.write( "\nLOCAL_ARM_MODE := arm\n" )

	if details.is_test:
		fout.write( "\nLOCAL_MODULE_PATH := $(TARGET_OUT)/bin/mali_tests\n" )
		fout.write( "\ninclude $(BUILD_EXECUTABLE)\n" )
	else:
		fout.write( "\ninclude $(BUILD_EXECUTABLE)\n")

# ------------------------------

def _write_common_includes( fout, declared_libs ):
	'''
	Output include paths from all Android release targets
	'''

	fout.write( "MALI_COMMON_C_INCLUDES :=" )

	includes = []
	for lib_key in declared_libs:
		details = declared_libs[lib_key]
		if details.libname == 'mali' or details.libname in ANDROID_RELEASE_TARGETS:
			for inc in _write_android_include_path_details( fout, details, True ):
				if inc not in includes:
					includes.append(inc)

	for item in includes:
		fout.write(" \\\n\t%s" % item);

def _update_android_cflags( cflags, env ):
	if env['android_version'] == '0':
		return

	if '-O1' in cflags:
		cflags[cflags.index('-O1')] = '-O2'

	if '-mthumb' not in cflags:
		cflags.append('-mthumb')

def _write_common_cflags( fout, declared_libs, env ):
	'''
	Output cflags from all Android release targets
	'''

	fout.write( "MALI_COMMON_CFLAGS :=" )

	cflags = []
	for lib_key in declared_libs:
		details = declared_libs[lib_key]
		if (details.libname == 'mali' or details.libname in ANDROID_RELEASE_TARGETS) \
		and details.arch == env['arch']:
			for flag in _write_android_cflags_details( fout, details, True ):
				if flag not in cflags:
					cflags.append(flag)

	_update_android_cflags( cflags, env )
	for item in cflags:
		fout.write(" \\\n\t%s" % item);

def _generate_android_build_files( env, required_lib_details, required_programs_details, required_others, exclude_libmali ):
	'''
	Create the Android.mk file. Any existing file will be
	replaced.

	Note that env is always the target environment
	The boolean exclude_libmali indicates if libmali build details will be output.
	'''

	def _write_section_divider( file, heading ):
		'''
		Write out section divider with heading to Android.mk file
		'''
		head_len = len(heading)
		if head_len > 96 or (head_len / 2) > 48:
			l_str = "="
			r_str = "="
		else:
			l_len = 49 - (head_len / 2)
			l_str = "=" * l_len
			r_str = "=" * (98 - (head_len + l_len ))
		file.write( "\n\n#%s %s %s\n\n" % (l_str, heading, r_str) )

	def _generate_auto_header_str( line_prefix='#' ):
		h_str = "#" * 19
		return "%s%s\n%s AUTOGENERATED FILE\n%s%s\n" % (line_prefix, h_str, line_prefix, line_prefix, h_str)

	def _write_auto_header( file, line_prefix='#' ):
		file.write( _generate_auto_header_str( line_prefix ) )

	def _copy_slib_to_root( file, lib ):
		'''
		Add lines to Android.mk to copy specfied library file from slib to
		the Android root directory
		'''
		file.write("include $(CLEAR_VARS)\n")
		file.write("LOCAL_MODULE := %s\n" % lib)
		file.write("LOCAL_MODULE_TAGS := eng optional tests\n")
		file.write("LOCAL_MODULE_CLASS := ETC\n")
		file.write("LOCAL_MODULE_PATH := $(TARGET_ROOT_OUT)\n")
		file.write("LOCAL_SRC_FILES := %s\n" % _shorten_abs_path_name( env['STATIC_LIB_PATH'] + os.sep + lib ) )
		file.write("include $(BUILD_PREBUILT)\n\n")

	def _append_insmod( ko ):
		kernel_modules.append( "    insmod %s" % ko )

	def _append_insmod_and_slib( ko ):
		_append_insmod( ko )
		copy_slibs.append( ko )

	def _append_chmod( dev ):
		chmods.append( "    chmod 0666 %s" % dev )

	# ------

	num_progs = 0
	num_libs = 0

	build_os = env['os']
	build_arch = env['arch']

	# Flag - were no targets specified
	no_targets = not ( required_lib_details or required_programs_details or required_others )
	if int(env['kernel_modules']) == 0:
		no_targets = False
 
	# Work out if we are building amba-clcd-ump kernel module
	if env['backend'] == 'kernel' and os.path.exists(os.path.join( "pl111", "sconscript" ) ) \
	and ( "slib/amba-clcd-ump.ko" in required_others or no_targets ):
		building_pl111 = True
		if os.path.exists(os.path.join( "pl111", "vsync", "sconscript" ) ) \
		and ( "slib/amba-clcd-ump-vsync.ko" in required_others or no_targets ):
			building_pl111_vsync = True
		else:
			building_pl111_vsync = False
	else:
		building_pl111 = False

	# ------

	if env['android_egl_cfg'] == "1":
		# Create the egl.cfg file.
		fout = open( "egl.cfg", 'w' )
		_write_auto_header( fout )
		fout.write("\n0 0 android\n")

		if env['hwver'] == "none":
			# Use standard Android software GLES, when running without real hardware,
			# otherwise its too slow.
			fout.write("#0 1 mali\n\n")
		else:
			fout.write("0 1 mali\n\n")
		fout.close()

	# ------
	# Build arrays of the kernel modules and their devices

	copy_slibs = []		# kernel modules that need to be copied to target root by Android build
	kernel_modules = []	# kernel modules that need an insmod line in init.rc
	chmods = []			# devices that need a chmod in init.rc

	_append_chmod( "/dev/graphics/fb0" )

	if build_os != 'android':
		if "slib/kds.ko" in required_others or no_targets:
			_append_insmod_and_slib( "kds.ko" )

	if env['ump'] == "1":
		if "slib/ump.ko" in required_others or no_targets:
			_append_insmod_and_slib( "ump.ko" )
			_append_chmod( "/dev/ump" )

	if "slib/mali_kbase.ko" in required_others or no_targets:
		_append_insmod_and_slib( "mali_kbase.ko" )
		_append_chmod( "/dev/mali0" )

	if building_pl111:
		kernel_modules.append( "" )
		_append_insmod( "cfbfillrect.ko" )
		_append_insmod( "cfbimgblt.ko" )
		_append_insmod( "cfbcopyarea.ko" )
		_append_insmod_and_slib( "amba-clcd-ump.ko" )
		if building_pl111_vsync:
			_append_insmod_and_slib( "amba-clcd-ump-vsync.ko" )

	chmods.append( "" )

	# Create an init.rc patch file. The linux kernel will need to load
	# our mali kernel modules so the Android root/init.rc file needs to be modified.
	# This patch file will make the necessary changes.

	patch = Patch( "init.rc.patch", _generate_auto_header_str() )
	patch.start_chunk( 11 )
	patch.append_unchanged_lines( [ "loglevel 3", "" ] )
	if kernel_modules:
		kernel_modules.append( "" )
		patch.append_new_lines( kernel_modules )
	patch.append_new_lines( chmods )
	patch.append_unchanged_lines( [
					"# setup the global environment",
					"    export PATH /sbin:/vendor/bin:/system/sbin:/system/bin:/system/xbin"
					])
	patch.close()

	# ------

	# Next create the Android.mk file
	fout = open( "Android.mk", 'w' )
	_write_auto_header( fout )

	if env['android_release'] == '1':
		fout.write(ANDROID_RELEASE_HEADER)

	# Settings common to everything in the make file.
	fout.write("\nLOCAL_PATH := $(call my-dir)\n")

	# Create a makefile rule to 'derive' the trace configuration from the chosen
	# configuration file.
	cfg = env['swtrace']
	fout.write("\n$(LOCAL_PATH)/cinstr/role/mali_active_plugin_config.h : $(LOCAL_PATH)/cinstr/role/%s\n" % cfg)
	fout.write("\tcp $< $@\n")

	# If we have a custom android make file, then include it.
	if os.path.exists( "bldsys/android_arm.mk" ):
		fout.write("\ninclude $(LOCAL_PATH)/bldsys/android_arm.mk\n")

	# Add the llvm/clang make file here if CL=1
	if env['cl'] == '1':
		fout.write("\n\n#--------\n# %s\n" % 'LLVM/Clang libs')
		fout.write("\nCACHED_LOCAL_PATH := $(LOCAL_PATH)\n")
		if env['no_clang'] == '1':
			fout.write("\nMALI_CLCC_COMPILER_NO_FRONTEND := true\n")
		fout.write("include $(LOCAL_PATH)/clcc/third_party/patched/llvm/Android.mk\n");
		fout.write("LOCAL_PATH := $(CACHED_LOCAL_PATH)\n");
    
	_write_section_divider( fout, "Shared & Static Libraries" )

	if env['android_release'] == '1':
		_write_common_includes( fout, declared_libs )
		fout.write("\n\n");
		_write_common_cflags( fout, declared_libs, env )

	# Shared & Static libraries.
	for lib_key in declared_libs:
		lib_details = declared_libs[lib_key]

		# For Android release we only want libGLES_mali in Android.mk
		if env['android_release'] == '0' or lib_details.libname == 'mali' or lib_details.libname in ANDROID_RELEASE_TARGETS:
			if ( lib_details.is_shared or android_use_static_libs) \
			and lib_details.os == build_os \
			and lib_details.arch == build_arch \
			and not lib_details.is_empty \
			and not lib_details.excluded:
				_write_android_lib_details( fout, lib_details )
				num_libs += 1

	if _mono_debug:
		fout.write("\n\n# ------------------\n# Total %d libraries\n" % num_libs )

	# For Android release we only want libGLES_mali in Android.mk
	if env['android_release'] == '0':
		_write_section_divider( fout, "Tests" )

		# Test executables
		for prog_key in programs:
			details = programs[prog_key]
			if details.os == build_os \
			and details.arch == build_arch \
			and not details.excluded:
				_write_android_prog_details( fout, details )
				num_progs += 1

		if _mono_debug:
			fout.write("\n\n# ------------------\n# Total %d programs\n" % num_progs )

	_write_section_divider( fout, "Miscellaneous" )

	# Need to copy the egl.cfg into the system library also
	if env['android_egl_cfg'] == "1":
		fout.write("include $(CLEAR_VARS)\n")
		fout.write("LOCAL_MODULE := egl.cfg\n")
		fout.write("LOCAL_MODULE_TAGS := eng optional tests\n")
		fout.write("LOCAL_MODULE_CLASS := ETC\n")
		fout.write("LOCAL_MODULE_PATH := $(TARGET_OUT)/lib/egl\n")
		fout.write("LOCAL_SRC_FILES := egl.cfg\n")
		fout.write("include $(BUILD_PREBUILT)\n\n")

	# Need to copy the default.instr_config into the system library
	# TARGET_OUT points to /system but the file need to go in /data
	# 
	if env['instr'] == "1" or env['instr'] == "2" :
		fout.write("include $(CLEAR_VARS)\n")
		fout.write("LOCAL_MODULE := default.instr_config\n")
		fout.write("LOCAL_MODULE_TAGS := optional\n")
		fout.write("LOCAL_MODULE_CLASS := ETC\n")
		fout.write("LOCAL_MODULE_PATH := $(TARGET_OUT)/../data/data/com.mali.testjava\n")
		fout.write("LOCAL_SRC_FILES := cinstr/plugin/default.instr_config_android\n")
		fout.write("include $(BUILD_PREBUILT)\n\n")
		#Set the default outputdir to /data/data/com.mali.testjava 
		fout.write("$(shell sed \"s/outdir = ./outdir= \/data\/data\/com.mali.testjava/g\" $(LOCAL_PATH)/cinstr/plugin/default.instr_config > $(LOCAL_PATH)/cinstr/plugin/default.instr_config_android)\n")

	# and some linux kernel modules
	for ko in copy_slibs:
		_copy_slib_to_root( fout, ko )

	fout.write('\nMALI_LOCAL_PATH := $(LOCAL_PATH)\n')

	# Include the gralloc make file
	if env['gralloc'] == '1':
		fout.write('\nMALI_ION := 1\n')
		fout.write('include $(MALI_LOCAL_PATH)/android/gralloc/src/Android.mk\n')

	if num_progs > 0:
		# Create a soft link to the unit test folder in the target build tree.
		os.system( "rm -f unit" )
		os.system( "ln -s %s unit" % os.path.join( _shorten_abs_path_name( env['BUILD_DIR_PATH'] ), "unit" ) )

		# Create a soft link to the slib folder in the target build tree.
		os.system( "rm -f libs" )
		os.system( "ln -s %s libs" % os.path.join( _shorten_abs_path_name( env['BUILD_DIR_PATH'] ), "slib" ) )

		# If we have built the performance tests, then then create a soft link to the perf folder
		# in the target build tree.
		os.system( "rm -f perf" )
		if env['perf'] == '1':
			os.system( "ln -s %s perf" % os.path.join( _shorten_abs_path_name( env['BUILD_DIR_PATH'] ), "perf" ) )

	# close the Android.mk file
	if env['android_release'] == '1':
		fout.write(ANDROID_RELEASE_FOOTER)

	fout.write("\n")
	fout.close()

	# ------

	if num_progs > 0:

		# Create a tpi specific Android.mk file for external system tests
	
		fout = open( "tpi_Android.mk", 'w' )
		_write_auto_header( fout )
	
		# Settings common to everything in the make file.
		fout.write("\nLOCAL_PATH := $(call my-dir)\n")
	
		# If we have a custom android make file, then include it.
		if os.path.exists( "bldsys/android_arm.mk" ):
			fout.write("\ninclude $(LOCAL_PATH)/bldsys/android_arm.mk\n")
	
		_write_section_divider( fout, "Static TPI Libraries" )

		for lib_key in declared_libs:
			lib_details = declared_libs[lib_key]
			if lib_details.libname == "tpi" or lib_details.libname == "tpi_egl":
				_write_android_lib_details( fout, lib_details )
	
		# close the tpi_Android.mk file
		fout.write("\n")
		fout.close()

# ------------------------------

def disable_building_all_targets( target_env ):
	"""
	Prevent SCons from actually building any of the object, library & program files
	for the target system.

	Note that this does not prevent the build system building mock 'C' and 'H' files
	for the target or tools & shaders for the host.

	This only works if the target and host os, or architecture are different
	"""

	global declared_object_nodes, declared_libs, programs

	target_os = target_env['os']
	target_arch = target_env['arch']

	# All object target files
	for obj_node in declared_object_nodes:
		# Is this an object file for the target system
		if declared_object_nodes[obj_node].env['os'] == target_os \
		and declared_object_nodes[obj_node].env['arch'] == target_arch :
			# reset the executor
			obj_node.get_executor(create=0).nullify()

	# All library target files
	for abs_lib in declared_libs:
		# Is this a library file for the target system
		if declared_libs[abs_lib].env['os'] == target_os \
		and declared_libs[abs_lib].env['arch'] == target_arch :
			# reset the executor
			declared_libs[ abs_lib ].dst_node.get_executor(create=0).nullify()

	# All program target files
	for prog_key in programs:
		# Is this a program file for the target system
		if programs[ prog_key ].env['os'] == target_os \
		and programs[ prog_key ].env['arch'] == target_arch :
			# reset the executor
			programs[ prog_key ].dst_node.get_executor(create=0).nullify()

# ------------------------------

def _mark_as_required(details):
	'''
	Recursive function to set the specified library/program and all the internal libraries
	it depends on as being required for libmali and/or the Android make file. 
	'''

	# No need to do anything if its already been updated
	if details.excluded:

		# It is currently excluded - it is now required.
		details.excluded = False

		# Call _flag_node_as_required() recursively for each of
		# the static libraries this library/program is dependent on.
		for lib in details.internal_static_libs:
			_mark_as_required( lib )

		# Call _flag_node_as_required() recursively for each of
		# the shared libraries this library/program is dependent on.
		for lib in details.internal_shared_libs:
			_mark_as_required( lib )

# ------------------------------

def _identify_derived_build_targets( env ):
	'''
	Identify any build targets specified on the command line.
	
	Returns three sets of the specified targets:
		library details
		program details
		other files
	'''

	def _identify_build_targets( node ):
		'''
		Process the source files of the specified node, adding the details of any which are
		known libraries or programs to required_lib_details[] and required_programs_details[].
		Any other targets are added to required_others[].
		
		If a source file of the node has been derived but it is not in the build tree, then
		recursivley process the files from which the source file is derived. 
		'''
		for src in node.sources:
			# We are looking for built libraries and programs so any file which
			# hasn't been derived can be ignored.
			if src.is_derived():
				key = env.File(src).abspath
				if key.startswith( env['BUILD_DIR_PATH'] ):
					if key in declared_libs:
						required_lib_details.add( declared_libs[key] )
					elif key in programs:
						required_programs_details.add( programs[key] )
					else:
						required_others.add( _shorten_abs_build_path_name( env, key ) )
				else:
					# We know it is derived but its not in the build tree.
					# Process any source files from which this file is derived (recursively)
					_identify_build_targets( src )

	build_targets = SCons.Script.BUILD_TARGETS

	required_lib_details = set()
	required_programs_details = set()
	required_others = set()

	if build_targets:
		# Specific build targets were specified

		nodes = env.arg2nodes( build_targets )

		if nodes:
			# By default none are excluded.

			# Create a list of the target build libraries and programs (and others) from the list of target aliases
			# The order isn't important so we use sets.

			for node in nodes:
				if isinstance( node, SCons.Node.Alias.Alias ):
					_identify_build_targets( node )

	return required_lib_details, required_programs_details, required_others

# ------------------------------


# The CL, EGL, and GLES builds require a large number of other module libraries.
# The following lists record just the key libraries that differentiate the
# CL, EGL, and GLES libraries. The GLES builds will also require EGL
# to be built and these lists also ensure that happens when build targets are
# specified.

COMMON_KEY_STATIC_LIBS = [ 'cutils_cstr', 'egl', 'egl_winsys' ]
KEY_STATIC_EGL_LIBS = COMMON_KEY_STATIC_LIBS + [ 'egl_core', 'egl_color_buffer' ]

COMMON_KEY_STATIC_GLES_LIBS = COMMON_KEY_STATIC_LIBS + [ 'gles', 'shadergen' ]
KEY_STATIC_GLES1_LIBS = COMMON_KEY_STATIC_GLES_LIBS + [ 'cutils_math' ]
KEY_STATIC_GLES2_LIBS = COMMON_KEY_STATIC_GLES_LIBS

KEY_STATIC_CL_LIBS = [ 'cutils_cstr', 'opencl', 'shadergen' ]

ALL_KEY_STATIC_LIBS = list( \
				set(KEY_STATIC_EGL_LIBS) \
				| set(KEY_STATIC_GLES1_LIBS) \
				| set(KEY_STATIC_GLES2_LIBS) \
				| set(KEY_STATIC_CL_LIBS) \
				)

def _mark_build_targets( env, required_lib_details, required_programs_details ):
	'''
	Marks the libraries and executables that should be specified in the Android make file
	and the libraries that should be included in libmali.

	If any targets were specified on the build command line then only those programs
	and libraries and the objects they depend on should be specified in the Android make
	file. Likewise for libmali.
	
	The program and library records have a flag that indicates if they should be excluded.
	This flag defaults to False hence when no specific targets are specified on the build
	command line, then all are eligible to be specified in the Android make file (and in
	libmali) and this function does nothing.
	
	IMPORTANT. If unit=1 and a target with unit tests is specified on the build command line,
	then any static libraries those unit tests require will be marked as eligible to go into
	libmali even if the tests don't link in libmali. This is an unfortunate side effect of the
	way libmali is built. Builds with unit=0 should produce a libmali with minimal contents.
	'''

	def _set_public_static_libs_exclude( libnames_list, exclude ):
		'''
		Set the exclude field of each public static library in the libnames_list to
		the (boolean) value of exclude specified.
		'''
		for libname in libnames_list:
			static_lib_name = _full_static_lib_filename( env, libname )
			lib_key = os.path.join( env['STATIC_LIB_PATH'], static_lib_name )
			if lib_key in declared_libs:
				declared_libs[lib_key].excluded = exclude

	def _clear_shared_libs_exclude( libnames_list ):
		'''
		Clear the exclude field of the specified shared libraries.
		'''
		for libname in libnames_list:
			shared_lib_name = _full_shared_lib_filename( env, libname )
			lib_key = os.path.join( env['SHARED_LIB_PATH'] , shared_lib_name )
			if lib_key in declared_libs:
				declared_libs[lib_key].excluded = False


	# Were any known library or program targets specified.
	# Note that libmali hasn't been declared yet so it can never be
	# a known target. Consequently if libmali is the only target specified
	# then the following if statement fails resulting in nothing being excluded
	# and hence everything will be built (which is the most desirable result).
	if required_lib_details or required_programs_details:

		# Initially exclude all programs
		for prog_key in programs:
			programs[prog_key].excluded = True

		# Create a list of required public static & shared library names
		required_libs = [ ld.libname for ld in required_lib_details if ld.is_public_static or ld.is_shared ]

		if 'gles' not in required_libs \
		and 'GLESv2' not in required_libs \
		and 'GLESv1_CM' not in required_libs \
		and 'opencl' not in required_libs \
		and 'OpenCL' not in required_libs \
		and 'egl' not in required_libs:
			# Initially exclude all shared and static libraries
			for lib_key in declared_libs:
				declared_libs[lib_key].excluded = True
		else:
			# The majority of public static libraries will be required.
			# Initially exclude all shared libraries and all non-public static libraries
			for lib_key in declared_libs:
				if not declared_libs[lib_key].is_public_static :
					declared_libs[lib_key].excluded = True

			# and initially exclude all the libraries that we know shouldn't go in libmali.
			# Some of these libraries will subsequently have their exlude flag cleared as
			# they will be required by programs, but they all will remain exluded from libmali.
			_set_public_static_libs_exclude( LIBMALI_EXCLUDED_STATIC_LIBS, exclude = True )

			# and exclude the CL, EGL, GLES1 & GLES2 key public static libraries
			_set_public_static_libs_exclude( ALL_KEY_STATIC_LIBS, exclude = True )

		# Now process the build targets, clearing the exclude flags as appropriate

		# First the programs and the libraries they require (recursively)
		for prog in required_programs_details:
			#print "Marking: " + prog.prog_filename
			_mark_as_required( prog )

		# Now the libraries (recursively)
		for lib in required_lib_details:
			#print "Marking: " + lib.libname
			_mark_as_required( lib )

		# Finally we have to clear the exclude flag of certain other key libraries
		# required by CL, GLES1 & GLES2 if those targets were specified. Build
		# up two sets of the libs that need their exclude flag to be cleared.

		static_libs_clear_set = set()
		shared_libs_clear_set = set()

		if 'opencl' in required_libs:
			static_libs_clear_set.update( KEY_STATIC_CL_LIBS )
			if env['CPPDEFINES']['MALI_CL_KHR_EGL_IMAGE'] == '1' or env['CPPDEFINES']['MALI_CL_KHR_GL_SHARING'] == '1':
				static_libs_clear_set.update( KEY_STATIC_GLES1_LIBS )
				static_libs_clear_set.update( KEY_STATIC_EGL_LIBS )
				shared_libs_clear_set.add( 'EGL' )

		if 'OpenCL' in required_libs:
			static_libs_clear_set.update( KEY_STATIC_CL_LIBS )
			if env['CPPDEFINES']['MALI_CL_KHR_EGL_IMAGE'] == '1' or env['CPPDEFINES']['MALI_CL_KHR_GL_SHARING'] == '1':
				static_libs_clear_set.update( KEY_STATIC_GLES1_LIBS )
				static_libs_clear_set.update( KEY_STATIC_EGL_LIBS )
				shared_libs_clear_set.add( 'EGL' )

		if 'egl' in required_libs:
			static_libs_clear_set.update( KEY_STATIC_EGL_LIBS )
			shared_libs_clear_set.add( 'EGL' )

		if ('gles' in required_libs and env['gles'] == '1') or 'GLESv1_CM' in required_libs or 'GLESv2' in required_libs:
			static_libs_clear_set.update( KEY_STATIC_GLES1_LIBS )
			static_libs_clear_set.update( KEY_STATIC_GLES2_LIBS )
			shared_libs_clear_set.add( 'EGL' )
			shared_libs_clear_set.add( 'GLESv1_CM' )
			shared_libs_clear_set.add( 'GLESv2' )

		if len(static_libs_clear_set):
			_set_public_static_libs_exclude( list(static_libs_clear_set), exclude = False )
		if len(shared_libs_clear_set):
			_clear_shared_libs_exclude( shared_libs_clear_set )

# ------------------------------

def post_script_processing( env ):
	'''
	This is the main entry point into the monolithic build system. Its called by
	the bldsys sconstruct after all the sconscripts have been processed.

	Note that env is always the target environment
	'''

	global _mono_debug, project_directory, build_android

	disable_building_targets = False

	if env['os'] == 'android':
		# Generating Android build files.
		build_android = True
		# No need to actually build the target objects, libraries & test executables.
		disable_building_targets = True

	# Set project_directory
	project_directory = os.getcwd()

	# And work out short build directory path
	abs_build_directory = env['BUILD_DIR_PATH']

	#print "\n - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n"

	# The _mono_debug flag can be set to True as appropriate to aid debugging an individual
	# section of the mechanism (which is usually far more useful then enabling debug of them all)
	_mono_debug = False

	# First we update the records of all the objects. This doesn't make any change to
	# how the objects are built.
	_process_objects()

	_mono_debug = False

	# Next copy the key env vars into each library record
	for abs_lib in declared_libs:
		declared_libs[ abs_lib ]._copy_key_env_details()

	# Next we update the records of the static libraries. This doesn't make any change to
	# how the static libraries are built.
	_process_static_libraries()

	_mono_debug = False

	# Then we fix up the dependencies of the shared libraries declaring them to be dependent on any
	# static libraries or objects they link against and change the LIBS of each library so it specifies
	# the nodes of the required internal static libraries and objects.
	_process_shared_libraries()

	_mono_debug = False

	# Process all the program Nodes. Each program executable is linked against the
	# object nodes of the static libraries it requires and the LIBS of each program is changed
	# to no longer specify the internal static libraries, and to specify the names of any
	# internal or external shared libraries.
	_process_programs()

	# Identify all derived targets (if any) that were specified on the command line.
	required_lib_details, required_programs_details, required_others = _identify_derived_build_targets( env )

	# Update list of static libraries that should be excluded from libmali
	_update_libmali_excludes(env)

	_mono_debug = False

	# Mark the specified targets and the libraries/objects that they are built from such
	# that they are eligable to be included in libmali (and the Android make file). All
	# the other libraries/objects will be excluded.
	_mark_build_targets( env, required_lib_details, required_programs_details )

	_mono_debug = False

	# Next we need to declare the Mali shared library build rule. The library is built from the 
	# nodes of the objects that go into the (majority of the) static libraries in STATIC_LIB_PATH
	libmali_details = _build_shared_mali_library( env )

	_mono_debug = False

	# Update all shared libraries and programs that link with libmali - set them to be dependent
	# on libmali.
	_update_libmali_users( libmali_details )

	_mono_debug = False

	if build_android:
		# Generate extra build data required for Android builds for each library and program
		# The order is important as shared libraries and programs get data from the static
		# libraries they are built from.
		_add_android_build_data_to_static_libs( env )
		_add_android_build_data_to_shared_libs( env )
		_add_android_build_data_to_programs( env )

		# Generate the android make file		
		_generate_android_build_files( env, required_lib_details, required_programs_details, required_others, exclude_libmali = libmali_details.excluded )

	_mono_debug = False

	if disable_building_targets:
		# This is only possible if the target and host os or architecture are different
		# Tell SCons not to build any of the target files (objects, libraries, programs).
		# Note that host files will still be built.
		disable_building_all_targets( env )

	#print "\n - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n"

