

TARGET: default

#include system specific makefile settings
include ../../Config.global
include ../../Config.site

# use configuration options to compile these files
PRVFLAGS = $(OT_OPTIONAL_INC)

TARGETS = $(OT_LIB_DIR)/libOpenTracker.a

HFILES = \
        ARToolKitModule.h \
        ARToolKitSource.h \
        CyberMouseModule.h \
        CyberMouseSource.h \
        InterTraxModule.h \
        InterTraxSource.h \
        WacomGraphireModule.h \
        WacomGraphireSource.h \
        InterSenseSource.h \
        InterSenseModule.h \
        FOBModule.h \
        FOBSource.h \
        ParButtonSource.h \
        ParButtonModule.h \
        FastTrakModule.h \
        FastTrakSource.h
        
C++FILES = \
        ARToolKitModule.cxx \
        CyberMouseModule.cxx \
        InterTraxModule.cxx \
        WacomGraphireModule.cxx \
        InterSenseModule.cxx \
        FOBModule.cxx \
        ParButtonModule.cxx \
        FastTrakModule.cxx

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
