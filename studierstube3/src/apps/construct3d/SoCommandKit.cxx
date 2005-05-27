// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       Cnd.cxx
//  TYPE:       c++ source code
//  PROJECT:    Studierstube Workspace
//  CONTENT:    Geometric Construction in n Dimensions
//  VERSION:    1.3
// ===========================================================================
//	AUTHOR	   kf	   Klaus Feiler 2002
// ===========================================================================
//  HISTORY:   Time of development: April 2003 - 
//
// ===========================================================================

#include "SoCommandKit.h"


SO_KIT_SOURCE(SoCommandKit);

void SoCommandKit::initClass() 
{ 
  SO_KIT_INIT_CLASS(SoCommandKit, SoBaseKit, "BaseKit");
} 

SoCommandKit::SoCommandKit() 
{ 
	initSoCommandKit();
}

SoCommandKit::SoCommandKit(SbName command, SbName objectName, SbName objectType, SoMFName* objects, int userID, int commandPos)
{
  initSoCommandKit();

  this->command.setValue(command);
  this->objectName.setValue(objectName);
  this->objectType.setValue(objectType);
  this->userID.setValue(userID);
  this->commandPos.setValue(commandPos);

  selectedObjectNames.setNum(objects->getNum());
  std::cout << "NUM OF OBJECTS " << objects->getNum() << std::endl;
  for (int i=0; i < objects->getNum(); i++)
  {
    SbString string;
    objects->get1(i, string);
    selectedObjectNames.set1(i, string.getSubString(1,string.getLength()-2).getString());
  }
}

SoCommandKit::SoCommandKit(SbName command, SbName objectName, SbName objectType, SbVec3f eventPos, SoMFName* objects, int userID, int commandPos)
{
  initSoCommandKit();
  
  this->command.setValue(command);
  this->objectName.setValue(objectName);
  this->objectType.setValue(objectType);
  this->position = eventPos;
  this->userID.setValue(userID);
  this->commandPos.setValue(commandPos);

  selectedObjectNames.setNum(objects->getNum());
  std::cout << "NUM OF OBJECTS " << objects->getNum() << std::endl;
  for (int i=0; i < objects->getNum(); i++)
  {
    SbString string;
    objects->get1(i, string);
    selectedObjectNames.set1(i, string.getSubString(1,string.getLength()-2).getString());
  }
}

SoCommandKit::SoCommandKit(SbName command, SbVec3f position, int commandPos)
{
  initSoCommandKit();

  this->command.setValue(command);
  this->position = position;
  this->commandPos.setValue(commandPos);
   
}

SoCommandKit::SoCommandKit(SbName command, SbName objectName, int commandPos)
{
  initSoCommandKit();

  this->command.setValue(command);
  this->objectName = objectName;
  this->commandPos.setValue(commandPos);
}

SoCommandKit::SoCommandKit(SbName command, SbName objectName, int userID, int commandPos)
{
  initSoCommandKit();

  this->command.setValue(command);
  this->objectName = objectName;
  this->userID.setValue(userID);
  this->commandPos.setValue(commandPos);
}


SoCommandKit::SoCommandKit(SbName command, int aktiveLayerNew, int aktiveLayerOld, int commandPos)
{
  initSoCommandKit();

  this->command.setValue(command);
  this->aktiveLayerNew.setValue(aktiveLayerNew);  
  this->aktiveLayerOld.setValue(aktiveLayerOld);  
  this->commandPos.setValue(commandPos);
}

SoCommandKit::SoCommandKit(SbName command, SoMFName* objects, int aktiveLayerNew, int aktiveLayerOld, int commandPos)
{
  initSoCommandKit();

  this->command.setValue(command);
  this->aktiveLayerNew.setValue(aktiveLayerNew);  
  this->aktiveLayerOld.setValue(aktiveLayerOld); 
  this->commandPos.setValue(commandPos);
  
  selectedObjectNames.setNum(objects->getNum());
  
  for (int i=0; i < objects->getNum(); i++)
  {
    SbString string;
    objects->get1(i, string);
    selectedObjectNames.set1(i, string.getSubString(1,string.getLength()-2).getString());
  }
}

