

TARGET: default

#include system specific makefile settings
include ../../Config.global
include ../../Config.site

# use configuration options to compile these files
PRVFLAGS = $(OT_CONFIG_OPT)

TARGETS = $(OT_LIB_DIR)/libOpenTracker.a

HFILES = 
        
C++FILES = 

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
