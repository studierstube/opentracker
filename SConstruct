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

# name for the dll and program that will be produced by the SConstruct
dlname = 'opentracker'
programname = 'opentracker'

libInstallDir = '#lib'
binInstallDir = '#bin'
dlInstallDir  = '#lib'
buildPrefix = ''

_lpath = ['/opt/local/lib','/usr/X11R6/lib']

if sys.platform == 'darwin':
	defs='-g -DDARWIN '
	_cpppath = ['/opt/local/include', './src']
	# Remove the following files from the darwin build
	input_source_files.remove('./src/input/InterSenseModule.cxx')
	input_source_files.remove('./src/input/ParButtonModule.cxx')
	_libs = LIBS=['m', 'ACE','xerces-c', 'stdc++', 'ncurses', 'X11']
elif sys.platform == 'linux':
	defs='-g -DLINUX'
	_cpppath = ['/opt/local/include', './src']
	_libs = LIBS=['m', 'ACE','xerces-c', 'stdc++', 'ncurses', 'X11']

# windows compile configuration
elif sys.platform == 'win32':
	# to be able to link in windows we need intersense
	input_source_files.append('./extras/intersense\isense.c')
	misc_source_files.append('./src/misc/portio.cxx')
	misc_source_files.append('./src/misc/xml/XMLWriter.cxx')

	# to get the include path for all libraries we need to retrieve 
	# the environment vars pointing to the root of each lib
	envvars = os.environ

	corbaLibPath = envvars['OMNIORB'] + '/lib/x86_win32'
	corbaIncPath = envvars['OMNIORB'] + '/include'

	# build prefix, all object files and targets will be first placed under
	# the directory designated by this variable
	buildPrefix=envvars['OTROOT']+'/build/SCons/'

	# Install directories for windows
	libInstallDir = envvars['OTROOT']+'/lib/win32'
	binInstallDir = envvars['OTROOT']+'/bin/win32'
	dlInstallDir  = envvars['OTROOT']+'/bin/win32'

	# flags for console program creation in VS8.0 for windows XP
	_programCppFlags = '/O2 /Ob1 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_VC80_UPGRADE=0x0710" /D "_MBCS" /GF /FD /EHsc /MD /Gy /Fp".'+buildPrefix+'/standalone.pch" /Fo"'+buildPrefix+'" /Fd"'+buildPrefix+'" /W3 /nologo /c /errorReport:prompt'

	_programIncFlags =[envvars['OTROOT']+ '/src',envvars['OTROOT']+ '/misc',envvars['OTROOT']]

	_programLibs= ['odbc32.lib', 'odbccp32.lib', 'kernel32.lib', 'user32.lib', 'gdi32.lib', 'winspool.lib', 'comdlg32.lib', 'advapi32.lib', 'shell32.lib', 'ole32.lib' , 'oleaut32.lib', 'uuid.lib', 'odbc32.lib', 'odbccp32.lib']

	_programLibDirs = [libInstallDir]

	_programLinkFlags = ' /INCREMENTAL:NO /NOLOGO /SUBSYSTEM:CONSOLE /MACHINE:X86 /ERRORREPORT:PROMPT'

	# flags for dll creation in VS8.0 for windows XP
	_dlCppFlags= '/O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "OPENTRACKER_EXPORTS" /D "__WIN32__" /D "__x86__" /D "_CRT_SECURE_NO_DEPRECATE" /D "TINYXML_MOD_DLL" /D "_WINDLL" /D "_MBCS" /FD /EHsc /MD /Fd"'+ buildPrefix +'/vc80.pdb" /W3 /nologo /Zi /TP /errorReport:prompt'

	_dlIncFlags = [envvars['ACEROOT'], envvars['TINYXMLROOT'], envvars['TINYXMLMODROOT'] + '/src', envvars['OTROOT']+ '/extras', envvars['OTROOT']+ '/extras/intersense', envvars['ARTOOLKITPLUSROOT'] + '/include', envvars['OPENVIDEOROOT'] + '/include', envvars['XERCESCROOT'] + '/include']

	_dlLibs = LIBS = ['ace', 'openvideo',  'kernel32', 'user32', 'gdi32', 'winspool', 'comdlg32', 'advapi32', 'shell32', 'ole32', 'oleaut32', 'uuid', 'odbc32', 'odbccp32'] 

	_dlLibDir = [envvars['ACEROOT']+ '/lib/Win32',envvars['ACEROOT']+ "\\lib", envvars['ACEROOT'], envvars['TINYXMLROOT']+'/lib',envvars['TINYXMLMODROOT'] + '/lib/Win32', envvars['XERCESCROOT']+"\\lib", envvars['ARTOOLKITPLUSROOT']+ '/lib/Win32', envvars['OPENVIDEOROOT']+"\\lib\\Win32"]

	_dlLinkFlags= '/MANIFEST /MANIFESTFILE:"'+ buildPrefix+'/opentracker.dll.intermediate.manifest" /DEBUG  /INCREMENTAL:NO /OPT:REF /OPT:ICF /SUBSYSTEM:WINDOWS /MACHINE:X86 /ERRORREPORT:PROMPT /PDB:'+ buildPrefix +'/' + dlname + '.pdb'



opts = Options('custom.py') 
opts.Add(BoolOption('corba', 'Set to 1 to build in CORBAModule', 0)) 
opts.Add(BoolOption('pyqt', 'Set to 1 to build in pyqt applications', 0)) 
env=Environment(ENV = os.environ, options=opts)
Help(opts.GenerateHelpText(env))

# Test to see whether the CORBAModule should be built
if env['corba']:
	if sys.platform == 'win32':
		_dlLibDir += [corbaLibPath]
		_dlIncFlags += [corbaIncPath]
		_dlLibs += ['omniORB4', 'omnithread', 'omniDynamic4']

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





allsources = common_source_files + core_source_files + network_source_files + tool_source_files + misc_source_files + input_source_files + skeletons
env['OBJPREFIX']= buildPrefix

buildTarget = buildPrefix + dlname

if sys.platform =='win32':
	defs = _dlCppFlags
	_libs = _dlLibs
	_cpppath = _dlIncFlags
	_lpath = _dlLibDir
	_linkflags = _dlLinkFlags
#	env['PDB']=buildPrefix + dlname +'.pdb'

otlib = env.SharedLibrary(buildTarget, allsources, 
			   CCFLAGS = defs, \
			   LIBS=_libs, CPPPATH=_cpppath, \
			   LIBPATH=_lpath, LINKFLAGS=_linkflags)


if sys.platform == 'win32':
	buildTarget = buildPrefix + programname
	_linkflags = _programLinkFlags
	_libs = _programLibs
	_lpath = _programLibDirs
	_cpppath = _programIncFlags
	defs = _programCppFlags
	env['PDB']= buildPrefix+ 'standalone.pdb'

ot = env.Program(buildTarget, [main_file] , \
		  LIBS= _libs + [dlname],\
		  CPPPATH=_cpppath, \
		  CCFLAGS = defs, \
		  LIBPATH= _lpath, LINKFLAGS=_linkflags)


# I still don't know how to compile middleware in windows
if sys.platform=='win32':
	middleware = []
else:
	middleware = env.Program('middleware', middleware_file, \
			  LIBS= _libs + [dlname],\
			  CPPPATH=_cpppath, \
			  CCFLAGS = defs, \
		  LIBPATH=['/opt/local/lib','/usr/X11R6/lib','./lib'])



if sys.platform == 'win32':
	print otlib


env.Install(binInstallDir, [ot, middleware])
env.Install(libInstallDir, otlib)
