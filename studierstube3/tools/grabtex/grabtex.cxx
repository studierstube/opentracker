// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       grabtex.cxx
//  TYPE:       c++ source code
//  PROJECT:    Studierstube
//  CONTENT:    Demo for onscreen renderer utility
//  VERSION:    1.3
// ===========================================================================
//  AUTHORS:    alf     Anton Fuhrmann
// ===========================================================================
//  HISTORY:
//
// ===========================================================================

/*

 grab inventor texture from screen  	alf000428

  MUCH faster than SoOffscreenRenderer since it's using the GL-hardware


 simple demo

 usage: grabtex file.iv


 "file.iv" gets displayed, all textured objects before the first "Texture2" 
 node are textured with screenshots

 in select mode the viewer recognizes the following keys:

	<SPACE>   toggle contiunuous texture update on/off
	<ENTER>   single texture update
	"R"       remove texture
 


*/

#include <stbapi/stbapi.h>
#include <Inventor/SoDB.h>
#include <Inventor/SoInput.h>
#ifndef USE_SOQT_LIB
#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/viewers/SoXtExaminerViewer.h>
#else
#include <Inventor/Qt/SoQt.h>
#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>
#endif
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/nodes/SoCube.h>

#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoTextureCoordinate2.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoSeparator.h>

#include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/actions/SoGLRenderAction.h>

#include <Inventor/nodes/SoCallback.h>


#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoFaceSet.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoNormal.h>
#include <Inventor/nodes/SoNormalBinding.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/nodes/SoLightModel.h>

#ifndef USE_SOQT_LIB
#include <Inventor/SoOffscreenRenderer.h>
#endif

#include <stbapi/misc/OnScreenRenderer.h>

#ifdef WIN32
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#else
#  ifdef _SGI_SOURCE
#  define INT8 dummy_INT8
#  define INT32 dummy_INT32
#  include <GL/glx.h>
#  include <GL/gl.h>
#  include <GL/glu.h>
#  undef INT8
#  undef INT32
#  endif
#endif
/////////////////////////////////////////////////////////////

#define USEMYGRAB // instead of standard SoOffscreenRenderer
#define TEX_SIZE 256

SoTexture2 *myTexture = new SoTexture2;
int continously = 1;

/////////////////////////////////////////////////////////////

SoSeparator *
readFile(const char *filename)
{
   // Open the input file
   SoInput mySceneInput;
   if (!mySceneInput.openFile(filename)) {
      fprintf(stderr, "Cannot open file %s\n", filename);
      return NULL;
   }

   // Read the whole file into the database
   SoSeparator *myGraph = SoDB::readAll(&mySceneInput);
   if (myGraph == NULL) {
      fprintf(stderr, "Problem reading file\n");
      return NULL;
   } 

   mySceneInput.closeFile();
   return myGraph;
}


/////////////////////////////////////////////////////////////

SbBool 
generateTextureMap (SoNode *root, SoTexture2 *texture, 
   short textureWidth, short textureHeight)
{
#ifndef USEMYGRAB
   SbViewportRegion myViewport(textureWidth, textureHeight);

   // Render the scene
   SoOffscreenRenderer *myRenderer = 
            new SoOffscreenRenderer(myViewport);
   myRenderer->setBackgroundColor(SbColor(0.3, 0.3, 0.3));
   if (!myRenderer->render(root)) {
      delete myRenderer;
      return FALSE;
   }

   // Generate the texture
   texture->image.setValue(SbVec2s(textureWidth, textureHeight),
            SoOffscreenRenderer::RGB, myRenderer->getBuffer());

   delete myRenderer;
#endif
   return TRUE; 
}


/////////////////////////////////////////////////////////////

// userData is the viewer.
void
myKeyPressCB(void *userData, SoEventCallback *eventCB)
{
   const SoEvent *event = eventCB->getEvent();

   if (SO_KEY_PRESS_EVENT(event, RETURN)) {
   		SbVec2s size;
		size.setValue(TEX_SIZE, TEX_SIZE);
   	short width, height;
	size.getValue(width, height);
	
//	fprintf(stderr, "Grabbing texture from screen. width=%d, height=%d.\n", width, height);
#ifdef USEMYGRAB
		onScreenRender((SoXtExaminerViewer *)userData, myTexture, TEX_SIZE, TEX_SIZE);
#else
	   	generateTextureMap(((SoXtExaminerViewer *)userData)->getSceneGraph(), myTexture, TEX_SIZE, TEX_SIZE);
#endif
   

   } else if (SO_KEY_PRESS_EVENT(event, R)) {
   		SbVec2s size;	
		size.setValue(0,0);
	  	fprintf(stderr, "Restoring texture"); 
		myTexture->image.setValue(0, 0, 0);
	} else if (SO_KEY_PRESS_EVENT(event, SPACE)) {
		continously=!continously;
	}
   eventCB->setHandled();
}

/////////////////////////////////////////////////////////////


void grabCallback(void *data, SoAction *action) {

static int reentry = 0;

if (reentry || !continously) return;
reentry=1;
              if (action->isOfType(SoGLRenderAction::getClassTypeId())) {
                              // Make my custom GL calls
#ifdef USEMYGRAB
   		onScreenRender((SoXtExaminerViewer *)data, myTexture, TEX_SIZE, TEX_SIZE);
#else
	   	generateTextureMap(((SoXtExaminerViewer *)data)->getSceneGraph(), myTexture, TEX_SIZE, TEX_SIZE);
#endif

// Invalidate the state so that a cache is not made
//        SoCacheElement::invalidate(action->getState());
              }
reentry=0;
}

/////////////////////////////////////////////////////////////

int
main(int argc, char **argv)
{
   // Initialize Inventor and Xt
#ifndef USE_SOQT_LIB
   Widget myWindow;
#else
   QWidget *myWindow;
#endif
   myWindow = SoXt::init(argv[0]);

   // Read the file
   SoSeparator *scene = readFile(argv[1]);

   // Create a viewer
   SoXtExaminerViewer *myViewer = 
            new SoXtExaminerViewer(myWindow);
			
	myViewer->setBackgroundColor(SbColor(0.2, 0.2, 0.25));


   SoSeparator *root = new SoSeparator;
   root->ref();
   
   root->addChild(new SoPerspectiveCamera);

   root->addChild(myTexture);
   myTexture->model=SoTexture2::DECAL;
//   myTexture->blendColor.setValue(0.5, 0.5, 0.5);
   root->addChild(scene);

// An event callback node so we can receive key press events
	SoEventCallback *myEventCB = new SoEventCallback;
	root->addChild(myEventCB);

	// add keyboard handler    
	myEventCB->addEventCallback(
         SoKeyboardEvent::getClassTypeId(),
         myKeyPressCB, myViewer);

   SoCallback *grab =  new SoCallback;
   grab->setCallback(grabCallback, myViewer);
   root->addChild(grab);

  // attach and show viewer
   myViewer->setSceneGraph(root);
   myViewer->setTitle("Texture Grabber");
   myViewer->viewAll();
   myViewer->show();



   // Loop forever
   SoXt::show(myWindow);
   SoXt::mainLoop();
   return 0;
}

