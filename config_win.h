 /* ========================================================================
  * Copyright (C) 2001  Vienna University of Technology
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Lesser General Public
  * License as published by the Free Software Foundation; either
  * version 2.1 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Lesser General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public
  * License along with this library; if not, write to the Free Software
  * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  * For further information please contact Gerhard Reitmayr under
  * <reitmayr@ims.tuwien.ac.at> or write to Gerhard Reitmayr,
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
  * Austria.
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** Configuration include file for OpenTracker
  * 
  * This file sets defines that control the compilation of device drivers in
  * OpenTracker. Some device drivers require additional libraries and might
  * not be available on all platforms. These source files are bracketed by
  * conditional compile clauses to only be compiled in, if desired. Edit this
  * file to set your configuration. This should only be changed, if you
  * recompile the library, as the include files that you use for other
  * projects will also require this file.
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/config_win.h,v 1.2 2003/03/22 15:26:27 kaufmann Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page config Configuration Options
 * 
 * Some device drivers require additional libraries and might
 * not be available on all platforms. These source files are bracketed by
 * conditional compile clauses to only be compiled in, if desired. You can select
 * these by enabling the defines in the file config.h. Edit this
 * file to set your configuration. This should only be changed, if you
 * recompile the library, as the include files that you use for other
 * projects will also require this file. Note that you
 * need to set the correct paths in the build system yourself, if automatic
 * discovery is not possible. This is necessary for VisualC++ where you will have 
 * to modify your project settings. For Unices we are working on autotools support
 * and will include it there as far as possible.
 *
 * The following defines are currently available :
 * @li @b USE_ARTOOLKIT
 *      \n enables ARToolkit support and the @ref artoolkitsource node and @ref artoolkitmodule
 * @li @b USE_CYBERMOUSE
 *      \n enables Logitech Cybermouse support and the @ref cybermousesource. This works only on
 *      Windows.
 * @li @b USE_WACOMGRAPHIRE
 *		\n enables Wacom Tablet support and the @ref wacomgraphiresource. This works only
 *      on Windows.
 * @li @b USE_JOYSTICK 
 *		\n enables joystick support via DirectInput and the @ref joysticksource. This works
 *      only on Windows.
 * @li @b USE_SPACEMOUSE
 *		\n enables SpaceMouse support and the @ref spacemousemodule. This works only
 *      on Windows.
 * @li @b USE_SAPISPEECH
 *		\n enables Speech Recognition and TextToSpeech support and the @ref speechmodule and
 *      @ref speechrecosource. By default only an empty implementation is present. This
 *      define enables support via the Microsoft SpeechAPI and works only under Windows.
 */

/** uncomment the following line to compile support for the ARToolKit library */
// #define USE_ARTOOLKIT 1

/** uncomment the following line to compile support for CyberMouse device.
  * Support for this is only available on windows. */
// #define USE_CYBERMOUSE 1

/** uncomment the following line to compile support for Wacom Graphire tablet.
  * Support for this is only available on windows. */
// #define USE_WACOMGRAPHIRE 1

/** uncomment the following line to compile support for Joysticks.
  * Support for this is only available on windows. */
// #define USE_JOYSTICK 1

/** uncomment the following line to compile support for 3Dconnexion SpacMouse.
  * Support for this is only available on windows. */
// #define USE_SPACEMOUSE 1

/** uncomment the following line to compile support for SAPI Speech Recognition and Text to Speech.
  * Support for this is only available on windows. Otherwise an empty implementation 
  * will be used */
// #define USE_SAPISPEECH 1

/** uncomment the following line to compile support for Essentialreality's P5 glove.
  * Support for this is only available on windows yet. Linux support is in development.*/
// #define USE_P5GLOVE 1
