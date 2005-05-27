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
/** c++ source for FurnitureFactory
  *
  * @author Manuel Linsmayer <manuel.linsmayer@expersite.com>, Reza Mostafaie
  *
  * $Id: FurnitureFactory.cxx 3835 2004-10-06 10:08:47Z reitmayr $
  * @file                                                                   */
 /* ======================================================================= */

#ifdef WIN32
#pragma warning (disable: 4786)
#endif

#include <iostream>
#include <Inventor/engines/SoCalculator.h> 
#include <Inventor/engines/SoTimeCounter.h> 
#include <Inventor/nodes/SoBaseColor.h> 
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoDrawStyle.h> 
#include <Inventor/nodes/SoLightModel.h> 
#include <Inventor/nodes/SoMatrixTransform.h>
#include <Inventor/nodes/SoRotationXYZ.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTranslation.h>
#include <stbapi/tracker/SoStationKit.h> 
#include "FurnitureFactory.h"

//----------------------------------------------------------------------------
// Exception class
//----------------------------------------------------------------------------

class FurnitureFactoryException { };

//----------------------------------------------------------------------------
// Constructs an open inventor with all specified component instance additions
// for the given furniture
//----------------------------------------------------------------------------

/*----------------------------------------------------------------------------
 * Structure for component additions:
 *
 *                               X Switch (SO_SWITCH_NONE | SO_SWITCH_ALL)
 *                               |
 *                               |
 *                               X Separator
 *                              /|\\_
 *                             / | \ \_
 *                            /  |  \  O Shape (SoCube, SoCylinder ...)
 *                           /   |   \
 *  Translation+Rotation(1) O    |    O Highlighting
 *                               O
 *                         Translation(2)
 *
 * (1) Connects the primitive to the specified connectors.
 * (2) Translates the primitive so that world and modeling coordinates match
 *     each other.
 *--------------------------------------------------------------------------*/

