

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
        EventGenerator.h \
        EventObserver.h \
        EventQueueImplementation.h \
        EventQueue.h \
        MathUtils.h \
        Module.h \
        NodeFactoryContainer.h \
        NodeFactory.h \
        Node.h \
        State.h \
        TestModule.h \
        TestSource.h \
        ThreadModule.h \
        TimeDependend.h \
        TreeNode.h \
        WrapperNode.h
        
C++FILES = \
        ConfigurationParser.cxx \
        Context.cxx \
        DOMTreeErrorReporter.cxx \
        EventGenerator.cxx \
        EventQueueImplementation.cxx \
        MathUtils.cxx \
        NodeFactoryContainer.cxx \
        TestModule.cxx \
        TreeNode.cxx

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
