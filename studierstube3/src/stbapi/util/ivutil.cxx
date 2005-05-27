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
/** Inventor tools implementation
  *
  * @author Gerd Hesina, Dieter Schmalstieg, Jan Prikryl
  *
  * $Id: ivutil.cxx 4138 2005-03-22 09:57:40Z kaufmann $
  * @file                                                                   */
 /* ======================================================================= */

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#include <Inventor/nodes/SoTransformSeparator.h>
#include <Inventor/nodes/SoCallback.h>
#include <Inventor/nodes/SoTransform.h> 
#include <Inventor/nodes/SoRotationXYZ.h>
#include <Inventor/nodes/SoDirectionalLight.h> 
#include <Inventor/nodes/SoPointLight.h> 
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/nodes/SoEnvironment.h>
#include <Inventor/SoInput.h>
#include <Inventor/actions/SoWriteAction.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

#include <stbapi/event3d/SoHandle3DEventAction.h>
#include <stbapi/widgets/So3DButton.h>
#include <stbapi/widgets/So3DTouchButton.h>
#include <stbapi/widgets/So3DRadioButton.h>
#include <stbapi/widgets/So3DTouchRadioButton.h>
#include <stbapi/widgets/So3DCheckBox.h>
#include <stbapi/widgets/So3DTouchCheckBox.h>
#include <stbapi/widgets/So3DSlider.h>
#include <stbapi/widgets/So3DTouchSlider.h>
#include <stbapi/widgets/So3DIntSlider.h>
#include <stbapi/widgets/So3DLogSlider.h>
#include <stbapi/widgets/So3DExpSlider.h>
#include <stbapi/widgets/So3DDial.h>
#include <stbapi/widgets/So3DIntDial.h>
#include <stbapi/widgets/SoAlphabetKit.h>
#include <stbapi/widgets/SoSlider.h>
#include <stbapi/widgets/SoSpeechSlider.h>
#include <stbapi/widgets/SoSimpleSlider.h>
#include <stbapi/widgets/SoIncrementalSlider.h>

#include <stbapi/misc/SoAbortGroup.h>
#include <stbapi/seams/SoSeamKit.h>
#include <stbapi/seams/SoClipCube.h>
#include <stbapi/seams/SoCubeSeamKit.h>
#include <stbapi/context/SoContextElement.h>
#include <stbapi/context/SoContext.h>
#include <stbapi/context/SoContextSwitch.h>
#include <stbapi/context/SoContextMultiSwitch.h>

#include <stbapi/context/SoContextSeparator.h>
#include <stbapi/context/SoContextReport.h>
#include <stbapi/context/SoNodeContext.h>
#include <stbapi/context/SoNodeContextReport.h>

#include <stbapi/misc/SoStreamLine.h>
#include <stbapi/misc/SoConstraint.h>
#include <stbapi/misc/SoLineConstraint.h>
#include <stbapi/util/ivio.h>
#include <stbapi/util/ivutil.h>
#include <stbapi/event3d/Stb3DEventGenerator.h>
#include <stbapi/window3d/SoWindowKit.h>
#include <stbapi/interaction/SoDragSeparator.h>
#include <stbapi/interaction/SoDragKit.h>
#include <stbapi/interaction/SoDraggerKit.h>
#include <stbapi/interaction/So3DSeparator.h>
#include <stbapi/interaction/So3DSensor.h>
#include <stbapi/misc/SoMultMatrix.h>
#include <stbapi/misc/SoFanIn.h>
#include <stbapi/misc/SoFactorRotation.h>
#include <stbapi/misc/ConvertToSFPlane.h>

#include <stbapi/interaction/SoGesture.h>
#include <stbapi/interaction/SoSpeechGesture.h>
#include <stbapi/interaction/SoEnterGesture.h>
#include <stbapi/interaction/SoExitGesture.h>
#include <stbapi/interaction/SoButtonPressGesture.h>
#include <stbapi/interaction/SoButtonReleaseGesture.h>
#include <stbapi/interaction/SoMoveGesture.h>
#include <stbapi/interaction/SoLayoutKit.h>

