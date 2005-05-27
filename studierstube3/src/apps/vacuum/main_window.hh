#ifndef _MAIN_WINDOW_HH
#define _MAIN_WINDOW_HH

#include "main_window_glade.hh"
#include "fileselection.hh"

class main_window : public main_window_glade
{
  Glib::RefPtr<Gdk::Window> drawable;
  fileselection *file_select;

  void on_open_activate();
  void on_quit_activate();
  void on_volume_activate();
  void on_orthoslice_activate();
  void on_obliqueslice_activate();
  void on_axis_x_activate();
  void on_axis_y_activate();
  void on_axis_z_activate();
  void on_interpolation_nearest_activate();
  void on_interpolation_linear_activate();
  void on_alpha_as_is_activate();
  void on_alpha_opaque_activate();
  void on_alpha_binary_activate();
  void on_clipping_front_activate();
  void on_clipping_back_activate();
  void on_about_activate();
  void on_grey_activate();
  void on_temperature_activate();
  void on_physics_activate();
  void on_standard_activate();
  void on_glow_activate();
  void on_blue_red_activate();
  void on_seismic_activate();
  void on_alpha_activate();
  void on_lumalpha_activate();
  void on_rgba_activate();
  void on_entry_remap_min_changed();
  void on_entry_remap_max_changed();
  void on_offset_scale_value_changed();
  void on_shift_scale_value_changed();
  bool on_main_window_delete_event(GdkEventAny *ev);

public:
  main_window();

};

#endif
