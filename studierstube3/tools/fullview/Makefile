## ===========================================================================
##  NAME:       Makefile
##  TYPE:       makefile
##  CONTENT:    Makefile for fullview
## ===========================================================================
##  AUTHORS:    jp      Jan Prikryl
## ===========================================================================
##  HISTORY:
##
##  19-Jan-01 16:07:07  jp      last modification
##  29-May-00 10:38:50  jp      created
## ===========================================================================

TARGET: default

include ../../Config.global
include ../../Config.site

PROGRAM  = fullview

HFILES   =

C++FILES = \
	fullview.cxx

OFILES   = $(C++FILES:.cxx=.o) $(INVENTOR_STUB_O)

STB_LIB  = $(STB_LIB_DIR)/libStbAPI.a

TARGETS  = $(PROGRAM)

default: $(TARGETS) $(PROGRAM) 

$(PROGRAM): $(OFILES) $(STB_LIB)
	@echo "--- $(PROGRAM) ---"
	$(CXXLINK) $(CXXFLAGS) -o $@ $(OFILES) $(STB_LIB) $(LDFLAGS) 

$(STB_LIB):
	(cd ../.. && $(MAKE))

sinclude $(DEPFILES)

