## Makefile.am -- Process this file with automake to produce Makefile.in

include $(top_srcdir)/src/Makefile.common

lib_LTLIBRARIES = chess_stb.la

chess_stb_la_SOURCES = \
        $(top_srcdir)/src/apps/chess/ChessKit.cxx \
        $(top_srcdir)/src/apps/chess/ChessKit.h \
        $(top_srcdir)/src/apps/chess/ChessManKit.cxx \
        $(top_srcdir)/src/apps/chess/ChessManKit.h \
        $(top_srcdir)/src/apps/chess/gnuchess/gnuchess.h \
        $(top_srcdir)/src/apps/chess/gnuchess/nondsp.cxx \
        $(top_srcdir)/src/apps/chess/gnuchess/gnuchess.cxx

chess_stb_la_LDFLAGS = -module -avoid-version
