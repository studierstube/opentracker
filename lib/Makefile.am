## Makefile.am -- Process this file with automake to produce Makefile.in

include $(top_builddir)/src/Makefile.common

LIBS = \
        @CURSES_LIBS@ \
		@stb_ac_ace_ldflags@ @stb_ac_ace_libs@ \
		@stb_ac_artoolkit_ldflags@ @stb_ac_artoolkit_libs@ \
		@stb_ac_xerces_ldflags@ @stb_ac_xerces_libs@

lib_LTLIBRARIES = libOpenTracker.la

libOpenTracker_la_SOURCES =
libOpenTracker_la_LIBADD = \
        $(top_builddir)/src/common/libcommon.la \
        $(top_builddir)/src/core/libcore.la \
        $(top_builddir)/src/input/libinput.la \
        $(top_builddir)/src/misc/libmisc.la \
        $(top_builddir)/src/misc/xml/libxml.la \
        $(top_builddir)/src/network/libnetwork.la \
        $(top_builddir)/extras/intersense/unix/libintersense.la

libOpenTracker_la_LDFLAGS = \
        -no-undefined -version-info 0:1:0 \
        @stb_ac_ace_rpath@ \
        @stb_ac_artoolkit_rpath@ \
        @stb_ac_xerces_rpath@
