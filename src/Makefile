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
XERCES_HOME = ../../xerces

PROGRAM = ../bin/maxwell

HFILES = \
    TrackerServer.h \
    CommonNodeFactory.h  \
    NetworkDriver.h  \
    ConfigurationParser.h \
    NodeFactoryContainer.h \
    Transformation.h \
    Marker.h  \
    NodeFactory.h \
    TreeNode.h \
    MathUtils.h \
    Node.h \
    VideoSource.h \
    MaxConfidenceFilter.h \
    State.h \
    VirtualTransformation.h \
    Module.h  \
    Station.h \
    multicast.h \
    Context.h  \
    TestModule.h \
    TestSource.h \
    DOMTreeErrorReporter.h \
    
C++FILES = \
    CommonNodeFactory.cxx  \
    NetworkDriver.cxx  \
    ConfigurationParser.cxx \
    NodeFactoryContainer.cxx \
    Transformation.cxx \
    Marker.cxx  \
    NodeFactory.cxx \
    TreeNode.cxx \
    MathUtils.cxx \
    Node.cxx \
    VideoSource.cxx \
    MaxConfidenceFilter.cxx \
    State.cxx \
    VirtualTransformation.cxx \
    Module.cxx  \
    Station.cxx \
    multicast.cxx \
    main.cxx \
    Context.cxx \
    TestModule.cxx \
    TestSource.cxx \
    DOMTreeErrorReporter.cxx \

OFILES = $(C++FILES:.cxx=.o)

LC++INCS= -I$(ARTOOL_HOME)/include  -I$(XERCES_HOME)/include -DIRIX

LC++OPTS= -g -O2 -woff 1171,1401

LD_QUICKSTART_INFO=

TARGETS = $(PROGRAM)

# Libraries to link with:

LLDLIBS = -L$(ARTOOL_HOME)/lib -L$(XERCES_HOME)/lib -lxerces-c1_2 -lARgsub_n32 -lARvideo_n32 -lAR_n32 -lm -lvl

LLDOPTS =  -no_prelink 

default: $(TARGETS)

apps: default

include $(COMMONRULES)

$(TARGETS): $(OBJECTS)
	CC -o $@ $(OFILES) $(LLDLIBS) $(LLOPTS)
