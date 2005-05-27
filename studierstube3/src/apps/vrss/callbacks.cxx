/* =========================================================================
 *  (c) 2000,2001 VRVis GmbH, Vienna, Austria
 *  (c) 1999,2000 Vienna University of Technology
 * =========================================================================
 *  PROJECT: Studierstube VR Slide Show system
 * ========================================================================= */
/** 
 * Callbacks used in VRSS.
 * The callbacks provided by this module are responsible mainly for moving 
 * between slides in the presentation. Some additional call backs responsible
 * for scene graph saving and proper transparent texturing are part of this
 * module.
 *
 * @author Jan Prikryl
 * 
 * $Id: callbacks.cxx 3426 2004-07-14 12:11:27Z tamer $
 * @ingroup VRSS
 * @file
 */
/* ========================================================================= */
/*@{*/


// #include <sys/types.h>
// #include <assert.h>
// #include <stdlib.h>
// #include <time.h>
// 
// #ifdef WIN32
// #include <windows.h>
// #define strdup _strdup
// #endif
// 
// #include <Inventor/Xt/SoXt.h>
// #include <Inventor/Xt/viewers/SoXtExaminerViewer.h>
#include <Inventor/actions/SoSearchAction.h>
// #include <Inventor/actions/SoWriteAction.h>
#include <Inventor/events/SoKeyboardEvent.h>
// #include <Inventor/sensors/SoAlarmSensor.h>
// #include <Inventor/nodes/SoNode.h>
// #include <Inventor/nodes/SoSeparator.h>
// #include <Inventor/nodes/SoCallback.h>
// #include <Inventor/nodes/SoEventCallback.h>
// #include <Inventor/nodes/SoFile.h>
// #include <Inventor/nodes/SoWWWInline.h>
// 
#ifdef USE_TGS
// #include <Inventor/VRMLnodes/SoVRMLSwitch.h>
// #include <Inventor/VRMLnodes/SoVRMLGroup.h>
#include <Inventor/VRMLnodes/SoVRMLColorInterpolator.h>
#include <Inventor/VRMLnodes/SoVRMLCoordinateInterpolator.h>
#include <Inventor/VRMLnodes/SoVRMLNormalInterpolator.h>
#include <Inventor/VRMLnodes/SoVRMLOrientationInterpolator.h>
#include <Inventor/VRMLnodes/SoVRMLPositionInterpolator.h>
#include <Inventor/VRMLnodes/SoVRMLScalarInterpolator.h>
#include <Inventor/VRMLnodes/SoVRMLTimeSensor.h>
// #include <Inventor/VRMLnodes/SoVRMLPROTODef.h>
#endif
// 
// #include <stbapi/tracker/TrackerBuilder.h>
// #include <stbapi/tracker/SoTrakEngine.h>
// #include <stbapi/event3d/Stb3DEventGenerator.h>
// #include <stbapi/misc/SoAppNode.h>
// #include <stbapi/util/option.h>
// #include <stbapi/util/ivutil.h>
// #include <stbapi/util/ivio.h>
// #include <stbapi/util/timerutility.h>
// #include <stbapi/widgets/So3DButton.h>
// #include <stbapi/resource/SoUserKit.h>
// #include <stbapi/resource/SoPipSheetKit.h>
// #include <stbapi/workspace/StbCommunicator.h>
#include <stbapi/workspace/StbWorkspace.h>

#include "vrss.h"
#include "tools.h"
#include "callbacks.h"

#ifdef USE_TGS

/**
 * Switches to the next slide.
 * After ensuring that there is another slide to switch to, this
 * function triggers the proper time sensor that in turn starts the
 * animated transition between the current and the next slide (or just
 * switches to the next slide if there is no animation). No further
 * transition is allowed until the animation ends.
 * @param slideSwitch The base VRML switch that holds the particular
 * pairs of slides. Used to get the information about the current
 * slide and to prevent switching beyond the last slide.
 */
