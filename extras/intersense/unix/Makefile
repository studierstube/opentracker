#!smake

TARGET: default

include /usr/include/make/commondefs

include $(COMMONRULES)

TARGETS = ../../../lib/libOpenTracker.a

HFILES = \

CFILES = \
 isdriver.c \
 isense.c \
 itcom.c \
 serial.c \
 timer.c

LC++OPTS= $(SYS_C++OPTS)

# LC++INCS= $(STB_STD_INCLUDES) -I$(STB_SRC_DIR)

LC++OPTS= $(SYS_C++OPTS)

CINC = \
 -I/usr/include/ \
 -Idriver 

LCOPTS = \
 -g3 \
 -nostdinc \
 -mips3 -n32 \
 -MDupdate Makedepend \
 -woff 1685,515,608,658,799,803,852,1048,1233,1499,1178,1174 \
 $(NULL)

COFILES = $(CFILES:.c=.o)

LD_QUICKSTART_INFO=

$(CFILES:.c=.o):
	/usr/bin/CC $(CINC) $(LCOPTS) -o $@ -c $(@:.o=.c)

$(TARGETS): $(OBJ) $(COFILES)
	ar -cru $(TARGETS) $(COFILES)

update: $(TARGETS)
	@ar -cru $(TARGETS) $(COFILES)

default:  update $(TARGETS)

