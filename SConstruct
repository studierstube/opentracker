import glob
import sys
import os

# All the other files 
common_source_files = glob.glob('./src/common/*.cxx')
core_source_files = glob.glob('./src/core/*.cxx')
network_source_files = glob.glob('./src/network/*.cxx')
skeletons = [] # This will be filled later on
tool_source_files = ['./src/tool/OT_ACE_Log.cxx']
input_source_files = glob.glob('./src/input/*.cxx')
misc_source_files = ['./src/misc/XKeys.cxx',
		     './src/misc/serialcomm.cxx', 
		     './src/misc/OpenTracker.cxx']
main_file = './src/misc/main.cxx'
middleware_file = './src/misc/middleware.cxx'

if sys.platform == 'darwin':
	defs='-g -DDARWIN '
	_cpppath = ['/opt/local/include', './src']
	# Remove the following files from the darwin build
	input_source_files.remove('./src/input/InterSenseModule.cxx')
	input_source_files.remove('./src/input/ParButtonModule.cxx')
elif sys.platform == 'linux':
	defs='-g -DLINUX'
	_cpppath = ['/opt/local/include', './src']
_libs = LIBS=['m', 'ACE','xerces-c', 'stdc++', 'ncurses', 'X11']

opts = Options('custom.py') 
opts.Add(BoolOption('corba', 'Set to 1 to build in CORBAModule', 0)) 
opts.Add(BoolOption('pyqt', 'Set to 1 to build in pyqt applications', 0)) 
env=Environment(ENV = os.environ, options=opts)
Help(opts.GenerateHelpText(env))

# Test to see whether the CORBAModule should be built
if env['corba']:
	from OTSConsBuilders import omniidl
	defs  += ' -DUSE_CORBA '
	_libs += ['omniORB4', 'omnithread', 'omniDynamic4']
	omniidl.generate(env)
	idl_path = './idl'
	idl_files = glob.glob(idl_path + '/*.idl')
	idl_dict  = {}
	for idl_file in idl_files:
		stem = str(idl_file).split('/')[-1][0:-4]
		idl_dict[stem] = File(idl_file)
	pydirs = []
	for _idl in idl_dict.values():
		# Call the OMNIIDL builder with the idl file
		idl_targets = env.OMNIIDL(_idl)
		stubSK = idl_targets[1]
		pystubs = idl_targets[3:]
		for py in pystubs:
			pydirs.append(os.path.dirname(str(py)))
		skeletons.append(stubSK)
	# append skeleton directory
	_cpppath.append(os.path.join(idl_path, 'skeletons'))
	# check to see whether there should be a cleanup of these directories
	if env.GetOption('clean'):
		print "Cleanup of the python stub directories"
		for pydir in pydirs:
			Execute(Delete(pydir))
else:
	corba_files = glob.glob('./src/network/CORBA*.cxx')
	# remove CORBA files from list of network source files
	for file in corba_files:
		network_source_files.remove(file)

if env['pyqt']:
	# PyQt: build the .ui files
	from OTSConsBuilders import pyuic
	pyuic.generate(env)
	ui_files = ['src_python/ManualTrackerPython/ManualTrackerGUI.ui']
	for ui_file in ui_files:
		env.PYUIC(ui_file)

otlib = env.SharedLibrary('OpenTracker', common_source_files + \
			   core_source_files + network_source_files + \
			   tool_source_files + misc_source_files + \
			   input_source_files + skeletons, \
			   CCFLAGS = defs, \
			   LIBS=_libs, CPPPATH=_cpppath, \
			   LIBPATH=['/opt/local/lib','/usr/X11R6/lib'])

ot = env.Program('opentracker', main_file, \
		  LIBS= _libs + otlib,\
		  CPPPATH=_cpppath, \
		  CCFLAGS = defs, \
		  LIBPATH=['/opt/local/lib','/usr/X11R6/lib','./lib'])

middleware = env.Program('middleware', middleware_file, \
		  LIBS= _libs + otlib,\
		  CPPPATH=_cpppath, \
		  CCFLAGS = defs, \
		  LIBPATH=['/opt/local/lib','/usr/X11R6/lib','./lib'])

#env.Depends(ot2, ot2lib)

env.Install('#bin', [ot, middleware])
env.Install('#lib', otlib)
