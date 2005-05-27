## Makefile.am -- Process this file with automake to produce Makefile.in

include $(top_srcdir)/src/Makefile.common

lib_LTLIBRARIES = vacuum_stb.la

vacuum_stb_la_SOURCES = \
        $(top_srcdir)/src/apps/vacuum/about_dialog.cc \
        $(top_srcdir)/src/apps/vacuum/about_dialog.hh \
        $(top_srcdir)/src/apps/vacuum/about_dialog_glade.cc \
        $(top_srcdir)/src/apps/vacuum/about_dialog_glade.hh \
        $(top_srcdir)/src/apps/vacuum/fileselection.cc \
        $(top_srcdir)/src/apps/vacuum/fileselection.hh \
        $(top_srcdir)/src/apps/vacuum/fileselection_glade.cc \
        $(top_srcdir)/src/apps/vacuum/fileselection_glade.hh \
        $(top_srcdir)/src/apps/vacuum/main_window.cc \
        $(top_srcdir)/src/apps/vacuum/main_window.hh \
        $(top_srcdir)/src/apps/vacuum/main_window_glade.cc \
        $(top_srcdir)/src/apps/vacuum/main_window_glade.hh \
        $(top_srcdir)/src/apps/vacuum/SoVacuumKit.cxx \
        $(top_srcdir)/src/apps/vacuum/SoVacuumKit.h

INCLUDES = @stb_ac_simvol_cppflags@ \
           -I/usr/include/gtkmm-2.0 -I/usr/lib/gtkmm-2.0/include \
           -I/usr/include/gtk-2.0 -I/usr/lib/sigc++-1.2/include \
           -I/usr/include/sigc++-1.2 -I/usr/include/glib-2.0 \
           -I/usr/lib/glib-2.0/include -I/usr/lib/gtk-2.0/include \
           -I/usr/include/pango-1.0 -I/usr/X11R6/include \
           -I/usr/include/freetype2 -I/usr/include/atk-1.0

vacuum_stb_la_LIBADD = \
        @stb_ac_simvol_rpath@ \
        @stb_ac_simvol_ldflags@ \
        @stb_ac_simvol_libs@ \
        -lgtkmm-2.0 -lgdkmm-2.0 -latkmm-1.0 -lgtk-x11-2.0 -lpangomm-1.0 \
        -lglibmm-2.0 -lsigc-1.2 -lgdk-x11-2.0 -latk-1.0 -lgdk_pixbuf-2.0 \
        -lm -lpangoxft-1.0 -lpangox-1.0 -lpango-1.0 -lgobject-2.0 -lgmodule-2.0 \
        -ldl -lglib-2.0

vacuum_stb_la_LDFLAGS = -module -avoid-version \
        -L/usr/lib -L/usr/X11R6/lib -Wl,--export-dynamic
