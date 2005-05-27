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
/** file browser - implementation
  *
  * @author  Rainer Splechtna, Jan Prikryl
  *
  * $Id: fileBrowser.cxx 3801 2004-09-30 14:02:02Z reitmayr $
  * @file                                                                   */
 /* ======================================================================= */


#ifdef WIN32
/* The stat call under NT doesn't define these macros */
#define S_ISDIR(mode)   ((mode&S_IFMT) == S_IFDIR)
#define S_ISREG(mode)   ((mode&S_IFMT) == S_IFREG)
#endif

#include <ace/Dirent.h>
#include <ace/OS.h>

#include <cassert>
#include <cstdio>
#include <cctype>

#include <cmath>
#include <string>
 
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/SbLinear.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/SoPath.h>

#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoCylinder.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoMatrixTransform.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoScale.h>
#include <Inventor/nodes/SoRotationXYZ.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoInfo.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransformSeparator.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/nodes/SoCallback.h>
#include <Inventor/nodes/SoFontStyle.h>
#include <Inventor/nodes/SoText3.h>

#include <Inventor/SoPickedPoint.h>
#include <Inventor/details/SoDetail.h>
#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/sensors/SoTimerSensor.h>

#include <Inventor/sensors/SoOneShotSensor.h>

#include <stbapi/workspace/StbWorkspace.h>
#include <stbapi/widgets/So3DButton.h> 
#include <stbapi/widgets/So3DCheckBox.h> 
#include <stbapi/util/ivio.h>
#include <stbapi/misc/ivmacros.h>

#include "fileBrowser.h"

using namespace std;

#define THUMBNAIL_BOX_SIZE 10.0f

#define FONT_SIZE 1.8

void fileBrowser::ivRotationSensorCallback(void *data, SoSensor *)
{
    SoRotation *moveThis = (SoRotation *)data;
    SbRotation currentRotation = moveThis->rotation.getValue();
    currentRotation = SbRotation(SbVec3f(1,1,1),M_PI/30.0) * currentRotation;
    moveThis->rotation.setValue(currentRotation);       
}


void fileBrowser::setFileButtonCallback(fbCallback func, void *data = NULL)
{
    fbCB = func;
    CBdata = data;
}


void fileBrowser::setFilter(char *filter, int readFiles)
{
    strcpy(currentFilter, filter);      
    strcpy(filterInput, filter);        
    if (readFiles)
        readFilesInDir();
}


void fileBrowser::fixFilter(int readFiles)
{
    filterFixed = 1;
    filterCheckBox->pressed = 1;
    if (readFiles)
        readFilesInDir();
}

// Changes the currentDir variable which defines the loading directory
// The workspace directory is still kept as .../bin/workspace
void fileBrowser::changeLoadPath(const char* path)
{
    // remember current Workspace directory
    string tempDir(currentDir);
    // change directory (by passing a relative path)
    // this ACE method is just used instead of string parsing
    ACE_OS::chdir(path);
    // set currentDir to the changed path
    ACE_OS::getcwd(currentDir, 1024);
    // change workspace directory back to previous workspace dir
    ACE_OS::chdir(tempDir.c_str());
}

fileBrowser::fileBrowser(void)
{
    actualFileNumber = 0;
    actualIVFiles    = NULL;
    fbCB             = NULL;
    CBdata           = NULL;
    filterFixed      = 0;

    // get current directory
    ACE_OS::getcwd(currentDir, 1024);
    strcpy(currentFilter, "");  
    strcpy(filterInput, ".iv"); 

    direntryGeometrySep        = new SoSeparator;
    direntryPressedGeometrySep = new SoSeparator;
    ivGeometrySep              = new SoSeparator;

    upDownRepeater = new SoTimerSensor(upDownRepeaterCallback, this);
    upDownRepeater->setInterval(0.2);

    ivRotation = new SoRotation;
    ivRotation->ref();
    ivRotationSensor = new SoTimerSensor(ivRotationSensorCallback, ivRotation);
    ivRotationSensor->setInterval(0.1);
    ivRotationSensor->schedule();
}


fileBrowser::~fileBrowser(void)
{
    if( upDownRepeater->isScheduled())
        upDownRepeater->unschedule();
    delete(upDownRepeater);
    if( ivRotationSensor->isScheduled())
        ivRotationSensor->unschedule();
    delete(ivRotationSensor);
    ivRotation->unref();
    freeEntries();
}


