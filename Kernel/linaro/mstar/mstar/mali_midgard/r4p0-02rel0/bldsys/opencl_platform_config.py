# This confidential and proprietary software may be used only as
# authorised by a licensing agreement from ARM Limited
# (C) COPYRIGHT 2010-2011, 2013 ARM Limited
# ALL RIGHTS RESERVED
# The entire notice above must be reproduced on all authorised
# copies and copies may only be made to the extent permitted
# by a licensing agreement from ARM Limited.

# This dictionary contains an entry per platform configuration.
# Its keys are the platform configuration names selectable with the opencl_platform_config= option when building the
# driver and its values are the list platforms and the devices they contain.
# configuration.
OpenCLPlatformConfigs = {'config_t6xx': [['t6xx', 't6xx_cg1']],
						 'config_arm': [['arm']],
						 'config_t6xx_arm': [['t6xx', 't6xx_cg1','arm']],
						 'config_t6xx_arm_mock': [['t6xx', 't6xx_cg1', 'arm', 'mock']],
						 'config_t6xx_arm_mock_cpu': [['t6xx', 't6xx_cg1', 'arm', 'mock', 'cpu']],
						 'config_example_multiple_platforms': [['t6xx', 't6xx_cg1'], ['arm']]}

# This dictionary contains devices name and the memory plugins they require
# (the 'host' memory plugin is mandatory, hence always included)
OpenCLDeviceMemoryPlugins = {'t6xx': ['t6xx']}

# This dictionary describes a list of OpenCL devices, and for each device the cache through which it accesses all of its
# supported physical memory types
# (devices not listed here are assumed to use the 'host' cache)
OpenCLDeviceCache = {'t6xx': 't6xx', 't6xx_cg1' : 't6xx_cg1'}

def __opencl_platform_config_get_devices( config ):
	"""
	Private function that returns the complete list of devices supported by the selected configuration (including devices
	which are on different platforms)
	"""
	devices = []
	for opencl_devices in OpenCLPlatformConfigs[ config ]:
		devices += opencl_devices
	return devices

def opencl_platform_config_get_opencl_devices_equivalent( env ):
	"""
	Try to find a platform configuration that contains a platform that has the the same devices as those specified in
	the deprecated opencl_devices= option. If no such configuration is found it returns an empty array.
	"""
	configs = []
	for config in OpenCLPlatformConfigs:
		for opencl_devices in OpenCLPlatformConfigs[ config ]:
			if set(opencl_devices) == set(env['opencl_devices']):
				configs += [config]
	return configs

def opencl_platform_config_has_device( env, device ):
	"""
	Returns True if 'device' is used by any platform of the selected configuration and False otherwise
	"""
	devices = __opencl_platform_config_get_devices( env['opencl_platform_config'] )
	return device in devices

def opencl_platform_config_get_devices( env ):
	"""
	Returns the complete list of devices supported by the selected configuration (including devices which are on different
	platforms)
	"""
	devices = __opencl_platform_config_get_devices( env['opencl_platform_config'] )
	return devices

def opencl_platform_config_get_platforms( env ):
	"""
	Returns the platforms defined in the selected configuration
	"""
	return OpenCLPlatformConfigs[ env['opencl_platform_config'] ]

def opencl_platform_config_get_memory_plugins( env ):
	"""
	Returns the complete set of memory plugins supported by the various devices in the selected configuration
	"""
	s = set()
	devices = __opencl_platform_config_get_devices( env['opencl_platform_config'] )
	for device in devices:
		if device in OpenCLDeviceMemoryPlugins:
			s = s.union(OpenCLDeviceMemoryPlugins[device])
	return s

def opencl_platform_config_get_device_caches( env ):
	"""
	Returns the complete set of caches used by the various devices in the selected configuration
	"""
	s = set()
	devices = __opencl_platform_config_get_devices( env['opencl_platform_config'] )
	for device in devices:
		if device in OpenCLDeviceCache:
			s.add(OpenCLDeviceCache[device])
	return s

def opencl_platform_config_get_names():
	"""
	Returns the names of the supported platform configurations
	"""
	return OpenCLPlatformConfigs.keys()



