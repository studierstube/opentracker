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
##  PROJECT:    XF86 videmode switch for iGlasses
##  CONTENT:    Makeile for the mode switch
## ===========================================================================
##  AUTHORS:    jp      Jan Prikryl
## ===========================================================================
##  HISTORY:
##
##  22-Aug-00 18:34:07  jp      last modification
##  01-Jan-00  1:44:16  jp      created
## ===========================================================================

TARGET: default

include ../../Config.global

CC     = gcc
CFLAGS = -I/usr/X11R6/include
LDLIBS = -L/usr/X11R6/lib -lX11 -lXext -lXxf86vm 

TARGETS = setmode

default: $(TARGETS)

setmode: setmode.o
	$(CC) -o $@ $< $(LDLIBS)

## ===========================================================================
##  End of Makefile
## ===========================================================================
