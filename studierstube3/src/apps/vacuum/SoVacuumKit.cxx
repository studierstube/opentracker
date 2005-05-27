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
 * implementation of the SoVacuumKit
 *
 * @author Tamer Fahmy
 *
 * $Id: SoVacuumKit.cxx 3575 2004-08-05 00:06:48Z bara $
 * @file                                                                   */
/* ======================================================================= */

#include <Inventor/nodes/SoSwitch.h>

#include <VolumeViz/nodes/SoObliqueSlice.h>
#include <VolumeViz/nodes/SoOrthoSlice.h>
#include <VolumeViz/nodes/SoTransferFunction.h>
#include <VolumeViz/nodes/SoVolumeData.h>
#include <VolumeViz/nodes/SoVolumeRender.h>
#include <VolumeViz/nodes/SoVolumeRendering.h>

#include <stdio.h>

#ifdef WIN32
#include "settings.h"
#endif

#include "SoVacuumKit.h"

SoVacuumKit *SoVacuumKit::vacuumKit = NULL;

SO_KIT_SOURCE(SoVacuumKit);

void
SoVacuumKit::initClass(void)
{
   SO_KIT_INIT_CLASS(SoVacuumKit, SoBaseKit, "BaseKit");
   SoVolumeRendering::init();
}

SoVacuumKit::SoVacuumKit() :
  initialize_sensors(true)
{
   SO_KIT_CONSTRUCTOR(SoVacuumKit);

   SO_KIT_ADD_CATALOG_ENTRY(volumeData, SoVolumeData, FALSE, this, \x0, TRUE);
   SO_KIT_ADD_CATALOG_ENTRY(transFunc, SoTransferFunction, FALSE, this, \x0, TRUE);
   SO_KIT_ADD_CATALOG_ENTRY(mode, SoSwitch, FALSE, this, \x0, TRUE);

   SO_KIT_ADD_FIELD(fileName, (""));
   SO_KIT_ADD_FIELD(reMap, (0));

   SO_KIT_INIT_INSTANCE();
   
   fileReader = new SoVRVolFileReader();

   if (!SoVacuumKit::vacuumKit) {
     create_gui();
     SoVacuumKit::vacuumKit = this;
   }
   else {
     initialize_sensors = false;
   }

   fileNameSensor = new SoFieldSensor(fileNameCB, this);
   reMapSensor = new SoFieldSensor(reMapCB, this);

   if (initialize_sensors) {
     initSensor = new SoOneShotSensor(initCB, this);
     idleSensor = new SoIdleSensor(idleCB, this);
   }
      
   setUpConnections(TRUE, TRUE);
}

SbBool
SoVacuumKit::setUpConnections(SbBool onOff, SbBool doItAlways)
{
   if (!doItAlways && connectionsSetUp == onOff) { return onOff; }

   if (onOff) {
      SoBaseKit::setUpConnections(onOff, doItAlways);

      fileNameSensor->attach(&fileName);
      SoVacuumKit::fileNameCB(this, NULL);

      reMapSensor->attach(&reMap);

      if (initialize_sensors) {
 	if (!idleSensor->isScheduled()) { 
 	  idleSensor->schedule();
 	}
 	if (!initSensor->isScheduled()) {
 	  initSensor->schedule();
 	}
      }
   }
   else {
      fileNameSensor->detach();
      reMapSensor->detach();

      if (initialize_sensors) {
	if (idleSensor->isScheduled()) { 
	  idleSensor->unschedule();
        }
      }
      
      SoBaseKit::setUpConnections(onOff, doItAlways);
   }

   return !(connectionsSetUp = onOff);
}

void
SoVacuumKit::initCB(void *data, SoSensor *sensor)
{
  SoVacuumKit *vacuum = (SoVacuumKit *)data;

  /* set sensible defaults */
#ifndef WIN32
  vacuum->mainwindow->alpha_as_is->set_active(true);
  vacuum->switch_alpha(SoOrthoSlice::ALPHA_AS_IS);
  vacuum->mainwindow->interpolation_nearest->set_active(true);
  vacuum->switch_interpolation(SoOrthoSlice::NEAREST);
#endif
}

void
SoVacuumKit::fileNameCB(void *data, SoSensor *sensor)
{
  SoVacuumKit *vacuum = (SoVacuumKit *)data;
  SbString fileName = vacuum->fileName.getValue();
  
  if (!(!fileName)) {
    SoVolumeData * volData = SO_GET_ANY_PART(vacuum, "volumeData", SoVolumeData);
    vacuum->fileReader->setUserData((void *)fileName.getString());
    volData->setReader(vacuum->fileReader);
    volData->touch();
  }
}