#include <stbapi/misc/GLTweak.h>
#include <stbapi/interaction/SoTouchDragKit.h>
#include <stbapi/misc/SoAudio.h>
#include <stbapi/event3d/SoKeyToggle.h>
#include <stbapi/interaction/SoTouchButtonDragKit.h>
#include <stbapi/misc/SoMultiSwitch.h>
#include <stbapi/misc/SoGroupSwitch.h>
#include <stbapi/misc/SoSwitchSelect.h>

#include <stbapi/resource/viewer/SoOffAxisCamera.h>
#include <stbapi/resource/viewer/SoStereoCameraKit.h>
#include <stbapi/resource/viewer/SoCameraControlMode.h>
#include <stbapi/resource/viewer/SoDesktopControlMode.h>
#include <stbapi/resource/viewer/SoFieldSequentialDisplayMode.h>
#include <stbapi/resource/viewer/SoDisplayMode.h>
#include <stbapi/resource/viewer/SoLineSequentialDisplayMode.h>
#include <stbapi/resource/viewer/SoDualChannelDisplayMode.h>
#include <stbapi/resource/viewer/SoAnaglyphDisplayMode.h>
#include <stbapi/resource/viewer/SoMonoDisplayMode.h>
#include <stbapi/resource/viewer/SoTrackedDisplayControlMode.h>
#include <stbapi/resource/viewer/SoTrackedViewpointControlMode.h>
#include <stbapi/resource/viewer/SoTrackedViewpointMobileDisplayControlMode.h>
#include <stbapi/resource/viewer/SoDisplayKit.h>
#include <stbapi/resource/viewer/MultRotRot.h>
#include <stbapi/resource/viewer/SoRotateFromTo.h>

#include <stbapi/resource/viewer/video/SoVideoBackground.h>

#include <stbapi/resource/SoTrackedArtifactKit.h>
//---------------------------------------------------
#include <stbapi/vc/SoVCWidgetKit.h>
#include <stbapi/vc/SoVCDraggerKit.h>
#include <stbapi/vc/SoVCPushWidgetKit.h>
#include <stbapi/vc/SoVCCalibrationWidgetKit.h>
#include <stbapi/vc/SoVCToggleWidgetKit.h>
#include <stbapi/vc/SoVCDialWidgetKit.h>
#include <stbapi/vc/SoVCBarKit.h>
#include <stbapi/vc/SoVCKit.h>
#include <stbapi/vc/SoVCMainPanelKit.h>
#include <stbapi/vc/SoVCSubPanelKit.h>
#include <stbapi/vc/SoVCManager.h>
#include <stbapi/vc/SoVCStatusBarKit.h>
//----------------------------------------------------
#include <stbapi/resource/SoPenKit.h>
#include <stbapi/resource/SoPipKit.h>
#include <stbapi/resource/SoPipSheetKit.h>
#include <stbapi/resource/SoTabKit.h>
#include <stbapi/resource/SoTabButtonKit.h>
#include <stbapi/resource/SoPullDownKit.h>
#include <stbapi/resource/SoUserKit.h>
#include <stbapi/resource/SoUserManagerKit.h>
#include <stbapi/workspace/SoContextManagerKit.h>
#include <stbapi/workspace/SoContextKit.h>
#include <stbapi/workspace/SoClassLoader.h>
#include <stbapi/workspace/SoApplicationKit.h>

#include <stbapi/tracker/SoStationKit.h>
#include <stbapi/tracker/SoPartitionKit.h>

