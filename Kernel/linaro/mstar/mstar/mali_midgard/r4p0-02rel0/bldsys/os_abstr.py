# -----------------------------------------------------------------------------
# This confidential and proprietary software may be used only as
# authorised by a licensing agreement from ARM Limited
# (C) COPYRIGHT 2010-2011 ARM Limited
# ALL RIGHTS RESERVED
# The entire notice above must be reproduced on all authorised
# copies and copies may only be made to the extent permitted
# by a licensing agreement from ARM Limited.
# -----------------------------------------------------------------------------

import sys, platform
from utils import info_print

# Canonical mapping of Python's view of world to our view of world
_dBuildOS = {
        "Windows"   : "windows",
        "WindowsCE" : "windows",
        "Linux"     : "linux",
        "Darwin"    : "osx",
        "Symbian"   : "symbian",
        "Android"   : "android",
}

# =============================================================================
def get_build_os():
	rHostOS = platform.system()
	try:
		rBuildOS = _dBuildOS[rHostOS]
	except KeyError:
		sys.stderr.write('ERROR: No host OS support [%s]' % rBuildOS)
		exit( 1 )

	return rBuildOS

# =============================================================================
def get_appropriate_os( sEnv ):

	if( sEnv['os'] == 'linux' ):
		# Check we can build for linux on the host platform
		host = sEnv['PLATFORM']
		if host != 'posix':
			sys.stderr.write('*** Cannot build for linux on %s\n' % host)
			exit(1)
		else:
			info_print(sEnv, "Configuring build for Linux.")
			return _OSLinux()

	elif( sEnv['os'] == 'osx' ):
		# Check we can build for osx on the host platform
		host = sEnv['PLATFORM']
		if host != 'darwin':
			sys.stderr.write('*** Cannot build for osx on %s\n' % host)
			exit(1)
		else:
			info_print(sEnv, "Configuring build for OSX.")
			return _OSX()

	elif( sEnv['os'] == 'android' ):
		# Check we can build for Android on the host platform
		host = sEnv['PLATFORM']
		if host != 'posix':
			sys.stderr.write('*** Cannot build for Android on %s\n' % host)
			exit(1)
		else:
			info_print(sEnv, "Configuring build for Android.")
			return _OSAndroid()

	else:
		sys.stderr.write('*** os "%s" is not supported \n' % (sEnv['os']))
		exit(1)


# =============================================================================
class _OSBase( object ):


	# -------------------------------------------------------------------------
	def __init__( self ):

		self.dCCDefines = dict()
		self.lLDFlags   = list()
		self.rKernel    = None

	# -------------------------------------------------------------------------
	def set_windowing_system( self, rWinSys ):

		if ( rWinSys =='dummy' ):
			self.dCCDefines.update( {'EGL_DUMMY' : 1} )

	# -------------------------------------------------------------------------
	def update_environment( self, sEnv ):

		sEnv.Append( CPPDEFINES =self.dCCDefines )
		sEnv.Append( LINKFLAGS  =self.lLDFlags )
		if not self.rKernel:
			self.rKernel = sEnv['os']
		sEnv.AppendUnique( kernel = self.rKernel, delete_existing=1 )

# =============================================================================
class _OSUnix( _OSBase ):


	# -------------------------------------------------------------------------
	def __init__( self ):

		_OSBase.__init__( self )

	# -------------------------------------------------------------------------
	def set_windowing_system( self, rWinSys ):

		if ( rWinSys =='fbdev' ):
			self.dCCDefines.update( { 'EGL_FBDEV' : 1} )
		elif ( rWinSys == 'x11'):
			self.dCCDefines.update( { 'EGL_X11'   : 1} )

		_OSBase.set_windowing_system(self, rWinSys)

# =============================================================================
class _OSLinux( _OSUnix ):


	# -------------------------------------------------------------------------
	def __init__( self ):

		_OSBase.__init__( self )
		self.rKernel = 'linux'
		self.dCCDefines.update( {'_GNU_SOURCE' : 1} )

	# -------------------------------------------------------------------------
	def set_windowing_system( self, rWinSys ):

		if ( rWinSys == 'directfb' ):
			self.dCCDefines.update( { 'EGL_DIRECTFB' : 1} )
		elif ( rWinSys == 'wayland' ):
			self.dCCDefines.update( { 'EGL_WAYLAND' : 1} )

		_OSUnix.set_windowing_system(self, rWinSys)

	# -------------------------------------------------------------------------
	def set_windowing_systems( self, rWinSys ):

		if ( rWinSys['winsys'] == 'multi' ):
			self.dCCDefines.update( { 'MSTAR_EGL_MULTI_WINSYS' : 1} )

		if ( rWinSys['directfb'] == 1 ):
			self.dCCDefines.update( { 'EGL_DIRECTFB' : 1} )
		if ( rWinSys['wayland'] == 1 ):
			self.dCCDefines.update( { 'EGL_WAYLAND' : 1} )

		_OSUnix.set_windowing_system(self, rWinSys['winsys'])

# =============================================================================
class _OSX( _OSUnix ):


	# -------------------------------------------------------------------------
	def __init__( self ):

		_OSBase.__init__( self )

		#For Mac OS X, the head file <sys/types.h> require _DARWIN_C_SOURCE
		#to be defined to enable u_int, u_long ...... types
		self.dCCDefines.update( {'_DARWIN_C_SOURCE' : 1} )

		#For Mac OS X, add the Application Services framework
		self.lLDFlags.append ( '-framework ApplicationServices' )

		#For Mac OS X, add I/O Kit framework
		self.lLDFlags.append ( '-framework IOKit' )

# =============================================================================
class _OSAndroid( _OSUnix ):


	# -------------------------------------------------------------------------
	def __init__( self ):

		_OSBase.__init__( self )
		self.rKernel = 'linux'
		self.dCCDefines.update( {'_GNU_SOURCE' : 1} )
