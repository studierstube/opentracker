

TARGET: default

#include system specific makefile settings
include ../../Config.global
include ../../Config.site

TARGETS = $(OT_LIB_DIR)/libOpenTracker.a

HFILES = \
        ConfigNode.h \
        ConfigurationParser.h \
        Context.h \
        DOMTreeErrorReporter.h \
        EventQueueImplementation.h \
        MathUtils.h \
        Module.h \
        NodeFactoryContainer.h \
        NodeFactory.h \
        Node.h \
        RefNode.h \
        State.h \
        TestModule.h \
        TestSource.h \
        ThreadModule.h \
        WrapperNode.h \
        StringTable.h
        
C++FILES = \
        ConfigurationParser.cxx \
        Context.cxx \
        DOMTreeErrorReporter.cxx \
        EventQueueImplementation.cxx \
        MathUtils.cxx \
        NodeFactoryContainer.cxx \
        Node.cxx \
        State.cxx \
        TestModule.cxx \
        ThreadModule.cxx \
        WrapperNode.cxx \
        StringTable.cxx

LC++INCS= $(OT_STD_INCLUDES)

LC++OPTS= $(SYS_C++OPTS)

OBJ = $(C++FILES:.cxx=.o)

LD_QUICKSTART_INFO=

$(TARGETS): $(OBJ)
	@echo "--- $(TARGETS) ---"
	ar -cru $(TARGETS) $(OBJ)

update: $(TARGETS)
	@echo "--- $(TARGETS) (update) ---"
	ar -cru $(TARGETS) $(OBJ)

default: $(TARGETS)

sinclude $(DEPFILES)
