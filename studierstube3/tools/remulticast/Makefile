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
##  PROJECT:    Studierstube - General unicast to multicast forwader
##  CONTENT:    Makefile for the forwarder
## ===========================================================================
##  AUTHORS:    jp      Jan Prikryl
## ===========================================================================
##  HISTORY:
##
##  29-Aug-00 10:48:27  jp      last modification
##  25-Aug-00 15:21:18  jp      created
## ===========================================================================

TARGET: default

include ../../Config.global
include ../../Config.site

PROGRAM  = remulticast

HFILES   =

C++FILES = \
	remulticast.cxx

OFILES   = $(C++FILES:.cxx=.o)

TARGETS  = $(PROGRAM)

PRVFLAGS = -I../../src/stbapi

default: $(TARGETS) $(PROGRAM) 

$(PROGRAM): $(OFILES) 
	@echo "--- $(PROGRAM) ---"
	$(CXX) $(CXXFLAGS) -o $@ $(OFILES) $(LDFLAGS)

sinclude $(DEPFILES)

## ===========================================================================
##  End of Makefile
## ===========================================================================
