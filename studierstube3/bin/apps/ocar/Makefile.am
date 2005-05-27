## Makefile.am -- Process this file with automake to produce Makefile.in

include $(top_srcdir)/src/Makefile.common

lib_LTLIBRARIES = ocar.la

ocar_la_SOURCES = \
        $(top_srcdir)/src/apps/ocar/AnnotationContext.cxx \
        $(top_srcdir)/src/apps/ocar/AnnotationContext.h \
        $(top_srcdir)/src/apps/ocar/CalibrationContext.cxx \
        $(top_srcdir)/src/apps/ocar/CalibrationContext.h \
        $(top_srcdir)/src/apps/ocar/NavigationContext.cxx \
        $(top_srcdir)/src/apps/ocar/NavigationContext.h \
        $(top_srcdir)/src/apps/ocar/UserContext.cxx \
        $(top_srcdir)/src/apps/ocar/UserContext.h

INCLUDES = @stb_ac_boost_cppflags@
ocar_la_LIBADD = \
        @stb_ac_boost_rpath@ \
        @stb_ac_boost_ldflags@ \
        @stb_ac_boost_libs@

ocar_la_LDFLAGS = -module -avoid-version