SoCommandKit::SoCommandKit(SbName command, SbName objectName, SbName objectType,int degreeold, int degree, int commandPos)
{
  initSoCommandKit();

  this->command.setValue(command);
  this->objectName.setValue(objectName);
  this->objectType.setValue(objectType);
  this->degree.setValue(degree);
  this->degreeold.setValue(degreeold);
  this->commandPos.setValue(commandPos);
}

SoCommandKit::SoCommandKit(SbName command, SbName objectName, SbVec3f startPosition, SbVec3f endPosition, int commandPos)
{
  initSoCommandKit();
  
  this->command.setValue(command);
  this->objectName.setValue(objectName);
  this->startPosition = startPosition;
  this->endPosition = endPosition;
  this->commandPos.setValue(commandPos);
}
  
SoCommandKit::SoCommandKit(SbName command, SbName objectName, float sliderValueNew, float sliderValueOld, int commandPos)
{
  initSoCommandKit();
  
  this->command.setValue(command);
  this->objectName.setValue(objectName);
  this->sliderValueNew.setValue(sliderValueNew);
  this->sliderValueOld.setValue(sliderValueOld);
  this->commandPos.setValue(commandPos);
}
  
SoCommandKit::SoCommandKit(SbName command, const char* filePath, int appID, int commandPos)
{
  initSoCommandKit();
  
  this->command.setValue(command);
  this->filePath.setValue(filePath);
  this->appID.setValue(appID);
  this->commandPos.setValue(commandPos);
}

/**
  * for transformation commands (translate, rotate, ...)
  */
SoCommandKit::SoCommandKit(SbName command, SoMFName* objects, int userID, int commandPos)
{
  initSoCommandKit();

  this->command.setValue(command);
  this->userID.setValue(userID);
  this->commandPos.setValue(commandPos);

  selectedObjectNames.setNum(objects->getNum());
  std::cout << "NUM OF OBJECTS " << objects->getNum() << std::endl;
  for (int i=0; i < objects->getNum(); i++)
  {
    SbString string;
    objects->get1(i, string);
    selectedObjectNames.set1(i, string.getSubString(1,string.getLength()-2).getString());
  }
}

// init all parts and fields
void SoCommandKit::initSoCommandKit()
{
  SO_KIT_CONSTRUCTOR(SoCommandKit); 
//  SO_KIT_ADD_CATALOG_ENTRY(command, (""));
  SO_NODE_ADD_FIELD(command, (""));
  SO_NODE_ADD_FIELD(commandPos, (0));
  SO_NODE_ADD_FIELD(objectName, (""));
  SO_NODE_ADD_FIELD(objectType, (""));
  SO_NODE_ADD_FIELD(filePath, (""));
  SO_NODE_ADD_FIELD(selectedObjectNames, (""));
  SO_NODE_ADD_FIELD(position, (0.0f, 0.0f, 0.0f));
  SO_NODE_ADD_FIELD(startPosition, (0.0f, 0.0f, 0.0f));
  SO_NODE_ADD_FIELD(endPosition, (0.0f, 0.0f, 0.0f));
  SO_NODE_ADD_FIELD(degree, (0));
  SO_NODE_ADD_FIELD(degreeold, (0));
  SO_NODE_ADD_FIELD(sliderValueNew, (0));
  SO_NODE_ADD_FIELD(sliderValueOld, (0));
  SO_NODE_ADD_FIELD(userID, (-1));
  SO_NODE_ADD_FIELD(aktiveLayerNew, (0));
  SO_NODE_ADD_FIELD(aktiveLayerOld, (0));
  SO_NODE_ADD_FIELD(time, (0.0));

  SO_KIT_INIT_INSTANCE();
  selectedObjectNames.setNum(0);
  SbTime t;
  time.setValue(t.getTimeOfDay());
}

// Destructor
SoCommandKit::~SoCommandKit()
{

}

void SoCommandKit::setCommandLine(SbName command, SbName objectName, SbName objectType, std::vector<Object3dKit*> objects, int numofObjects, int userID)
{
  this->command.setValue(command);
  this->objectName.setValue(objectName);
  this->objectType.setValue(objectType);
  this->userID.setValue(userID);

  SbTime t;
  time.setValue(t.getTimeOfDay());

  selectedObjectNames.setNum(numofObjects);
  
  for(int i=0; i < numofObjects; i++)
  {
    selectedObjectNames.set1(i,((Object3dKit*)objects[i])->getName().getString());
  }
}

