## Makefile.am -- Process this file with automake to produce Makefile.in

include $(top_srcdir)/src/Makefile.common

lib_LTLIBRARIES = construct3d_stb.la

construct3d_stb_la_SOURCES = \
        $(top_srcdir)/src/apps/construct3d/c3d.cxx \
        $(top_srcdir)/src/apps/construct3d/c3d.h \
        $(top_srcdir)/src/apps/construct3d/CnDconstants.h \
        $(top_srcdir)/src/apps/construct3d/construct3d.h \
        $(top_srcdir)/src/apps/construct3d/GridKit.cxx \
        $(top_srcdir)/src/apps/construct3d/GridKit.h \
        $(top_srcdir)/src/apps/construct3d/MaterialConstants.h \
        $(top_srcdir)/src/apps/construct3d/MaterialConstants.cxx \
        $(top_srcdir)/src/apps/construct3d/Object3dKit.cxx \
        $(top_srcdir)/src/apps/construct3d/Object3dKit.h \
        $(top_srcdir)/src/apps/construct3d/SoBoolKit.cxx \
        $(top_srcdir)/src/apps/construct3d/SoBoolKit.h \
        $(top_srcdir)/src/apps/construct3d/SoCommandKit.cxx \
        $(top_srcdir)/src/apps/construct3d/SoCommandKit.h \
        $(top_srcdir)/src/apps/construct3d/SoConeKit.cxx \
        $(top_srcdir)/src/apps/construct3d/SoConeKit.h \
        $(top_srcdir)/src/apps/construct3d/SoCubeKit.cxx \
        $(top_srcdir)/src/apps/construct3d/SoCubeKit.h \
        $(top_srcdir)/src/apps/construct3d/SoCurveKit.cxx \
        $(top_srcdir)/src/apps/construct3d/SoCurveKit.h \
        $(top_srcdir)/src/apps/construct3d/SoCylinderKit.cxx \
        $(top_srcdir)/src/apps/construct3d/SoCylinderKit.h \
        $(top_srcdir)/src/apps/construct3d/SoIntersectKit.cxx \
        $(top_srcdir)/src/apps/construct3d/SoIntersectKit.h \
        $(top_srcdir)/src/apps/construct3d/SoLineKit.cxx \
        $(top_srcdir)/src/apps/construct3d/SoLineKit.h \
        $(top_srcdir)/src/apps/construct3d/SoPlaneKit.cxx \
        $(top_srcdir)/src/apps/construct3d/SoPlaneKit.h \
        $(top_srcdir)/src/apps/construct3d/SoPointKit.cxx \
        $(top_srcdir)/src/apps/construct3d/SoPointKit.h \
        $(top_srcdir)/src/apps/construct3d/SoPolygonKit.cxx \
        $(top_srcdir)/src/apps/construct3d/SoPolygonKit.h \
        $(top_srcdir)/src/apps/construct3d/SoSphereKit.cxx \
        $(top_srcdir)/src/apps/construct3d/SoSphereKit.h \
        $(top_srcdir)/src/apps/construct3d/SoSurfaceKit.cxx \
        $(top_srcdir)/src/apps/construct3d/SoSurfaceKit.h \
        $(top_srcdir)/src/apps/construct3d/SoTextKit.cxx \
        $(top_srcdir)/src/apps/construct3d/SoTextKit.h \
        $(top_srcdir)/src/apps/construct3d/SoUndoRedoListKit.cxx \
        $(top_srcdir)/src/apps/construct3d/SoUndoRedoListKit.h

INCLUDES = @stb_ac_acis_cppflags@
construct3d_stb_la_LIBADD = \
        @stb_ac_acis_rpath@ \
        @stb_ac_acis_ldflags@ \
        @stb_ac_acis_libs@

construct3d_stb_la_LDFLAGS = -module -avoid-version
