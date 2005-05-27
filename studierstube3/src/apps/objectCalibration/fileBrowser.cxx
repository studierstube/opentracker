/* ===========================================================================
    (C) 2000 Vienna University of Technology
   ===========================================================================
    NAME:       fileBrowser
    TYPE:       c++ code
    PROJECT:    Studierstube
    CONTENT:    file browser - implementation
    VERSION:    2.1
   ===========================================================================
    AUTHORS:    rcs     Rainer Splechtna
                jp      Jan Prikryl
   ===========================================================================
    HISTORY:

    20-Nov-00 17:28:32  jp      last modification
    28-oct-99 18:15:00  rcs     created
=========================================================================== */

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <math.h>

#include <sys/dir.h>
#include <sys/types.h>
#include <sys/stat.h>


#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <X11/IntrinsicP.h>
#include <Xm/XmStrDefs.h>
  
#include <Inventor/Xt/SoXt.h>
#include <Inventor/SoDB.h>
#include <Inventor/Xt/SoXtRenderArea.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/SbLinear.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/SoPath.h>

#include <Inventor/Xt/viewers/SoXtViewer.h>
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
#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoText3.h>

#include <Inventor/SoPickedPoint.h>
#include <Inventor/details/SoDetail.h>
#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/sensors/SoTimerSensor.h>

#include <Inventor/sensors/SoOneShotSensor.h>

//#include <stbapi/viewer/StereoViewer.h>
#include <stbapi/tracker/SoTrakEngine.h>
#include <stbapi/workspace/StbCommunicator.h>
#include <stbapi/widgets/So3DButton.h> 
#include <stbapi/widgets/So3DCheckBox.h> 
#include <stbapi/util/ivio.h>
#include <stbapi/util/zpthread.h>

#include "fileBrowser.h"

#ifdef WIN32
#define main ivMain
#endif

#define THUMBNAIL_BOX_SIZE 10.0

#define FONT_SIZE 2.0


// #define DEBUG_ON

FILE *fp;

#undef printLn

#ifdef DEBUG_ON
#define printLn fprintf
#else
#define printLn //
#endif


void fileBrowser::initClass()
{

      	#ifdef DEBUG_ON
               	fp = fopen("apploader.dat","wb");
               	char command[1000];
               	sprintf(command,"xterm -display knieriem:0.0 -title \"App-Loader\" -e debug/debugout apploader.dat &\n");
               	system(command);
       	#endif

}


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