void SoCommandKit::setCommandLine(SbName command, SbName objectName, SbName objectType, SoMFName* objects, int userID)
{
  this->command.setValue(command);
  this->objectName.setValue(objectName);
  this->objectType.setValue(objectType);
  this->userID.setValue(userID);

  SbTime t;
  time.setValue(t.getTimeOfDay());

  selectedObjectNames.setNum(objects->getNum());
  for (int i=0; i < objects->getNum(); i++)
  {
    SbString string;
    objects->get1(i, string);
    selectedObjectNames.set1(i, string.getSubString(1,string.getLength()-2).getString());
  }
}

void SoCommandKit::PrintoutNode()
{
  if (getCommand() == "move")
  {
    std::cout << command.getValue().getString() <<  ", " << commandPos.getValue() << ", " << objectName.getValue().getString() << "," << startPosition.getValue()[0] << "x" << startPosition.getValue()[1] << "x" << startPosition.getValue()[2] << " ," << endPosition.getValue()[0] << "x" << endPosition.getValue()[1] << "x" << endPosition.getValue()[2];
  }
  if (getCommand() == "add")
  {
    if (getObjectType() == "Point")
      std::cout << command.getValue().getString() << ", Userid: " << userID.getValue() << " " << ", " << objectName.getValue().getString()<< ", " << objectType.getValue().getString() << position.getValue()[0] << "x" << position.getValue()[1] << "x" << position.getValue()[2] << ", userID: " << userID.getValue() << " ";
    else
      std::cout << command.getValue().getString() << ", Userid: " << userID.getValue() << " " << ", " << objectName.getValue().getString()<< ", " << objectType.getValue().getString() << ", userID: " << userID.getValue() << " ";
    
    for(int i=0; i < selectedObjectNames.getNum(); i++)
    {
      SbString string;
      selectedObjectNames.get1(i,string);
      std::cout<< ", " << string.getSubString(1,(string.getLength()-2)).getString(); 
    }
  }
  if (getCommand() == "delete" || getCommand() == "deselectAll")
  {
    std::cout << command.getValue().getString() << ", Userid: " << userID.getValue() << " " << ", "  << objectType.getValue().getString();
    for(int i=0; i < selectedObjectNames.getNum(); i++)
    {
      SbString string;
      selectedObjectNames.get1(i,string);
      std::cout<< ", " << string.getSubString(1,(string.getLength()-2)).getString(); 
    }
  }
  if ((getCommand() == "select") || (getCommand() == "selectdepenentObjects"))
  {
    std::cout << command.getValue().getString() << ", Userid: " << userID.getValue() << " " << ", "<< objectName.getValue().getString() << ", " << position.getValue()[0] << ", " << position.getValue()[1] << ", " << position.getValue()[2];
  }
  if (getCommand() == "rebuild")
  {
    std::cout << command.getValue().getString() << ", Userid: " << userID.getValue() << " " << ", "<< objectName.getValue().getString() << ", " << objectType.getValue().getString() << ", " << degree.getValue();
  }
  if (getCommand() == "slider")
  {
    std::cout << command.getValue().getString() << ", Userid: " << userID.getValue() << " " << ", "<< objectName.getValue().getString() << ", " << sliderValueNew.getValue() << ", " << sliderValueOld.getValue();
  }
  if (getCommand() == "deleteOne")
  {
    std::cout << command.getValue().getString() << ", Userid: " << userID.getValue() << " " << ", "<< objectName.getValue().getString();
  }
  if (getCommand() == "change")
  {
    std::cout << command.getValue().getString() << ", Userid: " << userID.getValue() << " " << ", "<< objectName.getValue().getString();
  }
  if (getCommand() == "layer")
  {
    std::cout << command.getValue().getString() << ", Userid: " << userID.getValue() << " " << ", "<< aktiveLayerNew.getValue() << ", "<< aktiveLayerOld.getValue();
  }
  if (getCommand() == "changeObjectLayer")
  {
    std::cout << command.getValue().getString() << ", Userid: " << userID.getValue() << " ";
    for(int i=0; i < selectedObjectNames.getNum(); i++)
    {
      SbString string;
      selectedObjectNames.get1(i,string);
      std::cout<< ", " << string.getSubString(1,(string.getLength()-2)).getString(); 
    }  
      
    std::cout << ", "<< aktiveLayerNew.getValue() << ", "<< aktiveLayerOld.getValue();
  }
  if ((getCommand() == "press") || (getCommand() == "release"))
  {
    std::cout << command.getValue().getString() << ", Userid: " << userID.getValue() << " " << ", "<< objectName.getValue().getString();
  }
  if (getCommand() == "loadFile")
  {
    std::cout << command.getValue().getString() << ", Userid: " << userID.getValue() << " " << ", "<< filePath.getValue().getString();
  }
  SbTime t;
  //std::cout << " , " << t.getTimeOfDay().format("%s%i").getString() << std::endl;
  std::cout << " , " << time.getValue().format("%h:%m:%s").getString() << std::endl;
}

