## ===========================================================================
##  Copyright (c) 2001
##  Vienna University of Technology
##  
##  This software is provided AS IS, without any express or implied
##  warranty.  In no event will the authors or the Vienna University of
##  Technology be held liable for any damages or loss of profit arising
##  from the use or non-fitness for a particular purpose of this software.
## ===========================================================================
##  NAME:       Makefile
##  TYPE:       makefile
##  PROJECT:    OpenTracker
##  CONTENT:    Makefile for OpenTracker API libraries
## ===========================================================================
##  AUTHORS:    jp      Jan Prikryl
##              gh      Gerd Hesina
##              gr      Gerhard Reitmayr
## ===========================================================================
##  HISTORY:
##
##  24-Mar-01 22:00:00  gr      copied from studierstube 
##  15-Dec-00 14:36:55  jp      last modification
##  22-Oct-99 18:11:12  gh      created
## ===========================================================================

default: all

include ../Config.global
include ../Config.site

SUBDIRS = \
    ./core \
    ./common \
    ./input \
    ./output \
    ./network \
    ./misc

## ---------------------------------------------------------------------------
## BUILD RULES
## ---------------------------------------------------------------------------

all: $(_FORCE)
	@for i in $(SUBDIRS) ; do \
	  echo "=== $$i ===" ; \
	  ( cd $$i && $(MAKE) $(NOPRINT)) || exit 1 ; \
	done

## ===========================================================================
##  End of Makefile
## ===========================================================================
