## Makefile.am -- Process this file with automake to produce Makefile.in

include $(top_srcdir)/src/Makefile.common

noinst_LTLIBRARIES = libvideo.la
libvideo_la_SOURCES = \
	dc1394wrapper.cxx \
	dc1394wrapper.h \
        H323Utils.cxx \
        H323Utils.h \
        SoVideoBackground.cxx \
        SoVideoBackground.h \
        SoVideoBackgroundOT.cxx \
        SoVideoBackgroundOT.h \
        SoVideoBackgroundSGI.cxx \
        SoVideoBackgroundSGI.h \
        SoVideoBackgroundUSB.cxx \
        SoVideoBackgroundUSB.h \
        SoVideoBackgroundIEEE1394.h \
        SoVideoBackgroundIEEE1394.cxx \
        SoVideoBackgroundDC1394.h \
        SoVideoBackgroundDC1394.cxx \
        SoVideoBackgroundH323.h \
        SoVideoBackgroundH323.cxx

INCLUDES = @stb_ac_h323_cppflags@
