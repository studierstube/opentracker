## ===========================================================================
##  Copyright (c) 1996,1997,1998,1999,2000
##  Vienna University of Technology
##  
##  This software is provided AS IS, without any express or implied
##  warranty.  In no event will the authors or the Vienna University of
##  Technology be held liable for any damages or loss of profit arising
##  from the use or non-fitness for a particular purpose of this software.
## ===========================================================================
##  NAME:       Makefile
##  TYPE:       makefile
##  PROJECT:    Studierstube - Undistortion tools
##  CONTENT:    Makefile for various undistortion tools
## ===========================================================================
##  AUTHORS:    jp      Jan Prikryl
## ===========================================================================
##  HISTORY:
##
##  01-Feb-01 19:06:14  jp      last modification
##  29-May-00 10:38:50  jp      created
## ===========================================================================

TARGET: default

include ../../Config.global
include ../../Config.site

PROGRAMS = fspext tmeas raw2sample raw2fsp tolookup

FSPEXT_C++FILES = \
	fspext.cxx

TMEAS_C++FILES = \
	tmeas.cxx

RAW2SAMPLE_C++FILES = \
	raw2sample.cxx

RAW2FSP_C++FILES = \
	raw2fsp.cxx

TOLOOKUP_C++FILES = \
	tolookup.cxx

TMEAS_C++FILES = \
	tmeas.cxx

FSPEXT_OFILES     = $(FSPEXT_C++FILES:.cxx=.o)
TMEAS_OFILES      = $(TMEAS_C++FILES:.cxx=.o)
RAW2SAMPLE_OFILES = $(RAW2SAMPLE_C++FILES:.cxx=.o)
RAW2FSP_OFILES    = $(RAW2FSP_C++FILES:.cxx=.o)
TMEAS_OFILES      = $(TMEAS_C++FILES:.cxx=.o)
TOLOOKUP_OFILES   = $(TOLOOKUP_C++FILES:.cxx=.o)

STB_LIB  = $(STB_LIB_DIR)/libStbAPI.a

# GNU/Linux requires -D_BOOL 
PRVFLAGS = -D_BOOL 

# This determines what shall be deleted by "make clobber"
TARGETS = $(PROGRAMS)

default: $(PROGRAMS) 

tmeas: $(TMEAS_OFILES) $(STB_LIB) $(WORKSPACE_LIB)
	$(CXXLINK) $(CXXFLAGS) -o $@ $(TMEAS_OFILES) $(STB_LIB) $(LDFLAGS)

raw2sample: $(RAW2SAMPLE_OFILES)
	$(CXXLINK) $(CXXFLAGS) -o $@ $(RAW2SAMPLE_OFILES) $(LDFLAGS)

# This is potentially incorrect as it ignores the dependency
# generation rules; see Config.Linux and Config.Irix for details
raw2fsp.o: raw2sample.cxx
	@echo "--- raw2sample.cxx ---"
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -D_RAW2FSP_ -c -o $@ raw2sample.cxx

raw2fsp: $(RAW2FSP_OFILES)
	$(CXXLINK) $(CXXFLAGS) -o $@ $(RAW2FSP_OFILES) $(LDFLAGS)

tolookup: $(TOLOOKUP_OFILES)
	$(CXXLINK) -D_BOOL $(CXXFLAGS) -o $@ $(TOLOOKUP_OFILES) $(LDFLAGS)

fspext: $(FSPEXT_OFILES)
	$(CXXLINK) $(CXXFLAGS) -o $@ $(FSPEXT_OFILES) $(LDFLAGS)

$(STB_LIB): $(_FORCE)
	( cd ../../src/stbapi && $(MAKE) )

sinclude $(DEPFILES)