#include <stbapi/interaction/SoBehaviorKit.h>
#include <stbapi/widgets/SoTextBox.h>
#include <stbapi/widgets/SoButtonKit.h>
#include <stbapi/widgets/SoPushButton.h>
#include <stbapi/widgets/SoRadioButtonGroup.h>
#include <stbapi/widgets/SoRadioButton.h>
#include <stbapi/widgets/SoToggleButton.h>
#include <stbapi/widgets/SoDial.h>
#include <stbapi/widgets/SoListBox.h>
#include <stbapi/widgets/SoLabelKit.h>
#include <stbapi/widgets/SoTooltipKit.h>
#include <stbapi/widgets/SoLabelListBox.h>
#include <stbapi/widgets/SoTextListBox.h>
#include <stbapi/widgets/SoStatusBox.h>
#include <stbapi/widgets/SoStatusBoxLayout.h>

#include <stbapi/misc/SoConstraint.h>
#include <stbapi/misc/SoBoxConstraint.h>
#include <stbapi/misc/SoLineConstraint.h>
#include <stbapi/misc/SoAxisRotateConstraint.h>
#include <stbapi/misc/SoRotateVector.h>
#include <stbapi/misc/SoBillboardKit.h>
#include <stbapi/misc/SoAlphaMaskKit.h>
#include <stbapi/misc/SoRenderSync.h>
#include <stbapi/misc/SoRenderSyncUDP.h>
#include <stbapi/misc/SoSocketModule.h>
#include <stbapi/misc/SoMD2NodeKit.h>
#include <stbapi/misc/SoRayPicker.h>
#include <stbapi/misc/SoRayPickerTracker.h>


#include <stbapi/misc/SoStroke.h>
#include <stbapi/misc/SoRouteDB.h>
#include <stbapi/misc/SoStringMap.h>
#include <stbapi/misc/SoScreenCapture.h>
#include <stbapi/misc/SoExtCubeKit.h>
#include <stbapi/misc/SoRefEngine.h>
#include <stbapi/misc/SoConditionalTrigger.h>
#include <stbapi/misc/SoEaseIn.h>
#include <stbapi/misc/SoSFFieldContainer.h>
#include <stbapi/misc/SoRoute.h>
#include <stbapi/misc/SoRouter.h>
#include <stbapi/misc/SoStringCalculator.h>
#include <stbapi/misc/SoVariable.h>
#include <stbapi/misc/SoFileSubgraph.h>
#include <stbapi/misc/SoGLFrameBufferElement.h>
#include <stbapi/misc/SoFrameBuffer.h>
#include <stbapi/misc/SoGLStencilBufferElement.h>
#include <stbapi/misc/SoStencilBuffer.h>
#include <stbapi/misc/SoNodeToName.h>
#include <stbapi/misc/SoEngineWrapper.h>
#include <stbapi/misc/SoUse.h>
#include <stbapi/workspace/SoLinkApp.h>

#include <stbapi/workspace/SoLocaleKit.h>
#include <stbapi/workspace/SoLocaleManagerKit.h>

#include <stbapi/interaction/SoFitInKit.h>
#include <stbapi/widgets/SoLabelObjectKit.h>

#include <stbapi/div/SoDivGroup.h>

#include "ivutil.h"

#if 0 // FIXME: required for TGS Inventor bug? 20030113 tamer.
//----------------------------------------------------------------------------
//forces a visual with stencil planes on NT
//withStereo: if TRUE: use a stereoMode visual
//            defaults to FALSE

