import glob
import sys
import os

if sys.platform == 'darwin':
	defs='-g -DDARWIN'
	_cpppath = ['/opt/local/include', './src']
elif sys.platform == 'linux':
	defs='-g -DLINUX'
	_cpppath = ['/opt/local/include', './src']

env=Environment(ENV = os.environ, CCFLAGS=defs)

common_source_files = glob.glob('./src/common/*.cxx')
core_source_files = glob.glob('./src/core/*.cxx')
network_source_files = glob.glob('./src/network/*.cxx')

#tool_source_files = glob.glob('./src/tool/*.cxx')
tool_source_files = ['./src/tool/OT_ACE_Log.cxx']
input_source_files = glob.glob('./src/input/*.cxx')
if sys.platform == 'darwin':
	input_source_files.remove('./src/input/InterSenseModule.cxx')
	input_source_files.remove('./src/input/ParButtonModule.cxx')
misc_source_files = ['./src/misc/XKeys.cxx','./src/misc/serialcomm.cxx', './src/misc/OpenTracker.cxx',]
	
main_file = './src/misc/main.cxx'	

ot2lib = env.SharedLibrary('OpenTracker', common_source_files + core_source_files + \
	                       network_source_files + tool_source_files + \
	                       misc_source_files + input_source_files, \
	                       LIBS=['m', 'omniORB4', 'omnithread', 'omniDynamic4', 'ACE','xerces-c', \
	                             'stdc++', 'ncurses', 'X11'], \
	                             CPPPATH=_cpppath, \
	                             LIBPATH=['/opt/local/lib','/usr/X11R6/lib'])
			       
ot2 = env.Program('opentracker', main_file, \
                       LIBS=['m', 'omniORB4', 'omnithread', 'omniDynamic4', 'ACE','xerces-c', \
                            'stdc++', 'ncurses', 'X11', ot2lib], \
		       CCPATH=defs, CPPPATH=_cpppath, LIBPATH=['/opt/local/lib','/usr/X11R6/lib','./lib'])
env.Depends(ot2, ot2lib)

env.Install('#bin', ot2)
env.Install('#lib', ot2lib)
