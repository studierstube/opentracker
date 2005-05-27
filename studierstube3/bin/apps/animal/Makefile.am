## Makefile.am -- Process this file with automake to produce Makefile.in

include $(top_srcdir)/src/Makefile.common

lib_LTLIBRARIES = animal_stb.la

animal_stb_la_SOURCES = \
        $(top_srcdir)/src/apps/animal/SoAnimalKit.cxx \
        $(top_srcdir)/src/apps/animal/SoAnimalKit.h

animal_stb_la_LDFLAGS = -module -avoid-version