void
fixInventorVisual(Widget theWindow, char withStereo, int forcedPixelFormat)
{
#ifndef USE_QT_LIB
#ifdef WIN32
    PIXELFORMATDESCRIPTOR pfd;
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    
    if(!withStereo)
        pfd.dwFlags = PFD_DRAW_TO_WINDOW
                    | PFD_SUPPORT_OPENGL
                    | PFD_DOUBLEBUFFER;
    else
        pfd.dwFlags = PFD_DRAW_TO_WINDOW
                    | PFD_SUPPORT_OPENGL
                    | PFD_DOUBLEBUFFER
                    | PFD_STEREO;

    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cRedBits = 0;
    pfd.cRedShift = 0;
    pfd.cGreenBits = 0;
    pfd.cGreenShift = 0;
    pfd.cBlueBits = 0;
    pfd.cBlueShift = 0;
    pfd.cAlphaBits = 0;
    pfd.cAlphaShift = 0;
    pfd.cAccumBits = 0;
    pfd.cAccumRedBits = 0;
    pfd.cAccumGreenBits = 0;
    pfd.cAccumBlueBits = 0;
    pfd.cAccumAlphaBits = 0;
    pfd.cDepthBits = 32;
    pfd.cStencilBits = 6;
    pfd.cAuxBuffers = 0;
    pfd.iLayerType = PFD_MAIN_PLANE;
    pfd.bReserved = 0;
    pfd.dwLayerMask = 0;
    pfd.dwVisibleMask = 0;
    pfd.dwDamageMask = 0;
    
    HDC hdc = GetDC(theWindow);
    int pixelFormat = ChoosePixelFormat(hdc, &pfd);
    if(pixelFormat<=0)
    {
        fprintf(stderr,"ALERT! Cannot set Pixelformat. See file ivutil.cxx for"
                " the current PIXELFORMATDESCRIPTOR.\n");
        fprintf(stderr,"Emergency exit taken!");
        exit(-1);
    }

    SbString envString="OIV_FORCE_PIXEL_FORMAT=";
    if(forcedPixelFormat>0)
        envString += forcedPixelFormat;
    else
        envString += pixelFormat;

    putenv(envString.getString());
    _unlink("pixel.txt");
    putenv("OIV_DEBUG_PIXEL_FORMAT=1");
#endif
#endif
}
#endif


//----------------------------------------------------------------------------
// init all Studierstube classes

