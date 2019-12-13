# -----------------------------------------------------------------------------
# This confidential and proprietary software may be used only as
# authorised by a licensing agreement from ARM Limited
# (C) COPYRIGHT 2010, 2013 ARM Limited
# ALL RIGHTS RESERVED
# The entire notice above must be reproduced on all authorised
# copies and copies may only be made to the extent permitted
# by a licensing agreement from ARM Limited.
# -----------------------------------------------------------------------------

import sys, platform
from utils import info_print

# Canonical mapping of Python's view of world to our view of world
_dBuildArch = {
        #"arm"     : "arm", Not used as return value no specific enough i.e. should be arm_v7 or arm_v8
        "mips"    : "mips",
        "x86"     : "x86_32",
        "i386"    : "x86_32",
        "i486"    : "x86_32",
        "i586"    : "x86_32",
        "i686"    : "x86_32",
        "i386_64" : "x86_64",
        "x86_64"  : "x86_64",
        "AMD64"   : "x86_64"
}

# =============================================================================
def get_build_platform():
        rHostAdxWidth = platform.architecture()[0]
        rHostArch = platform.machine()

        # Work around the Mac-Mini Python implementation thinking i386 is 64-bit!
        if( ( "i386" == rHostArch ) and ( "64bit" == rHostAdxWidth ) ):
                rHostArch = "i386_64"

        try:
                rBuildArch = _dBuildArch[rHostArch]

        except KeyError:
                sys.stderr.write('ERROR: No host architecture support [%s]\n' % rHostArch)
                exit( 1 )

        return rBuildArch

# =============================================================================
def get_appropriate_platform(sEnv):

	if( sEnv['arch'].find('x86')  != -1):

		info_print(sEnv, "Configuring build for architecture X86.")
		return _PlatformX86(sEnv['arch'])

	elif( sEnv['arch'].find('arm') != -1):
		info_print(sEnv, "Configuring build for architecture ARM.")
		return _PlatformARM(sEnv['arch'])

	else:
		sys.stderr.write('*** Platform "%s" is not supported!\n' % (sEnv['arch']))
		exit(1)

# =============================================================================

class _PlatformBase( object ):
	
	"""
	This class implements a base class from which other compilers are derived.
	"""

	# -------------------------------------------------------------------------
	def __init__( self, rTargetArch ):

		self.dCCDefines  = dict()
		self.lCCFlags    = list()
		self.rTargetArch = rTargetArch

	# -------------------------------------------------------------------------
	def update_environment( self, sEnv ):

		if (    self.rTargetArch == 'x86_32' or self.rTargetArch == 'arm_v7'):
			sEnv.Append( CPPDEFINES = {'MALI_32' : 1} )
			sEnv.Append( CPPDEFINES = {'MALI_64' : 0} )

		elif (  self.rTargetArch == 'x86_64' or self.rTargetArch == 'arm_v8'):
			sEnv.Append( CPPDEFINES = {'MALI_32' : 0} )
			sEnv.Append( CPPDEFINES = {'MALI_64' : 1} )

		else:
			sys.stderr.write('Unsupported target architecture "%s".' % (self.rTargetArch))
			exit(1)

# =============================================================================
class _PlatformX86( _PlatformBase):

	"""
	This class implements an instance of the platform abstraction for X86.
	"""

	# -------------------------------------------------------------------------
	def __init__( self, rTargetArch ):

		_PlatformBase.__init__(self, rTargetArch)


# =============================================================================
class _PlatformARM( _PlatformBase ):

	"""
	This class implements an instance of the platform abstraction for ARM.
	"""

	# -------------------------------------------------------------------------
	def __init__( self, rTargetArch):

		_PlatformBase.__init__(self, rTargetArch)
