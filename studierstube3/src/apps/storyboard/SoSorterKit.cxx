/* ===========================================================================
    (C) 2000 Vienna University of Technology
   ===========================================================================
    NAME:       SoSorterKit.cxx
    TYPE:       c++ header
    PROJECT:    Studierstube
    CONTENT:    Implementation of SoSorterKit
    VERSION:    2.3
   ===========================================================================
    AUTHORS:    ds      Dieter Schmalstieg
                jp      Jan Prikryl
   ===========================================================================
    HISTORY:

=========================================================================== */

#include "SoSorterKit.h"
#include <assert.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/actions/SoWriteAction.h>
#include <stbapi/util/math2.h>
#include <stbapi/util/ivio.h>
#include <stbapi/misc/ivmacros.h>

SO_KIT_SOURCE(SoSorterKit);

//----------------------------------------------------------------------------

void
SoSorterKit::initClass()
{
    SO_KIT_INIT_CLASS(SoSorterKit, SoBaseKit, "BaseKit");
}

//----------------------------------------------------------------------------

SoSorterKit::SoSorterKit()
:
    selectedDrag(NULL)
{
    SO_KIT_CONSTRUCTOR(SoSorterKit);
    //-----------------------name---|-type-------|nuldef|par |rite|public
    SO_KIT_ADD_CATALOG_ENTRY(icons  ,SoGroup     ,FALSE ,this,\x0 ,TRUE);
    SO_KIT_ADD_FIELD(rows, (3));
    SO_KIT_ADD_FIELD(xspacing, (1.0));
    SO_KIT_ADD_FIELD(yspacing, (1.0));
    SO_KIT_INIT_INSTANCE();
    setSearchingChildren(TRUE);
	setMasterMode(TRUE); //defaults to master mode
}

//----------------------------------------------------------------------------

SoSorterKit::~SoSorterKit()
{
    // nil
}

//----------------------------------------------------------------------------

SbVec3f
SoSorterKit::getTranslation(int index)
{
    float x = (index % rows.getValue()) * xspacing.getValue();
    float y = (index / rows.getValue()) * yspacing.getValue();
    return SbVec3f(x,0,y);
}

//----------------------------------------------------------------------------

void
SoSorterKit::resort()
{
    for(int i=0; i<getNumIcons(); i++)
    {
        AGETCHILD(drag,SoDragKit,getIcons(),i);
        drag->translation = getTranslation(i);
    }
}

//----------------------------------------------------------------------------

SoTexture2*
SoSorterKit::getTexture(SbString id)
{
	fprintf(stderr,"getTexture with id: %s\n",id.getString());
    SoDragKit* drag = (SoDragKit*)findNode(getIcons(),id);
	SoGroup* cont = (SoGroup*)drag->getPart("content",FALSE);
	GETCHILD(iconSep,SoSeparator,cont,0);
	assert(iconSep->getNumChildren() == 2);
	AGETCHILD(tx,SoTexture2,iconSep,0);
	return tx;
}

//----------------------------------------------------------------------------

void
SoSorterKit::add(SbString id, SoTexture2* icon, int insertAt)
{
    NEWNODE(drag,SoDragKit);
//--setup dragger
    drag->setName(id);
    if(isMaster)
	{
        drag->setPressCallback(pressCallback,this);
        drag->setMoveCallback(moveCallback,this);
        drag->setReleaseCallback(releaseCallback,this);
	}
    drag->rotationOn = FALSE;
    drag->yTranslationOn = FALSE;
//--build icon geometry
    SoSeparator* content = (SoSeparator*)drag->getPart("content", FALSE);
    NEWNODE(iconSep, SoSeparator);
    NEWNODE(texturedCube, SoCube);
    texturedCube->width  = xspacing.getValue() * 0.8;
    texturedCube->height = xspacing.getValue() * 0.4;
    texturedCube->depth  = yspacing.getValue() * 0.8;
    iconSep->addChild(icon);
    iconSep->addChild(texturedCube);
    content->addChild(iconSep);
    NEWNODE(selectSep, SoSeparator);
    NEWNODE(selectCube, SoCube);
    selectCube->width  = xspacing.getValue() * 0.9;
    selectCube->height = xspacing.getValue() * 0.399;
    selectCube->depth  = yspacing.getValue() * 0.9;
    selectSep->addChild(new SoMaterial);
    selectSep->addChild(selectCube);
    content->addChild(selectSep);
//--add to group
    int index = m2min(insertAt, getNumIcons());
    if(index == -1) index = getNumIcons();
    SoGroup* g = getIcons();
    assert(g);
	fprintf(stderr,"SoSorterKit::add id: %s index: %d\n",
		id.getString(),index);
    g->insertChild(drag, index);
    resort();
    select(drag);
}

