## Makefile.am -- Process this file with automake to produce Makefile.in

include $(top_srcdir)/src/Makefile.common

SUBDIRS = xml

otmiscincdir = $(otincdir)/misc
otmiscinc_HEADERS = \
        serialcomm.h \
	XKeys.h \
	GeoidModel.h \
	GPSGeoid.h \
	WGS84Geoid.h

noinst_LTLIBRARIES = libmisc.la
libmisc_la_SOURCES = \
        OpenTracker.cxx \
        serialcomm.cxx \
	XKeys.cxx \
        serialcomm.h