void
nextSlide(SoVRMLSwitch *slideSwitch)
{
    DPRINTF(("At nextSlide\n"));

    SoSFInt32 * whichChoiceField = & slideSwitch->whichChoice;
    int whichChoice = whichChoiceField->getValue();

    // Switch to the next slide only if transitions are allowed (that
    // is, if there is no slide transition in progress) and if we are
    // not at the last slide.

    if (whichChoice < slideSwitch->getNumChoices()-1 && _noTransition)
    {
	printf("       Switching %02d->%02d out of %d slides\n",
	       whichChoice, whichChoice+1, slideSwitch->getNumChoices());
		
	// Launch the transition timer if there is one
	int whichTimer = whichChoice-1;
	SoVRMLTimeSensor * ts  = TimeSensors[whichTimer];
	if (ts)
	{
	    SoSFTime         * now = & (ts->startTime);
	    now->setValue(SbTime::getTimeOfDay());
		
	    // Call us back when timer ends
	    now = & TimeSensors[whichTimer]->cycleInterval;
	    EndTransitionSensor->setTimeFromNow(now->getValue());
	}
	else
	{
	    // No timer = no animation.
	    EndTransitionSensor->setTimeFromNow(SbTime(1.0));
	}

	// Schedule the sensor. I am no very sure about the necessity
	// of the `if' part - it may be left out probably.
	if (EndTransitionSensor->isScheduled())
	{
	    EndTransitionSensor->unschedule();
	}
	EndTransitionSensor->schedule();

	// No further transition is allowed until the animation ends.  
	_noTransition = FALSE;
    }
}

/**
 * Switch to the previous slide.
 * Now this is a bit complicated. After ensuring that there realy is a
 * slide before the current one, we just decrease the slide
 * pointer. This brings us to the previous slide right after the next
 * SoRenderAction. We also have to take reset the transition timer -
 * otherwise the will be no animated transition to the next
 * slide. However, only resetting the time does not help very much
 * without resetting all the possible interpolators that are used
 * during the transition process. We will therefore reset them as
 * well, supposing that their default state was the initial one.
 * @param slideSwitch The base VRML switch that holds the particular
 * pairs of slides. Used to get the information about the current
 * slide and to prevent switching before the first slide.
 */

void
prevSlide(SoVRMLSwitch *slideSwitch)
{
    DPRINTF(("At prevSlide\n"));

    SoSFInt32 * whichChoiceField = & slideSwitch->whichChoice;
    int whichChoice = whichChoiceField->getValue();

    // Is there a previous slide?
    if (whichChoice > 1)
    {
	printf("       Switching %02d->%02d out of %d slides\n",
	       whichChoice, whichChoice-1, slideSwitch->getNumChoices());

	// Decrease the slide pointer
	whichChoice--;
	whichChoiceField->setValue(whichChoice);
			
	/**
	 * Reset the transition timer. This won't help much however,
	 * since this will not reset the interpolators to their
	 * initial values. Therefore we have to manually reset all the
	 * possible interpolators in the slide: we will look for
	 * everything that is a ancestor of SoVRMLInterpolator and
	 * reset it to its initial state. Let's assume the initial
	 * state corresponds to the default state.
	 */

	SoSearchAction sa;
	sa.setType(SoVRMLInterpolator::getClassTypeId());
	sa.setInterest(SoSearchAction::ALL);
	sa.setSearchingAll(TRUE);
	sa.apply(slideSwitch->getChild(whichChoice));

	SoPathList & p = sa.getPaths();
	if (p == NULL)
	{
	    fprintf(stderr,"ERROR: No SoVRMLInterpolator found!\n");
	    exit(1);
	}

	for (int i=0; i< p.getLength(); i++)
	{
	    SoNode * n = p[i]->getTail();
	    if      (n->isOfType(SoVRMLColorInterpolator::getClassTypeId()))
	    {
		DPRINTF(("color\n"));
		SoVRMLColorInterpolator * c = (SoVRMLColorInterpolator *) n;
		SoSFFloat * fraction = & c->set_fraction;
		fraction->setValue(0.0);
	    }
	    else if (n->isOfType(SoVRMLCoordinateInterpolator::getClassTypeId()))
	    {
		DPRINTF(("coordinate\n"));
		SoVRMLCoordinateInterpolator * c = (SoVRMLCoordinateInterpolator *) n;
		SoSFFloat * fraction = & c->set_fraction;
		fraction->setValue(0.0);
	    }
	    else if (n->isOfType(SoVRMLNormalInterpolator::getClassTypeId()))
	    {
		DPRINTF(("normal\n"));
		SoVRMLNormalInterpolator * c = (SoVRMLNormalInterpolator *) n;
		SoSFFloat * fraction = & c->set_fraction;
		fraction->setValue(0.0);
	    }
	    else if (n->isOfType(SoVRMLOrientationInterpolator::getClassTypeId()))
	    {
		DPRINTF(("orientation\n"));
		SoVRMLOrientationInterpolator * c = (SoVRMLOrientationInterpolator *) n;
		SoSFFloat * fraction = & c->set_fraction;
		fraction->setValue(0.0);
	    }
	    else if (n->isOfType(SoVRMLPositionInterpolator::getClassTypeId()))
	    {
		DPRINTF(("position\n"));
		SoVRMLPositionInterpolator * c = (SoVRMLPositionInterpolator *) n;
		SoSFFloat * fraction = & c->set_fraction;
		fraction->setValue(0.0);
	    }
	    else if (n->isOfType(SoVRMLScalarInterpolator::getClassTypeId()))
	    {
		DPRINTF(("scalar\n"));
		SoVRMLScalarInterpolator * c = (SoVRMLScalarInterpolator *) n;
		SoSFFloat * fraction = & c->set_fraction;
		fraction->setValue(0.0);
	    }
	    else if (n->isOfType(SoVRMLTimeSensor::getClassTypeId()))
	    {
		DPRINTF(("time\n"));
		SoVRMLTimeSensor * c = (SoVRMLTimeSensor *) n;
		SoSFTime * stop = & c->stopTime;
		stop->setValue(SbTime::zero());
	    }
	    else
	    {
		fprintf (stderr,
			 "WARNING: "
			 "Trying to reset unknown SoVRMLInterpolator!\n");
	    }
	}
    }
}

