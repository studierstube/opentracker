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
##  CONTENT:    Toplevel Makefile
## ===========================================================================
##  AUTHORS:    jp      Jan Prikryl
##              gr      Gerhard Reitmayr
## ===========================================================================
##  HISTORY:
##  
##  24-Mar-01 22:00:00  gr      copied from studierstube 
## ===========================================================================
##
##  Available targets:
##
##  * build targets
##
##    default:     builds everything
##    libs:        builds OpenTracker libraries
##    docs:        extracts the documentation using doxygen
##    all:         same as default
##
##  * clean & clobber targets #####
##
##    clean:       cleans all except target files
##    cleanlibs:   cleans OpenTracker libraries
##    cleanall:    same as clean
##    clobber:     cleans everything inclusive targets 
##    clobberlibs: clobbers OpenTracker libraries
##    clobberall:  same as clobber
##
## ===========================================================================

default: all

## .SUFFIXES: .c .cxx

include Config.global
sinclude Config.site

LIBSDIR = ./src
EXTRADIR = ./extras

## This is needed by the `clean' target. SUBDIRS contains all
## subdirectories that are to be visited

SUBDIRS = $(LIBSDIR) $(EXTRADIR) $(SUBDIRS_OSDEP)

## ---------------------------------------------------------------------------
## BUILD RULES
## ---------------------------------------------------------------------------

all: libs

sysconfig:
	@echo "=== Identifying build environment ==="
	@_un=`uname`; \
	case $$_un in \
	Linux) ln -sf Config.Linux Config.site; \
	       echo "You are running Linux";; \
	IRIX*) ln -sf Config.Irix  Config.site; \
	       echo "You are running Irix";; \
	*)     echo "ERROR: Unknown build environemnt!"; \
	       exit;; \
	esac

docs:
	cd doc && doxygen opentracker.dxy

libs: $(_FORCE) sysconfig extras
	@echo "=== Making OpenTracker libraries ==="
	cd $(LIBSDIR) && $(MAKE) $(NOPRINT)
	cd $(OT_LIB_DIR) && $(LDSHARED) -o libOpenTracker.so $(LDFLAGS)

extras: $(_FORCE) sysconfig
	@echo "=== Making extras ==="
	cd $(EXTRADIR) && $(MAKE) $(NOPRINT)

## ---------------------------------------------------------------------------
## CLEAN RULES
## ---------------------------------------------------------------------------


cleanlibs: $(_FORCE) sysconfig
	echo "=== Cleaning OpenTracker (except libraries itself) ==="
	cd $(LIBSDIR) && $(MAKE) $(NOPRINT) clean

clobberlibs: $(_FORCE) sysconfig
	echo "=== Removing OpenTracker libraries ==="
	cd $(LIBSDIR) && $(MAKE) $(NOPRINT) clobber

cleanextras: $(_FORCE) sysconfig
	echo "=== Cleaning extras (except executables) ==="
	cd $(EXTRADIR) && $(MAKE) $(NOPRINT) clean

clobberextras: $(_FORCE) sysconfig
	echo "=== Removing extras binaries ==="
	cd $(EXTRADIR) && $(MAKE) $(NOPRINT) clobber

clean-config:
	-rm Config.site

cleanall: clean

clobberall: clobber clean-config

## ===========================================================================
##  End of Makefile
## ===========================================================================
