## ===========================================================================
##  NAME:       Makefile
##  TYPE:       makefile
##  CONTENT:    Makefile for mproxy
## ===========================================================================
##  AUTHORS:    gh      Gerd Hesina
## ===========================================================================
##  HISTORY:
##
##  14-Mar-01 16:42:27  gh      last modification
## ===========================================================================

TARGET: default

include ../../Config.global
include ../../Config.site

PROGRAM  = mproxy

HFILES   = \
	mproxy.h

C++FILES = \
	mproxy.cxx \
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
