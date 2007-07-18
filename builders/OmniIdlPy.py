import os, re
from itertools import izip, count

import SCons.Node.FS
import SCons.Environment
import SCons.Builder
import SCons.Scanner

import _omniidl
from omniidl.main import *

def omniidl_emitter(target, source, env):
    tlist, slist = [], source # source list is unchanged
    for src in source:
        trgs = []
        idl_file = str(src)
        include_args = ['-I' + directory for directory in env['CPPPATH']]
        print include_args + [idl_file]
        idl = parseOmniIdlArgs(include_args + [idl_file])
        modules = [instance for instance in idl.declarations() if (instance.__class__==idlast.Module) and (instance.file()==idl_file)]
        tlist.append(os.path.join(env['OMNIIDL_INSTALL_DIRECTORY'], os.path.basename(idl_file)[:-4] + '_idl.py'))
        for module in modules:
            module_name = module.scopedName()[0]
            tlist.append(os.path.join(env['OMNIIDL_INSTALL_DIRECTORY'], module_name, '__init__.py'))
            tlist.append(os.path.join(env['OMNIIDL_INSTALL_DIRECTORY'], module_name + '__POA', '__init__.py'))
        idlast.clear()
        idltype.clear()
        _omniidl.clear()
    return tlist, slist

def filterOutComments(contents):
    return re.sub(r'(//[^\n]*)', '', contents)

def parseOmniIdlArgs(args):
    global preprocessor_args, preprocessor_only, preprocessor_cmd
    global no_preprocessor, backend, backend_args, dump_only, cd_to
    global verbose, quiet, print_usage, interactive, temp_file

    files = parseArgs(args)

    if _omniidl.alwaysTempFile():
        temp_file = genTempFileName()

    #if len(backends) == 0 and \
    #   not (quiet or dump_only or preprocessor_only or interactive):
    #    sys.stderr.write(cmdname + ": Warning: No back-ends specified; " \
    #                     "checking IDL for validity\n")

    for name in files:
        if name != "-" and not os.path.isfile(name):
            if not quiet:
                sys.stderr.write(cmdname + ": '" + name + "' does not exist\n")
            sys.exit(1)

        if sys.platform != 'OpenVMS' or len(preprocessor_args)==0:
            preproc_cmd = '%s %s "%s"' % (preprocessor_cmd,
                                          string.join(preprocessor_args, ' '),
                                          name)
        else:
            preproc_cmd = '%s "%s" %s' % (preprocessor_cmd,
                                          string.join(preprocessor_args,'" "'),
                                          name)
        if not no_preprocessor:
            if verbose:
                sys.stderr.write(cmdname + ": Preprocessing '" +\
                                 name + "' with '" + preproc_cmd + "'\n")

            if preprocessor_only:
                err = os.system(preproc_cmd)
                if err:
                    if not quiet:
                        sys.stderr.write(cmdname + \
                                         ": Error running preprocessor\n")
                    sys.exit(1)
                sys.exit(0)

            if temp_file:
                if verbose:
                    sys.stderr.write(cmdname + \
                                     ": cpp output to temporary file '" + \
                                     temp_file + "'\n")
                err = os.system(preproc_cmd + " >" + temp_file)
                if err:
                    if not quiet:
                        sys.stderr.write(cmdname + \
                                         ": Error running preprocessor\n")
                    try:
                        os.remove(temp_file)
                    except:
                        pass
                    sys.exit(1)
                file = temp_file

            else:
                file = os.popen(preproc_cmd, "r")

        else: # No preprocessor
            file = name

        if verbose: sys.stderr.write(cmdname + ": Running front end\n")

        tree = _omniidl.compile(file)

        if not isinstance(file, StringType):
            if file.close():
                if not quiet:
                    sys.stderr.write(cmdname + \
                                     ": Error running preprocessor\n")
                sys.exit(1)

        if temp_file and not no_preprocessor:
            os.remove(temp_file)
    
            #idlast.clear()
            #idltype.clear()
            #_omniidl.clear()
    return tree

########################################################################
#                                                                      #
# Build problem is here with the "for_signature" argument              #
# Need to generate a Build Signature                                   #
########################################################################
def generate_omniidl_actions(source, target, env, for_signature):
    include_args = ''
    listCmd = []
    for directory in env['CPPPATH']:
        include_args += "-I%s " % (directory)
    try:
        defines = "-D%s "*len(env['CPPDEFINES']) % tuple(env['CPPDEFINES'])
    except KeyError:
        defines = ""
    for src in source:
        idl_file = src.get_string(for_signature)
        command = 'omniidl -bpython %s %s -C %s %s' % (defines, include_args, env['OMNIIDL_INSTALL_DIRECTORY'], idl_file)
        listCmd.append(command)
    return listCmd

def generate(env):
    env['OMNIIDL_INSTALL_DIRECTORY'] = '.'
    #idlscan = SCons.Scanner.Scanner(function = idlfile_scan,
                                    #skeys = ['.idl'])
    #env.Append(SCANNERS = idlscan)
    try:
        if '.' not in env['CPPPATH']:
            env.Append(CPPPATH = '.')
    except KeyError:
        env.Append(CPPPATH = '.')
    env['BUILDERS']['OMNIIDLPY'] = \
        SCons.Builder.Builder(generator = generate_omniidl_actions,
                              src_suffix = '.idl',
                              suffix = '.py',
                              emitter=omniidl_emitter)
        
def test():
    env = SCons.Environment.Environment()
    generate(env)
    target, source = idlj_emitter([], [env.File('graph.idl')], env)
