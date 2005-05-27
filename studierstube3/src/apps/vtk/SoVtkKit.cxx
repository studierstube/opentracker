 /* ======================================================================= */
/** source file for SoSampleKit Node.
  *
  * @author Stefan Bruckner 
  *			Rudolf Seemann
  *
  * $Id: SoVtkKit.cxx 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <stbapi/util/ivio.h>
#include <stbapi/event3d/Stb3DEventGenerator.h>
#include <stbapi/resource/SoPipSheetKit.h>
#include <stbapi/resource/SoTrackedArtifactKit.h>
#include <stbapi/widgets/So3DButton.h>
#include <stbapi/workspace/SoApplicationKit.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <Inventor/SbLinear.h>
#include <Inventor/nodes/SoCallback.h>
#include <Inventor/engines/SoElapsedTime.h>
#include <Inventor/engines/SoCalculator.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoLightModel.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoRotationXYZ.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/elements/SoGLLazyElement.h>
#include <Inventor/nodes/SoText3.h>
#include SOGUI_H
#include SOGUI_EXAMINERVIEWER_H

#ifdef USE_SOQT
#include <qwidget.h>
#elif USE_SOXT
#include <X11/IntrinsicP.h>
#endif

#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkCamera.h>
#include <vtkActor.h>
#include <vtkVolume.h>
#include <vtkVolumeProperty.h>
#include <vtkVolumeRayCastMapper.h>
#include "vtkVolumeShearWarpMapper.h"
#include <vtkStructuredPointsReader.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkVolumeRayCastCompositeFunction.h>
#include <vtkPerspectiveTransform.h>

#include "SoVTKProxyProp3D.h"
#include "SoVTKProxyCamera.h"
#include "SoVTKOpenGLRenderWindow.h"
#include "SoVTKOpenGLRenderer.h"

#include "SoVtkKit.h"

//--initialization of kit structure in Inventor
SO_KIT_SOURCE(SoVtkKit);

//----------------------------------------------------------------------------
// static class initialization
//----------------------------------------------------------------------------

void SoVtkKit::initClass(void)
{
    SO_KIT_INIT_CLASS(SoVtkKit, SoContextKit, "SoContextKit");
	SoVTKProxyProp3D::initClass();
	SoVTKProxyCamera::initClass();
}

//----------------------------------------------------------------------------
// constructor
//----------------------------------------------------------------------------

SoVtkKit::SoVtkKit()
{
    SO_KIT_CONSTRUCTOR(SoVtkKit);
    SO_KIT_INIT_INSTANCE();
}

//----------------------------------------------------------------------------
// destructor
//----------------------------------------------------------------------------
SoVtkKit::~SoVtkKit()
{
}

//----------------------------------------------------------------------------
// object factory
//----------------------------------------------------------------------------
SoContextKit* SoVtkKit::factory()
{
  return new SoVtkKit;
}


//----------------------------------------------------------------------------
 /* ----------------------------------------------------------------------- */
/** create window and attach scene graph to it.                             */
 /* ----------------------------------------------------------------------- */
void forceVTKRender(void *renWin0, SoAction *action)
{
#ifdef WIN32
	vtkWin32OpenGLRenderWindow* renWin = ((vtkWin32OpenGLRenderWindow*)renWin0);
#else
	vtkXOpenGLRenderWindow* renWin = ((vtkXOpenGLRenderWindow*)renWin0);
#endif
	renWin->OpenGLInit();
	renWin->Render();
}

