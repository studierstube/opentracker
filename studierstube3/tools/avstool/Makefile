#	"@(#)Makefile	8.11 AVS 93/10/28"
#			Copyright 1989,1990,1991,1992 by
#			Advanced Visual Systems Inc.
#			All Rights Reserved
#	
#	This software comprises unpublished confidential information of
#	Advanced Visual Systems Inc. and may not be used, copied or made
#	available to anyone, except in accordance with the license
#	under which it is furnished.
#	
#	This file is under sccs control at AVS in:
#	/nfs/caffeine/root/src1/sccs/avs/examples/s.Makefile
#	
# AVS example modules Makefile for Generic-AVS
#	
# There are some definitions like INC_FILE, F77_INC, etc that do not
# ordinarily need to be separated out - but this Makefile may be used
# from a variety of build environments.

# If AVS is not installed in /usr/avs, then set the AVS_PATH environment
# variable and override it during the make as in:
#    setenv AVS_PATH /my_dir/avs
#    make -e

AVS_PATH=/usr/cg/avs
INC_FILE=$(AVS_PATH)/include/Makeinclude
MLIB=-lm
include $(INC_FILE)

AVS_LIBS=$(AVS_PATH)/lib
BASELIBS=-lgeom -lutil -lm
FBASELIBS=-lgeom -lutil
FLOWLIBS=-L$(AVS_LIBS) -lflow_c $(BASELIBS) $(LASTLIBS)
CSIMLIBS=-L$(AVS_LIBS) -L$(CG_LIBS) -lsim_c $(BASELIBS) $(LASTLIBS)
MODLIBS=-L$(AVS_LIBS) -lmdata -lmfilt -lmmapp -lmrend -lrf
AVS_INC=-I. -I$(AVS_PATH)/include
CFLAGS=$(ACFLAGS) $(AVS_INC) -D_BSD -D_BSD_INCLUDES -Dsgi $(AVS_INC)
CC=cc

# In order to use C++, you will need to modify the CPP and CPPLIBS definitions
# to match your local system conventions.

CPP_MAIN_LIB=-lC -LD_MSG:off=84
CPPFLOWLIBS=$(FLOWLIBS) -lc $(CPP_MAIN_LIB) -lc
CPPSIMLIBS=$(CSIMLIBS) -lc $(CPP_MAIN_LIB) -lc
CPPHOME=/usr
CPPLIBS=-L$(CPPHOME)/lib
CPP=$(CPPHOME)/bin/CC
CPPFLAGS=$(CFLAGS) $(CPPLIBS)


AVS_EXE = $(AVS_PATH)/bin/avs
AVS_MOD= stbWin stbGeom stbInt stbFloat stbBool stbPipgroup stbPoint stbLine stbUser stbDist stbPolyline


.SUFFIXES: .o .C .cpp .c

.c.o: $(*.c)
	$(CC) $(CFLAGS) -c $(*c).c


.C.o: $*.C
	$(CPP) $(CFLAGS) -c $*.C

.cpp.o: $*.cpp
	$(CPP) $(CFLAGS) -c $*.cpp

.cxx.o: $(*.cxx)
	$(CPP) $(CFLAGS) -c $(*.cxx).cxx

CXXFILES= stbFifo.o Geom2SerIV.o


all: $(AVS_MOD) mod_lib

clobber: 
	rm -f *.o $(AVS_MOD) core stbModules
#
# Make sure to clean all out files that were created so that pkgrm can
# remove this directory
#
clean:
	rm -f *.o $(AVS_MOD) core stbModules



#**********************************************************************
# Modules and support files
stbWin: stbWin.o stbComm.o stbFifo.o
	$(CC) $(CPPFLAGS) -o stbWin stbComm.o stbFifo.o stbWin.o $(CPPSIMLIBS)

stbDist: stbDist.o stbComm.o stbFifo.o
	$(CC) $(CPPFLAGS) -o stbDist stbComm.o stbFifo.o stbDist.o $(CPPSIMLIBS)

stbUser: stbUser.o
	$(CC) $(CPPFLAGS) -o stbUser stbUser.o $(CPPFLOWLIBS)

stbGeom: stbGeom.o Geom2SerIV.o stbComm.o
	$(CC) $(CPPFLAGS) -o stbGeom Geom2SerIV.o stbFifo.o stbGeom.o stbComm.o $(CPPFLOWLIBS)

stbInt: stbInt.o stbComm.o stbFifo.o
	$(CC) $(CPPFLAGS) -o stbInt stbComm.o stbFifo.o stbInt.o $(CPPFLOWLIBS)

stbFloat: stbFloat.o stbComm.o stbFifo.o
	$(CC) $(CPPFLAGS) -o stbFloat stbComm.o stbFifo.o stbFloat.o $(CPPFLOWLIBS)

stbBool: stbBool.o stbComm.o stbFifo.o
	$(CC) $(CPPFLAGS) -o stbBool stbComm.o stbFifo.o stbBool.o $(CPPFLOWLIBS)

stbPipgroup: stbPipgroup.o stbComm.o stbFifo.o
	$(CC) $(CPPFLAGS) -o stbPipgroup stbComm.o stbFifo.o stbPipgroup.o $(CPPFLOWLIBS)

stbPoint: stbPoint.o stbComm.o stbFifo.o
	$(CC) $(CPPFLAGS) -o stbPoint stbComm.o stbFifo.o stbPoint.o $(CPPFLOWLIBS)

stbLine: stbLine.o stbComm.o stbFifo.o
	$(CC) $(CPPFLAGS) -o stbLine stbComm.o stbFifo.o stbLine.o $(CPPFLOWLIBS)

stbPolyline: stbPolyline.o stbComm.o stbFifo.o
	$(CC) $(CPPFLAGS) -o stbPolyline stbComm.o stbFifo.o stbPolyline.o $(CPPFLOWLIBS)


#**********************************************************************
# This target will create the StbModules Module Library file using
# the "compile_library" option to avs.

mod_lib: $(AVS_MOD)
	rm -f stbModules
	$(AVS_EXE) -compile_library stbMod.src stbModules

CFILES = `ls *.c`
lint:
	$(LINT) $(LINTFLAGS) $(AVS_INC) $(CFILES) > stb.lint

