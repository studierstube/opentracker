

TARGET: default

#include system specific makefile settings
include ../../../Config.global
include ../../../Config.site

# use configuration options to compile these files
PRVFLAGS = $(OT_OPTIONAL_INC)

TARGETS = $(OT_LIB_DIR)/libOpenTracker.a

HFILES = \
        isdriver.h \
        isense.h \
        itcom.h \
        serial.h \
        timer.h
        
CFILES = \
        isdriver.c \
        isense.c \
        itcom.c \
        serial.c \
        timer.c 
   
LCINCS= $(OT_STD_INCLUDES)

LCOPTS= $(SYS_COPTS)

OBJ = $(CFILES:.c=.o)

LD_QUICKSTART_INFO=

$(TARGETS): $(OBJ)
	@echo "--- $(TARGETS) ---"
	ar -cru $(TARGETS) $(OBJ)

update: $(TARGETS)
	@echo "--- $(TARGETS) (update) ---"
	ar -cru $(TARGETS) $(OBJ)

default: $(TARGETS)

sinclude $(DEPFILES)
