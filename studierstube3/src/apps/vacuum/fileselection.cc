#include "fileselection.hh"
#include "SoVacuumKit.h"

void
fileselection::on_fs_ok_button_clicked()
{  
  char * file_name = g_strdup(this->get_filename().c_str());
  char * low_str = g_ascii_strdown(file_name, -1);

  /* process only if user really selected a valid file */
  if (file_name[strlen(file_name) - 2] == '/' || !g_strrstr(low_str, ".vol")) {
    g_free(file_name);  
    g_free(low_str);
    return;
  }

  SoVacuumKit::vacuumKit->switch_volume(file_name);
  printf("%s\n", file_name);

  g_free(file_name);  
  g_free(low_str);

  this->hide();
}
