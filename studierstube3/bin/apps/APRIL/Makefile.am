## Makefile.am -- Process this file with automake to produce Makefile.in

include $(top_srcdir)/src/Makefile.common

lib_LTLIBRARIES = april.la

april_la_SOURCES = \
        $(top_srcdir)/src/apps/APRIL/SAXReadHandler.cxx \
        $(top_srcdir)/src/apps/APRIL/SAXReadHandler.h \
        $(top_srcdir)/src/apps/APRIL/SoLightProjector.cxx \
        $(top_srcdir)/src/apps/APRIL/SoLightProjector.h \
        $(top_srcdir)/src/apps/APRIL/SoStateEngine.cxx \
        $(top_srcdir)/src/apps/APRIL/SoStateEngine.h \
        $(top_srcdir)/src/apps/APRIL/StateEngine.cxx \
        $(top_srcdir)/src/apps/APRIL/StateEngine.h

april_la_LDFLAGS = -module -avoid-version
