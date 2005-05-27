## Makefile.am -- Process this file with automake to produce Makefile.in

include $(top_srcdir)/src/Makefile.common

lib_LTLIBRARIES = vampire_stb.la

vampire_stb_la_SOURCES = \
        $(top_srcdir)/src/apps/vampire/SoVampireKit.cxx \
        $(top_srcdir)/src/apps/vampire/SoVampireKit.h

vampire_stb_la_LDFLAGS = -module -avoid-version
