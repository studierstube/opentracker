

TARGET: default

#include system specific makefile settings
include ../../../Config.global
include ../../../Config.site

TARGETS = $(OT_LIB_DIR)/libOpenTracker.a

HFILES = \
        XMLWriter.h


C++FILES = \
        XMLWriter.cxx

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