SbName SoCommandKit::getObjectName()
{
  return objectName.getValue();
}

Object3dKit* SoCommandKit::getObject()
{
  Object3dKit *object = (Object3dKit*)SoNode::getByName(getObjectName());
  return object;
}

SbName SoCommandKit::getCommand()
{
  return command.getValue();
}

SbName SoCommandKit::getObjectType()
{
  return objectType.getValue();
}

SbName SoCommandKit::getObjectNames(int num)
{
  return selectedObjectNames[num];
}

int SoCommandKit::getNumofObjectNames()
{
  return selectedObjectNames.getNum();
}

SbVec3f SoCommandKit::getPosition()
{
  return position.getValue();
}

SbVec3f SoCommandKit::getEndPosition()
{
  return endPosition.getValue();
}

SbVec3f SoCommandKit::getStartPosition()
{
  return startPosition.getValue();
}

int SoCommandKit::getDegree()
{
  return degree.getValue();
}

int SoCommandKit::getDegreeOld()
{
  return degreeold.getValue();
}

float SoCommandKit::getSliderValueOld()
{
  return sliderValueOld.getValue();
}
  
float SoCommandKit::getSliderValueNew()
{
  return sliderValueNew.getValue();
}

int SoCommandKit::getUserID()
{
  return userID.getValue();
}

int SoCommandKit::getAktiveLayerNew()
{
  return aktiveLayerNew.getValue();
}

int SoCommandKit::getAktiveLayerOld()
{
  return aktiveLayerOld.getValue();
}

int SoCommandKit::getAppID()
{
  return appID.getValue();
}

void SoCommandKit::setAppID(int appID)
{
  this->appID.setValue(appID);
}

SbName SoCommandKit::getFilePath()
{
  return filePath.getValue();
}

SbName SoCommandKit::getinvolvedObjectName(int num)
{
  if ((num >= 0) && (num < selectedObjectNames.getNum()))
    return selectedObjectNames[num];
  else
    return "null";
}

void SoCommandKit::Undo(SoSelection* SelectionRoot, SoSFInt32* ObjSum)
{ 
  SelectInvolvedObjects(SelectionRoot);
  RemoveAllCallbacks();

  //delete the object
  Object3dKit* object = getObject();
  object->Undo(SelectionRoot, ObjSum);
  DeleteSelectedObjectsNames();
}

void SoCommandKit::SelectInvolvedObjects(SoSelection* SelectionRoot)
{
  //select all involved objects
  int i=0;
  while ((this->getinvolvedObjectName(i) != "null") && (this->getinvolvedObjectName(i) != ""))
  {
    Object3dKit* object = (Object3dKit*)SoNode::getByName(this->getinvolvedObjectName(i));
    object->select(SelectionRoot);
    i++;
  }
}

void SoCommandKit::DeleteSelectedObjectsNames()
{
  int i=0;
  while ((this->getinvolvedObjectName(i) != "null") && (this->getinvolvedObjectName(i) != ""))
  {
    Object3dKit* object = (Object3dKit*)SoNode::getByName(this->getinvolvedObjectName(i));
    object->deleteDependentObjectsName(this->getObjectName());
    i++;
  }
}

void SoCommandKit::RemoveAllCallbacks()
{
  Object3dKit* object = getObject();
  int i=0;
  while ( (this->getinvolvedObjectName(i)) != "null" && (this->getinvolvedObjectName(i) != "") )
  {
    std::cout << object->getName().getString() << " CommandKit removeCB von " << this->getinvolvedObjectName(i).getString() << std::endl;
    Object3dKit* obj = (Object3dKit*)SoNode::getByName(this->getinvolvedObjectName(i));
    obj->removeAllCallbacks(object);
    i++;
  }
}
