## ===========================================================================
##  NAME:       Makefile
##  TYPE:       makefile
##  CONTENT:    Makefile for sman
## ===========================================================================
##  AUTHORS:    jp      Jan Prikryl
## ===========================================================================
##  HISTORY:
##
##  22-Jan-01 16:42:27  jp      last modification
##  28-May-00 19:38:50  jp      created
## ===========================================================================

TARGET: default

include ../../Config.global
include ../../Config.site

PROGRAM  = sman

HFILES   = \
	sman.h

C++FILES = \
	sman.cxx \
	main.cxx

OFILES   = $(C++FILES:.cxx=.o)

TARGETS  = $(PROGRAM)

STB_LIB  = $(STB_LIB_DIR)/libStbAPI.a

TARGETS  = $(PROGRAM)

default: $(TARGETS) $(PROGRAM) 

$(PROGRAM): $(OFILES) $(WORKSPACE_LIB) $(STB_LIB)
	@echo "--- $(PROGRAM) ---"
	$(CXXLINK) $(CXXFLAGS) -o $@ $(OFILES) $(STB_LIB) $(LDFLAGS)

$(STB_LIB):
	(cd ../.. && $(MAKE))

sinclude $(DEPFILES)
