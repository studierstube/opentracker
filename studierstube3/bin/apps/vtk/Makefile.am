## Makefile.am -- Process this file with automake to produce Makefile.in

include $(top_srcdir)/src/Makefile.common

lib_LTLIBRARIES = vtk_stb.la

vtk_stb_la_SOURCES = \
        $(top_srcdir)/src/apps/vtk/SoVtkKit.h \
        $(top_srcdir)/src/apps/vtk/SoVtkKit.cxx \
        $(top_srcdir)/src/apps/vtk/SoVTKOpenGLRenderer.h \
        $(top_srcdir)/src/apps/vtk/SoVTKOpenGLRenderer.cxx \
        $(top_srcdir)/src/apps/vtk/SoVTKOpenGLRenderWindow.h \
        $(top_srcdir)/src/apps/vtk/SoVTKOpenGLRenderWindow.cxx \
        $(top_srcdir)/src/apps/vtk/SoVTKProxyCamera.h \
        $(top_srcdir)/src/apps/vtk/SoVTKProxyCamera.cxx \
        $(top_srcdir)/src/apps/vtk/SoVTKProxyProp3D.h \
        $(top_srcdir)/src/apps/vtk/SoVTKProxyProp3D.cxx \
        $(top_srcdir)/src/apps/vtk/vtk.h \
        $(top_srcdir)/src/apps/vtk/vtkOpenGLVolumeShearWarpMapper.cxx \
        $(top_srcdir)/src/apps/vtk/vtkOpenGLVolumeShearWarpMapper.h \
        $(top_srcdir)/src/apps/vtk/vtkVolumeShearWarpMapper.cxx \
        $(top_srcdir)/src/apps/vtk/vtkVolumeShearWarpMapper.h

INCLUDES = @stb_ac_vtk_cppflags@
vtk_stb_la_LIBADD = \
        @stb_ac_vtk_rpath@ \
        @stb_ac_vtk_ldflags@ \
        @stb_ac_vtk_libs@

vtk_stb_la_LDFLAGS = -module -avoid-version
