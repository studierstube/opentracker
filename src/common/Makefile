

TARGET: default

#include system specific makefile settings
include ../../Config.global
include ../../Config.site

TARGETS = $(OT_LIB_DIR)/libOpenTracker.a

HFILES = \
        CommonNodeFactory.h \
        ConsoleModule.h \
        ConsoleSink.h \
        ConsoleSource.h \
        DynamicTransformation.h \
        EventQueueNode.h \
        MergeNode.h \
        StaticTransformation.h \
        Transformation.h \
        VirtualTransformation.h

        
C++FILES = \
        CommonNodeFactory.cxx \
        ConsoleModule.cxx \
        DynamicTransformation.cxx \
        EventQueueNode.cxx \
        MergeNode.cxx \
        StaticTransformation.cxx \
        Transformation.cxx \
        VirtualTransformation.cxx

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