void fileBrowser::freeEntries()
{
    direntry *hptr;
    while(actualIVFiles != NULL)
    {
        hptr = actualIVFiles;
        actualIVFiles = actualIVFiles->next;
        free(hptr);
    }
}


SoSeparator *fileBrowser::createFilterText(void)
{
    SoTransform *textTransformation = new SoTransform;
    textTransformation->translation.setValue(0,1.1,0.5);
    textTransformation->rotation.setValue(SbVec3f(1,0,0), -1.57);
    SoTransform *textTransformationPressed = new SoTransform;
    textTransformationPressed->translation.setValue(0,0.1,0.5);
    textTransformationPressed->rotation.setValue(SbVec3f(1,0,0), -1.57);

    // set font
    SoSeparator *textRoot = new SoSeparator;
    
    SoFontStyle *myFont = new SoFontStyle;
    //myFont->name.setValue("Times New Roman");
    myFont->family.setValue( SoFontStyle::SANS );
    myFont->size = FONT_SIZE;
    
    filterText = new SoText3;
    filterText->string.setValue(filterInput);
    filterText->justification = SoText3::CENTER;
    
    SoMaterial *textMaterial = new SoMaterial;
    textMaterial->diffuseColor.setValue(0.5,0.5,0.5);
    textMaterial->specularColor.setValue(0.0,0.0,0.0);
    textMaterial->ambientColor.setValue(0.0,0.0,0.0);
    textMaterial->emissiveColor.setValue(0.0,0.0,0.0); 
    
    SoMaterial *textMaterialPressed = new SoMaterial;
    textMaterialPressed->diffuseColor.setValue(1.0,0.0,0.0);
    textMaterialPressed->specularColor.setValue(0.0,0.0,0.0);
    textMaterialPressed->ambientColor.setValue(0.0,0.0,0.0);
    textMaterialPressed->emissiveColor.setValue(0.0,0.0,0.0); 

    SoGroup *textMode = new SoGroup;
    textMode->addChild(textTransformation);
    textMode->addChild(textMaterial);
    
    SoGroup *textModePressed = new SoGroup;
    textModePressed->addChild(textTransformationPressed);
    textModePressed->addChild(textMaterialPressed);
    
    SoSwitch *pressedSwitch = new SoSwitch;
    textRoot->addChild(pressedSwitch);  
    pressedSwitch->addChild(textMode);
    pressedSwitch->addChild(textModePressed);
    
    textRoot->addChild(myFont);
    textRoot->addChild(filterText);
    
    pressedSwitch->whichChild.connectFrom(&filterCheckBox->pressed);
    
    return textRoot;
}

// callback functions ---------------------------------------------------------

void fileBrowser::filterKeyEventHandle(void *data, SoEventCallback *myEvent) 
{
    fileBrowser *ol = (fileBrowser *) data;     
    
    if (ol->filterFixed)
        return;
    
    const SoEvent *event = myEvent->getEvent();
    SoKeyboardEvent::Key pressed;
    
    if (SoKeyboardEvent::isKeyPressEvent(event, SoKeyboardEvent::ANY))
    {
        pressed = ((SoKeyboardEvent *)event)->getKey(); 
        if ((pressed == SoKeyboardEvent::RETURN) ||
            (pressed == SoKeyboardEvent::PAD_ENTER))
        {
            fileBrowser::filterCallback(data, ol->filterCheckBox);
        }
        else if (pressed == SoKeyboardEvent::BACKSPACE)
        {
            if (strlen(ol->filterInput)>0)
            {
                ol->filterInput[strlen(ol->filterInput)-1] = '\0';
                ol->filterText->string.setValue(ol->filterInput);
            }
        }
        else if ((pressed == SoKeyboardEvent::PAD_PERIOD) || 
                 (pressed == SoKeyboardEvent::PERIOD))
        {
            strcat(ol->filterInput,     ".");
            ol->filterText->string.setValue(ol->filterInput);
        }
        else
        {
            char c = ((SoKeyboardEvent *)event)->getPrintableCharacter();
            strcat(ol->filterInput,     &c);
            ol->filterText->string.setValue(ol->filterInput);
        }
    }
    
    myEvent->setHandled();
}