void
SoVacuumKit::reMapCB(void *data, SoSensor *sensor)
{
  char min[4], max[4];
  SoVacuumKit *vacuum = (SoVacuumKit *)data;
  SoTransferFunction * trans = SO_GET_ANY_PART(vacuum, "transFunc", SoTransferFunction);
  
  trans->reMap(vacuum->reMap[0], vacuum->reMap[1]);
  sprintf(min, "%d", vacuum->reMap[0]);
  sprintf(max, "%d", vacuum->reMap[1]);
}


void
SoVacuumKit::idleCB(void *data, SoSensor *sensor)
{
  SoIdleSensor *idleSensor = (SoIdleSensor*)sensor;
#ifndef WIN32
  while (Gtk::Main::events_pending()) {
    Gtk::Main::iteration(false);
  }
#endif
  if (!idleSensor->isScheduled()) { idleSensor->schedule(); }
}

void
SoVacuumKit::create_gui()
{ 
#ifndef WIN32
  /* fake the command-line args and open a window */
  int argc = 1;
  char **argv;

  argv    = g_new(gchar *, 1);
  argv[0] = g_strdup("Vacuum");

  gtk_main = new Gtk::Main(&argc, &argv);

  mainwindow = new main_window();
  mainwindow->show();
#else
  Settings *dialog=new Settings;
  dialog->setParent(this);
  dialog->show();
#endif
}

void
SoVacuumKit::switch_mode(int child)
{
  SoSwitch * mode = SO_GET_ANY_PART(this, "mode", SoSwitch);
  mode->whichChild = child;
}

void
SoVacuumKit::switch_axis(int axis)
{
  SoSwitch * mode = SO_GET_ANY_PART(this, "mode", SoSwitch);
  ((SoOrthoSlice*)mode->getChild(ORTHO_NODE))->axis.setValue(axis);

}

void
SoVacuumKit::switch_interpolation(int interpol)
{
  SoSwitch * mode = SO_GET_ANY_PART(this, "mode", SoSwitch);
  ((SoVolumeRender*)mode->getChild(VOLUME_NODE))->interpolation = interpol;
  ((SoOrthoSlice*)mode->getChild(ORTHO_NODE))->interpolation = interpol;
  ((SoObliqueSlice*)mode->getChild(OBLIQUE_NODE))->interpolation = interpol;
}

void
SoVacuumKit::switch_alpha(int alpha)
{
  SoSwitch * mode = SO_GET_ANY_PART(this, "mode", SoSwitch);
  ((SoOrthoSlice*)mode->getChild(ORTHO_NODE))->alphaUse = alpha;
  ((SoObliqueSlice*)mode->getChild(OBLIQUE_NODE))->alphaUse = alpha;
}

void
SoVacuumKit::switch_clippingSide(int clippingSide)
{
  SoSwitch * mode = SO_GET_ANY_PART(this, "mode", SoSwitch);
  ((SoOrthoSlice*)mode->getChild(ORTHO_NODE))->clippingSide = clippingSide;
}

void
SoVacuumKit::switch_predefColorMap(int predefColorMap)
{
  SoTransferFunction * trans = SO_GET_ANY_PART(this, "transFunc", SoTransferFunction);
  trans->predefColorMap = predefColorMap;
}

void
SoVacuumKit::switch_colorMapType(int colorMapType)
{
  SoTransferFunction * trans = SO_GET_ANY_PART(this, "transFunc", SoTransferFunction);
  trans->colorMapType = colorMapType;
}

void
SoVacuumKit::switch_volume(char *fileName)
{
  this->fileName.setValue(fileName);
}

void
SoVacuumKit::switch_trans_remap(int min, int max)
{
  int values[2] = {min, max};
  if (min <= max) {
    this->reMap.setValues(0, 2, values);
  }
}

void
SoVacuumKit::switch_trans_shift(int shift)
{
  SoTransferFunction * trans = SO_GET_ANY_PART(this, "transFunc", SoTransferFunction);
  trans->shift = shift;
}

void
SoVacuumKit::switch_trans_offset(int offset)
{
  SoTransferFunction * trans = SO_GET_ANY_PART(this, "transFunc", SoTransferFunction);
  trans->offset = offset;
}