fileBrowser::fileBrowser(void)
{
   actualFileNumber = 0;
   actualIVFiles = NULL;
	fbCB = NULL;
	CBdata = NULL;
	filterFixed = 0;

	// get current directory
   getcwd(currentDir, 1024);
   strcat(currentDir, "/");
   strcpy(currentFilter, "");	
   strcpy(filterInput, ".iv");	

   direntryGeometrySep = new SoSeparator;
   direntryPressedGeometrySep = new SoSeparator;
   ivGeometrySep = new SoSeparator;

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
	upDownRepeater->unschedule();
	delete(upDownRepeater);
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



SoNode *readNode(const char *filename)
{
   // Open the input file
   SoInput mySceneInput;
   if (!mySceneInput.openFile(filename)) {
      fprintf(stderr, "Cannot open file %s\n", filename);
      return NULL;
   }

   // Read the whole file into the database
   SoNode *myNode;
	SoDB::read(&mySceneInput, myNode);
   if (myNode == NULL) {
      fprintf(stderr, "error reading node from file\n");
      return NULL;
   } 

   mySceneInput.closeFile();
   return myNode;
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

   SoFont *myFont = new SoFont;
   myFont->name.setValue("Times New Roman");
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

/*	SoTranslation *textTrans = new SoTranslation;
	textTrans->translation = (-14,0,0);	
	textRoot->addChild(textTrans);
*/
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

void filterKeyEventHandle(void *data, SoEventCallback *myEvent) 
{
   fileBrowser *ol = (fileBrowser *) data;	

	if (ol->filterFixed)
		return;

	const SoEvent *event = myEvent->getEvent();
   SoKeyboardEvent::Key pressed;

	if (SoKeyboardEvent::isKeyPressEvent(event, SoKeyboardEvent::ANY)) {
		pressed = ((SoKeyboardEvent *)event)->getKey();	
		if ((pressed == SoKeyboardEvent::RETURN) || (pressed == SoKeyboardEvent::PAD_ENTER)){
			fileBrowser::filterCallback(data, ol->filterCheckBox);
		}
		else if (pressed == SoKeyboardEvent::BACKSPACE) {
			if (strlen(ol->filterInput)>0) {
	      	ol->filterInput[strlen(ol->filterInput)-1] = '\0';
			   ol->filterText->string.setValue(ol->filterInput);
			}
		}
		else if ((pressed == SoKeyboardEvent::PAD_PERIOD) || (pressed == SoKeyboardEvent::PERIOD)) {
			strcat(ol->filterInput,	".");
		   ol->filterText->string.setValue(ol->filterInput);
		}
		else {
			char c = ((SoKeyboardEvent *)event)->getPrintableCharacter();
			strcat(ol->filterInput,	&c);
		   ol->filterText->string.setValue(ol->filterInput);
		}
	}

	myEvent->setHandled();
	
}








void fileBrowser::filterCallback(void *data, So3DCheckBox *fb)
{
   fileBrowser *ol = (fileBrowser *) data;	

	if (ol->filterFixed) {
		fb->pressed = 1;
		return;
	}

	if (fb->pressed.getValue()) {
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

	printLn(fp," %d \n",(int) data);fflush(fp);
	printLn(fp," %d %d %d\n",(int) data, ol->currentFilePos,ol->actualFileNumber);fflush(fp);

   if ((button == ol->upButton) && (ol->currentFilePos > 0))
	{
      ol->currentFilePos--;
	   ol->setButtons();
	}
   if ((button == ol->downButton) && ((ol->actualFileNumber)-(ol->currentFilePos) > 1))
	{
       ol->currentFilePos++;
	    ol->setButtons();
	}

	ol->upDownButtonPressed = button;
	ol->upDownRepeater->schedule();	
}


void fileBrowser::upDownReleaseCallback(void *data, So3DButton *button)
{
   fileBrowser *ol = (fileBrowser *) data;
	ol->upDownRepeater->unschedule();	
}


void fileBrowser::objectLoadCallback(void *data, So3DButton *button)
{
   fileBrowser *ol = (fileBrowser *) data;

	printLn(fp," %d \n",(int) data);fflush(fp);
//   loadParameter *loadStruct = (loadParameter *) malloc(sizeof(loadParameter));

    int filenum,i;
    direntry *currentFileEntry = ol->actualIVFiles;
    char *lastDir;

	for (i=0; i<MAX_BROWSER_BUTTONS; i++)	{
	   if (button ==  ol->direntryButton[i]) {
       	filenum =  ol->currentFilePos + i;
			break;
		}
	}	

    for (i=0;i<filenum;i++) currentFileEntry = currentFileEntry->next;
    printLn(fp,"ENTRY: %s %d %d\n",currentFileEntry->name, currentFileEntry->type,filenum);fflush(fp);

    if (currentFileEntry->type == 1)
    {       // directory
        if (strcmp(currentFileEntry->name, "..") == 0)
        {
				// delete last '/'
            ol->currentDir[strlen(ol->currentDir)-1] = '\0';
				// delete last pathentry
            lastDir = strrchr(ol->currentDir,'/');
            lastDir++;
            *lastDir = '\0';
        }
        else if (strcmp( currentFileEntry->name, ".") != 0)
        {
            strcat( ol->currentDir, currentFileEntry->name);
            strcat( ol->currentDir, "/");
        }
			

			ol->readFilesInDir();
	
   }
	// file
   else
   {     
		if (ol->fbCB)
			(*ol->fbCB)(ol->CBdata,currentFileEntry->name);
   }

}




SoSeparator *fileBrowser::setupLoaderSheet(StbCommunicator *comMan)
{
	int i;
	SoSeparator *sheetSep = new SoSeparator;

	char ivName[512];
	strcpy(ivName, comMan->workingDirectory);
	strcat(ivName, "filter.iv");

	filterCheckBox = (So3DCheckBox *)readNode(ivName);
	filterCheckBox->setName("");
	SoNode *textNode;
	filterCheckBox->setText(textNode = createFilterText());	
	filterCheckBox->setPressedText(textNode);	

	strcpy(ivName, comMan->workingDirectory);
	strcat(ivName, "browserbutton.iv");

	for (i=0;i<MAX_BROWSER_BUTTONS;i++) {
		direntryButton[i] = (So3DButton *)readNode(ivName);
		direntryButton[i]->setName("");
	}

//	upButton = new So3DButton(3,2,9,fileBrowser::upTexture);
//	downButton = new So3DButton(3, 2, 9,fileBrowser::downTexture);

	strcpy(ivName, comMan->workingDirectory);
	strcat(ivName, "updownbutton.iv");

	upButton = (So3DButton *)readNode(ivName);
	upButton->setName("");
//	upButton->setTexture(fileBrowser::upTexture);
	downButton = (So3DButton *)readNode(ivName);
	downButton->setName("");

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

	sheetSep->addChild(startTrans);	
	sheetSep->addChild(filterCheckBox);	
	sheetSep->addChild(z4Trans);

	for (i=0;i<MAX_BROWSER_BUTTONS;i++) {
		sheetSep->addChild(buttonSwitch[i]);
		sheetSep->addChild(zTrans);
	}

	sheetSep->addChild(downTrans);
	sheetSep->addChild(downButton);

	sheetSep->addChild(upTrans);
	sheetSep->addChild(upButton);

	SoDrawStyle *pipDS = new SoDrawStyle;
	pipDS->style = SoDrawStyle::INVISIBLE;
	        
	SoCube *surroundCube = new SoCube;
	surroundCube->width = 30;
	surroundCube->height = 2;
	surroundCube->depth = 26;

	SoTranslation *cubeTrans = new SoTranslation;
	cubeTrans->translation.setValue(-18,0,6);

	sheetSep->addChild(pipDS);
	sheetSep->addChild(cubeTrans);
	sheetSep->addChild(surroundCube);

	upButton->addReleaseCallback(fileBrowser::upDownReleaseCallback, (void *) this);
	downButton->addReleaseCallback(fileBrowser::upDownReleaseCallback, (void *) this);
	upButton->addPressCallback(fileBrowser::upDownPressCallback, (void *) this);
	downButton->addPressCallback(fileBrowser::upDownPressCallback, (void *) this);

	filterCheckBox->addPressCallback(filterCallback, this);
	for (i=0;i<MAX_BROWSER_BUTTONS;i++)
		direntryButton[i]->addReleaseCallback(fileBrowser::objectLoadCallback, (void *)this);

	// set key-event-handler
   SoEventCallback *mEvent = new SoEventCallback;
	sheetSep->addChild(mEvent);
	mEvent->addEventCallback(SoKeyboardEvent::getClassTypeId(), filterKeyEventHandle, this);		

	readFilesInDir();

	return sheetSep;
}



SoSeparator *fileBrowser::readThumbnail(char *path)
{
	SoSeparator *thumbnail = new SoSeparator;

	SbViewportRegion vp;
	SoGetBoundingBoxAction ivBBox(vp);
	SoSeparator *ivFile = readFile(path);

	// if ivFile is not valid
	if (!ivFile)
		return thumbnail;

	ivFile->ref();
	ivBBox.apply(ivFile);

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
   int num = ivGeometrySep->getNumChildren();

	char path[1024];

	while(currentEntry != NULL)
   {
		if ((currentEntry->type == 0) && (strstr(currentEntry->name, ".iv") != NULL)) {		
	 		strcpy( path, currentDir);
   	   strcat( path, currentEntry->name);
 		   ivGeometrySep->addChild(readThumbnail(path));
		}
		else 
 		   ivGeometrySep->addChild(new SoSeparator);

      currentEntry = currentEntry->next;
    }
}



void insertDirentryInList(direntry *newEntry, direntry *&listStart)
{
   direntry *temp, *tempPrev;

	if (listStart) {
		temp = listStart;
		tempPrev = NULL;
		while((temp != NULL) && (strcmp(newEntry->name,temp->name) > 0)) {
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
	struct direct *dp; 
	struct stat statRec;
   char absFilename[1024];
   direntry * hptr, *fileEntries = NULL, *lastDirEntry;

   actualFileNumber = 0;
   freeEntries();

    // try to open the directory
	DIR *dirp = opendir(currentDir);
	if (dirp == NULL)
   {
       printLn(fp,"Dir not found\n");fflush(fp);
       return;
   }
       
       // and read all stb-files and directories
   while ((dp = readdir(dirp)) != NULL)
   {
       strcpy(absFilename, currentDir);
       strcat(absFilename, dp->d_name);

       if (stat(absFilename, &statRec) == 0) {
            if (S_ISDIR(statRec.st_mode)) {
            	hptr = (direntry *) malloc(sizeof(direntry));
               strcpy(hptr->name, dp->d_name);
               hptr->type = 1;
               hptr->next = NULL;	
					insertDirentryInList(hptr, actualIVFiles);
               actualFileNumber++;

            }
        }
    }
   closedir(dirp);
	
	lastDirEntry = actualIVFiles;	
	while(lastDirEntry->next != NULL)
		lastDirEntry = lastDirEntry->next;

	dirp = opendir(currentDir);
	if (dirp == NULL)
   {
       printLn(fp,"Dir not found\n");fflush(fp);
       return;
   }
       
       // and read all stb-files and directories
   while ((dp = readdir(dirp)) != NULL)
   {
       strcpy(absFilename, currentDir);
       strcat(absFilename, dp->d_name);

       if (stat(absFilename, &statRec) == 0) {
            if ((S_ISREG(statRec.st_mode)) && (strstr(dp->d_name,currentFilter) != NULL)) {
            	hptr = (direntry *) malloc(sizeof(direntry));
               strcpy(hptr->name, dp->d_name);
               hptr->type = 0;
               hptr->next = NULL;
					insertDirentryInList(hptr, fileEntries);
               actualFileNumber++;
            }
        }
    }
   closedir(dirp);
	lastDirEntry->next = fileEntries;

	currentFilePos=0;
	printf("filenumber: %d\n", actualFileNumber);

   createButtonGeometry();
	loadIVFiles();
   setButtons();

	printLn(fp,"END %d %d %d\n",actualFileNumber, currentFilePos, (int) this); fflush(fp);
}


SoSeparator *createText(char *name, int mode)
{
	// set font
   SoSeparator *textRoot = new SoSeparator;

   SoFont *myFont = new SoFont;
   myFont->name.setValue("Times New Roman");
   myFont->size = FONT_SIZE;
   
   SoText3 *text = new SoText3;
   text->string.setValue(name);
   text->justification = SoText3::CENTER;

   SoMaterial *textMaterial = new SoMaterial;
	if (mode == 0)
		textMaterial->diffuseColor.setValue(0.0,0.0,0.0);
	else if (mode == 1)
		textMaterial->diffuseColor.setValue(0.0,1.0,0.0);
	textMaterial->specularColor.setValue(0.0,0.0,0.0);
	textMaterial->ambientColor.setValue(0.0,0.0,0.0);
   textMaterial->emissiveColor.setValue(0.0,0.0,0.0); 

/*	SoTranslation *textTrans = new SoTranslation;
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
    iconTransformation->translation.setValue(0,1.1,0.5);
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
	int buttonCount= MAX_BROWSER_BUTTONS;
	int i;

	int maxButtons = actualFileNumber - currentFilePos;

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


