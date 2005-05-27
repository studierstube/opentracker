##----------------------------------------------------------------------------
##       Makefile.am,v 4.2 2004/01/22 06:07:50 ossama Exp
##
##       Makefile for repeating token client application
##----------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

AM_CPPFLAGS = -I$(top_builddir) -I$(top_srcdir)

noinst_PROGRAMS = \
  collection

collection_SOURCES = collection.cpp
collection_LDADD = $(top_builddir)/netsvcs/lib/libnetsvcs.la \
                   $(top_builddir)/ace/libACE.la
