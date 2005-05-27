/* =========================================================================
 *  (c) 2000,2001 VRVis GmbH, Vienna, Austria
 * =========================================================================
 *  PROJECT: Studierstube VR Slide Show system
 * ========================================================================= */
/** 
 * Definitions for WIN32 API.
 * 
 * @author Jan Prikryl
 * 
 * $Id: vrss.h 3426 2004-07-14 12:11:27Z tamer $
 * @ingroup VRSS
 * @file
 */
/* ========================================================================= */
/**
 * @defgroup VRSS Virtual reality slide show system
 * @ingroup apps
 */
/*@{*/

#ifndef _STBAPP_VRSS_H_
#define _STBAPP_VRSS_H_

#ifdef USE_TGS
#include <Inventor/VRMLnodes/SoVRMLTimeSensor.h>
#endif

/**
 * Set the proper WIN32 calling convetion.
 * WIN32 function declaration differs when compiling the DLL or when
 * linking the already compiled DLL. This takes care of this anomaly.
 */

#ifdef WIN32
#ifdef VRSS_EXPORTS
#define VRSS_API __declspec(dllexport)
#else
#define VRSS_API __declspec(dllimport)
#endif
#else
#define VRSS_API
#endif

/**
 * The presentation directory is fixed by this path.
 * This directory will be prepended to the OIV search path. It would
 * be much much better if we allowed for a named node or whatever that
 * would let the used to specify this directory without the need of
 * recompiling.
 * @bug This is an ugly hack. The directory shall be rather defined by a
 * named node within the presentation data.
 * @warning Don't forget to either escape backslashes or replace backslashes
 * with ordinary slashes if you use it on WIN32.
 */
#define VRSS_DATA_DIR "C:/CVS_studierstube/bin/apps/vrss/DEMO"
// #define VRSS_DATA_DIR "/raid3/users/vrvis/honza/DEMO"

/**
 * Debug printout.
 */
#undef DEBUG_PRINT
#undef DEBUG_SAVE

#ifndef DEBUG_PRINT
#define DPRINTF(_a) 
#else
#define DPRINTF(_a) { fflush(stderr) ; printf _a ; fflush(stdout); }
#endif

#ifdef USE_TGS
extern SoVRMLTimeSensor * TimeSensors[];
extern SoAlarmSensor    * EndTransitionSensor;
extern int                _noTransition;
#endif