void fileBrowser::filterCallback(void *data, So3DCheckBox *fb)
{
    fileBrowser *ol = (fileBrowser *) data;     
    
    if (ol->filterFixed)
    {
        fb->pressed = 1;
        return;
    }

    if (fb->pressed.getValue())
    {
        strcpy(ol->currentFilter, ol->filterInput);
    }   
    else
        strcpy(ol->currentFilter, "");
    
    ol->readFilesInDir();
}


void fileBrowser::upDownRepeaterCallback(void *data, SoSensor *)
{
    fileBrowser *ol = (fileBrowser *) data;
    upDownPressCallback(data, ol->upDownButtonPressed);
}


void fileBrowser::upDownPressCallback(void *data, So3DButton *button)
{
    fileBrowser *ol = (fileBrowser *) data;
        
    if ((button == ol->upButton) && (ol->currentFilePos > 0))
    {
        ol->currentFilePos--;
        ol->setButtons();
    }
    if ((button == ol->downButton) && 
        ((ol->actualFileNumber)-(ol->currentFilePos) > 1))
    {
        ol->currentFilePos++;
        ol->setButtons();
    }

    ol->upDownButtonPressed = button;
    if (!ol->upDownRepeater->isScheduled())
        ol->upDownRepeater->schedule();
}


void fileBrowser::upDownReleaseCallback(void *data, So3DButton *button)
{
    fileBrowser *ol = (fileBrowser *) data;
    if (ol->upDownRepeater->isScheduled())
        ol->upDownRepeater->unschedule();
}


void fileBrowser::objectLoadCallback(void *data, So3DButton *button)
{
    fileBrowser *ol = (fileBrowser *) data;
    
#ifdef WIN32
    const char *delim = "\\";
#else
    const char *delim = "/";
#endif

    int filenum = 0,i;
    direntry *currentFileEntry = ol->actualIVFiles;
    
    for (i=0; i<MAX_BROWSER_BUTTONS; i++)
    {
        if (button ==  ol->direntryButton[i])
        {
            filenum =  ol->currentFilePos + i;
            break;
        }
    }   
    
    for (i=0;i<filenum;i++) currentFileEntry = currentFileEntry->next;
    
    string curDir(ol->currentDir);
    
    // directory
    if (currentFileEntry->type == 1)
    {
        if (strcmp(currentFileEntry->name, "..") == 0)
        {
            // get previous entry in the directory hierarchy
            curDir = curDir.substr(0,curDir.rfind(delim, curDir.size()-1));

            curDir.copy(ol->currentDir, curDir.size());
            ol->currentDir[curDir.size()] = '\0';
        }
        else if (strcmp( currentFileEntry->name, ".") != 0)
        {
            curDir += delim;            
            curDir += currentFileEntry->name;
            
            curDir.copy(ol->currentDir, curDir.size());
            ol->currentDir[curDir.size()] = '\0';
        }
        
        ol->readFilesInDir();        
    }
    // file
    else if (ol->fbCB)
    {
        curDir += delim;
#ifdef WIN32
        // sgi oiv under windows needs /'s not \'s as directory delimiters
        // FIXME: check if this is also valid for Coin under Windows. 20021226 tamer.
        string::size_type idx = 0; 
        while((idx = curDir.find("\\", idx)) != string::npos)
            curDir.replace(idx++, 1, "/");
		// sgi oiv also doesn't appreciate the leading drive letters. 
		// therefore we remove them here as well
		curDir = curDir.substr(2);
#endif

        curDir += currentFileEntry->name;
        (*ol->fbCB)(ol->CBdata, curDir.c_str());
    }
}


