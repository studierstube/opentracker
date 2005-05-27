#include "main_window.hh"
#include "about_dialog.hh"
#include "SoVacuumKit.h"

#include <Inventor/Qt/SoQt.h>

#include <VolumeViz/nodes/SoOrthoSlice.h>
#include <VolumeViz/nodes/SoTransferFunction.h>

#include <stbapi/resource/SoUserManagerKit.h>
#include <stbapi/resource/viewer/SoStudierstubeViewer.h>
#include <stbapi/resource/viewer/video/SoVideoBackground.h>

main_window::main_window() :
  drawable(NULL),
  file_select(NULL)
{
  axis->get_submenu()->set_active(2);
  interpolation->get_submenu()->set_active(0);
  alpha_use->get_submenu()->set_active(0);
  clipping_side->get_submenu()->set_active(0);
  colormap_optionmenu->get_menu()->set_active(0);
  colormaptype_optionmenu->get_menu()->set_active(0);
  drawable = drawingarea->get_window();
  drawable->draw_rectangle(drawingarea->get_style()->get_black_gc(), true, 0,0, 255, 20);
}

void main_window::on_open_activate()
{ 
  if (!file_select) {
    file_select = new fileselection();
  }
  file_select->show();
}

void main_window::on_quit_activate()
{
  SoStudierstubeViewer* viewer = SoUserManagerKit::getInstance()->getUserKit(1)->getDisplay()->getViewer();

  if (viewer->isVideoActive) {
    delete viewer->videoBackground;
    viewer->isVideoActive = FALSE;
  }
  
  SoGui::exitMainLoop();
}

void main_window::on_volume_activate()
{  
  SoVacuumKit::vacuumKit->switch_mode(SoVacuumKit::VOLUME_NODE);
}

void main_window::on_orthoslice_activate()
{  
  SoVacuumKit::vacuumKit->switch_mode(SoVacuumKit::ORTHO_NODE);
}

void main_window::on_obliqueslice_activate()
{
  SoVacuumKit::vacuumKit->switch_mode(SoVacuumKit::OBLIQUE_NODE);
}

void main_window::on_axis_x_activate()
{
  SoVacuumKit::vacuumKit->switch_axis(SoOrthoSlice::X);
}

void main_window::on_axis_y_activate()
{
  SoVacuumKit::vacuumKit->switch_axis(SoOrthoSlice::Y);
}

void main_window::on_axis_z_activate()
{
  SoVacuumKit::vacuumKit->switch_axis(SoOrthoSlice::Z);
}

void main_window::on_interpolation_nearest_activate()
{
  SoVacuumKit::vacuumKit->switch_interpolation(SoOrthoSlice::NEAREST);
}

void main_window::on_interpolation_linear_activate()
{
  SoVacuumKit::vacuumKit->switch_interpolation(SoOrthoSlice::LINEAR);
}

void main_window::on_alpha_as_is_activate()
{
  SoVacuumKit::vacuumKit->switch_alpha(SoOrthoSlice::ALPHA_AS_IS);
}

void main_window::on_alpha_opaque_activate()
{
  SoVacuumKit::vacuumKit->switch_alpha(SoOrthoSlice::ALPHA_OPAQUE);
}

void main_window::on_alpha_binary_activate()
{
  SoVacuumKit::vacuumKit->switch_alpha(SoOrthoSlice::ALPHA_BINARY);
}

void main_window::on_clipping_front_activate()
{
  SoVacuumKit::vacuumKit->switch_clippingSide(SoOrthoSlice::FRONT);
}

void main_window::on_clipping_back_activate()
{
  SoVacuumKit::vacuumKit->switch_clippingSide(SoOrthoSlice::BACK);
}

void main_window::on_about_activate()
{
  about_dialog *about = new about_dialog();  
  about->show();
}

void main_window::on_grey_activate()
{
  SoVacuumKit::vacuumKit->switch_predefColorMap(SoTransferFunction::GREY);
}

void main_window::on_temperature_activate()
{
  SoVacuumKit::vacuumKit->switch_predefColorMap(SoTransferFunction::TEMPERATURE);
}

void main_window::on_physics_activate()
{
  SoVacuumKit::vacuumKit->switch_predefColorMap(SoTransferFunction::PHYSICS);
}

void main_window::on_standard_activate()
{
  SoVacuumKit::vacuumKit->switch_predefColorMap(SoTransferFunction::STANDARD);
}

void main_window::on_glow_activate()
{
  SoVacuumKit::vacuumKit->switch_predefColorMap(SoTransferFunction::GLOW);
}

void main_window::on_blue_red_activate()
{
  SoVacuumKit::vacuumKit->switch_predefColorMap(SoTransferFunction::BLUE_RED);
}

void main_window::on_seismic_activate()
{
  SoVacuumKit::vacuumKit->switch_predefColorMap(SoTransferFunction::SEISMIC);
}

void main_window::on_alpha_activate()
{
  SoVacuumKit::vacuumKit->switch_colorMapType(SoTransferFunction::ALPHA);
}

void main_window::on_lumalpha_activate()
{
  SoVacuumKit::vacuumKit->switch_colorMapType(SoTransferFunction::LUM_ALPHA);
}

void main_window::on_rgba_activate()
{
  SoVacuumKit::vacuumKit->switch_colorMapType(SoTransferFunction::RGBA);
}

void main_window::on_entry_remap_min_changed()
{
  int min = atoi(entry_remap_min->get_text().c_str());
  int max = atoi(entry_remap_max->get_text().c_str());
  SoVacuumKit::vacuumKit->switch_trans_remap(min, max);
}

void main_window::on_entry_remap_max_changed()
{
  int min = atoi(entry_remap_min->get_text().c_str());
  int max = atoi(entry_remap_max->get_text().c_str());
  SoVacuumKit::vacuumKit->switch_trans_remap(min, max);
}

void main_window::on_offset_scale_value_changed()
{
  SoVacuumKit::vacuumKit->switch_trans_offset((int)shift_scale->get_value());
}

void main_window::on_shift_scale_value_changed()
{
  SoVacuumKit::vacuumKit->switch_trans_shift((int)shift_scale->get_value());
}

bool main_window::on_main_window_delete_event(GdkEventAny *ev)
{
  this->on_quit_activate();
  return 0;
}