std::vector<SoSwitch*>* FurnitureFactory::buildComponentInstanceList(std::map<std::string, ComponentInstance>& componentInstances,
														             std::vector<AssemblyStep*>& assemblySteps) {
	char num[7];   // Enough for 'step' + <integer>, so sprintf("step%d") is save
	std::vector<SoSwitch*>* switches = new std::vector<SoSwitch*>;

	SoTimeCounter* colorCounter = new SoTimeCounter;   // Counter for highlighting
	colorCounter->min = 0;
	colorCounter->max = 359;
	colorCounter->frequency = 0.25;

	SoCalculator* colorCalc = new SoCalculator;
	colorCalc->a.connectFrom(&colorCounter->output);
	colorCalc->expression.set1Value(0, "tb = fabs(cos(a / 180 * M_PI)) / 2");
	colorCalc->expression.set1Value(1, "tc = fabs(cos(a / 180 * M_PI)) / 2");
	colorCalc->expression.set1Value(2, "oA = vec3f(1,tb,tc)");

	for (unsigned int i=0;i<assemblySteps.size();i++) {
		ComponentInstanceAddition* componentInstanceAddition;

		if ((componentInstanceAddition = dynamic_cast<ComponentInstanceAddition *> (assemblySteps[i])) != NULL) {   // Component additions
			std::map<std::string, ComponentInstance>::iterator componentInstance = componentInstances.find(componentInstanceAddition->getComponentInstanceName());
			if (componentInstance != componentInstances.end()) {
				SoSwitch *root = new SoSwitch;       // Root (SoSwitch)
				root->whichChild = SO_SWITCH_NONE;
				sprintf(num, "step%d", i+1);
				root->setName(num);

				SoSeparator *rootSeparator = new SoSeparator;   // Separator
				root->addChild(rootSeparator);

				SoMatrixTransform* positioning = new SoMatrixTransform;   // Translation+Rotation

				// TODO: Put this section into a seperate function

				if (componentInstanceAddition->getConnections().size() >= 3) {
					float x1,y1,z1,x2,y2,z2,x3,y3,z3;
					
					////////////////////
					// Get 2x3 points //
					////////////////////

					SbVec3f selfPoint1 = SbVec3f(componentInstance->second.getConnectors()[componentInstanceAddition->getConnections()[0].connectorName1].getX(enum_unit(M)),
					                             componentInstance->second.getConnectors()[componentInstanceAddition->getConnections()[0].connectorName1].getY(enum_unit(M)),
					                             componentInstance->second.getConnectors()[componentInstanceAddition->getConnections()[0].connectorName1].getZ(enum_unit(M)));

					std::map<std::string, ComponentInstance>::iterator otherComponentInstance1 = componentInstances.find(componentInstanceAddition->getConnections()[0].componentInstanceName2);
					SbVec3f otherPoint1 = SbVec3f(otherComponentInstance1->second.getConnectors()[componentInstanceAddition->getConnections()[0].connectorName2].getX(enum_unit(M)),
						                          otherComponentInstance1->second.getConnectors()[componentInstanceAddition->getConnections()[0].connectorName2].getY(enum_unit(M)),
												  otherComponentInstance1->second.getConnectors()[componentInstanceAddition->getConnections()[0].connectorName2].getZ(enum_unit(M)));
					otherComponentInstance1->second.getPositioning().matrix.getValue().multVecMatrix(otherPoint1, otherPoint1);

					SbVec3f selfPoint2 = SbVec3f(componentInstance->second.getConnectors()[componentInstanceAddition->getConnections()[1].connectorName1].getX(enum_unit(M)),
					                             componentInstance->second.getConnectors()[componentInstanceAddition->getConnections()[1].connectorName1].getY(enum_unit(M)),
					                             componentInstance->second.getConnectors()[componentInstanceAddition->getConnections()[1].connectorName1].getZ(enum_unit(M)));

					std::map<std::string, ComponentInstance>::iterator otherComponentInstance2 = componentInstances.find(componentInstanceAddition->getConnections()[1].componentInstanceName2);
					SbVec3f otherPoint2 = SbVec3f(otherComponentInstance2->second.getConnectors()[componentInstanceAddition->getConnections()[1].connectorName2].getX(enum_unit(M)),
						                          otherComponentInstance2->second.getConnectors()[componentInstanceAddition->getConnections()[1].connectorName2].getY(enum_unit(M)),
												  otherComponentInstance2->second.getConnectors()[componentInstanceAddition->getConnections()[1].connectorName2].getZ(enum_unit(M)));
					otherComponentInstance2->second.getPositioning().matrix.getValue().multVecMatrix(otherPoint2, otherPoint2);
					
					SbVec3f selfPoint3 = SbVec3f(componentInstance->second.getConnectors()[componentInstanceAddition->getConnections()[2].connectorName1].getX(enum_unit(M)),
				                                 componentInstance->second.getConnectors()[componentInstanceAddition->getConnections()[2].connectorName1].getY(enum_unit(M)),
   					                             componentInstance->second.getConnectors()[componentInstanceAddition->getConnections()[2].connectorName1].getZ(enum_unit(M)));

					std::map<std::string, ComponentInstance>::iterator otherComponentInstance3 = componentInstances.find(componentInstanceAddition->getConnections()[2].componentInstanceName2);
					SbVec3f otherPoint3 = SbVec3f(otherComponentInstance3->second.getConnectors()[componentInstanceAddition->getConnections()[2].connectorName2].getX(enum_unit(M)),
                                                  otherComponentInstance3->second.getConnectors()[componentInstanceAddition->getConnections()[2].connectorName2].getY(enum_unit(M)),
												  otherComponentInstance3->second.getConnectors()[componentInstanceAddition->getConnections()[2].connectorName2].getZ(enum_unit(M)));
					otherComponentInstance3->second.getPositioning().matrix.getValue().multVecMatrix(otherPoint3, otherPoint3);

					/////////////////////////////////////
					// Grahm-Schmidt Orthogonalization //
					/////////////////////////////////////
					
					SbVec3f selfAxis1 = selfPoint2 - selfPoint1;
					SbVec3f selfAxis2 = selfPoint3 - selfPoint1;
					selfAxis1.normalize();
					selfAxis1.getValue(x1,y1,z1);
					selfAxis2.getValue(x2,y2,z2);
					selfAxis2 -= ((x1*x2+y1*y2+z1*z2) * selfAxis1);
					selfAxis2.normalize();

					SbVec3f otherAxis1 = otherPoint2 - otherPoint1;
					SbVec3f otherAxis2 = otherPoint3 - otherPoint1;
					otherAxis1.normalize();
					otherAxis1.getValue(x1,y1,z1);
					otherAxis2.getValue(x2,y2,z2);
					otherAxis2 -= ((x1*x2+y1*y2+z1*z2) * otherAxis1);
					otherAxis2.normalize();

					//////////////////////
					// Compute upvector //
					//////////////////////

					SbVec3f selfAxis3 = selfAxis1.cross(selfAxis2);
					SbVec3f otherAxis3 = otherAxis1.cross(otherAxis2);

					//////////////////////
					// Compute matrices //
					//////////////////////

					selfPoint1.getValue(x1,y1,z1);
					SbMatrix translation1 = SbMatrix(1,  0,  0, x1,
						                             0,  1,  0, y1,
													 0,  0,  1, z1,
												     0,  0,  0,  1).transpose().inverse();

					selfAxis1.getValue(x1,y1,z1);
					selfAxis2.getValue(x2,y2,z2);
					selfAxis3.getValue(x3,y3,z3);
					SbMatrix rotation1 = SbMatrix(x1, y1, z1, 0,
						                          x2, y2, z2, 0,
												  x3, y3, z3, 0,
												   0,  0,  0, 1).transpose();

					otherAxis1.getValue(x1,y1,z1);
					otherAxis2.getValue(x2,y2,z2);
					otherAxis3.getValue(x3,y3,z3);
					SbMatrix rotation2 = SbMatrix(x1, y1, z1, 0,
						                          x2, y2, z2, 0,
												  x3, y3, z3, 0,
												   0,  0,  0, 1).transpose().inverse();

					otherPoint1.getValue(x1,y1,z1);
					SbMatrix translation2 = SbMatrix(1,  0,  0, x1,
						                             0,  1,  0, y1,
													 0,  0,  1, z1,
												     0,  0,  0, 1).transpose();

					positioning->matrix.setValue(translation1 * rotation1 * rotation2 * translation2);
				}
				
				/////////////////////////////////////////////
				
				rootSeparator->addChild(positioning);
				componentInstance->second.setPositioning(*positioning);

				SoTranslation* initialTranslation = new SoTranslation;   // Translation
				initialTranslation->translation.setValue(componentInstance->second.getWidth(enum_unit(M)) / 2,
											 	         componentInstance->second.getHeight(enum_unit(M)) / 2,
				                                         componentInstance->second.getDepth(enum_unit(M)) / 2);
				rootSeparator->addChild(initialTranslation);

				SoBaseColor *highlightColor = new SoBaseColor;   // Highlight
				highlightColor->rgb.connectFrom(&colorCalc->oA);

				SoSwitch *highlightSwitch = new SoSwitch;
				highlightSwitch->addChild(highlightColor);
				highlightSwitch->whichChild = SO_SWITCH_NONE;
				rootSeparator->addChild(highlightSwitch);
				
				Cube* shape_cube;   // Shape
				if ((shape_cube = dynamic_cast<Cube*> (componentInstance->second.getShape())) != NULL) {
					SoCube* cube = new SoCube;
					cube->width = componentInstance->second.getWidth(enum_unit(M));
					cube->height = componentInstance->second.getHeight(enum_unit(M));
					cube->depth = componentInstance->second.getDepth(enum_unit(M));
					rootSeparator->addChild(cube);
				} else {
					throw FurnitureFactoryException();
				}



				switches->push_back(root);
			} else {
				throw FurnitureFactoryException();
			}
		}
	}

	return (switches);
}