/**
 * Switch to the first slide in the slide show.
 * Calls prevSlide() until it reaches the beginning of the slide
 * show. Every switch to the previous slide decreases the value of
 * whichChoiceField->getValue().
 * @param slideSwitch The base VRML switch that holds the particular
 * pairs of slides. The whichChoice field of the switch is used to get
 * the information about the current slide and to prevent switching
 * before the first slide.
 */

void
firstSlide(SoVRMLSwitch *slideSwitch)
{
    SoSFInt32 * whichChoiceField = & slideSwitch->whichChoice;        
    while (whichChoiceField->getValue() > 1)
    {
	prevSlide(slideSwitch);
    }
}

/**
 * Switch to the next slide using a PIP button.
 * @param userData Contains the pointer to the base VRML switch that holds
 * the pairs of slides. The switch is a required parameter to nextSlide().
 * @param button Button data, ignored.
 */

void
buttonNextCB(void *userData, So3DButton *button)
{
    DPRINTF(("buttonNext pressed\n"));
    nextSlide((SoVRMLSwitch *)userData);
}

/**
 * Switch to the previous slide using a PIP button.
 * @param userData Contains the pointer to the base VRML switch that holds
 * the pairs of slides. The switch is a required parameter to prevSlide().
 * @param button Button data, ignored.
 */

void
buttonPrevCB(void *userData, So3DButton *button)
{
    DPRINTF(("buttonPrev pressed\n"));
    prevSlide((SoVRMLSwitch *)userData);
}

/**
 * Switch to the firs presentation slide using a PIP button.
 * @param userData Contains the pointer to the base VRML switch that holds
 * the pairs of slides. The switch is a required parameter to firstSlide().
 * @param button Button data, ignored.
 */

void
buttonFirstCB(void *userData, So3DButton *button)
{
    DPRINTF(("buttonFirst pressed\n"));
    firstSlide((SoVRMLSwitch *)userData);
}

/**
 * Called at the end of the slide transtion.
 * This callback is called after the animated slide transition has
 * ended. It completes the slide transition by switching the slideshow
 * switch to the next pair of slides.
 * @param slideSwitch The base VRML switch that holds the particular
 * pairs of slides. The whichChoice field of the switch is used to get
 * the information about the current slide and to prevent switching
 * before the first slide.
 * @param sensor Sensor data, ignored.
 */

void
myEndTransitionCB(void *userData, SoSensor *sensor)
{
    DPRINTF(("At myEndTransitionCB\n"));

    SoVRMLSwitch * slideSwitch = (SoVRMLSwitch *) userData;
    SoSFInt32 * whichChoiceField = & slideSwitch->whichChoice;
    int whichChoice = whichChoiceField->getValue();

    DPRINTF(("sensor triggered\n"));

    if (whichChoice < slideSwitch->getNumChoices()-1)
    {
	whichChoiceField->setValue(whichChoice+1);
    }
    _noTransition = TRUE;
}

/**
 * Process keyboard input.
 */