SoSeparator *fileBrowser::setupLoaderSheet()
{
    int i;
    SoSeparator *sheetSep = new SoSeparator;
    
    char ivName[512];
    strcpy(ivName, StbWorkspace::getInstance()->getWorkspaceDirectory().getString());
    strcat(ivName, "system/filter.iv");
    
    filterCheckBox = (So3DCheckBox *)readNodeFromFile(ivName);
    filterCheckBox->setName("");
    SoNode *textNode;
    filterCheckBox->setText(textNode = createFilterText());     
    filterCheckBox->setPressedText(textNode);   
    
    strcpy(ivName, StbWorkspace::getInstance()->getWorkspaceDirectory().getString());
    strcat(ivName, "system/browserbutton.iv");
    
    for (i=0;i<MAX_BROWSER_BUTTONS;i++)
    {
        direntryButton[i] = (So3DButton *)readNodeFromFile(ivName);
        direntryButton[i]->setName("");
    }

    strcpy(ivName, StbWorkspace::getInstance()->getWorkspaceDirectory().getString());
    strcat(ivName, "system/updownbutton.iv");
    SoSeparator *upDown = readFile(ivName);
    assert(upDown);
    AVGETNODE(upButton,So3DButton,"BROWSER_UP_BUTTON");
    AGETNODE(texture,SoTexture2,"BROWSER_UP_TEXTURE");
    texture->setName("");
    upButton->setupTexture(texture,0,0);
    upButton->setName("");
    AVGETNODE(downButton,So3DButton,"BROWSER_DOWN_BUTTON");
    downButton->setName("");
    AVGETNODE(texture,SoTexture2,"BROWSER_DOWN_TEXTURE");
    downButton->setupTexture(texture,0,0);
    texture->setName("");
    
    SoTranslation *zTrans = new SoTranslation;
    zTrans->translation.setValue(0,0,3);
    SoTranslation *z4Trans = new SoTranslation;
    z4Trans->translation.setValue(0,0,4);
    
    SoTranslation *downTrans = new SoTranslation;
    downTrans->translation.setValue(20,0,-8);
    
    SoTranslation *upTrans = new SoTranslation;
    upTrans->translation.setValue(0,0,-13);
    
    SoTranslation *startTrans = new SoTranslation;
    startTrans->translation.setValue(0,1,0);
    
    for (i=0;i<MAX_BROWSER_BUTTONS;i++) {
        buttonSwitch[i] = new SoSwitch;
        buttonSwitch[i]->addChild(direntryButton[i]);
        buttonSwitch[i]->whichChild=0;
        direntryButtonIVSwitch[i] = new SoSwitch;                       
        direntryButtonIVSwitch[i]->addChild(new SoSeparator);
        direntryButtonIVSwitch[i]->addChild(new SoSeparator);
        direntryButtonIVSwitch[i]->addChild(new SoSeparator);
        direntryButtonIVSwitch[i]->whichChild.connectFrom(&direntryButton[i]->highlighted);
        direntryButton[i]->setAdditionalGeometry(direntryButtonIVSwitch[i]);
    }

    SoRotationXYZ *startRot = new SoRotationXYZ;
    startRot->axis = SoRotationXYZ::X;
    startRot->angle = 1.57;
    sheetSep->addChild(startRot);
    sheetSep->addChild(startTrans);     
    sheetSep->addChild(filterCheckBox); 
    sheetSep->addChild(z4Trans);
    
    for (i=0;i<MAX_BROWSER_BUTTONS;i++)
    {
        sheetSep->addChild(buttonSwitch[i]);
        sheetSep->addChild(zTrans);
    }

    sheetSep->addChild(downTrans);
    sheetSep->addChild(downButton);
    
    sheetSep->addChild(upTrans);
    sheetSep->addChild(upButton);
    
    upButton->addReleaseCallback(fileBrowser::upDownReleaseCallback,
                                 (void *) this);
    downButton->addReleaseCallback(fileBrowser::upDownReleaseCallback,
                                   (void *) this);
    upButton->addPressCallback(fileBrowser::upDownPressCallback,
                               (void *) this);
    downButton->addPressCallback(fileBrowser::upDownPressCallback,
                                 (void *) this);

    filterCheckBox->addPressCallback(filterCallback, this);
    for (i=0;i<MAX_BROWSER_BUTTONS;i++)
        direntryButton[i]->addReleaseCallback(fileBrowser::objectLoadCallback,
                                              (void *)this);
    
    // set key-event-handler
    SoEventCallback *mEvent = new SoEventCallback;
    sheetSep->addChild(mEvent);
    mEvent->addEventCallback(SoKeyboardEvent::getClassTypeId(),
                             filterKeyEventHandle, this);               
    
    readFilesInDir();
    
    return sheetSep;
}