//----------------------------------------------------------------------------

void
SoSorterKit::unselect()
{
    if(getIcons()->findChild(selectedDrag) == -1) return;
    SoSeparator* content=(SoSeparator*)selectedDrag->getPart("content",FALSE);
    AGETCHILD(selectSep,SoSeparator,content,1);
    AGETCHILD(mat,SoMaterial,selectSep,0);
    mat->diffuseColor = SbColor(1,1,1);
}

//----------------------------------------------------------------------------

void
SoSorterKit::select(SbString id)
{
    SoDragKit* drag = (SoDragKit*)findNode(getIcons(),id);
    if(drag != NULL) select(drag);
}

//----------------------------------------------------------------------------

void
SoSorterKit::select(SoDragKit* drag)
{
    if(getIcons()->findChild(drag) == -1) return;
    unselect();
    selectedDrag = drag;
    SoSeparator* content = (SoSeparator*)drag->getPart("content", FALSE);
    AGETCHILD(selectSep,SoSeparator,content,1);
    AGETCHILD(mat,SoMaterial,selectSep,0);
    mat->diffuseColor = SbColor(1,0,0);
}

//----------------------------------------------------------------------------

void
SoSorterKit::remove(SbString id)
{
    SoNode* child = findNode(getIcons(),id);
    if(child != NULL) getIcons()->removeChild(child);
    resort();
}

//----------------------------------------------------------------------------

int
SoSorterKit::getNumIcons()
{
    return getIcons()->getNumChildren();
}

//----------------------------------------------------------------------------

SoGroup*
SoSorterKit::getIcons()
{
    return (SoGroup*)getPart("icons",FALSE);
}

//----------------------------------------------------------------------------

SbString
SoSorterKit::getId(int index)
{
    GETCHILD(drag,SoDragKit,getIcons(),index);
	if(drag == NULL) printf("SORTER warning: icon %d not found\n",index);
    return (drag != NULL) ? drag->getName().getString() : SbString("");
}

//----------------------------------------------------------------------------

void
SoSorterKit::computeSnapping(SoDragKit* drag, 
    int& oldIndex, int& newIndex, SbVec3f& newTranslation, SbBool& hasMoved)
{
    int x, y; float xf, yf;
    oldIndex = getIcons()->findChild(drag);
    assert(oldIndex != -1);
    SbVec3f transform = drag->translation.getValue();
    x = (int) ((transform[0] + 0.5) / (double) xspacing.getValue());
    x = m2min(x, rows.getValue()-1);
    x = m2max(0, x);
    y = (int) ((transform[2] + 0.5) / (double) yspacing.getValue());
    y = m2min(y, (getNumIcons() / rows.getValue()));
    y = m2max(0, y);
    newIndex = m2min(x + y * rows.getValue(), getNumIcons());
    hasMoved = (newIndex != oldIndex);
    xf = x * xspacing.getValue();
    yf = y * yspacing.getValue();
    if(hasMoved) 
    {
        xf -= xspacing.getValue() * 0.1;
        yf -= yspacing.getValue() * 0.1;
    }
    newTranslation = SbVec3f(xf,xspacing.getValue()*0.001,yf);
}
 
//----------------------------------------------------------------------------

void
SoSorterKit::pressCallback(void* userData, SoDragKit* drag)
{
    SoSorterKit* self = (SoSorterKit*)userData;
    self->select(drag);
}

//----------------------------------------------------------------------------

void
SoSorterKit::moveCallback(void* userData, SoDragKit* drag)
{
    SoSorterKit* self = (SoSorterKit*)userData;
    int oldIndex, newIndex; SbVec3f newTranslation; SbBool hasMoved;
    self->computeSnapping(drag,oldIndex,newIndex,newTranslation,hasMoved);
    drag->translation = newTranslation;
}

//----------------------------------------------------------------------------

void
SoSorterKit::releaseCallback(void* userData, SoDragKit* drag)
{
    SoSorterKit* self = (SoSorterKit*)userData;
    int oldIndex, newIndex; SbVec3f newTranslation; SbBool hasMoved;
    self->computeSnapping(drag,oldIndex,newIndex,newTranslation,hasMoved);
    drag->translation = newTranslation;
    if(hasMoved)
    {
        self->getIcons()->insertChild(drag, newIndex);
        if(newIndex < oldIndex) oldIndex++;
        self->getIcons()->removeChild(oldIndex);
        self->resort();
    }
}
//----------------------------------------------------------------------------

void
SoSorterKit::setMasterMode(SbBool masterMode)
{
	isMaster=masterMode;
}

//----------------------------------------------------------------------------
//eof

