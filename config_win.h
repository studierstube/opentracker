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
  * $Id$
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
 * @li @b USE_P5GLOVE
 *		\n enables support for Essentialreality's P5 glove using the @ref p5glovemodule and
 *       @ref p5glovesource. Support for this is only available on windows yet. 
 *       Linux support is under development. 
 * @li @b USE_DWARF
 *		\n enables interoperability of DWARF and OpenTracker. Using the @ref dwarfmodule
 *       allows you configure an OpenTracker process as a DWARF service where @ref 
 *       dwarfsink act as abilities and @ref dwarfsource act as needs.
 * @li @b USE_VRPN 
 *      \n enables interoperability with VRPN. OpenTracker can act both as a server and
 *       as a client to other VRPN processes. There is @ref vrpnmodule which allows setting
 *       the server parameters and two nodes @ref vrpnsource to receive data from a VRPN
 *       server and @ref vrpnsink to act as a device in the server. On Windows, set the
 *       environment variable @c VRPNROOT to the vrpn directory to configure the build system.
 * 
 */

/** uncomment the following line to compile support for the ARToolKit library */
// #define USE_ARTOOLKIT 1

/** uncomment the following line to compile support for the ARToolKitPlus library */
//#define USE_ARTOOLKITPLUS 1

/**
 * This define enables the correction of ARToolkit transformation matrices
 * on systems that provide camera images the right way around. ARToolkit seems
 * to expect image buffers up side down. Therefore it will compute mirrored
 * transformation matrices on normal image buffers. The correction avoids
 * the flipping of the image itself. This define must be enabled on virtually
 * any system as we do not use flipping anymore. Currently this is true only on
 * the Windows platform.
 */
#ifdef WIN32
#define ARTOOLKIT_UNFLIP_V
#endif

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

/** uncomment the following line to compile support for the DWARF.
  */
// #define USE_DWARF 1

/** uncomment the following line to compile support for VRPN. Don't forget to set the
  * @c VRPNROOT environment variable !
  */
// #define USE_VRPN 1
