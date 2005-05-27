## Makefile.am -- Process this file with automake to produce Makefile.in

include $(top_srcdir)/src/Makefile.common

noinst_PROGRAMS = workspace

workspace_SOURCES = $(top_srcdir)/src/stbapi/workspace/workspace.cxx
workspace_LDADD = $(top_builddir)/lib/libStbAPI.la

workspace_LDFLAGS = -L$(top_builddir)/lib @sogui_ldflags@ -no-install

