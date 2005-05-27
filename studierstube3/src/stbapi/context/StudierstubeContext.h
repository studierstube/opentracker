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
/**
 * defines the standard Studierstube context indices and their meaning
 *
 * @author Gerhard Reitmayr
 *
 * $Id: StudierstubeContext.h 4141 2005-03-29 10:46:07Z kaufmann $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _STUDIERSTUBECONTEXT_H_
#define _STUDIERSTUBECONTEXT_H_

#include <Inventor/SbString.h>

#include <stbapi/stbapi.h>

/**
 * This class only provides a namespace for a set of constants. These constants
 * define the context indices and values used by the Studierstube system. The index names
 * use the prefix "Stb.". The values are usually integer values, so you need to create
 * them from the strings returned by the context. The index names are documented so that
 * you can use them in the file format as well. Within C++ code, one should use the 
 * variables introduced here to keep the code independend from string constants.
 *
 * @ingroup context
 */
class STBAPI_API StudierstubeContext {    
public:
    
    // The index strings defined for Studierstube system information
    /// index for user id of current render traversal = "Stb.User"
    static SbName User; 
    /// index for application id = "Stb.Application"
    static SbName Application;
    /// index for window id = "Stb.Window"
    static SbName Window;
    /// index for eye rendered in stereo mode = "Stb.Eye"
    static SbName Eye;
    /// index for distribution mode = "Stb.DIVMode"
    static SbName DivMode;
    /// index for user id of owner of userkit geometry = "Stb.Owner"
    static SbName Owner;

    /*
    enum Index {
        User = -1,              ///< user id, only present in a render action
        Application = -2,       ///< application id, any action, below the contextkit node
        Window = -3,            ///< window id, any action, below the window node
        Eye = -4,               ///< eye index, only present in a render action, see Stereo
        DivMode = -5             ///< master/slave mode of application, any action, below the contextkit node
    };
    */

    /// values defined for the eye index 
    enum Stereo {
        Left = 0, 
        Right = 1
    };

    /// values defined for the master mode index 
    enum MasterMode {
        Slave = 0, 
        Master = 1
    };
};

#endif // _STUDIERSTUBECONTEXT_H_