/*! \page vrsspage Virtual reality slide show system

This page contains an overview of capabilites of VRSS and a short
overview of the structure of this application. 

\section intro Introduction

VRSS is a tool for presenting 3D slide shows on stereo displays. It is
based on Studierstube and allows for all video modi that Studierstube
supports. In its present form, the application relies on several VRML
features and requires use of TGS Open Inventor.  Every STB application can
be embedded into the presented slide show. The user may include into the
presentation whatever widget or object that is supported by Studierstube or
TGS Open Inventor.

The input of VRSS is a slide show in VRML format that has a defined
strucutre and contains several important named VRML nodes. As it is quite
tedious to set up a presentation directly in VRML, we provide a custom
macro processor and a corresponding set of macros that can be used for this
purpose.

\section desing Designing a slide show

The slideshow has to have the name "sequence.wrl" (or "sequence.pym" which
will be automatically converted to "sequence.wrl" by the macro
processor). The file has to reside in a directory that is defined by the
content of VRSS_DATA_DIR variable.

The slideshow consists of several slides that are places in a master VRML
switch node named SLIDE_SHOW. The first subnode of the VRML switch is
supposed to contain a series of named transform nodes that hold all slides
that will be presented. The transform nodes have to be named (we suggest
using names like SLIDE_00 to SLIDE_99). The contents of every slide is
supposed to be read from an external file.

The nodes follwing the first VRML switch node describe node transitions
bwtween two slides. Every transition is driven by a single VRML timer node
and VRSS supposes that all VRML timers originally present in "sequence.wrl"
are transition timers. It is however safe to put your own animation timers
into file nodes that are referenced from "sequence.wrl".

The following example shows a shortened version of our current
"sequence.wrl":

\include examples/vrml1.wrl

The presentation scene graph will have this structure:

\image html  examples/vrssnodes.png
\image latex examples/vrssnodes.eps "Scene graph of the VR slide show" width=12cm

\subsection pym Python Macro Processor (PYM)

The above example suggests that it is quite tedious to set up a
presentation directly in VRML format. That, and also some technical aspects
of how TGS Open Inventor handles prototype nodes, leasd us to development
of our own macro processor tool PYM. The macro processor is based on
Python language. The "sequence.pym" that expands to the above file has the
following structure:

\include examples/sequence.pym

The macro processor understands simple directives like \#include or \#begin
python and \#end python. It takes everything between @[ and ]@ as a call to
a defined Python function and tries to execute it. The return value of the
funcion then replaces the @[ ... ]@ block. Custom functions can be defined
between \#begin python and \#end python directives.

In the above example, slide transitions are programmed as PYM macros. The
corresponding PYM code looks as follows

\include examples/macro1.pym

The function NextSlide() is resposible for generating a timer-driven
transformation that transofmrs the current view from slide1 to slide2.

It is behind the scope of this document to describe the full functionality
of the PYM processor. Please consult the corresponding VRVis Technical
Report written by Robert F. Tobler that is available from
http://www.vrvis.at/TR/2001/TR_VRVis_2001_012_Full.pdf in PDF format.

\subsection buttons PIP buttons

When presenting your slide show, the pad of the personal interaction panel
(PIP) will use a "PIP-sheet" that will be searched in the current working
directory under the name "sheet.iv". This sheet may use three named
buttons that move to the next slide ("NEXT_BUTTON", texture in "next.png"),
to the previous slide ("PREV_BUTTON", texture in "prev.png"), and that
rewind the presentation again to the beginning ("FIRST_BUTTON", texture in
"first.png").

\section present Presenting a presentation

Presentation can be started by

\verbatim
cd $STB_ROOT/bin/workspace
./workspace -a ../apps/vrss/vrss_stb.dll [studierstube options]
\endverbatim

After VRSS has started, the application reacts on the following keys:
`N' switches to the next slide, `BACKSPACE' moves back one slide, `1'
rewinds the presentation to the first slide, and `S' saves the actual
scene graph in file \c scenesnapshot.iv.

Alternatively, the presentator may use PIP with the buttons mentioned in
the previous section to navigate forwards and backwards in the
presentation and to rewind the presentation back to the first slide.  

\section vrssapp Structure of the VRSS Application 

The VRSS application works as follows: first, the
PYM macro tool will be called to expand all .pym files to .wrl
format. The scene graph will be read from a file called \c sequence.wrl in
the current directory or in a directory specified by VRSS_DATA_DIR. The
scene graph will be searched for the slide transition timers (every
transition is supposed to be driven by a single transition timer) and
referenced to these timers will be stored for use by the slide transiton
callbacks. The main file then initializes all the necessary keyboard
callbacks, creates the PIP sheet, opens a maximized application window and
returns the control back to Studierstube.

Open Inventor builds so called render caches at some nodes to speed up the
rendering of the scene graph. The process of building the caches
significantly reduces the rendering performance of Open Inventor and
specially after switching to a rather complex slide we may observe a
drastic decrease of rendering speed. We address these problems by
physically eliminating all SoFile nodes in the slide show scene graph and
by initialising all relevant SoNode chaches before the actual presentation
begins. 

\subsection nuking Nuking SoFile Nodes

First step for eliminating animation problems after slide transitions is
to physically save all SoFile nodes in memory. This is accomplished by a
simple trick where every SoFile node is read into memory as a SoGroup
(or SoVRMLGroup) node and then the original reference to SoFile node is
replaced by a reference to the new group node. In order to save memory a
simple associative list is used that assures that for a given file will
be expanded only once. All other references to the same file will be
replaced by a reference to the already existing group node. For more
details about the implementation please consult comments in
nukeFileNodes() in \c vrss.cxx.

\subsection caching Initialising the SoNode caches

After expanding all the nodes we may finally force Inventor to build all
the necessary SoNode chackes. We know that the node cache is built
during a SoRenderAction - the default behaviour is that after completing
typically several passes over the scene graph OIV decides what parts of
the scene graph shall be built as OpenGL display lists and transform the
content of these nodes into display lists. We simulate this behaviour by
applying a SoRenderAction to every pair of slides in the slide show
once. Our experience says that this is enough - in any case it
eliminated the irregularity of animation when swithing the slide pairs
after the transition. For more details about the implementation please
consult comments in cacheSlides() in vrss.cxx.

*/

#endif

/*@}*/
/* ===========================================================================
    End of vrss.h
=========================================================================== */

