## Makefile.am -- Process this file with automake to produce Makefile.in

include $(top_srcdir)/src/Makefile.common

bin_SCRIPTS = opentracker-config

noinst_PROGRAMS = opentracker ottest opentracker-config

opentracker_SOURCES =  $(top_srcdir)/src/misc/main.cxx
opentracker_LDADD = $(top_builddir)/lib/libOpenTracker.la @stb_ac_ace_libs@ @stb_ac_xerces_libs@ @stb_ac_artoolkit_libs@
opentracker_LDFLAGS = -L$(top_builddir)/lib @stb_ac_ace_ldflags@ @stb_ac_xerces_ldflags@ @stb_ac_artoolkit_ldflags@ -no-install

ottest_SOURCES =  $(top_srcdir)/src/misc/test.cxx
ottest_LDADD = $(top_builddir)/lib/libOpenTracker.la @stb_ac_ace_libs@ @stb_ac_xerces_libs@ @stb_ac_artoolkit_libs@
ottest_LDFLAGS = -L$(top_builddir)/lib @stb_ac_ace_ldflags@ @stb_ac_xerces_ldflags@ @stb_ac_artoolkit_ldflags@ -no-install

opentracker-config$(EXEEXT): $(top_builddir)/config.status opentracker-config.in
	@$(SHELL) $(top_builddir)/config.status --file=$@:opentracker-config.in
	@chmod 755 opentracker-config
