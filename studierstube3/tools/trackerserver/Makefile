## ===========================================================================
##  NAME:       Makefile
##  TYPE:       makefile
##  CONTENT:    Makefile for the tracker server and port forwarder
## ===========================================================================
##  AUTHORS:    jp      Jan Prikryl
## ===========================================================================
##  HISTORY:
##
##  25-Aug-00 15:15:16  jp      last modification
##  29-May-00 10:38:50  jp      created
## ===========================================================================

TARGET: default

include ../../Config.global
include ../../Config.site

PROGRAM = trackerserver

HFILES   =

C++FILES = \
	TServer.o \
	NetComm.o \
	SerComm.o \
	ParComm.o \
	CfgOpts.o \
	Fob.o \
	Polhemus.o \
	NewKFilt.o

OFILES   = $(C++FILES:.cxx=.o)

TARGETS  = $(PROGRAM)

PRVFLAGS = -I../../src/stbapi

default: $(TARGETS) $(PROGRAM) 

$(PROGRAM): $(OFILES) $(STB_LIB)
	@echo "--- $(PROGRAM) ---"
	$(CXXLINK) $(CXXFLAGS) -o $@ $(OFILES) $(LDFLAGS) -lncurses

sinclude $(DEPFILES)