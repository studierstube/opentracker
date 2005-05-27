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
/* SoFileSubgraph.cxx contains the implementation of class SoFileSubgraph
 *
 * @author Istvan Barakonyi
 *
 * $Id: SoFileSubgraph.cxx 3520 2004-07-21 07:49:00Z tomp $
 * @file
 */
/* ======================================================================== */

#include "SoFileSubgraph.h"
#include <Inventor/nodes/SoSeparator.h>

SO_KIT_SOURCE(SoFileSubgraph);

void SoFileSubgraph::initClass()
{
    SO_KIT_INIT_CLASS(SoFileSubgraph, SoBaseKit, "BaseKit");
}

SoFileSubgraph::SoFileSubgraph()
{
    SO_KIT_CONSTRUCTOR(SoFileSubgraph);
    SO_KIT_ADD_CATALOG_ENTRY(root, SoSeparator, FALSE, this, \0 ,TRUE);
    SO_KIT_ADD_FIELD(containerNode, (NULL));
    SO_KIT_ADD_FIELD(fileName, (""));
	SO_KIT_INIT_INSTANCE();

    setSearchingChildren(TRUE);

    fileNameSensor=new SoFieldSensor(fileNameCB,this);
    setUpConnections(TRUE,TRUE);
}

SoFileSubgraph::~SoFileSubgraph()
{
    fileNameSensor->detach();
}

SbBool SoFileSubgraph::setUpConnections(SbBool onOff, SbBool doItAlways)
{
    if (!doItAlways && connectionsSetUp == onOff)
        return onOff;

    if (onOff) {
        fileNameSensor->attach(&fileName);
        fileNameChanged();
    }
    else {
        fileNameSensor->detach();
    }
    return !(connectionsSetUp = onOff);
}

void SoFileSubgraph::fileNameCB(void *data,SoSensor *sensor)
{
    SoFileSubgraph *fileSubGraph=(SoFileSubgraph*)data;
    fileSubGraph->fileNameChanged();
}

void SoFileSubgraph::fileNameChanged()
{
    if (fileName.getValue() == "") return;
    readFile(fileName.getValue().getString());  
}

void SoFileSubgraph::readFile(const char *fileName){

    // open the input file
    SoInput sceneInput;
    if (!sceneInput.openFile(fileName)) {
        printf("FILESUBGRAPH: Cannot open file '%s'\n", fileName);
        return;
    }
    if (!sceneInput.isValidFile()){
       printf("FILESUBGRAPH: file '%s' is not a valid Inventor file\n",fileName);
       return;
    }
    else{
        printf("FILESUBGRAPH: file '%s' read successfully\n",fileName);
    }

    // read the whole file into the database
    SoSeparator *subgraph=SoDB::readAll(&sceneInput);
    subgraph->ref();

    if (subgraph ==NULL) {
        printf("FILESUBGRAPH: problem reading file %s\n",fileName);
        return;
    }
    sceneInput.closeFile();
	SoNode *container = containerNode.getValue();
	if ( container != NULL) 
	{
		if (container->getTypeId().isDerivedFrom(SoGroup::getClassTypeId()))
		{
			SoGroup *group = (SoGroup *)container;
			group->addChild(subgraph);
		}
	}
	else
	{

		SoSeparator *graphRoot=SO_GET_ANY_PART(this,"root",SoSeparator);
		graphRoot->addChild(subgraph);
	}
	
	subgraph->unref();
}
