
TARGET: default

include ../../Config.global
include ../../Config.site

# use configuration options to compile these files
PRVFLAGS = $(OT_OPTIONAL_INC)

OPENTRACKER   = $(OT_BIN_DIR)/opentracker
OT_LIB        = $(OT_LIB_DIR)/libOpenTracker.a

PROGRAM = $(OPENTRACKER) $(OT_LIB)

HFILES =

C++FILES = \
        main.cxx \
        OpenTracker.cxx

OPENTRACKER_OBJ = main.o

OT_LIB_OBJ = OpenTracker.o

OBJFILES = $(C++FILES:.cxx=.o)

TARGETS = $(PROGRAM) 

default: $(PROGRAM) 

$(OT_LIB): $(OT_LIB_OBJ)
	@echo "--- $(STB_LIB) ---"
	ar -cru $@ $(OT_LIB_OBJ)

$(OPENTRACKER): $(OPENTRACKER_OBJ) $(OT_LIB)
	@echo "--- $(OPENTRACKER) ---"
	$(CXXLINK) $(CXXFLAGS) -o $@ $(OPENTRACKER_OBJ) $(LDFLAGS)


sinclude $(DEPFILES)
