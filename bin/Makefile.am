## Makefile.am -- Process this file with automake to produce Makefile.in

include $(top_srcdir)/src/Makefile.common

if WANT_OTQT
otqt_PROGS = otqt-mem-calib
endif

bin_PROGRAMS = opentracker opentracker-config $(otqt_PROGS)

opentracker_SOURCES =  $(top_srcdir)/src/misc/main.cxx
opentracker_LDADD = $(top_builddir)/lib/libOpenTracker.la @stb_ac_ace_libs@ @stb_ac_xerces_libs@ @stb_ac_artoolkit_libs@ -L/usr/X11R6/lib -L/usr/X11R6/lib64 -lX11
opentracker_LDFLAGS = -L$(top_builddir)/lib @stb_ac_ace_ldflags@ @stb_ac_xerces_ldflags@ @stb_ac_artoolkit_ldflags@ -no-install

opentracker_config_SOURCES = opentracker-config.in

#ottest_SOURCES =  $(top_srcdir)/src/misc/test.cxx
#ottest_LDADD = $(top_builddir)/lib/libOpenTracker.la @stb_ac_ace_libs@ @stb_ac_xerces_libs@ @stb_ac_artoolkit_libs@
#ottest_LDFLAGS = -L$(top_builddir)/lib @stb_ac_ace_ldflags@ @stb_ac_xerces_ldflags@ @stb_ac_artoolkit_ldflags@ -no-install

opentracker-config$(EXEEXT): $(top_builddir)/config.status opentracker-config.in
	@$(SHELL) $(top_builddir)/config.status --file=$@:opentracker-config.in
	@chmod 755 opentracker-config

otqt_mem_calib_SOURCES = \
	$(top_srcdir)/src/otqt/otqt_mem_calib_main.cxx
otqt_mem_calib_LDADD = \
	$(top_builddir)/lib/libOpenTracker.la
otqt_mem_calib_LDFLAGS = \
	-L$(top_builddir)/lib \
	-no-install