void
myKeyPressCB(void *userData, SoEventCallback *eventCB)
{
    DPRINTF(("DEBUG: Entering myKeyPressCB()\n"));

    SoVRMLSwitch  * slideSwitch  = (SoVRMLSwitch *) userData;
    const SoEvent *event = eventCB->getEvent();

    // check for spacebar and backspace being pressed
    // whichChoice == 0 is the Transform node => skip
	
    DPRINTF(("keypress callback\n"));
	
    //SO_KEY_PRESS_EVENT(event, SPACE) ||
    if ( SO_KEY_PRESS_EVENT(event, N))
    {
        DPRINTF(("space key pressed\n"));
        nextSlide(slideSwitch);
        eventCB->setHandled();
    } 
    else if	 (SO_KEY_PRESS_EVENT(event, BACKSPACE))
    {
        DPRINTF(("backspace key pressed\n"));
        prevSlide(slideSwitch);
        eventCB->setHandled();
    }
    else if	 (SO_KEY_PRESS_EVENT(event, NUMBER_1))
    {
        DPRINTF(("setting to the first slide\n"));
	firstSlide(slideSwitch);
        eventCB->setHandled();
    }

    DPRINTF(("DEBUG: Leaving myKeyPressCB()\n"));
}
#endif

/**
 * Save the current scene to a file after used hit the `S' key.
 */

void
saveSceneCB(void *userRoot, SoEventCallback *eventCB)
{
    DPRINTF(("DEBUG: saveSceneCB()\n"));

    const SoEvent * event = eventCB->getEvent();

    // check for spacebar and backspace being pressed
    // whichChoice == 0 is the Transform node => skip
	
    if (SO_KEY_PRESS_EVENT(event, S))
    {
	
        printf("INFO:  Saving scene graph in 'scenesnapshot.iv'\n");
        writeScene(StbWorkspace::getInstance()->auxRoot, "scenesnapshot.iv");
        eventCB->setHandled();
    }
}

/**
 * SoWWWInline read callback.
 * This callback has been used in an atempt to first intialize the
 * caching withing the OIV graph. Without cachech being properly built
 * before we start displaying our slides it may happen that sometimes
 * - mainly at slides that contain complex goemetry or a lot of
 * textures - the OIV spends too much time initializing the node
 * caches and some animations stop working for a short time. 
 */

void
myReadCB(const SbString &url, void *userData, SoWWWInline *node)
{
    DPRINTF(("At myReadCBCB\n"));
    const char * filename = url.getString();
    DPRINTF(("myReadCB: reading '%s'\n", filename));

    SoInput * in = new SoInput;
    in->openFile(filename);
    SoSeparator * data = SoDB::readAll(in);
    if (data)
    {
        data->ref();
        node->setChildData((SoNode *)data);
    }
    else
    {
        node->setChildData((SoNode *)new SoSeparator);
        DPRINTF(("WARNING(myReadCB): data==NULL\n"));
    }
    delete in;
}

/**
 * Prepare for transparent texture rendering.
 * This callback sends several GL commands to the graphics subsystem that
 * switch on the proper texturing mode for rendering transparent textures.
 */

void
transparentTexturingCB(void *d, SoAction *action)
{
    if (action->isOfType(SoGLRenderAction::getClassTypeId()))
    {
	/** Filtering off (we need sharp contours and anyway, sorting
	    wouldn't work). */
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	/** DECAL for correct opacity. */
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	
	/** Mask the pixels with alpha < 127 */
	
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.5);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	
	/** With Inventor 2.1, it's necessary to reset SoGLLazyElement
	    after making calls (such as glColor3f()) that affect
	    material state.  In this case, the diffuse color and
	    light model are being modified, so the logical-or of
	    DIFFUSE_MASK and LIGHT_MODEL_MASK is passed to
	    SoGLLazyElement::reset().

	    Additional information can be found in the publication
	    "Open Inventor 2.1 Porting and Performance Tips". */

#ifdef SOGLLAZYELEMENT_RESET
	SoState *state = action->getState();
	SoGLLazyElement* lazyElt =
	    (SoGLLazyElement*)SoLazyElement::getInstance(state);
	lazyElt->reset(state,
		       (SoLazyElement::DIFFUSE_MASK) | 
		       (SoLazyElement::LIGHT_MODEL_MASK)
		       );
#endif
    }
}

/*@}*/
/* ===========================================================================
   End of callbacks.cxx
   ===========================================================================
   Automatic Emacs configuration follows.
   Local Variables:
   mode:c++
   c-basic-offset: 4
   eval: (c-set-offset 'substatement-open 0)
   eval: (c-set-offset 'case-label '+)
   eval: (c-set-offset 'statement 'c-lineup-runin-statements)
   End:
=========================================================================== */