//----------------------------------------------------------------------------
// Constructs an open inventor graph with all specified rotations for the
// given furniture
//----------------------------------------------------------------------------

/*----------------------------------------------------------------------------
 * Structure for rotations:
 *
 *                     X Switch (SO_SWITCH_NONE | SO_SWITCH_ALL)
 *                     |
 *                     |
 *                     O Rotation (SoRotationXYZ)
 *--------------------------------------------------------------------------*/

std::vector<SoSwitch*>* FurnitureFactory::buildRotationList(std::vector<AssemblyStep*>& assemblySteps) {
	char num[7];   // Enough for 'step' + <integer>, so sprintf("step%d") is save
	std::vector<SoSwitch*>* switches = new std::vector<SoSwitch*>;

	for (unsigned int i=0;i<assemblySteps.size();i++) {
		Rotation* rotation;

		if ((rotation = dynamic_cast<Rotation *> (assemblySteps[i])) != NULL) {   // Rotation

			SoRotationXYZ *rotationMatrix = new SoRotationXYZ;
			switch (rotation->getAxis()) {
			case XPOS:
				rotationMatrix->axis.setValue("X");
				rotationMatrix->angle =(float)(rotation->getAngle() * M_PI / 180);
				break;
			case XNEG:
				rotationMatrix->axis.setValue("X");
				rotationMatrix->angle =(float)((360 - rotation->getAngle()) * M_PI / 180);
				break;
			case ZPOS:
				rotationMatrix->axis.setValue("Z");
				rotationMatrix->angle =(float)(rotation->getAngle() * M_PI / 180);
				break;
			case ZNEG:
				rotationMatrix->axis.setValue("Z");
				rotationMatrix->angle =(float)((360 - rotation->getAngle()) * M_PI / 180);
				break;
			}

			SoSwitch *root = new SoSwitch;
			root->whichChild = SO_SWITCH_NONE;
			sprintf(num, "step%d", i+1);
			root->setName(num);
			root->addChild(rotationMatrix);

			switches->push_back(root);
		}
	}

	return (switches);
}

