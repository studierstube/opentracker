#!smake

TARGET: default

#include system specific makefile settings

# /usr/include/make/commondefs and /usr/include/make/commonrules
# define some useful Makefile rules.  For example they
# defines a 'clean' target so you can type 'make clean'
# to remove all .o files (or other files generated during
# compilation). See the file /usr/include/make/commonrules for
# documentation on what targets are supported.

include /usr/include/make/commondefs

ARTOOL_HOME = ../../artool
XERCES_HOME = /projects/cg/dev/xerces
ACE = /projects/cg/dev/ACE_wrappers

PROGRAM = ../../bin/opentracker

HFILES = \
    OpenTracker.h \
    CommonNodeFactory.h \
    NodeFactory.h \
    ConfigurationParser.h \
    Node.h \
    Context.h \
    DOMTreeErrorReporter.h \
    State.h \
    EventGenerator.h \
    TestModule.h \
    EventObserver.h \
    TestSource.h \
    EventQueueImplementation.h \
    TimeDependend.h \
    EventQueueNode.h \
    Transformation.h \
    EventQueue.h \
    TreeNode.h \
    MathUtils.h \
    VirtualTransformation.h \
    Module.h \
    WrapperNode.h \
    NodeFactoryContainer.h \
    TestModule.h \
    TestSource.h \
    ConfigNode.h \
    ThreadModule.h

C++FILES = \
    CommonNodeFactory.cxx \
    MathUtils.cxx \
    ConfigurationParser.cxx \
    Context.cxx \
    NodeFactoryContainer.cxx \
    DOMTreeErrorReporter.cxx \
    EventGenerator.cxx \
    OpenTracker.cxx \
    EventQueueImplementation.cxx \
    Transformation.cxx \
    EventQueueNode.cxx \
    TreeNode.cxx \
    VirtualTransformation.cxx \
    main.cxx \
    TestModule.cxx

OFILES = $(C++FILES:.cxx=.o)

LC++INCS= -I$(ARTOOL_HOME)/include  -I$(XERCES_HOME)/include -I$(ACE) -DIRIX

LC++OPTS= -g -O2 -woff 1171,1401

LD_QUICKSTART_INFO=

TARGETS = $(PROGRAM)

# Libraries to link with:

LLDLIBS = -L$(ARTOOL_HOME)/lib -L$(XERCES_HOME)/lib -L$(ACE)/ace -lxerces-c1_2 -lm -lACE -lvl -lcurses

LLDOPTS =  -no_prelink 

default: $(TARGETS)

apps: default

include $(COMMONRULES)

$(TARGETS): $(OBJECTS)
	CC -o $@ $(OFILES) $(LLDLIBS) $(LLOPTS)
