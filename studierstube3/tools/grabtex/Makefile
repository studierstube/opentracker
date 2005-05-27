## ===========================================================================
##  NAME:       Makefile
##  TYPE:       makefile
##  CONTENT:    Makefile for grabtex
## ===========================================================================
##  AUTHORS:    jp      Jan Prikryl
## ===========================================================================
##  HISTORY:
##
##  22-Aug-00 17:16:33  jp      last modification
##  29-May-00 10:38:50  jp      created
## ===========================================================================


TARGET: default

include ../../Config.global
include ../../Config.site

PROGRAM = grabtex

HFILES = 

C++FILES = \
	grabtex.cxx

OFILES  = $(C++FILES:.cxx=.o) $(INVENTOR_STUB_O)

STB_LIB = $(STB_LIB_DIR)/libStbAPI.a

# This determines what shall be deleted by "make clobber"
TARGETS = $(PROGRAM)

default: $(PROGRAM) 

$(PROGRAM): $(OFILES) $(STB_LIB)
	@echo "--- $(PROGRAM) ---"
	$(CXXLINK) $(CXXFLAGS) -o $@ $(OFILES) $(STB_LIB) $(LDFLAGS)

$(STB_LIB):
	(cd ../.. && $(MAKE))

sinclude $(DEPFILES)
