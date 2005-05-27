

//wird nicht alles gebraucht - check it irgendwann
#include <Inventor/SoDB.h>
#include <Inventor/SoInput.h> 
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoCylinder.h>
#include <Inventor/nodes/SoSphere.h> 
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoPointLight.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoTranslation.h> 
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/nodes/SoRotationXYZ.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoShuttle.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoTransformSeparator.h>
#include <Inventor/engines/SoElapsedTime.h>
#include <Inventor/engines/SoTimeCounter.h>
#include <Inventor/sensors/SoTimerSensor.h>
#include <Inventor/actions/SoSearchAction.h> 
#include <Inventor/nodes/SoFont.h> 
#include <Inventor/nodes/SoText2.h> 
#include <Inventor/nodes/SoTexture2.h> 
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoMaterialBinding.h> 
#include <Inventor/nodes/SoLightModel.h> 

#include <Inventor/manips/SoTrackballManip.h>


void error(const char * message)
{
	MessageBox(0,message,"Error",MB_OK);
}


SoSeparator * readFile(const char *filename)
{
	// Open the input file
	SoInput mySceneInput;
	if (!mySceneInput.openFile(filename)) 
	{
		error("Cannot open .iv file!");//%s\n", filename);
		return NULL;
	}
	// Read the whole file into the database
	SoSeparator *myGraph = SoDB::readAll(&mySceneInput);
	if (myGraph != NULL) 
/*	{
		fprintf(stderr, "Problem reading file\n");
		return NULL;
	}*/
		mySceneInput.closeFile();

	return myGraph;
}


void loadSceneGraph(const char * filename, SoSeparator * root, const SbViewportRegion &vpRegion )
{
    SoOrthographicCamera *myCamera = new SoOrthographicCamera;
	//SoPerspectiveCamera *myCamera = new SoPerspectiveCamera;
//	myCamera->nearDistance = -25.1;
//	myCamera->farDistance = 100;
	//myCamera->position.setValue(0,5,0);
	myCamera->orientation.setValue(SbVec3f(1,0,0),-1.57);
	root->addChild(myCamera);

/*
	SoDrawStyle *ds = new SoDrawStyle();
	ds->style = SoDrawStyle::LINES;
	ds->lineWidth = 3;
	clientVolume->addChild(ds); 
*/
	SoLightModel *lm = new SoLightModel();
	lm->model = SoLightModel::BASE_COLOR;
	root->addChild(lm); 

/*	
	SoDirectionalLight *myDirLight = new SoDirectionalLight;
	root->addChild(myDirLight);

*/
	SoSeparator* geometry = readFile(filename);

	if( geometry == NULL )
	{
		//MessageBox(0,filename, "Error",MB_OK);
        error("Error loading the converted *.iv file !!\n");
		exit (1);
	}


	root->addChild( geometry );
/*

	SoMaterial *matRed;
	matRed = new SoMaterial;
	matRed->diffuseColor.setValue(1.0, 0.0, 0.0);
	root->addChild(matRed);

	/*
	SoCube *cube = new SoCube;
	root->addChild(cube);
*/
	// Material Binding
/*	SoMaterialBinding *myMaterialBinding = new SoMaterialBinding;
	myMaterialBinding->value = SoMaterialBinding::OVERALL;
	root->addChild(myMaterialBinding);

	SoCoordinate3 *myCoords = new SoCoordinate3;

		myCoords->point.set1Value(0, 0,0,0);
		myCoords->point.set1Value(1, 1,1,1);
	
		myCoords->point.set1Value(2, 3,2,1);
		myCoords->point.set1Value(3, 3,2,3);

		root->addChild(myCoords);
*/

	myCamera->viewAll(root, vpRegion );
}


/*
void testSceneGraph( SoSeparator * root, const SbViewportRegion &vpRegion )
{
    SoPerspectiveCamera *myCamera = new SoPerspectiveCamera;
//	myCamera->nearDistance = -25.1;
//	myCamera->farDistance = 100;
	root->addChild(myCamera);

	SoDirectionalLight *myDirLight = new SoDirectionalLight;
	root->addChild(myDirLight);

	SoMaterial *matRed;
	matRed = new SoMaterial;
	matRed->diffuseColor.setValue(1.0, 0.0, 0.0);

	SoCube *cube = new SoCube;
	
	root->addChild(matRed);
	root->addChild(cube);


	myCamera->viewAll(root, vpRegion );
}
*/