## Makefile.am -- Process this file with automake to produce Makefile.in

include $(top_srcdir)/src/Makefile.common

otnetworkincdir = $(otincdir)/network
otnetworkinc_HEADERS = \
        Network.h \
        NetworkSink.h \
        NetworkSinkModule.h \
        NetworkSource.h \
        NetworkSourceModule.h \
        TCPModule.h \
        TCPSink.h \
		DwarfModule.h \
		DwarfSink.h \
		DwarfSource.h

noinst_LTLIBRARIES = libnetwork.la
libnetwork_la_SOURCES = \
        Network.h \
        NetworkSink.h \
        NetworkSinkModule.cxx \
        NetworkSinkModule.h \
        NetworkSource.h \
        NetworkSourceModule.cxx \
        NetworkSourceModule.h \
        TCPModule.cxx \
        TCPModule.h \
        TCPSink.h \
		DwarfModule.cxx \
		DwarfModule.h \
		DwarfSink.h \
		DwarfSource.h
