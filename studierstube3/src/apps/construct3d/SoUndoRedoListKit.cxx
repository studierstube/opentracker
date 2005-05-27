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
//  HISTORY:   Time of development: Mai 2003 - 
//
// ===========================================================================

#include "SoUndoRedoListKit.h"


SO_KIT_SOURCE(SoUndoRedoListKit);

void SoUndoRedoListKit::initClass() 
{ 
  SO_KIT_INIT_CLASS(SoUndoRedoListKit, SoBaseKit, "BaseKit");
} 

SoUndoRedoListKit::SoUndoRedoListKit() 
{ 
	initSoUndoRedoListKit();
  initVariables();
}

// init all parts and fields
void SoUndoRedoListKit::initSoUndoRedoListKit()
{
  SO_KIT_CONSTRUCTOR(SoUndoRedoListKit); 
  
  SO_NODE_ADD_FIELD(undo_redo_List, (NULL)); //init undo redo List
  SO_NODE_ADD_FIELD(undo_redo_List_pos, (0));
  SO_NODE_ADD_FIELD(num_of_Items, (0)); 
  SO_NODE_ADD_FIELD(startTime, (0.0));

  SO_KIT_INIT_INSTANCE();
}


// init all variable
void SoUndoRedoListKit::initVariables()
{
//  undo_redo_List_pos.setValue(-1);
  undo_redo_List_pos.setValue(-1);
//  num_of_Items.setValue(0);
  num_of_Items.setValue(0);
  SbName name = "undo_redo_List";
  this->setName(name);
  SbTime t;
  startTime.setValue(t.getTimeOfDay());
}

// Destructor
SoUndoRedoListKit::~SoUndoRedoListKit()
{

}

void SoUndoRedoListKit::addCommandtoUndoReduList(SbName command, SbName objectName, SbName objectType, SoMFName* objects, int userID)
{
  incListPos();
  setNumofItems();
  
  SoCommandKit *c = new SoCommandKit(command, objectName, objectType, objects, userID, getListPos());
  undo_redo_List.set1Value(getListPos(), c);
  //printoutUndoRedoList();
}

void SoUndoRedoListKit::addCommandtoUndoReduList(SbName command, SbName objectName, SbName objectType, SbVec3f eventPos, SoMFName* objects, int userID)
{
  incListPos();
  setNumofItems();

  SoCommandKit *c = new SoCommandKit(command, objectName, objectType, eventPos, objects, userID, getListPos());
  undo_redo_List.set1Value(getListPos(), c);
  //printoutUndoRedoList();
}

void SoUndoRedoListKit::addCommandtoUndoReduList(SbName command, SbVec3f position)
{
  incListPos();
  setNumofItems();
  
  SoCommandKit *c = new SoCommandKit(command, position, getListPos());
  undo_redo_List.set1Value(getListPos(), c);
  //printoutUndoRedoList();
}

void SoUndoRedoListKit::addCommandtoUndoReduList(SbName command, SbName objectName)
{
  incListPos();
  setNumofItems();

  SoCommandKit *c = new SoCommandKit(command, objectName, getListPos());
  undo_redo_List.set1Value(getListPos(), c);
  //printoutUndoRedoList();
}

void SoUndoRedoListKit::addCommandtoUndoReduList(SbName command, SbName objectName, int userID)
{
  incListPos();
  setNumofItems();

  SoCommandKit *c = new SoCommandKit(command, objectName, userID, getListPos());
  undo_redo_List.set1Value(getListPos(), c);
  //printoutUndoRedoList();
}

void SoUndoRedoListKit::addCommandtoUndoReduList(SbName command, int activeLayerNew, int activeLayerOld)
{ 
  incListPos();
  setNumofItems();

  SoCommandKit *c = new SoCommandKit(command, activeLayerNew, activeLayerOld, getListPos());
  undo_redo_List.set1Value(getListPos(), c);
  //printoutUndoRedoList();
}

void SoUndoRedoListKit::addCommandtoUndoReduList(SbName command, SoMFName* objects, int activeLayerNew, int activeLayerOld)
{ 
  incListPos();
  setNumofItems();

  SoCommandKit *c = new SoCommandKit(command, objects, activeLayerNew, activeLayerOld, getListPos());
  undo_redo_List.set1Value(getListPos(), c);
}

void SoUndoRedoListKit::addCommandtoUndoReduList(SbName command, SbName objectName, SbVec3f startPosition, SbVec3f endPosition)
{
  incListPos();
  setNumofItems();

  SoCommandKit *c = new SoCommandKit(command, objectName, startPosition, endPosition, getListPos());
  undo_redo_List.set1Value(getListPos(), c);
  //printoutUndoRedoList();
}

void SoUndoRedoListKit::addCommandtoUndoReduList(SbName command, SbName objectName, SbName objectType, int degreeold, int degree)
{
  incListPos();
  setNumofItems();

  SoCommandKit *c = new SoCommandKit(command, objectName, objectType, degreeold, degree, getListPos());
  undo_redo_List.set1Value(getListPos(), c);
  //printoutUndoRedoList();
}