SoSeparator *fileBrowser::readThumbnail(const char *path)
{
    SoSeparator *thumbnail = new SoSeparator;
    return thumbnail;
    SbViewportRegion vp;
    SoGetBoundingBoxAction ivBBox(vp);
    SoSeparator *ivFile = readFile(path);
    
    // if ivFile is not valid
    if (!ivFile)
        return thumbnail;
    
    ivFile->ref();
    ivBBox.apply(ivFile);
    ivFile->unrefNoDelete();
    
    SbBox3f bBox= ivBBox.getBoundingBox();
    if (bBox.isEmpty())
        return thumbnail;
    else {
        SbVec3f center = bBox.getCenter();
        float bx,by,bz;
        bBox.getSize(bx,by,bz);
        float max = bx;
        if (by>max)
            max = by;
        if (bz>max)
            max = bz;           
        
        float boxSF = THUMBNAIL_BOX_SIZE/max;
        SoTranslation *boxCenter = new SoTranslation;           
        boxCenter->translation = -center;
        SoTranslation *thumbnailCenter = new SoTranslation;             
        thumbnailCenter->translation = SbVec3f(0,10,0);
        SoScale *boxScale = new SoScale;
        boxScale->scaleFactor = SbVec3f(boxSF,boxSF,boxSF);
        
        thumbnail->addChild(thumbnailCenter);           
        thumbnail->addChild(ivRotation);                
        thumbnail->addChild(boxCenter);         
        thumbnail->addChild(boxScale);                          
        thumbnail->addChild(ivFile);                            
        
        return thumbnail;
    }
}


void fileBrowser::loadIVFiles() 
{
    direntry *currentEntry = actualIVFiles;
    ivGeometrySep->removeAllChildren();
        
    while(currentEntry != NULL)
    {
        if ((currentEntry->type == 0) && 
            (strstr(currentEntry->name, ".iv") != NULL))
        {               
            string path(currentDir);
            path += "/";
            path += currentEntry->name;
            ivGeometrySep->addChild(readThumbnail(path.c_str()));
        }
        else 
            ivGeometrySep->addChild(new SoSeparator);
        
        currentEntry = currentEntry->next;
    }
}


void insertDirentryInList(direntry *newEntry, direntry *&listStart)
{
    direntry *temp, *tempPrev;
    
    if (listStart)
    {
        temp = listStart;
        tempPrev = NULL;
        while((temp != NULL) && (strcmp(newEntry->name,temp->name) > 0))
        {
            tempPrev = temp;
            temp = temp->next;
        }
        // newEntry is new start of the list
        if (!tempPrev)
            listStart = newEntry;
        else
            tempPrev->next = newEntry;
        newEntry->next = temp;
        
    }
    else 
        listStart = newEntry;                                           
}


void fileBrowser::readFilesInDir()
{
   dirent *dp;
   struct stat statRec;
   direntry * hptr, *fileEntries = NULL, *lastDirEntry;

   actualFileNumber = 0;
   freeEntries();

    // try to open the directory
   ACE_DIR *dirp = ACE_OS_Dirent::opendir(currentDir);
   if (dirp == NULL)
   {
       return;
   }

   ACE_OS_Dirent::rewinddir(dirp);
       
   // and read all directories
   while ((dp = ACE_OS_Dirent::readdir(dirp)) != NULL)
   {
       string absFilename(currentDir);
       absFilename += "/";
       absFilename += dp->d_name;
       
       if (stat(absFilename.c_str(), &statRec) == 0)
       {
           if (S_ISDIR(statRec.st_mode))
           {
               hptr = (direntry *) malloc(sizeof(direntry));
               strcpy(hptr->name, dp->d_name);
               hptr->type = 1;
               hptr->next = NULL;       
               insertDirentryInList(hptr, actualIVFiles);
               actualFileNumber++;   
           }
       }
   }
   
   ACE_OS_Dirent::closedir(dirp);
   
   lastDirEntry = actualIVFiles;        
   while(lastDirEntry->next != NULL) lastDirEntry = lastDirEntry->next;

   dirp = ACE_OS_Dirent::opendir(currentDir);
   if (dirp == NULL)
   {
       return;
   }
   
   ACE_OS_Dirent::rewinddir(dirp);
   
   // and read all files
   while ((dp = ACE_OS_Dirent::readdir(dirp)) != NULL)
   {
       string absFilename(currentDir);
       absFilename += "/";
       absFilename += dp->d_name;
       
       if (stat(absFilename.c_str(), &statRec) == 0) {
           if ((S_ISREG(statRec.st_mode)) && 
               (strstr(dp->d_name,currentFilter) != NULL))
           {
               hptr = (direntry *) malloc(sizeof(direntry));
               strcpy(hptr->name, dp->d_name);
               hptr->type = 0;
               hptr->next = NULL;
               insertDirentryInList(hptr, fileEntries);
               actualFileNumber++;
           }
       }
   }

   ACE_OS_Dirent::closedir(dirp);
   lastDirEntry->next = fileEntries;
   
   currentFilePos=0;   
   createButtonGeometry();
   loadIVFiles();
   setButtons();   
}


