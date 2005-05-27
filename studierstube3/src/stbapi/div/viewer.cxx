/* ========================================================================
 * Copyright (C) 2000-2003  Vienna University of Technology
 *
 * This framework is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This framework is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this framework; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * For further information please contact Dieter Schmalstieg under
 * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
 * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
 * Austria.
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/** simple DIV viewer
  *
  * @author   Gerd Hesina
  *
  * $Id: viewer.cxx 3692 2004-09-10 15:36:42Z reitmayr $
  * @file                                                                   */
 /* ======================================================================= */
#include <ace/ACE.h>
#include <Inventor/SoDB.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoRotor.h>
#include <Inventor/Qt/SoQt.h>
#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/sensors/SoAlarmSensor.h>
#include <cassert>
#include "../util/option.h"
#include "divmain.h"
#include "SoDivGroup.h"

#include <Inventor/manips/SoTransformBoxManip.h>

CDivMain::Mode mode = CDivMain::MODE_SLAVE;
SoQtExaminerViewer *myViewer;
SoSeparator *root;

void callMe(void *data, SoSensor *s)
{
    SoDivGroup * div = (SoDivGroup *) data;
    if( div->isMaster.getValue() == FALSE )
    {
        printf("Requesting master mode !\n");
        div->getMaster.setValue();
    }
    SoAlarmSensor * alarm = (SoAlarmSensor *) s;
    alarm->setTimeFromNow( SbTime((double)(rand() % 10 + 10) ));
    alarm->schedule();
}

void master( void * data, SoSensor *)
{
    SoDivGroup * div = (SoDivGroup *) data;    
    printf("Master mode changed to %s\n",div->isMaster.getValue()?"MASTER":"SLAVE");
}

int main(int argc, char **argv)
{
   OptionString optMode("mode", "mode", "slave");
   OptionString optFile("file", "file", "input.iv");
   OptionString optAddr("addr", "address", "224.100.200.202");
   OptionLong optPort("port", "port", 32456);

#ifdef DROPRATE
   OptionDouble optDropRate("drop", "drop", 0.5);
#endif
   if(!Option::parse(argc,argv)) { Option::printUsage(); exit(-1); }
   if(strcmp(optMode.getValue(),"slave"))
       mode = CDivMain::MODE_MASTER;

#ifdef DROPRATE
    double dropRate=optDropRate.getValue();
#endif

   QWidget * myWindow = SoQt::init(argv[0]);
   if (myWindow == NULL) exit(1);

   // CDivMain *div1=new CDivMain(optAddr.getValue(), optPort.getValue(), mode, true);
   // div1->setMasterChangeCB( master );

   SoDivGroup::initClass();

   char title[1000];
   myViewer = new SoQtExaminerViewer(myWindow);

   SoDivGroup * divGroup = new SoDivGroup;
   divGroup->ref();
   divGroup->multicastGroup.setValue( optAddr.getValue() );
   divGroup->port.setValue( optPort.getValue());

   if(mode == CDivMain::MODE_MASTER)
   {        
        sprintf(title,"DIV -- Master");

        divGroup->isMaster.setValue( TRUE );
        divGroup->setName("divRootNode");

        SoInput myInput;
        if(!myInput.openFile(optFile.getValue())) exit(1);
        SoSeparator* rootFile = SoDB::readAll(&myInput);
        if(rootFile == NULL) exit(1);
        // rootFile->ref();
        divGroup->addChild(rootFile);
        divGroup->active.setValue( TRUE );
        //SoNode * testNode = SoNode::getByName( "TEST" );
        //CDivMain::globalDisableNode( testNode );
    }
   else
   {
      sprintf(title,"DIV -- Slave");
      
      divGroup->setName("divRootNode");
      divGroup->active.setValue( TRUE );
   }
   
   CDivMain::startEventHook();

   SoAlarmSensor * alarm = new SoAlarmSensor( callMe, divGroup );
   alarm->setTimeFromNow( SbTime((double) (rand() % 10 + 10) ));
   // alarm->schedule();

   SoFieldSensor * masterSense = new SoFieldSensor( master, divGroup );
   masterSense->attach( &divGroup->isMaster );

   myViewer->setSceneGraph(divGroup);
   myViewer->show();
   myViewer->setTitle(title);
   SoQt::show(myWindow);

   SoQt::mainLoop();   

   divGroup->active.setValue( FALSE );
   divGroup->unref();
   return 0;
}