void SoUndoRedoListKit::addCommandtoUndoReduList(SbName command, SbName objectName, float sliderValueNew, float sliderValueOld)
{
  incListPos();
  setNumofItems();

  SoCommandKit *c = new SoCommandKit(command, objectName, sliderValueNew, sliderValueOld, getListPos());
  undo_redo_List.set1Value(getListPos(), c);
  //printoutUndoRedoList();
}

void SoUndoRedoListKit::addCommandtoUndoReduList(SbName command, const char* filePath, int appID)
{
  incListPos();
  setNumofItems();

  SoCommandKit *c = new SoCommandKit(command, filePath, appID, getListPos());
  undo_redo_List.set1Value(getListPos(), c);
  printoutNode();
  //printoutUndoRedoList();
}

/**
  * for transformation commands (translate, rotate, ...)
  */
void SoUndoRedoListKit::addCommandtoUndoReduList(SbName command, SoMFName* objects, int userID)
{
  incListPos();
  setNumofItems();

  SoCommandKit *c = new SoCommandKit(command, objects, userID, getListPos());
  undo_redo_List.set1Value(getListPos(), c);
  printoutNode();
  //printoutUndoRedoList();
}

void SoUndoRedoListKit::printoutUndoRedoList()
{
  std::cout << "undo_redo_pos: " << getListPos() << " num_of_itmes:" << getNumofItems() << std::endl;
  for(int i=0; i < undo_redo_List.getNum(); i++)
  {
    SbString string;
    SoNode* node = undo_redo_List[i];
    ((SoCommandKit*)node)->PrintoutNode();
  }
}

void SoUndoRedoListKit::printoutNode()
{
  SbString string;
  SoNode* node = undo_redo_List[getListPos()];
  ((SoCommandKit*)node)->PrintoutNode(); 
}

int SoUndoRedoListKit::getListPos()
{
  return undo_redo_List_pos.getValue();
  //return undo_redo_List_pos;
}

void SoUndoRedoListKit::setListPos(int pos)
{
  undo_redo_List_pos.setValue(pos);
  //undo_redo_List_pos = pos;
}

void SoUndoRedoListKit::incListPos()
{
  undo_redo_List_pos.setValue(getListPos() + 1);
  //undo_redo_List_pos = (getListPos() + 1);
}

void SoUndoRedoListKit::decListPos()
{
  undo_redo_List_pos.setValue(getListPos() - 1);
  //undo_redo_List_pos = (getListPos() - 1);
}

int SoUndoRedoListKit::getNumofItems()
{
  return num_of_Items.getValue();
  //return num_of_Items;
}

void SoUndoRedoListKit::setNumofItems()
{
  num_of_Items.setValue(getListPos());
  //num_of_Items = getListPos();
}

void SoUndoRedoListKit::setNumofItems(int numofItems)
{
  num_of_Items.setValue(numofItems);
  //num_of_Items = numofItems;
}

Object3dKit* SoUndoRedoListKit::getObject()
{
  SoNode* node = undo_redo_List[getListPos()];
  Object3dKit *object = (Object3dKit*)SoNode::getByName(((SoCommandKit*)node)->getObjectName());
  return object;
}

Object3dKit* SoUndoRedoListKit::getObject(int pos)
{
  SoNode* node = undo_redo_List[pos];
  Object3dKit *object = (Object3dKit*)SoNode::getByName(((SoCommandKit*)node)->getObjectName());
  return object;
}

SbName SoUndoRedoListKit::getCommand()
{
  SoNode* node = undo_redo_List[getListPos()];
  return ((SoCommandKit*)node)->getCommand();  
}

SoCommandKit* SoUndoRedoListKit::getCommandLine()
{
  SoNode* node = undo_redo_List[getListPos()];
  return ((SoCommandKit*)node);  
}

SoCommandKit* SoUndoRedoListKit::getCommandLine(int num)
{
	return ((SoCommandKit*)undo_redo_List[num]);  
}

int SoUndoRedoListKit::countNumofItems()
{
//  //return getNumofItems();
//    int counter = 0;
//    SoCommandKit* commandLine = NULL;
//    commandLine = getCommandLine(counter);
//    while (commandLine)
//    {
//      counter++;
//      commandLine = NULL;
//      commandLine = getCommandLine(counter);
//    }
//    return counter - 1;

	// most reliable way to determine the size of the undo_redo_List
	// all other ways of stepping through the list fail since we
	// get pointers to Nirvana at the end of the list and the last 
	// garbage entry is not NULL
	return undo_redo_List.getNumNodes()-1;
}

void SoUndoRedoListKit::cleanList()
{
  if (getListPos() < getNumofItems())
  {
    undo_redo_List.deleteValues(getListPos()+1, getNumofItems()-getListPos());
    //for (int index = getListPos() + 1; index <= getNumofItems(); index++)
    //  undo_redo_List[index].delet;
    setNumofItems(countNumofItems()-1);
  }
}