//----------------------------------------------------------------------------
/* ------------------------------------------------------------------------ */
/* create window and attach scene graph to it.                              */
/* ------------------------------------------------------------------------ */
SbBool SoVtkKit::checkWindowGeometry()
{
	// get the root node of the scenegraph
	SoSeparator *clientVolume = this->getWindow(0)->getClientVolume();
	this->getWindow(0)->style.setValue( SoWindowKit::CLIENTVOLUMEVISIBLE | SoWindowKit::VISIBLE);

	// create VTK objects
	// this prop is a volume
	vtkStructuredPointsReader *reader = vtkStructuredPointsReader::New();
	reader->SetFileName("../apps/vtk/feet.128.vtk");
	// Create transfer mapping scalar value to opacity
	vtkPiecewiseFunction *opacityTransferFunction = vtkPiecewiseFunction::New();
    opacityTransferFunction->AddPoint(0, 0.0);
	opacityTransferFunction->AddPoint(50, 0.0);
    opacityTransferFunction->AddPoint(255, 1.0);
	// Create transfer mapping scalar value to color
	vtkColorTransferFunction *colorTransferFunction = vtkColorTransferFunction::New();
    colorTransferFunction->AddRGBPoint(0.0, 0.2, 0.0, 0.0);
    colorTransferFunction->AddRGBPoint(64.0, 0.2, 0.0, 0.0);
    colorTransferFunction->AddRGBPoint(128.0, 0.6, 0.6, 0.6);
    colorTransferFunction->AddRGBPoint(192.0, 0.5, 0.5, 0.0);
    colorTransferFunction->AddRGBPoint(255.0, 1.0, 0.2, 1.0);

	vtkPiecewiseFunction *clippingOpacityTransferFunction = vtkPiecewiseFunction::New();
    clippingOpacityTransferFunction->AddPoint(0.0, 0.1);
    clippingOpacityTransferFunction->AddPoint(255.0, 0.6);
	// Create transfer mapping scalar value to color
	vtkColorTransferFunction *clippingColorTransferFunction = vtkColorTransferFunction::New();
    clippingColorTransferFunction->AddRGBPoint(0.0, 0.2, 0.0, 0.0);
    clippingColorTransferFunction->AddRGBPoint(64.0, 0.2, 0.0, 0.0);
    clippingColorTransferFunction->AddRGBPoint(128.0, 0.6, 0.6, 0.6);
    clippingColorTransferFunction->AddRGBPoint(192.0, 0.5, 0.5, 0.0);
    clippingColorTransferFunction->AddRGBPoint(255.0, 1.0, 0.2, 1.0);

	// The property describes how the data will look
	vtkVolumeProperty *volumeProperty = vtkVolumeProperty::New();
    volumeProperty->SetColor(colorTransferFunction);
    volumeProperty->SetScalarOpacity(opacityTransferFunction);
	volumeProperty->SetInterpolationTypeToLinear();

	// The mapper knows how to render the data
	/*
	vtkVolumeRayCastMapper *volumeMapper = vtkVolumeRayCastMapper::New();
	volumeMapper->SetVolumeRayCastFunction(vtkVolumeRayCastCompositeFunction::New());
	volumeMapper->SetInput((vtkImageData*)reader->GetOutput());
	*/
	vtkVolumeShearWarpMapper *volumeMapper = vtkVolumeShearWarpMapper::New();
	volumeMapper->SetInput((vtkImageData*)reader->GetOutput());
	volumeMapper->SetFunctionTypeToComposite();
	volumeMapper->SetClippingModeToTransparent();
	volumeMapper->RunlengthEncodingOn();
	volumeMapper->IntermixIntersectingGeometryOn();
	volumeMapper->SetClippingRenderer(NULL);
	volumeMapper->GetClippingProperty()->SetColor(clippingColorTransferFunction);
	volumeMapper->GetClippingProperty()->SetScalarOpacity(clippingOpacityTransferFunction);
	
	SoVTKOpenGLRenderWindow *renWin = new SoVTKOpenGLRenderWindow();	
	renWin->SetWindowId(SoGui::getTopLevelWidget()->winId());
	renWin->Initialize();
	renWin->EraseOff();

	SoVTKOpenGLRenderer *ren1= new SoVTKOpenGLRenderer();
	renWin->AddRenderer( ren1 );
	renWin->SetSize(300,300);
	
	SoVTKProxyCamera *proxycamera = new SoVTKProxyCamera();
	//	proxycamera->ParallelProjectionOff();
	ren1->SetActiveCamera(proxycamera);
	//	ren1->ResetCamera();

	vtkVolume *volume = vtkVolume::New();
    volume->SetMapper(volumeMapper);
    volume->SetProperty(volumeProperty);
	volume->SetPosition(0.0,0.0,-100);
 	volume->SetScale(0.1f);
	//volume->SetPosition(-volume->GetCenter()[0],-volume->GetCenter()[1],volume->GetCenter()[2]);
	//volume->SetOrigin(volume->GetCenter());
	ren1->AddVolume(volume);
	ren1->ResetCamera();
	
	SoVTKProxyProp3D *prop = new SoVTKProxyProp3D();
	prop->SetVTKProp3D(volume);
	//prop->SetVTKProp3D(sphereActor);
	proxycamera->AddProxyProp(prop);

	clientVolume->addChild(proxycamera);
	
	clientVolume->addChild(prop);
	//clientVolume->addChild(new SoDirectionalLight);
	SoTrackedArtifactKit *tracker2 = new SoTrackedArtifactKit;
	tracker2->station.setValue(66);
	//tracker2->setGeometry(prop);

	//clientVolume->addChild(tracker2); // a VTK volume
	// hang a VTK prop into the Open Inventor scenegraph
	// a callback node at the end of the scenegraph
	// it forces vtk to render its pipeline
	SoCallback *myCallback = new SoCallback;
	myCallback->setCallback(forceVTKRender, renWin);
	clientVolume->addChild(myCallback); 
    return SoContextKit::checkWindowGeometry();
}

SbBool SoVtkKit::checkPipGeometry()
{
    // implement this method, if you want to do something with your template
    // pip sheet stored in templatePipSheet. It will be called once after
    // loading the application.
    return SoContextKit::checkPipGeometry();
}

void SoVtkKit::checkPipConnections( int uid, SoNode * pipSheetContents )
{
    // this method is called for each user that is registered in the system 
    // and whenever a user is added to the system. It allows you to setup
    // connections, sensors, callbacks and such on the user's pip sheet.
}

void SoVtkKit::checkPipMasterMode( SoNode * pipSheetContents, SbBool masterMode )
{
    // this method is called for each pip sheet, if the master mode for
    // that sheet changes. This is only necessary for distributed applications
}

SbBool SoVtkKit::windowEventCB( void* data, int messageId, SoWindowKit* win, int uid,
                     So3DEvent* event, SoPath* path )
{
    // this callback is executed for all 3D events passed to any child window
    // of your application. Implement it to react to window and 3D events.
    return SoContextKit::windowEventCB( data, messageId, win, uid, event, path );
}

void SoVtkKit::focusChange( int uid, SbBool focus  )
{
    // this method is called, if your application gets or looses the focus
    // of a certain user. Override it to add your own implementation.
}