//----------------------------------------------------------------------------
// Constructs an open inventor with all specified component instance additions
// for the given furniture
//----------------------------------------------------------------------------

std::vector<SoStationKit*>* FurnitureFactory::buildTrackerList(std::map<std::string, ComponentInstance>& componentInstances,
															   std::vector<AssemblyStep*>& assemblySteps) {
	char num[7];   // Enough for 'step' + <integer>, so sprintf("step%d") is save
	std::vector<SoStationKit*>* stationKits = new std::vector<SoStationKit*>;

	// Highlighting style
	SoDrawStyle* wireframe = new SoDrawStyle;
	wireframe->style.setValue("LINES");
	wireframe->lineWidth = 3;
	SoBaseColor* baseColor = new SoBaseColor;
	baseColor->rgb.setValue(1,1,0);
	SoLightModel* lightModel = new SoLightModel;
	lightModel->model.setValue("BASE_COLOR");

	for (unsigned int i=0;i<assemblySteps.size();i++) {
		ComponentInstanceAddition* componentInstanceAddition;

		if ((componentInstanceAddition = dynamic_cast<ComponentInstanceAddition *> (assemblySteps[i])) != NULL) {   // Component additions
			std::map<std::string, ComponentInstance>::iterator componentInstance = componentInstances.find(componentInstanceAddition->getComponentInstanceName());
			if (componentInstance != componentInstances.end()) {
				if (componentInstance->second.getTracker() > 0) {
					SoStationKit* stationKit = new SoStationKit;
					sprintf(num, "step%d", i+1);
					stationKit->setName(num);
					stationKit->stations.setValue(componentInstance->second.getTracker());
					stationKit->stationPosition.setValue(0, 0, 0);
					stationKit->stationOrientation.setValue(0, 0, 1, 0);
					stationKit->timeOut.setValue(3.0);

					SoGroup* content = reinterpret_cast<SoGroup*>(stationKit->getPart("content", FALSE));
					if (content != NULL) {
						SoSwitch* contentSwitch = new SoSwitch;
						contentSwitch->whichChild = SO_SWITCH_NONE;
						
						// Add highlighing style
						contentSwitch->addChild(wireframe);
						contentSwitch->addChild(baseColor);
						contentSwitch->addChild(lightModel);

						// Add geometry
						Cube* shape_cube;
						if ((shape_cube = dynamic_cast<Cube*> (componentInstance->second.getShape())) != NULL) {						
							SoCube* cube = new SoCube;
							cube->width = componentInstance->second.getWidth(enum_unit(M));
							cube->height = componentInstance->second.getHeight(enum_unit(M));
							cube->depth = componentInstance->second.getDepth(enum_unit(M));
							contentSwitch->addChild(cube);
						} else {
							throw FurnitureFactoryException();
						}

						content->addChild(contentSwitch);
					}
					stationKits->push_back(stationKit);
				}
			}
		}
	}

	return (stationKits);
}
																	 
/* ===========================================================================
    End of FurnitureFactory.cxx
=========================================================================== */
