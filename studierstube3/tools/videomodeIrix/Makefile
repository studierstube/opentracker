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
##  PROJECT:    Studierstube
##  CONTENT:    Toplevel Makefile
## ===========================================================================
##  AUTHORS:    jp      Jan Prikryl
##              gh      Gerd Hesina
## ===========================================================================
##  HISTORY:
##
##  27-Nov-00 12:24:23  jp      last modification
##  22-Oct-99 18:11:12  gh      created
## ===========================================================================

TARGET: default

include ../../Config.global
include ../../Config.site

PROGRAM  = videomode

HFILES   = 

C++FILES = \
	videomode.cxx

OFILES   = $(C++FILES:.cxx=.o)

STB_LIB = $(STB_LIB_DIR)/libStbAPI.a

## What has shall "make clobber" delete
TARGETS = $(PROGRAM)

default: $(PROGRAM) 

$(PROGRAM): $(OFILES)
	@echo "--- $(PROGRAM) ---"
	$(CXXLINK) $(CXXFLAGS) -o $@ $(OFILES) $(LDFLAGS)

sinclude $(DEPFILES)
