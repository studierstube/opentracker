## Makefile.am -- Process this file with automake to produce Makefile.in

include $(top_srcdir)/src/Makefile.common

noinst_PROGRAMS = main test

main_SOURCES =  $(top_srcdir)/src/misc/main.cxx
main_LDADD = $(top_builddir)/lib/libOpenTracker.la @stb_ac_ace_libs@ @stb_ac_xerces_libs@ @stb_ac_artoolkit_libs@
main_LDFLAGS = -L$(top_builddir)/lib @stb_ac_ace_ldflags@ @stb_ac_xerces_ldflags@ @stb_ac_artoolkit_ldflags@

test_SOURCES =  $(top_srcdir)/src/misc/test.cxx
test_LDADD = $(top_builddir)/lib/libOpenTracker.la @stb_ac_ace_libs@ @stb_ac_xerces_libs@ @stb_ac_artoolkit_libs@
test_LDFLAGS = -L$(top_builddir)/lib @stb_ac_ace_ldflags@ @stb_ac_xerces_ldflags@ @stb_ac_artoolkit_ldflags@
