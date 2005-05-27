 /* ========================================================================
  * Copyright (C) 2000,2001  Vienna University of Technology
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Lesser General Public
  * License as published by the Free Software Foundation; either
  * version 2.1 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Lesser General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public
  * License along with this library; if not, write to the Free Software
  * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  * For further information please contact Dieter Schmalstieg under
  * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
  * Austria.
  * ======================================================================== 
  * PROJECT: Studierstube
  * ======================================================================== */
/**
 * header file of the SoVacuumKit
 *
 * @author Tamer Fahmy
 *
 * $Id: SoVacuumKit.h 3575 2004-08-05 00:06:48Z bara $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _SoVacuumKit_H_
#define _SoVacuumKit_H_

#include "vacuum.h"

#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoMFInt32.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/sensors/SoIdleSensor.h>
#include <Inventor/sensors/SoOneShotSensor.h>

#include <VolumeViz/readers/SoVRVolFileReader.h>

#include <VolumeViz/readers/SoVRVolFileReader.h>

#ifndef WIN32
#include <gtkmm/main.h>
#include "main_window.hh"
#endif

class VACUUM_API SoVacuumKit : public SoBaseKit
{
   SO_KIT_HEADER(SoVacuumKit);

   SO_KIT_CATALOG_ENTRY_HEADER(volumeData);
   SO_KIT_CATALOG_ENTRY_HEADER(transFunc);
   SO_KIT_CATALOG_ENTRY_HEADER(mode);

  public:
   enum SwitchNodes {DEBUG_NODE, VOLUME_NODE, ORTHO_NODE, OBLIQUE_NODE};
   
   static void initClass();

   SoVacuumKit();

   SoSFString fileName;
   SoMFInt32 reMap;

   SoFieldSensor *reMapSensor, *fileNameSensor;
   SoIdleSensor *idleSensor;
   SoOneShotSensor *initSensor;

   SoVRVolFileReader *fileReader;

   void switch_mode(int child);
   void switch_axis(int axis);
   void switch_interpolation(int interpol);
   void switch_alpha(int alpha);
   void switch_clippingSide(int clippingSide);
   void switch_predefColorMap(int predefColorMap);
   void switch_colorMapType(int colorMapType);
   void switch_volume(char *fileName);
   void switch_trans_remap(int min, int max);
   void switch_trans_shift(int shift);
   void switch_trans_offset(int offset);

   // FIXME: this is sooooooo dirty!!! 20030903 tamer.
   static SoVacuumKit *vacuumKit;

  protected:
   static void initCB(void *data, SoSensor *sensor);
   static void fileNameCB(void *data, SoSensor *sensor);
   static void reMapCB(void *data, SoSensor *sensor);
   static void idleCB(void *data, SoSensor *sensor);

   virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways = FALSE);   

   virtual void create_gui();
#ifndef WIN32
   main_window *mainwindow;
#endif

 private:
   ~SoVacuumKit() {}
#ifndef WIN32
   // FIXME: dirty! 20030903 tamer.
   Gtk::Main * gtk_main;
#endif
   SbBool initialize_sensors;
};

#endif //_SoVacuumKit_H_