SoSeparator *createText(char *name, int mode)
{
    // set font
    SoSeparator *textRoot = new SoSeparator;
    
    SoFontStyle *myFont = new SoFontStyle;
    //myFont->name.setValue("Times New Roman");
    myFont->family.setValue( SoFontStyle::SANS );
    myFont->size = FONT_SIZE;
    
    SoText3 *text = new SoText3;
    text->string.setValue(name);
    text->justification = SoText3::CENTER;
    
    SoMaterial *textMaterial = new SoMaterial;
    if (mode == 0)
        textMaterial->diffuseColor.setValue(0.0,0.0,0.0);
    else if (mode == 1)
        textMaterial->diffuseColor.setValue(0.0,0.0,1.0);
    textMaterial->specularColor.setValue(0.0,0.0,0.0);
    textMaterial->ambientColor.setValue(0.0,0.0,0.0);
    textMaterial->emissiveColor.setValue(0.0,0.0,0.0); 
    
    /*  SoTranslation *textTrans = new SoTranslation;
        textTrans->translation = (-14,0,0);     
        textRoot->addChild(textTrans);
    */
    
    textRoot->addChild(myFont);
    textRoot->addChild(textMaterial);
    textRoot->addChild(text);
    
   return textRoot;
}



void fileBrowser::createButtonGeometry()
{
    direntry *currentEntry = actualIVFiles;
    SoSeparator *iconSep, *iconPressedSep, *iconText;

    SoTransform *iconTransformation = new SoTransform;
    iconTransformation->translation.setValue(0,1.6,0.5);
    SoTransform *iconPressedTransformation = new SoTransform;
    iconPressedTransformation->translation.setValue(0,0.1,0.5);
    
    iconTransformation->rotation.setValue(SbVec3f(1,0,0), -1.57);
    iconPressedTransformation->rotation.setValue(SbVec3f(1,0,0), -1.57);
    
    direntryGeometrySep->removeAllChildren();
    direntryPressedGeometrySep->removeAllChildren();
    
    while(currentEntry != NULL)
    {
        iconSep = new SoSeparator;
        iconPressedSep = new SoSeparator;
        iconText = createText(currentEntry->name, currentEntry->type);
        
        iconSep->addChild(iconTransformation);
        iconSep->addChild(iconText);
        
        iconPressedSep->addChild(iconPressedTransformation);
        iconPressedSep->addChild(iconText);
        
        direntryGeometrySep->addChild(iconSep);
        direntryPressedGeometrySep->addChild(iconPressedSep);
        
        currentEntry = currentEntry->next;
    }
}




void fileBrowser::setButtons()
{
    int i;
    int buttonCount = MAX_BROWSER_BUTTONS;
    int maxButtons  = actualFileNumber - currentFilePos;

    // remove buttons if necessary
    if (maxButtons < MAX_BROWSER_BUTTONS)
    {
        buttonCount = maxButtons;
        for (i=maxButtons; i<MAX_BROWSER_BUTTONS; i++) 
            buttonSwitch[i]->whichChild = -1;
    }
    
    for (i=0; i<buttonCount; i++)               
    {
        buttonSwitch[i]->whichChild=0;
        direntryButton[i]->setText(direntryGeometrySep->getChild(currentFilePos+i));
        direntryButton[i]->setPressedText(direntryPressedGeometrySep->getChild(currentFilePos+i));
        direntryButtonIVSwitch[i]->replaceChild(1, ivGeometrySep->getChild(currentFilePos+i));
    }
}

/* ===========================================================================
    End of fileBrowser.cxx
   ===========================================================================
    Automatic Emacs configuration follows.
    Local Variables:
    mode:c++
    c-basic-offset: 4
    eval: (c-set-offset 'substatement-open 0)
    eval: (c-set-offset 'case-label '+)
    eval: (c-set-offset 'statement 'c-lineup-runin-statements)
    eval: (setq indent-tabs-mode nil)
    End:
=========================================================================== */
