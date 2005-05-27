## Makefile.am -- Process this file with automake to produce Makefile.in

include $(top_srcdir)/src/Makefile.common

noinst_PROGRAMS = sman2

sman2_SOURCES = \
        Application.cxx \
        Application.h \
        DIVAddressFactory.cxx \
        DIVAddressFactory.h \
        Host.cxx \
        Host.h \
        Locale.cxx \
        Locale.h \
        MonitorHandler.cxx \
        MonitorHandler.h \
        SessionHandler.cxx \
        SessionHandler.h \
        SessionManager.cxx \
        SessionManager.h \
        SessionVisitorAdaptor.cxx \
        SessionVisitorAdaptor.h \
        User.cxx \
        User.h \
        main.cxx

sman2_LDADD = $(top_builddir)/lib/libStbAPI.la -lGLU \
      @LIBLTDL@ @LIBADD_DL@ @sogui_libs@
sman2_LDFLAGS = @sogui_ldflags@