void
initStbClasses()
{
    // init classes
    SoHandle3DEventAction::initClass();
    SoTrakEngine::initClass();
    SoWindowKit::initClass();
    SoBase3DButton::initClass();
    So3DButton::initClass();


    So3DTouchButton::initClass();
    So3DCheckBox::initClass();
    So3DTouchCheckBox::initClass();
    So3DRadioButton::initClass();
    So3DTouchRadioButton::initClass();
    So3DSlider::initClass();
    So3DTouchSlider::initClass();

    So3DIntSlider::initClass();
    So3DLogSlider::initClass();
    So3DExpSlider::initClass();
    So3DDial::initClass();
    So3DIntDial::initClass();
    SoAlphabetKit::initClass();
    SoAbortGroup::initClass();
    SoSeamKit::initClass();
    SoCubeSeamKit::initClass();
    SoClipCube::initClass();
    SoContextElement::initClass();
    SoContext::initClass();    
    SoContextSwitch::initClass();
    SoMultiSwitch::initClass();
    SoContextMultiSwitch::initClass();

    SoContextSeparator::initClass();
    SoContextReport::initClass();
    SoNodeContext::initClass();
    SoNodeContextReport::initClass();

    SoStreamLine::initClass();
    So3DSensor::initClass();
    So3DSeparator::initClass();
    SoDragSeparator::initClass();
    SoMultMatrix::initClass();
    SoDragKit::initClass();
    SoFanIn::initClass();
    SoFactorRotation::initClass();
    ConvertToSFPlane::initClass();

    SoConstraint::initClass();
    SoLineConstraint::initClass();
    SoBoxConstraint::initClass();
    SoAxisRotateConstraint::initClass();
    SoRotateVector::initClass();

    GLTweak::initClass();
    SoTouchDragKit::initClass();
    SoAudio::initClass();
    SoKeyToggle::initClass();
    SoTouchButtonDragKit::initClass();

    SoTrackedArtifactKit::initClass();
    SoPenKit::initClass();
    SoPipKit::initClass();
    SoPipSheetKit::initClass();
    SoTabKit::initClass();
    SoTabButtonKit::initClass();
    SoPullDownKit::initClass();
    SoUserKit::initClass();
    SoUserManagerKit::initClass();

    MultRotRot::initClass();
    SoRotateFromTo::initClass();
    SoOffAxisCamera::initClass();
    SoStereoCameraKit::initClass();
    SoCameraControlMode::initClass();
    SoDesktopControlMode::initClass();
    SoDisplayMode::initClass();
    SoFieldSequentialDisplayMode::initClass();
    SoLineSequentialDisplayMode::initClass();
    SoDualChannelDisplayMode::initClass();
    SoAnaglyphDisplayMode::initClass();
    SoMonoDisplayMode::initClass();
    SoTrackedDisplayControlMode::initClass();
    SoTrackedViewpointControlMode::initClass();
    SoTrackedViewpointMobileDisplayControlMode::initClass();
    SoDisplayKit::initClass();

    SoContextManagerKit::initClass();
    SoContextKit::initClass();
    SoClassLoader::initClass();
    SoApplicationKit::initClass();

    SoStationKit::initClass();
    SoPartitionKit::initClass();
    SoGroupSwitch::initClass();
    SoSwitchSelect::initClass();

    SoLabelKit::initClass();
	SoTooltipKit::initClass();
	SoLayoutKit::initClass();

	SoBehaviorKit::initClass();
	SoTextBox::initClass();
	SoButtonKit::initClass();
    SoPushButton::initClass();
    SoToggleButton::initClass();
    SoDraggerKit::initClass();
    SoSlider::initClass();
    SoSpeechSlider::initClass();
    SoSimpleSlider::initClass();
	SoRadioButtonGroup::initClass();
	SoRadioButton::initClass();
    SoDial::initClass();
    SoBillboardKit::initClass();
    SoListBox::initClass();
	SoStatusBox::initClass();
    SoStatusBoxLayout::initClass();

    //------------------------------------------
    SoVCWidgetKit::initClass();
    SoVCDraggerKit::initClass();
    SoVCPushWidgetKit::initClass();
    SoVCCalibrationWidgetKit::initClass();
    SoVCToggleWidgetKit::initClass();
    SoVCDialWidgetKit::initClass();
    SoVCBarKit::initClass();
    SoVCSubPanelKit::initClass();
    SoVCMainPanelKit::initClass();
    SoVCKit::initClass();
    SoVCStatusBarKit::initClass();
    SoVCManager::initClass();
    //------------------------------------------    
	
	SoIncrementalSlider::initClass();
	
	SoLabelListBox::initClass();
	SoTextListBox::initClass();

    SoLinkApp::initClass();

    SoLocaleKit::initClass();
    SoLocaleManagerKit::initClass();

    SoDivGroup::initClass();

    SoGesture::initClass();
    SoSpeechGesture::initClass();
    SoEnterGesture::initClass();
    SoExitGesture::initClass();
    SoButtonPressGesture::initClass();
    SoButtonReleaseGesture::initClass();
    SoMoveGesture::initClass();

    SoMD2NodeKit::initClass();
    SoRayPicker::initClass();
	
	SoRayPickerTracker::initClass();

    SoStroke::initClass();
    SoRouteDB::initClass();
    SoStringMap::initClass();
    SoScreenCapture::initClass();
    SoExtCubeKit::initClass();
    SoConditionalTrigger::initClass();
    SoEaseIn::initClass();
    SoSFFieldContainer::initClass();
    SoRoute::initClass();
    SoRouter::initClass();
    SoStringCalculator::initClass();
    SoVariable::initClass();
    SoFileSubgraph::initClass();
    SoGLFrameBufferElement::initClass();
    SoFrameBuffer::initClass();
    SoGLStencilBufferElement::initClass();
    SoStencilBuffer::initClass();
    SoRefEngine::initClass();
    SoNodeToName::initClass();
    SoEngineWrapper::initClass();
    SoUse::initClass();

    SoAlphaMaskKit::initClass();
    SoRenderSync::initClass();
    SoRenderSyncUDP::initClass();
    SoSocketModule::initClass();
	SoFitInKit::initClass();
	SoLabelObjectKit::initClass();

    SoVideoBackground::initClass();

    SoWriteAction::addMethod(SoTexture2::getClassTypeId(), writeTexture);
}

/* ===========================================================================
    End of ivutil.cxx
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
