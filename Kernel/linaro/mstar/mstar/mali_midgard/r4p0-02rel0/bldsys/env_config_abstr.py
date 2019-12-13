# -----------------------------------------------------------------------------
# This confidential and proprietary software may be used only as
# authorised by a licensing agreement from ARM Limited
# (C) COPYRIGHT 2010-2011, 2013 ARM Limited
# ALL RIGHTS RESERVED
# The entire notice above must be reproduced on all authorised
# copies and copies may only be made to the extent permitted
# by a licensing agreement from ARM Limited.
# -----------------------------------------------------------------------------
__docformat__ = "restructuredtext en" 

import os, sys, subprocess

from os_abstr import get_appropriate_os, get_build_os
from toolchain_abstr import get_appropriate_toolchain
from platform_abstr  import get_appropriate_platform, get_build_platform

# =============================================================================

class BaseEnvironmentConfig( object ):
	
	"""
	This class implements a base class from which other environments are derived.
	"""

	# -------------------------------------------------------------------------
	def __init__( self ):

		self.rBuildDir = ""

	# -------------------------------------------------------------------------
	def construct_variant_name( self, sEnv, lSupportedOptions ):
		'''
		Sets ``self.rBuildDir`` to the name (str) of the current variant.

		Consider only the options marked as "build" in the list of supported
		options.  When a build tOptions is found, check the environment to see
		if it is present.  If it is present, add it & its value to the list of
		atoms. Once all supported options have been checked, assemble the
		variant name from the list of atoms.

		:Parameters:
			sEnv : `SConcsEnvironment`
				SCons state
			lSupportedOptions : list
				Sequence of::

					(str,
						(str, str, int, callable, callable)
					)
		'''
		from zlib import crc32

		lAtoms = []
		for rOpt, tOptions in sorted( lSupportedOptions ):
			if rOpt != "build":
				# This option doesn't affect build output
				continue

			rFlag = tOptions[0]
			if sEnv.has_key(rFlag):
				lAtoms.append( '%s=%s' % (rFlag, sEnv[rFlag]) )

		rVariant = "\n".join( lAtoms )
		rBuildDirHash = "%x" % abs( crc32(rVariant) )

		self.rBuildDir = os.path.join( 'build', rBuildDirHash )

		if not os.path.exists( 'build' ):
			os.makedirs( 'build' )

		rVariantFile = str("%s.variant.txt" % self.rBuildDir,)

		with file(rVariantFile, "w") as sFH:
			sFH.write( rVariant )
			sFH.write( "\n" )

	# -------------------------------------------------------------------------
	def update_environment( self, sEnv, rSrcDir ):

		rStaticLibPath = os.path.join( os.getcwd(), os.path.join( self.rBuildDir, 'slib' ) )
		sEnv.Append( STATIC_LIB_PATH=rStaticLibPath )
		
		rSharedLibPath = os.path.join( os.getcwd(), os.path.join( self.rBuildDir, 'dlib' ) )
		sEnv.Append( SHARED_LIB_PATH=rSharedLibPath )
		
		rUnitTestPath = os.path.join( os.getcwd(), os.path.join( self.rBuildDir, 'unit' ) )
		sEnv.Append( UNITTEST_PROGS_PATH=rUnitTestPath )
		
		rPerfTestPath = os.path.join( os.getcwd(), os.path.join( self.rBuildDir, 'perf' ) )
		sEnv.Append( PERFTEST_PROGS_PATH=rPerfTestPath )

		rBuildDirPath = os.path.join( os.getcwd(), self.rBuildDir )
		sEnv.Append( BUILD_DIR_PATH=rBuildDirPath )

		if sEnv['os'] == 'android':
			sEnv['ANDROID_PREBUILT_DIR'] = os.path.join(sEnv.GetLaunchDir(), 'android_prebuilt')
	
	# -------------------------------------------------------------------------
	def apply_os_settings(self, sEnv):
		sOS = get_appropriate_os( sEnv )
		if sEnv['winsys'] == 'multi':
			sOS.set_windowing_systems( sEnv )
		else:
			sOS.set_windowing_system( sEnv['winsys'] )
		sOS.update_environment( sEnv )

	# -------------------------------------------------------------------------
	def apply_platform_settings(self, sEnv):
		sPlatform = get_appropriate_platform( sEnv )
		sPlatform.update_environment( sEnv )

	# -------------------------------------------------------------------------
	def apply_toolchain_settings(self, sEnv):
		
		sToolchain = get_appropriate_toolchain( sEnv )
		if isinstance(self, HostEnvironmentConfig) == True:
			sToolchain.set_toolchain_prefix( sEnv['toolchain_prefix_host'] )
		if isinstance(self, TargetEnvironmentConfig) == True:
			sToolchain.set_toolchain_prefix( sEnv['toolchain_prefix_target'] )
		sToolchain.set_debug_symbol_flags( sEnv['symbols'] )
		sToolchain.set_optimization_flags( sEnv )
		if sEnv.has_key( 'exclude' ) != False:
			sToolchain.set_exclude_flags( sEnv['exclude'] )

		# Check the GCC version. We don't use the cross-compiler to build target user-side
		# code on Android builds as we get the Android tree to do the build using its own tools,
		# consequently we only check the version of GCC for linux and host side Android.
		if isinstance(self, HostEnvironmentConfig) == True or "linux" == sEnv['os']:
			sProcessHandle = subprocess.Popen(["%s -dumpversion" % sToolchain.rCC ], shell=True, stderr=subprocess.PIPE, stdout=subprocess.PIPE)
			rGCC_stdout = sProcessHandle.stdout.readline()
			sProcessHandle.stdout.read()
			sProcessHandle.wait()
			if 0 == sProcessHandle.returncode:
				lTokens = rGCC_stdout.split(".")
				iMajor = int(lTokens[0])
				iMinor = int(lTokens[1])
				if ((iMajor < 4) or ((iMajor == 4) and (iMinor < 7))):
					sys.stderr.write( "bldsys warning: Versions of gcc older than 4.7.x for %s have been deprecated.\n" % sToolchain.rTargetArch )

		sToolchain.configure_linker( sEnv )
		sToolchain.update_environment( sEnv )

# =============================================================================
class HostEnvironmentConfig( BaseEnvironmentConfig):

	"""
	This class implements an instance of the environment abstraction for the host.
	"""

	# -------------------------------------------------------------------------
	def __init__( self ):

		BaseEnvironmentConfig.__init__(self)

	# -------------------------------------------------------------------------
	def add_build_dir_suffix( self ):
		
		self.rBuildDir = self.rBuildDir +'-host' 		

	# -------------------------------------------------------------------------
	def apply_os_settings(self, sEnv):
		rBuildOS = get_build_os()
		sEnv['os'] = rBuildOS
		BaseEnvironmentConfig.apply_os_settings( self, sEnv )
		
	# -------------------------------------------------------------------------
	def apply_platform_settings(self, sEnv):
		rBuildArch = get_build_platform()
		sEnv['arch'] = rBuildArch
		BaseEnvironmentConfig.apply_platform_settings( self, sEnv )

# =============================================================================
class TargetEnvironmentConfig( BaseEnvironmentConfig ):

	"""
	This class implements an instance of the environment abstraction for the target.
	"""

	# -------------------------------------------------------------------------
	def __init__( self ):

		BaseEnvironmentConfig.__init__(self)

	# -------------------------------------------------------------------------
	def add_build_dir_suffix( self ):
		
		self.rBuildDir = self.rBuildDir + '-target' 		
