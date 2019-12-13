# -----------------------------------------------------------------------------
# This confidential and proprietary software may be used only as
# authorised by a licensing agreement from ARM Limited
# (C) COPYRIGHT 2010-2013 ARM Limited
# ALL RIGHTS RESERVED
# The entire notice above must be reproduced on all authorised
# copies and copies may only be made to the extent permitted
# by a licensing agreement from ARM Limited.
# -----------------------------------------------------------------------------

import sys, os, commands, subprocess, re
from utils import info_print

# =============================================================================
def get_appropriate_toolchain( sEnv ):

	if( sEnv['os'] in ['linux', 'osx', 'android'] ):

		dOptimize = dict()
		if( sEnv['arch'].find('x86')  != -1):
			info_print(sEnv, "Configure build toolchain Gnu.\n")
			return _ToolchainGnu( sEnv['arch'], sEnv['os'], dOptimize )

		elif( sEnv['arch'].find('arm') != -1):

			if( sEnv['toolchain']  == 'rvct'):
				info_print(sEnv, "Configure build toolchain RVCT.\n")
				return _ToolchainRVCT( sEnv['arch'], sEnv['os'], dOptimize )

			info_print(sEnv, "Configure build toolchain Gnu.\n")
			if( 'core' in sEnv ):
				dOptimize['core'] = sEnv['core']
			return _ToolchainGnu( sEnv['arch'], sEnv['os'], dOptimize )
		else:
			sys.stderr.write('*** "%s" is not supported"\n' % (sEnv['arch']))
			exit(1)

	else:
		sys.stderr.write('*** "%s" is not supported"\n' % (sEnv['os']))
		exit(1)

# =============================================================================

class _ToolchainBase( object ):

	"""
	This class implements a base class from which other toolchains are derived.
	"""

	# -------------------------------------------------------------------------
	def __init__( self, rTargetArch, dOptimize=dict() ):

		self.rTargetArch = rTargetArch
		self.lCFlags     = list()
		self.lCCFlags    = list()
		self.lCXXFlags   = list()
		self.lASFlags    = list()
		self.lLDFlags    = list()
		self.lSHCCFlags  = list()
		self.dOptimize   = dOptimize

	# -------------------------------------------------------------------------
	def update_environment( self, sEnv ):

		sEnv.Append( CFLAGS     = self.lCFlags )
		sEnv.Append( CCFLAGS    = self.lCCFlags )
		sEnv.Append( SHCCFLAGS  = self.lSHCCFlags )
		sEnv.Append( CXXFLAGS   = self.lCXXFlags )
		sEnv.Append( LINKFLAGS  = self.lLDFlags )
		sEnv.Append( ASFLAGS    = self.lASFlags )
		if( 'MALI_GCC_WORKAROUND_MIDCOM_4598' not in sEnv['CPPDEFINES'] ):
			sEnv.Append( CPPDEFINES = {'MALI_GCC_WORKAROUND_MIDCOM_4598' : '0'} )

