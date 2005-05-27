## Makefile.am -- Process this file with automake to produce Makefile.in

include $(top_builddir)/src/Makefile.common

if USE_LIBCCVT
libccvt = -lccvt
else
libccvt =
endif

LIBS = \
	@stb_ac_ace_ldflags@ @stb_ac_ace_libs@ \
        @stb_ac_ot_ldflags@ @stb_ac_ot_libs@ \
        @stb_ac_h323_ldflags@ @stb_ac_h323_libs@ \
        @stb_ac_simvol_ldflags@ @stb_ac_simvol_libs@ \
        $(libccvt) -lGLU @LIBADD_DL@ @sogui_ldflags@ @sogui_libs@

lib_LTLIBRARIES = libStbAPI.la

libStbAPI_la_SOURCES =
libStbAPI_la_LIBADD = \
        $(top_builddir)/libltdl/libltdlc.la \
        $(top_builddir)/src/stbapi/context/libcontext.la \
        $(top_builddir)/src/stbapi/div/libdiv.la \
        $(top_builddir)/src/stbapi/event3d/libevent3d.la \
        $(top_builddir)/src/stbapi/interaction/libinteraction.la \
        $(top_builddir)/src/stbapi/misc/libmisc.la \
        $(top_builddir)/src/stbapi/resource/libresource.la \
        $(top_builddir)/src/stbapi/resource/viewer/libviewer.la \
        $(top_builddir)/src/stbapi/resource/viewer/video/libvideo.la \
        $(top_builddir)/src/stbapi/seams/libseams.la \
        $(top_builddir)/src/stbapi/tracker/libtracker.la \
        $(top_builddir)/src/stbapi/util/libutil.la \
        $(top_builddir)/src/stbapi/vc/libvc.la \
        $(top_builddir)/src/stbapi/widgets/libwidgets.la \
        $(top_builddir)/src/stbapi/window3d/libwindow3d.la \
        $(top_builddir)/src/stbapi/workspace/libworkspace.la \
        $(top_builddir)/src/stbapi/workspace/distrManager/libdistrManager.la

libStbAPI_la_LDFLAGS = \
        -no-undefined -avoid-version \
        @stb_ac_ace_rpath@ \
        @stb_ac_ot_rpath@ \
        @stb_ac_simvol_rpath@

