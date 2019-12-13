# This confidential and proprietary software may be used only as
# authorised by a licensing agreement from ARM Limited
# (C) COPYRIGHT 2011 ARM Limited
# ALL RIGHTS RESERVED
# The entire notice above must be reproduced on all authorised
# copies and copies may only be made to the extent permitted
# by a licensing agreement from ARM Limited.

import sys

# ------------------------------
#
# Class to aid creating simple unified patch files
#
# Patch files are composed of patch chunks. The caller is responsible for specifying
# each patch chunk. The major advantage of using this class is that it generates an
# appropriate chunk header for each chunk.
#
# Each patch chunk should have:
#
#   * The line number in the original file where the chunk is to be applied.
#   * Some unchanged lines (for context).
#   * One or more of (in any combination):
#       new lines to add,
#       lines to delete,
#       unchanged lines.
#   * Some unchanged lines (for context).
#

class Patch(object):
	def __init__( self, filename, header=None ):
		'''
		Creates a patch file with the specified path/name and
		writes the optional header string (which must end with a \n).
		'''
		fout = open( filename, 'w' )
		if header:
			fout.write( header )
		self.file = fout
		self.start_line = 0
		self.running_offset = 0

	def _fatal_error( self, str ):
		sys.stderr.write("\nFATAL ERROR in patch.py:\n%s\n\n" % str)
		exit(1)

	def _check_patch_open( self, op ):
		if self.file == None:
			self._fatal_error( "Patch %s failed - patch file already closed." % op  )

	def _check_chunk_started( self, op ):
		self._check_patch_open(op)
		if self.start_line == 0:
			self._fatal_error( "Patch %s failed - must start a chunk first." % op )

	def write_chunk( self ):
		'''
		Writes the current chunk to the patch file.
		
		Use of this function is optional as it will automatically be called
		whenever a new chunk is started or when the file is closed.
		'''
		# check patch file open and a chunk has been started
		self._check_chunk_started("write chunk")

		# Calculate chunk header
		pre_patch_chunk_size = self.num_orig_lines + self.num_deleted_lines
		post_patch_chunk_size = self.num_orig_lines + self.num_new_lines
		post_chunk_start_line_no = self.start_line + self.running_offset

		self.file.write( "@@ -%d,%d +%d,%d @@\n" %
				(	self.start_line, pre_patch_chunk_size,
					post_chunk_start_line_no, post_patch_chunk_size
				) )
	
		for line in self.chunk:
			self.file.write( "%s\n" % line )

		self.running_offset += post_patch_chunk_size - pre_patch_chunk_size
		self.start_line = 0

	def start_chunk( self, start_line_no ):
		'''
		Start a new chunk definition.
		If there is already a pending chunk defined then it is written first.
		'''
		# check patch file is open
		self._check_patch_open( "start chunk" )

		if self.start_line != 0:
			# Write existing chunk before starting a new one
			self.write_chunk()

		self.chunk = []				# array of patch lines
		self.start_line = start_line_no		# line number in original file where this chunk applies
		self.num_orig_lines = 0
		self.num_post_patch_lines = 0
		self.num_new_lines = 0
		self.num_deleted_lines = 0

	def append_unchanged_lines( self, lines ):
		'''
		Append unchanged lines to the current chunk definition.
		They must not have the unified ' ' prefix or trailing '\n'
		as these will be applied automatically.
		'''
		
		# check patch file open and a chunk has been started
		self._check_chunk_started("append unchanged lines")

		for line in lines:
			# Add line with a leading space
			self.chunk.append( " %s" % line )
		self.num_orig_lines += len( lines )

	def append_new_lines( self, lines ):
		'''
		Append new lines to the current chunk definition.
		They must not have the unified '+' prefix or trailing '\n'
		as these will be applied automatically.
		'''

		# check patch file open and a chunk has been started
		self._check_chunk_started("append new lines")

		for line in lines:
			# Add line with a leading +
			self.chunk.append( "+%s" % line )
		self.num_new_lines += len( lines )

	def append_deleted_lines( self, lines ):
		'''
		Append deleted lines to the current chunk definition.
		They must not have the unified '-' prefix or trailing '\n'
		as these will be applied automatically.
		'''

		# check patch file open and a chunk has been started
		self._check_chunk_started("append deleted lines")

		for line in lines:
			# Add line with a leading -
			self.chunk.append( "-%s" % line )
		self.num_deleted_lines += len( lines )

	def append_unchanged_line( self, line ):
		''' See append_unchanged_lines() '''
		self.append_unchanged_lines( [ line ] )

	def append_new_line( self, line ):
		''' See append_new_lines() '''
		self.append_new_lines( [ line ] )

	def append_deleted_line( self, line ):
		''' See append_deleted_lines() '''
		self.append_deleted_lines( [ line ] )

	def append_lines( self, lines ):
		'''
		Append raw lines to the current chunk definition.
		Each line must start with a '+', '-' or ' ' as per unified patch syntax.
		'''

		# check patch file open and a chunk has been started
		self._check_chunk_started("append lines")

		for line in lines:
			if line == None or line == '':
				# Empty lines are assumed to be unchanged lines
				self.append_unchanged_lines( [line] )
			elif line[0] == ' ':
				self.append_unchanged_lines( [line[1:]] )
			elif line[0] == '+':
				self.append_new_lines( [line[1:]] )
			elif line[0] == '-':
				self.append_deleted_lines( [line[1:]] )
			else:
				print "Invalid patch line: \n%s" % line

	def append_line( self, line ):
		''' See append_lines() '''
		self.append_lines( [line] )

	def close( self ):
		'''
		Writes any pending chunk to the patch file and then closes the patch file.
		No further chunks can then be appended to the patch file.
		
		Use of this function is optional as it will be called when the Patch
		object is deleted if the patch file hasn't already been closed.
		'''
		# check patch file is open
		self._check_patch_open("close")

		if self.start_line != 0:
			# Write existing chunk before closing the file
			self.write_chunk()

		# Close the file
		self.file.close()
		
		# mark file closed - prevents any futher usage.
		self.file = None

	def __exit__( self ):
		# If the patch file hasn't already been closed then close it
		if self.file:
			self.close()