# =============================================================================
class _ToolchainGnu( _ToolchainBase ):

	"""
	This class implements an instance of the toolchain for Gnu.
	"""

	# -------------------------------------------------------------------------
	def __init__( self, rTargetArch, rTargetOS, dOptimize=dict() ):

		_ToolchainBase.__init__(self, rTargetArch, dOptimize)

		self.dTargetArch = {
			'arm_v7'   : '-march=armv7-a',
			'arm_v8'   : '-march=armv8-a',
			'x86_32'   : '-m32',
			'x86_64'   : '-m64'
		}

		self.lCCFlags.append( self.dTargetArch[rTargetArch] )
		self.lASFlags.append( self.dTargetArch[rTargetArch] )
		self.lLDFlags.append( self.dTargetArch[rTargetArch] )

		self.lCFlags.append( '-std=c89' )
		self.lCFlags.append( '-Wstrict-prototypes' )

		self.lCCFlags.append( '-pedantic' )
		self.lCCFlags.append( '-pthread' )
		self.lCCFlags.append( '-Wall' )
		self.lCCFlags.append( '-Wno-long-long' )
		self.lCCFlags.append( '-Wno-variadic-macros' )
		self.lCCFlags.append( '-Wno-overlength-strings' )
		self.lCCFlags.append( '-Wno-unused' )
		self.lCCFlags.append( '-Wno-attributes' )

		if( "x86_64" == rTargetArch ):
			self.lCCFlags.append( '-Werror=implicit-int' )
			self.lCCFlags.append( '-Werror=implicit-function-declaration' )
		else:
			self.lCCFlags.append( '-Werror' )

		# Force march to new core to enable atomic builtins
		if( "x86_32" == rTargetArch ):
			self.lCCFlags.append( '-march=pentium' )

		self.lCXXFlags.append( '-fno-rtti' )

		self.lLDFlags.append( '-pthread' )
		self.lLDFlags.append( '-Wl,--allow-shlib-undefined' )

		self.rCC     = 'gcc'
		self.rAS     = 'as'
		self.rAR     = 'ar'
		self.rCXX    = 'g++'
		self.rRANLIB = 'ranlib'
		self.rTOOLPREFIX = ''
		self.rTargetArch = rTargetArch
		self.rTargetOS   = rTargetOS

		# we should keep these defaults to match what most people use
		if ( self.rTargetArch == 'arm_v7' ):
			self.set_toolchain_prefix( 'arm-none-linux-gnueabi-' );
		elif ( self.rTargetArch == 'arm_v8' ):
			self.set_toolchain_prefix( 'aarch64-linux-gnu-' );

	def configure_linker( self, sEnv ):
		# for x86 builds query the host CXX linker for the path to the libstdc++ shared library
		if( "x86_32" == self.rTargetArch or "x86_64" == self.rTargetArch ):
			command_string = 'g++ -print-file-name=libstdc++.so ' + self.dTargetArch[self.rTargetArch]
			# MIDCL-1885 - need to get the path at the end of the output.  Tools such as 
			# BullsEyeCoverage intercept prefix the output with a copyright string.
			output = os.path.dirname(os.path.abspath(commands.getoutput(command_string))).split()
			libstdcxx_dir = output[len(output)-1]
			self.lLDFlags.append('-L'+libstdcxx_dir )
			self.lLDFlags.append('-Wl,-R,'+libstdcxx_dir)
			ld_path="/usr/bin/ld"

		if ( self.rTargetArch.find('arm') != -1):
			ld_path=self.rTOOLPREFIX + 'ld'

		# Determine linker parameters needed by local ld
		sProcessHandle = subprocess.Popen(["%s --version" % ld_path], shell=True, stderr=subprocess.PIPE, stdout=subprocess.PIPE)
		rLD_stdout = sProcessHandle.stdout.readline()
		sProcessHandle.stdout.read()
		sProcessHandle.wait()
		if 0 == sProcessHandle.returncode:
			use_NO_AS_NEEDED = 0
			use_COPY_DT_NEEDED = 0

			if re.search( '(2\.20)|(2\.21)', rLD_stdout):
				use_NO_AS_NEEDED = 1

			if re.search( '(2\.22)|(2\.23)', rLD_stdout):
				use_NO_AS_NEEDED = 1
				use_COPY_DT_NEEDED = 1

			if use_NO_AS_NEEDED:
				self.lLDFlags.append('-Wl,--no-as-needed')
			if use_COPY_DT_NEEDED:
				# in 2.22 --no-copy-dt-needed-entries became the default
				self.lLDFlags.append('-Wl,--copy-dt-needed-entries')

	# -------------------------------------------------------------------------
	def set_debug_symbol_flags( self, rSymbols ):

		if( rSymbols == '1'):
			self.lCCFlags.append( '-g' )
			self.lCCFlags.append( '-fno-omit-frame-pointer' )
		else:
			self.lCCFlags.append( '-fomit-frame-pointer' )

	# -------------------------------------------------------------------------
	def set_optimization_flags( self, sEnv ):

		if ( self.rTargetArch == 'arm_v7'):
			# Optimize for specific core, as specified in the profile:  
			#  - by default for A9, 
			#  - could also be A15.
			rCore = 'cortex-a9'
			if( 'core' in self.dOptimize ):
				rCore = self.dOptimize['core']
			self.lCCFlags.append( '-mtune=%s' % rCore )

			self.lCCFlags.append( '-mfloat-abi=' + sEnv['floatabi'] )
			self.lCCFlags.append( '-fno-math-errno' )
			self.lCCFlags.append( '-fno-signed-zeros' )

			if( int( sEnv["simd"] ) ):
				self.lCCFlags.append( '-mfpu=neon' )
				self.lASFlags.append( '-mfpu=neon' )
			else:
				self.lCCFlags.append( '-mfpu=vfpv3' )
				self.lASFlags.append( '-mfpu=vfpv3' )

		if ('1' == sEnv['optimize'] ):

			if( sEnv['flavour'] == 'small' ):
				# Note: Older versions (2009Q3) generate incorrect code for
				# Cortex-A9 with -Os -mthumb. This works around that problem
				# by compiling with lower optimization levels until the tool-chain
				# has been updated everywhere. See MIDARC-380.
				self.lCCFlags.append( '-O1' )

			if( sEnv['flavour'] == 'fast' ):
				self.lCCFlags.append( '-O3' )
				self.lCCFlags.append( '-Wno-array-bounds' )
		else:
			self.lCCFlags.append( '-O0' )

		if ( self.rTargetArch == 'arm_v7'):
			if ('1' == sEnv['thumb'] ):
				self.lCCFlags.append( '-mthumb-interwork' )
				self.lCCFlags.append( '-mthumb' )
			else:
				if( sEnv['flavour'] == 'fast' ):
					self.lCCFlags.append( '-mno-thumb' )

		if sEnv['debug'] == '1':
			self.lSHCCFlags.append( '-fvisibility=default' )
		else:
			self.lSHCCFlags.append( '-fvisibility=hidden' )

	# -------------------------------------------------------------------------
	def set_toolchain_prefix( self, rPrefix):

		if ( (self.rTargetArch.find('arm') != -1) and (rPrefix == '') ):
			return

		self.rCC     = rPrefix + 'gcc'
		self.rAS     = rPrefix + 'as'
		self.rAR     = rPrefix + 'ar'
		self.rCXX    = rPrefix + 'g++'
		self.rRANLIB = rPrefix + 'ranlib'
		self.rTOOLPREFIX = rPrefix

	# -------------------------------------------------------------------------
	def update_environment( self, sEnv ):

		sEnv.Replace( CC = self.rCC, CXX = self.rCXX, AS = self.rAS, AR = self.rAR, RANLIB = self.rRANLIB, TOOLPREFIX=self.rTOOLPREFIX )

		if sEnv['ccache'] == '1':
			sEnv.Replace(CC = 'ccache ' + sEnv['CC'], CXX = 'ccache ' + sEnv['CXX'])

		# GCC 4.7 on AOSP 4.3 contains a defect which didn't allow us to use the "_Pragma" syntax, where an error message 
		# is returned:   
		#
		# "internal compiler error: unspellable token PRAGMA"  
		# 
		# This regression has thus far only been seen on the GCC 4.7 compiler bundled with AOSP 4.3.0.  GCC 4.7 
		# issue tracked at MIDCOM-4598.  
		# 
		# The workaround entails: 
		# - Overriding the global Android GCC 'Werror=address' flag, by adding the GCC flag 'Wno-address' to 
		#   local C flags in the Android makefile for the DDK, as is done below.  
		# - Conditionally compiled use of "_Pragma("..."), so it is _not_ compiled if the workaround is active.  
		# 
		# The C code workarounds have a GCC version range bigger than just v4.7, because of a pre-existing workaround, 
		# presumably to a similar defect.  
		if sEnv['debug'] == '1' and sEnv['os'] == 'android' and sEnv['android_version'] == 'jbmr2':
			self.lCCFlags.append( '-Wno-address' )
			sEnv.Append( CPPDEFINES = {'MALI_GCC_WORKAROUND_MIDCOM_4598' : '1'} )

		if ( 'linux' == self.rTargetOS ) :
			sEnv['_LIBFLAGS'] = sEnv['_LIBFLAGS'] + ' -Wl,--gc-sections -Wl,--no-undefined -Wl,-rpath-link,$SHARED_LIB_PATH'

		_ToolchainBase.update_environment( self, sEnv )

