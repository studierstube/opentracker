
TARGET: default

include ../../Config.global
include ../../Config.site

# use configuration options to compile these files
PRVFLAGS = $(OT_OPTIONAL_INC)

OPENTRACKER   = $(OT_BIN_DIR)/opentracker
TEST          = $(OT_BIN_DIR)/ottest
OT_LIB        = $(OT_LIB_DIR)/libOpenTracker.a

PROGRAM = $(OPENTRACKER) $(OT_LIB) $(TEST)

HFILES =

C++FILES = \
        main.cxx \
        OpenTracker.cxx \
        test.cxx

TEST_OBJ = test.o

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

$(TEST): $(TEST_OBJ) $(OT_LIB)
	@echo "--- $(TEST) ---"
	$(CXXLINK) $(CXXFLAGS) -o $@ $(TEST_OBJ) $(LDFLAGS)

sinclude $(DEPFILES)
