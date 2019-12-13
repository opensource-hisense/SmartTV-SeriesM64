# This confidential and proprietary software may be used only as
# authorised by a licensing agreement from ARM Limited
# (C) COPYRIGHT 2010-2011 ARM Limited
# ALL RIGHTS RESERVED
# The entire notice above must be reproduced on all authorised
# copies and copies may only be made to the extent permitted
# by a licensing agreement from ARM Limited.

import os, sys

def get_available_profiles():
	'''
	Helper to determine the available profiles.
	Used by the top level sconstruct when generating the online help display.
	'''
		
	available_profiles = []
	if not os.path.isdir(sys.path[0]+os.sep+'profiles'):
		sys.stderr.write('*** Dir %s doesn\'t exist.\n' % ( sys.path[0]+os.sep+'profiles'  ))
	else:
		# sys.path[0] give us the directory containing this script
		for p in os.listdir(sys.path[0]+os.sep+'profiles'):
			try:
				name, ext = p.split(os.extsep)
				if 'py' == ext:
					available_profiles.append( name )
			except ValueError:
				sys.stderr.write('*** Unknown profile file: %s.\n' % ( p  ))

	return available_profiles


def load_profiles(args=[]):
	'''
	Helper to process all 'profile=value' arguments into a list
	Used by the top level sconstruct.
	'''
	profiles = []
	for k, v in args:
		if 'profile' == k:
			fname = sys.path[0]+os.sep+os.path.join('profiles','%s.py') % v
			try:
				os.stat( fname )
				profiles.append( fname )
			except StandardError as ex:
				sys.stderr.write('*** Failed to load profile "%s":\n\n%s\n' % (v, ex))
				exit(1)
	return profiles


def lib_target(env, name, source, *args, **kw):
	'''
	Helper to expose a library target as alias that can be specified on
	the command line.
	Added to the environment as LibTarget.
	'''
	if env.has_key('libs_install'):
		i = env.Install(env['libs_install'], source)
		env.Alias(name, i)
	else:
		env.Alias(name, source)


def prog_target(env, name, source, *args, **kw):
	'''
	Helper to expose a program target as alias that can be specified on
	the command line.
	Added to the environment as ProgTarget.
	'''
	if env.has_key('progs_install'):
		i = env.Install(env['progs_install'], source)
		env.Alias(name, i)
	else:
		env.Alias(name, source)


def info_print(env, s):
	'''
	Print informational messages, respecting the verbose (v) command line
	switch. Used within the bldsys module only.
	'''
	if env['v'] == '1':
		print s 

import collections
import SCons.Util

class _OrderedListVariable(collections.UserList):
	'''
	This class and associated functions (including _converter and OrderedListVariable)
	is derived from the SCons ListVariable code located here:
	http://www.scons.org/doc/2.0.1.beta.20100627/HTML/scons-api/SCons.Variables.ListVariable%27-pysrc.html

	The original SCons ListVariable behaves similar to a Set rather than a list and
	does not maintain the order of the variables specified on the command line. This
	list variable type maintains the order specified during creation.
	'''
	def __init__( self, initlist=[] ):
		collections.UserList.__init__(self, [_f for _f in initlist if _f])

	def __cmp__(self, other):
		raise NotImplementedError
	def __eq__(self, other):
		raise NotImplementedError
	def __ge__(self, other):
		raise NotImplementedError
	def __gt__(self, other):
		raise NotImplementedError
	def __le__(self, other):
		raise NotImplementedError
	def __lt__(self, other):
		raise NotImplementedError
	def __str__( self ):
		return '_'.join( self )
	def prepare_to_store(self):
		return self.__str__()

def _converter( val, allowedElems, mapdict ):
	'''
	This converter maps the values given during creation
	to new values and validates them. If no map was specified
	then the original values are validated and used.
	'''
	val = [_f for _f in val.split(',') if _f]
	val = [mapdict.get(v, v) for v in val]
	notAllowed = [v for v in val if not v in allowedElems]
	if notAllowed:
		raise ValueError("Invalid value(s) for option: %s" %
				 ','.join(notAllowed))
	return _OrderedListVariable(val)

def OrderedListVariable( key, help, default, names, map={} ):
	'''
	The input parameters describe a list of variable names and are
	returned with the correct converter appended. The result is
	usable for input to opts.Add().

	Unlike ListVariables, OrderedListVariables do not support the
	'all' or 'none' value.
	'''
	names_str = 'allowed names: %s' % ' '.join( names )
	if SCons.Util.is_List(default):
		default = ','.join(default)
	help = '\n    '.join(
		(help, '(comma-separated list of names)', names_str))
	return (key, help, default,
		None, lambda val: _converter(val, names, map))


def expand_list(env , varList):
	'''
	Expands an environment variable or list into a list of values.
	Similar to env.subst but does not need splitting.
	Similar to (undocumented) env.subst_list but does not 
	return nested list.

	WARNING: If the sconscript in which this is used is not read
	after the definition of environment variables then the references
	are lost, so needed values may be missed.
	'''
	# Substitutes each list element with expanded form which are split into
	# sublists. sum(_,[]) is used to concatenate the result.
	return sum([env.subst(listItem).split() for listItem in varList],[])
