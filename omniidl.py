import os
import SCons.Builder
import SCons.SConsign
import re

module_re = re.compile(('module\s\s*(\w*)'))

def scan_for_modules(idl_file):
	content = idl_file.get_contents()
	module_names = module_re.findall(content)
	return module_names

def omniidl_emitter(target, source, env):
	idl_file = str(source[0])
	stem = str(idl_file).split('/')[-1][0:-4]
	header_file = stem + '.hh'
	skeleton_file = stem + 'SK.cc'
	idl_directory = idl_file[0:-len(idl_file.split('/')[-1])-1]
	skeleton_directory = os.path.join(idl_directory, 'skeletons')
	skeleton_files = []
	skeleton_files.append(os.path.join(skeleton_directory, stem + '.hh'))
	skeleton_files.append(os.path.join(skeleton_directory, stem + 'SK.cc'))
	skeleton_files.append(os.path.join(skeleton_directory, stem + '_idl.py'))
	module_names = scan_for_modules(source[0])
	for name in module_names:
		#skeleton_files.append(os.path.join(skeleton_directory, name))
		skeleton_files.append(os.path.join(skeleton_directory, name, '__init__.py'))
		#skeleton_files.append(os.path.join(skeleton_directory, name + '__POA'))
		skeleton_files.append(os.path.join(skeleton_directory, name + '__POA', '__init__.py'))
	target = skeleton_files
	return target, source

def generate_omniidl_actions(source, target, env, for_signature):
	idl_file = str(target[0])
	skeleton_directory = idl_file[0:-len(idl_file.split('/')[-1])-1]
	return 'omniidl -bpython -bcxx -Wbtp -C %s %s' % (skeleton_directory, source[0])

def generate(env):
    	env['BUILDERS']['OMNIIDL'] = \
	    SCons.Builder.Builder(generator = generate_omniidl_actions, \
			  emitter=omniidl_emitter)