# =============================================================================
class _ToolchainRVCT( _ToolchainBase ):

	"""
	This class implements an instance of the toolchain for RVCT.
	"""

	def __init__( self, rTargetArch, rTargetOS, dOptimize=dict() ):
		_ToolchainBase.__init__(self, rTargetArch, dOptimize)

		# Call armcc in GCC compatability mode:
		#  --arm_linux_config_file Use GNU standard libraries.  Uses a configuration
		#                          that points to a Code Sourcery install in /arm.
		#  --translate_gcc         Translate arguments from their GCC counterparts.
		#  --fpmode=ieee_full      Enable the rounding mode to be set.
		self.rCC     = 'armcc --arm_linux_config_file=$ARMCC_CONFIG --translate_gcc -Warmcc,--fpmode=ieee_full'
		# This is presumably not used as the config file is not set.
		self.rCXX    = 'armcc --translate_g++'
		# RVCT does not provide RANLIB, use ar directly.
		self.rRANLIB = '$AR -s'
		self.rTargetArch = rTargetArch
		self.rTargetOS   = rTargetOS
		self.dTargetArch = {
			'arm_v7'   : '-march=armv7-a',
			'x86_32'   : '-m32',
			'x86_64'   : '-m64'
		}

		self.lCCFlags.append( self.dTargetArch[rTargetArch] )
		self.lLDFlags.append( self.dTargetArch[rTargetArch] )

	def set_debug_symbol_flags( self, rSymbols ):
		if( rSymbols == '1'):
			self.lCCFlags.append( '-g' )

	def set_optimization_flags( self, sEnv ):
		if ('1' == sEnv['optimize'] ):
			self.lCCFlags.append( '-O2' )
	
	def update_environment( self, sEnv ):
		sEnv['ARMCC_CONFIG'] = sEnv.File('#/bldsys/armcc_configuration')
		sEnv['AR'] = sEnv.File('#/bldsys/armar-wrapper')

		sEnv.Replace( CC = self.rCC, CXX = self.rCXX, RANLIB = self.rRANLIB)

		if sEnv['ccache'] == '1':
			sEnv.Replace(CC = 'ccache ' + sEnv['CC'], CXX = 'ccache ' + sEnv['CXX'])

		if ( 'linux' == self.rTargetOS ) :
			sEnv['_LIBFLAGS'] = sEnv['_LIBFLAGS'] + ' -Wl,--gc-sections -Wl,--no-undefined -Wl,-rpath-link,$SHARED_LIB_PATH'

		_ToolchainBase.update_environment( self, sEnv )

	def set_toolchain_prefix( self, rPrefix):
		# For now do not allow a prefix
		print "Ignoring prefix {0} for RVCT".format(rPrefix)
	
	def configure_linker ( self, sEnv ):
		pass

