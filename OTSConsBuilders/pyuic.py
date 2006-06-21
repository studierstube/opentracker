import os
import SCons.Builder
import SCons.SConsign
import re

def pyqt_emitter(target, source, env):
	target = os.path.splitext(str(source[0]))[0] + '.py'
	return target, source

def generate_pyqt_actions(source, target, env, for_signature):
	return 'pyuic %s -o %s' % (source[0], target[0])

def generate(env):
    	env['BUILDERS']['PYUIC'] = \
	    SCons.Builder.Builder(generator = generate_pyqt_actions, \
				  emitter = pyqt_emitter)
