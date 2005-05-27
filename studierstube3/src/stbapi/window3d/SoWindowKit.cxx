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
/** SoWindowKit.cxx  - implementation of class SoWindowKit (version 2).
  * SoWindowKit is the standard 3D container for graphical objects of
  * Studierstube applications.
  *
  * @author  Dieter Schmalstieg
  * @author  Gottfried Eibner
  *
  * $Id: SoWindowKit.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <cassert>
#include <stbapi/util/math2.h>
#include <stbapi/misc/ivmacros.h>
#include <stbapi/util/ivio.h>

#include <Inventor/SbBox.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/SoInput.h>

#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>

#include <Inventor/nodes/SoBaseColor.h>
#include <Inventor/nodes/SoCallback.h>
#include <Inventor/nodes/SoClipPlane.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoIndexedTriangleStripSet.h>
#include <Inventor/nodes/SoLightModel.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoVertexProperty.h>

#include <Inventor/elements/SoLazyElement.h>

#include <stbapi/workspace/SoContextKit.h>
#include <stbapi/workspace/SoContextManagerKit.h>
#include <stbapi/workspace/StbWorkspace.h>
#include <stbapi/resource/SoUserManagerKit.h>
#include <stbapi/widgets/So3DButton.h>

#include "SoWindowKit.h"

static char *vertexProperty =
"VertexProperty {\n"

"    vertex  [\n"
"        # top: +y   right: +x   front: +z\n"
"        # top right front               index  0.. 7\n"
"         1.00  1.00  1.00,   1.25  1.00  1.00,\n"
"         1.25  1.25  1.00,   1.00  1.25  1.00,\n"
"         1.00  1.00  1.25,   1.25  1.00  1.25,\n"
"         1.25  1.25  1.25,   1.00  1.25  1.25,\n"

"        # top left front                index  8..15\n"
"        -1.00  1.00  1.00,  -1.25  1.00  1.00,\n"
"        -1.25  1.25  1.00,  -1.00  1.25  1.00,\n"
"        -1.00  1.00  1.25,  -1.25  1.00  1.25,\n"
"        -1.25  1.25  1.25,  -1.00  1.25  1.25,\n"

"        # bottom left front             index 16..23\n"
"        -1.00 -1.00  1.00,  -1.25 -1.00  1.00,\n"
"        -1.25 -1.25  1.00,  -1.00 -1.25  1.00,\n"
"        -1.00 -1.00  1.25,  -1.25 -1.00  1.25,\n"
"        -1.25 -1.25  1.25,  -1.00 -1.25  1.25,\n"

"        # bottom right front            index 24..31\n"
"         1.00 -1.00  1.00,   1.25 -1.00  1.00,\n"
"         1.25 -1.25  1.00,   1.00 -1.25  1.00,\n"
"         1.00 -1.00  1.25,   1.25 -1.00  1.25,\n"
"         1.25 -1.25  1.25,   1.00 -1.25  1.25,\n"

"        # top right back                index 32..39\n"
"         1.00  1.00 -1.00,   1.25  1.00 -1.00,\n"
"         1.25  1.25 -1.00,   1.00  1.25 -1.00,\n"
"         1.00  1.00 -1.25,   1.25  1.00 -1.25,\n"
"         1.25  1.25 -1.25,   1.00  1.25 -1.25,\n"

"        # top left back                 index 40..47\n"
"        -1.00  1.00 -1.00,  -1.25  1.00 -1.00,\n"
"        -1.25  1.25 -1.00,  -1.00  1.25 -1.00,\n"
"        -1.00  1.00 -1.25,  -1.25  1.00 -1.25,\n"
"        -1.25  1.25 -1.25,  -1.00  1.25 -1.25,\n"

"        # bottom left back              index 48..55\n"
"        -1.00 -1.00 -1.00,  -1.25 -1.00 -1.00,\n"
"        -1.25 -1.25 -1.00,  -1.00 -1.25 -1.00,\n"
"        -1.00 -1.00 -1.25,  -1.25 -1.00 -1.25,\n"
"        -1.25 -1.25 -1.25,  -1.00 -1.25 -1.25,\n"

"        # bottom right back             index 56..63\n"
"         1.00 -1.00 -1.00,   1.25 -1.00 -1.00,\n"
"         1.25 -1.25 -1.00,   1.00 -1.25 -1.00,\n"
"         1.00 -1.00 -1.25,   1.25 -1.00 -1.25,\n"
"         1.25 -1.25 -1.25,   1.00 -1.25 -1.25,\n"
"    ]\n"

"    normal  [\n"
"         1  0  0,  0  1  0,  0  0  1,\n"
"        -1  0  0,  0 -1  0,  0  0 -1\n"
"    ]\n"

"    texCoord            [   ]\n"
       // color values for front wall    border      corner    back wall
"    orderedRGBA         [ 0xCCCCCCFF, 0xCCCCCCFF, 0xCCCCCCFF, 0xCCCCCCFF ]\n"

"    materialBinding  PER_FACE_INDEXED\n"

"    normalBinding  PER_PART_INDEXED\n"

"}";

static char *frontfaceSet =
"IndexedFaceSet {\n"

"    vertexProperty      NULL\n"

"    coordIndex          [\n"

"         0, 8,16,24,-1,\n"     //  front side (+z)
"         0,24,56,32,-1,\n"     //  right side (+x)
"        32,56,48,40,-1,\n"     //   back side (-z)
"         8,40,48,16,-1,\n"     //   left side (-x)
"         8, 0,32,40,-1,\n"     //    top side (+y)
"        16,48,56,24,-1 \n"     // bottom side (-y)

"    ]\n"

"    materialIndex       [0,0,0,0,0,0]\n"

"    normalIndex         [5,3,2,0,4,1]\n"

"    textureCoordIndex   -1\n"

"}";

static char *backfaceSet =
"IndexedFaceSet {\n"

"    vertexProperty      NULL\n"

"    coordIndex          [\n"

"        24,16, 8, 0,-1,\n"     //  front side (+z)
"        32,56,24, 0,-1,\n"     //  right side (+x)
"        40,48,56,32,-1,\n"     //   back side (-z)
"        16,48,40, 8,-1,\n"     //   left side (-x)
"        40,32, 0, 8,-1,\n"     //    top side (+y)
"        24,56,48,16,-1 \n"     // bottom side (-y)

"    ]\n"

"    materialIndex       [3,3,3,3,3,3]\n"

"    normalIndex         [5,3,2,0,4,1]\n"

"    textureCoordIndex   -1\n"

"}";

static char *triangleSet =
"IndexedTriangleStripSet {\n"

"    vertexProperty      NULL\n"

"    coordIndex          [\n"
"        # outer skin\n"
"            # +z face\n"
"         6,  5,  7,  4, 15, 12, -1,\n"
"        14, 15, 13, 12, 21, 20, -1,\n"
"        22, 21, 23, 20, 31, 28, -1,\n"
"        30, 31, 29, 28,  5,  4, -1,\n"
"            # +x face\n"
"         6,  2,  5,  1, 29, 25, -1,\n"
"        30, 29, 26, 25, 58, 57, -1,\n"
"        62, 58, 61, 57, 37, 33, -1,\n"
"        38, 37, 34, 33,  2,  1, -1,\n"
"            # -z face\n"
"        38, 37, 39, 36, 47, 44, -1,\n"
"        46, 47, 45, 44, 53, 52, -1,\n"
"        54, 53, 55, 52, 63, 60, -1,\n"
"        62, 63, 61, 60, 37, 36, -1,\n"
"            # -x face\n"
"        14, 10, 13,  9, 21, 17, -1,\n"
"        22, 21, 18, 17, 50, 49, -1,\n"
"        54, 50, 53, 49, 45, 41, -1,\n"
"        46, 45, 42, 41, 10,  9, -1,\n"
"            # +y face\n"
"         6,  7,  2,  3, 34, 35, -1,\n"
"        38, 34, 39, 35, 47, 43, -1,\n"
"        46, 47, 42, 43, 10, 11, -1,\n"
"        14, 10, 15, 11,  7,  3, -1,\n"
"            # -y face\n"
"        30, 31, 26, 27, 58, 59, -1,\n"
"        62, 58, 63, 59, 55, 51, -1,\n"
"        54, 55, 50, 51, 18, 19, -1,\n"
"        22, 18, 23, 19, 31, 27, -1,\n"

"        # inner skin\n"
"            # +z face\n"
"         4,  0, 12,  8, -1,\n"
"        12,  8, 20, 16, -1,\n"
"        20, 16, 28, 24, -1,\n"
"        28, 24,  4,  0, -1,\n"
"            # +x face\n"
"        33, 32,  1,  0, -1,\n"
"         1,  0, 25, 24, -1,\n"
"        25, 24, 57, 56, -1,\n"
"        57, 56, 33, 32, -1,\n"
"            # -z face\n"
"        36, 32, 44, 40, -1,\n"
"        44, 40, 52, 48, -1,\n"
"        52, 48, 60, 56, -1,\n"
"        60, 56, 36, 32, -1,\n"
"            # -x face\n"
"        41, 40,  9,  8, -1,\n"
"         9,  8, 17, 16, -1,\n"
"        17, 16, 49, 48, -1,\n"
"        49, 48, 41, 40, -1,\n"
"            # +y face\n"
"         3,  0, 11,  8, -1,\n"
"        11,  8, 43, 40, -1,\n"
"        43, 40, 35, 32, -1,\n"
"        35, 32,  3,  0, -1,\n"
"            # -y face\n"
"        27, 24, 19, 16, -1,\n"
"        19, 16, 51, 48, -1,\n"
"        51, 48, 59, 56, -1,\n"
"        59, 56, 27, 24, -1\n"
"    ]\n"

"    materialIndex       [\n"
"        # outer skin\n"
"        2,2,1,1, 2,2,1,1, 2,2,1,1, 2,2,1,1,\n"
"        2,2,1,1, 2,2,1,1, 2,2,1,1, 2,2,1,1,\n"
"        2,2,1,1, 2,2,1,1, 2,2,1,1, 2,2,1,1,\n"
"        2,2,1,1, 2,2,1,1, 2,2,1,1, 2,2,1,1,\n"
"        2,2,1,1, 2,2,1,1, 2,2,1,1, 2,2,1,1,\n"
"        2,2,1,1, 2,2,1,1, 2,2,1,1, 2,2,1,1,\n"
"        # inner skin\n"
"        1,1, 1,1, 1,1, 1,1,\n"
"        1,1, 1,1, 1,1, 1,1,\n"
"        1,1, 1,1, 1,1, 1,1,\n"
"        1,1, 1,1, 1,1, 1,1,\n"
"        1,1, 1,1, 1,1, 1,1,\n"
"        1,1, 1,1, 1,1, 1,1\n"
"    ]\n"

"    normalIndex  [\n"
"          2, 2, 2, 2,\n"
"          0, 0, 0, 0,\n"
"          5, 5, 5, 5,\n"
"          3, 3, 3, 3,\n"
"          1, 1, 1, 1,\n"
"          4, 4, 4, 4,\n"

"          4, 0, 1, 3,\n"
"          4, 5, 1, 2,\n"
"          4, 0, 1, 3,\n"
"          4, 5, 1, 2,\n"
"          5, 0, 2, 3,\n"
"          5, 0, 2, 3 \n"
"    ]\n"

"    textureCoordIndex   -1\n"

"}";


/// Defines the colors of a window depending on its state
enum COLOR_WK {
    COL_INACTIVE = 0x606060ff,  /// inactive window front wall's color
    COL_INASHADOW= 0x0c0c0cff,  /// inactive window back wall's color

    COL_ACTIVE   = 0x000090ff,  /// pen inside transform/client volume
    COL_HIT      = 0x404090ff,  /// pen inside drag (border/corner) volume
    COL_SELECT   = 0x808090ff   /// pen inside drag volume, button pressed
};

// button states
enum BUTTON_WK {
    BT_RELEASE,
    BT_PRESS,
    BT_MOVE
};

// volume states
enum VOLUME_WK {
    VL_OUTSIDE,         /// outside the window volume
    VL_CLIENT,          /// inside the client volume
    VL_TRANSFORM,       /// inside window but outside client volume
    VL_BORDER,          /// inside a border volume
    VL_BORDER_SEL,      /// inside a border and button pressed
                        ///  (i.e. the border is dragged)
    VL_CORNER,          /// inside a corner volume
    VL_CORNER_SEL       /// inside a corner and button pressed
                        ///  (i.e. the corner is dragged)
};

// grabber states
enum GRABBER_WK {
    GR_SET,
    GR_RELEASE,
    GR_DONT_CARE
};

// highlight states
enum HIGHLIGHT_WK {
    HL_NONE,
    HL_HIT,
    HL_DRAG,
    HL_DONT_CARE
};

// input vector access
enum INPUT_WK {
    IN_EVENT,           // values: see BUTTON_WK
    IN_VOLUME           // values: see VOLUME_WK
};

// output vector access
enum OUTPUT_WK {
    OUT_GRABBER,        // values: see GRABBER_WK
    OUT_HIGHLIGHT,      // values: see HIGHLIGHT_WK
    OUT_HANDLED,        // values: TRUE, FALSE
    OUT_VOLUME          // values: see VOLUME_WK
};

AutomatonTransition SoWindowKit::transitions[105] =
{
    // outside -> ...
    {
        0, VL_OUTSIDE, VL_OUTSIDE,              //index, srcState, dstState
        {BT_MOVE, VL_OUTSIDE, 0,0,0},           // event, volume
        {GR_RELEASE, HL_DONT_CARE, FALSE, VL_OUTSIDE,0},
        NULL
    },
    {
        1, VL_OUTSIDE, VL_OUTSIDE,              //index, srcState, dstState
        {AUT_DONT_CARE, VL_OUTSIDE, 0,0,0},   // event, volume
        {GR_RELEASE, HL_NONE, FALSE, VL_OUTSIDE,0},
        NULL
    },
    {
        2, VL_OUTSIDE, VL_OUTSIDE,              //index, srcState, dstState
        {AUT_DONT_CARE, VL_OUTSIDE, 0,0,0},   // event, volume
        {GR_RELEASE, HL_NONE, FALSE, VL_OUTSIDE,0},
        NULL
    },

    {
        3, VL_OUTSIDE, VL_TRANSFORM,            //index, srcState, dstState
        {AUT_DONT_CARE, VL_TRANSFORM, 0,0,0}, // event, volume
        {GR_SET, HL_HIT, TRUE, VL_TRANSFORM,0},
        NULL
    },
    {
        4, VL_OUTSIDE, VL_TRANSFORM,            //index, srcState, dstState
        {AUT_DONT_CARE, VL_TRANSFORM, 0,0,0}, // event, volume
        {GR_SET, HL_HIT, TRUE, VL_TRANSFORM,0},
        NULL
    },
    {
        5, VL_OUTSIDE, VL_TRANSFORM,            //index, srcState, dstState
        {AUT_DONT_CARE, VL_TRANSFORM, 0,0,0}, // event, volume
        {GR_SET, HL_HIT, TRUE, VL_TRANSFORM,0},
        NULL
    },

    {
        6, VL_OUTSIDE, VL_CLIENT,               //index, srcState, dstState
        {BT_MOVE, VL_CLIENT, 0,0,0},            // event, volume
        {GR_RELEASE, HL_HIT, FALSE, VL_CLIENT,0},
        NULL
    },
    {
        7, VL_OUTSIDE, VL_CLIENT,               //index, srcState, dstState
        {BT_PRESS, VL_CLIENT, 0,0,0},           // event, volume
        {GR_RELEASE, HL_HIT, FALSE, VL_CLIENT,0},
        NULL
    },
    {
        8, VL_OUTSIDE, VL_CLIENT,               //index, srcState, dstState
        {BT_RELEASE, VL_CLIENT, 0,0,0},         // event, volume
        {GR_RELEASE, HL_HIT, FALSE, VL_CLIENT,0},
        NULL
    },

    {
        9, VL_OUTSIDE, VL_BORDER,               //index, srcState, dstState
        {BT_MOVE, VL_BORDER, 0,0,0},            // event, volume
        {GR_SET, HL_HIT, TRUE, VL_BORDER,0},
        NULL
    },
    {
        10,VL_OUTSIDE, VL_BORDER_SEL,           //index, srcState, dstState
        {BT_PRESS, VL_BORDER, 0,0,0},           // event, volume
        {GR_SET, HL_DRAG, TRUE, VL_BORDER,0},
        NULL
    },
    {
        11,VL_OUTSIDE, VL_BORDER,               //index, srcState, dstState
        {BT_RELEASE, VL_BORDER, 0,0,0},         // event, volume
        {GR_SET, HL_HIT, TRUE, VL_BORDER,0},
        NULL
    },

    {
        12,VL_OUTSIDE, VL_CORNER,               //index, srcState, dstState
        {BT_MOVE, VL_CORNER, 0,0,0},            // event, volume
        {GR_SET, HL_HIT, TRUE, VL_CORNER,0},
        NULL
    },
    {
        13,VL_OUTSIDE, VL_CORNER_SEL,           //index, srcState, dstState
        {BT_PRESS, VL_CORNER, 0,0,0},           // event, volume
        {GR_SET, HL_DRAG, TRUE, VL_CORNER,0},
        NULL
    },
    {
        14,VL_OUTSIDE, VL_CORNER,               //index, srcState, dstState
        {BT_RELEASE, VL_CORNER, 0,0,0},         // event, volume
        {GR_SET, HL_HIT, TRUE, VL_CORNER,0},
        NULL
    },


    // transform -> ...
    {
        15,VL_TRANSFORM, VL_OUTSIDE,            //index, srcState, dstState
        {BT_MOVE, VL_OUTSIDE, 0,0,0},           // event, volume
        {GR_RELEASE, HL_HIT, TRUE, VL_OUTSIDE,0},
        NULL
    },
    {
        16,VL_TRANSFORM, VL_OUTSIDE,            //index, srcState, dstState
        {BT_PRESS, VL_OUTSIDE, 0,0,0},          // event, volume
        {GR_RELEASE, HL_NONE, TRUE, VL_OUTSIDE,0},
        NULL
    },
    {
        17,VL_TRANSFORM, VL_OUTSIDE,            //index, srcState, dstState
        {BT_RELEASE, VL_OUTSIDE, 0,0,0},        // event, volume
        {GR_RELEASE, HL_HIT, TRUE, VL_OUTSIDE,0},
        NULL
    },

    {
        18,VL_TRANSFORM, VL_TRANSFORM,          //index, srcState, dstState
        {AUT_DONT_CARE, VL_TRANSFORM, 0,0,0}, // event, volume
        {GR_DONT_CARE, HL_DONT_CARE, TRUE, VL_TRANSFORM,0},
        NULL
    },
    {
        19,VL_TRANSFORM, VL_TRANSFORM,          //index, srcState, dstState
        {AUT_DONT_CARE, VL_TRANSFORM, 0,0,0}, // event, volume
        {GR_DONT_CARE, HL_DONT_CARE, TRUE, VL_TRANSFORM,0},
        NULL
    },
    {
        20,VL_TRANSFORM, VL_TRANSFORM,          //index, srcState, dstState
        {AUT_DONT_CARE, VL_TRANSFORM, 0,0,0}, // event, volume
        {GR_DONT_CARE, HL_DONT_CARE, TRUE, VL_TRANSFORM,0},
        NULL
    },

    {
        21,VL_TRANSFORM, VL_CLIENT,             //index, srcState, dstState
        {AUT_DONT_CARE, VL_CLIENT, 0,0,0},    // event, volume
        {GR_RELEASE, HL_HIT, FALSE, VL_CLIENT,0},
        NULL
    },
    {
        22,VL_TRANSFORM, VL_CLIENT,    //index, srcState, dstState
        {AUT_DONT_CARE, VL_CLIENT, 0,0,0},    // event, volume
        {GR_RELEASE, HL_HIT, FALSE, VL_CLIENT,0},
        NULL
    },
    {
        23,VL_TRANSFORM, VL_CLIENT,    //index, srcState, dstState
        {AUT_DONT_CARE, VL_CLIENT, 0,0,0},    // event, volume
        {GR_RELEASE, HL_HIT, FALSE, VL_CLIENT,0},
        NULL
    },

    {
        24,VL_TRANSFORM, VL_BORDER,    //index, srcState, dstState
        {BT_MOVE, VL_BORDER, 0,0,0},    // event, volume
        {GR_DONT_CARE, HL_HIT, TRUE, VL_BORDER,0},
        NULL
    },
    {
        25,VL_TRANSFORM, VL_BORDER_SEL,    //index, srcState, dstState
        {BT_PRESS, VL_BORDER, 0,0,0},    // event, volume
        {GR_DONT_CARE, HL_DRAG, TRUE, VL_BORDER,0},
        NULL
    },
    {
        26,VL_TRANSFORM, VL_BORDER,    //index, srcState, dstState
        {BT_RELEASE, VL_BORDER, 0,0,0},    // event, volume
        {GR_DONT_CARE, HL_HIT, TRUE, VL_BORDER,0},
        NULL
    },

    {
        27,VL_TRANSFORM, VL_CORNER,    //index, srcState, dstState
        {BT_MOVE, VL_CORNER, 0,0,0},    // event, volume
        {GR_DONT_CARE, HL_HIT, TRUE, VL_CORNER,0},
        NULL
    },
    {
        28,VL_TRANSFORM, VL_CORNER_SEL,    //index, srcState, dstState
        {BT_PRESS, VL_CORNER, 0,0,0},    // event, volume
        {GR_DONT_CARE, HL_DRAG, TRUE, VL_CORNER,0},
        NULL
    },
    {
        29,VL_TRANSFORM, VL_CORNER,    //index, srcState, dstState
        {BT_RELEASE, VL_CORNER, 0,0,0},    // event, volume
        {GR_DONT_CARE, HL_HIT, TRUE, VL_CORNER,0},
        NULL
    },


    // client -> ...
    {
        30,VL_CLIENT, VL_OUTSIDE,      //index, srcState, dstState
        {BT_MOVE, VL_OUTSIDE, 0,0,0},      // event, volume
        {GR_DONT_CARE, HL_HIT, TRUE, VL_OUTSIDE,0},
        NULL
    },
    {
        31,VL_CLIENT, VL_OUTSIDE,      //index, srcState, dstState
        {BT_PRESS, VL_OUTSIDE, 0,0,0}, // event, volume
        {GR_DONT_CARE, HL_NONE, TRUE, VL_OUTSIDE,0},
        NULL
    },
    {
        32,VL_CLIENT, VL_OUTSIDE,      //index, srcState, dstState
        {BT_RELEASE, VL_OUTSIDE, 0,0,0},   // event, volume
        {GR_DONT_CARE, HL_HIT, TRUE, VL_OUTSIDE,0},
        NULL
    },

    {
        33,VL_CLIENT, VL_TRANSFORM,    //index, srcState, dstState
        {AUT_DONT_CARE, VL_TRANSFORM, 0,0,0},    // event, volume
        {GR_SET, HL_HIT, TRUE, VL_TRANSFORM,0},
        NULL
    },
    {
        34,VL_CLIENT, VL_TRANSFORM,    //index, srcState, dstState
        {AUT_DONT_CARE, VL_TRANSFORM, 0,0,0},    // event, volume
        {GR_SET, HL_HIT, TRUE, VL_TRANSFORM,0},
        NULL
    },
    {
        35,VL_CLIENT, VL_TRANSFORM,    //index, srcState, dstState
        {AUT_DONT_CARE, VL_TRANSFORM, 0,0,0},    // event, volume
        {GR_SET, HL_HIT, TRUE, VL_TRANSFORM,0},
        NULL
    },

    {
        36,VL_CLIENT, VL_CLIENT,    //index, srcState, dstState
        {AUT_DONT_CARE, VL_CLIENT, 0,0,0},    // event, volume
        {GR_DONT_CARE, HL_DONT_CARE, FALSE, VL_CLIENT,0},
        NULL
    },
    {
        37,VL_CLIENT, VL_CLIENT,    //index, srcState, dstState
        {AUT_DONT_CARE, VL_CLIENT, 0,0,0},    // event, volume
        {GR_DONT_CARE, HL_DONT_CARE, FALSE, VL_CLIENT,0},
        NULL
    },
    {
        38,VL_CLIENT, VL_CLIENT,    //index, srcState, dstState
        {AUT_DONT_CARE, VL_CLIENT, 0,0,0},    // event, volume
        {GR_DONT_CARE, HL_DONT_CARE, FALSE, VL_CLIENT,0},
        NULL
    },

    {
        39,VL_CLIENT, VL_BORDER,    //index, srcState, dstState
        {BT_MOVE, VL_BORDER, 0,0,0},    // event, volume
        {GR_SET, HL_HIT, TRUE, VL_BORDER,0},
        NULL
    },
    {
        40,VL_CLIENT, VL_BORDER_SEL,    //index, srcState, dstState
        {BT_PRESS, VL_BORDER, 0,0,0},    // event, volume
        {GR_SET, HL_DRAG, TRUE, VL_BORDER,0},
        NULL
    },
    {
        41,VL_CLIENT, VL_BORDER,    //index, srcState, dstState
        {BT_RELEASE, VL_BORDER, 0,0,0},    // event, volume
        {GR_SET, HL_HIT, TRUE, VL_BORDER,0},
        NULL
    },

    {
        42,VL_CLIENT, VL_CORNER,    //index, srcState, dstState
        {BT_MOVE, VL_CORNER, 0,0,0},    // event, volume
        {GR_SET, HL_HIT, TRUE, VL_CORNER,0},
        NULL
    },
    {
        43,VL_CLIENT, VL_CORNER_SEL,    //index, srcState, dstState
        {BT_PRESS, VL_CORNER, 0,0,0},    // event, volume
        {GR_SET, HL_DRAG, TRUE, VL_CORNER,0},
        NULL
    },
    {
        44,VL_CLIENT, VL_CORNER,    //index, srcState, dstState
        {BT_RELEASE, VL_CORNER, 0,0,0},    // event, volume
        {GR_SET, HL_HIT, TRUE, VL_CORNER,0},
        NULL
    },


    // border -> ...
    {
        45,VL_BORDER, VL_OUTSIDE,      //index, srcState, dstState
        {BT_PRESS, VL_OUTSIDE, 0,0,0},      // event, volume
        {GR_RELEASE, HL_NONE, TRUE, VL_OUTSIDE,0},
        NULL
    },
    {
        46,VL_BORDER, VL_OUTSIDE,      //index, srcState, dstState
        {AUT_DONT_CARE, VL_OUTSIDE, 0,0,0}, // event, volume
        {GR_RELEASE, HL_HIT, TRUE, VL_OUTSIDE,0},
        NULL
    },
    {
        47,VL_BORDER, VL_OUTSIDE,      //index, srcState, dstState
        {AUT_DONT_CARE, VL_OUTSIDE, 0,0,0},   // event, volume
        {GR_RELEASE, HL_HIT, TRUE, VL_OUTSIDE,0},
        NULL
    },

    {
        48,VL_BORDER, VL_TRANSFORM,    //index, srcState, dstState
        {AUT_DONT_CARE, VL_TRANSFORM, 0,0,0},    // event, volume
        {GR_DONT_CARE, HL_HIT, TRUE, VL_TRANSFORM,0},
        NULL
    },
    {
        49,VL_BORDER, VL_TRANSFORM,    //index, srcState, dstState
        {AUT_DONT_CARE, VL_TRANSFORM, 0,0,0},    // event, volume
        {GR_DONT_CARE, HL_HIT, TRUE, VL_TRANSFORM,0},
        NULL
    },
    {
        50,VL_BORDER, VL_TRANSFORM,    //index, srcState, dstState
        {AUT_DONT_CARE, VL_TRANSFORM, 0,0,0},    // event, volume
        {GR_DONT_CARE, HL_HIT, TRUE, VL_TRANSFORM,0},
        NULL
    },

    {
        51,VL_BORDER, VL_CLIENT,    //index, srcState, dstState
        {AUT_DONT_CARE, VL_CLIENT, 0,0,0},    // event, volume
        {GR_RELEASE, HL_HIT, FALSE, VL_CLIENT,0},
        NULL
    },
    {
        52,VL_BORDER, VL_CLIENT,    //index, srcState, dstState
        {AUT_DONT_CARE, VL_CLIENT, 0,0,0},    // event, volume
        {GR_RELEASE, HL_HIT, FALSE, VL_CLIENT,0},
        NULL
    },
    {
        53,VL_BORDER, VL_CLIENT,    //index, srcState, dstState
        {AUT_DONT_CARE, VL_CLIENT, 0,0,0},    // event, volume
        {GR_RELEASE, HL_HIT, FALSE, VL_CLIENT,0},
        NULL
    },

    {
        54,VL_BORDER, VL_BORDER,    //index, srcState, dstState
        {BT_MOVE, VL_BORDER, 0,0,0},    // event, volume
        {GR_DONT_CARE, HL_DONT_CARE, TRUE, VL_BORDER,0},
        NULL
    },
    {
        55,VL_BORDER, VL_BORDER_SEL,    //index, srcState, dstState
        {BT_PRESS, VL_BORDER, 0,0,0},    // event, volume
        {GR_DONT_CARE, HL_DRAG, TRUE, VL_BORDER,0},
        NULL
    },
    {
        56,VL_BORDER, VL_BORDER,    //index, srcState, dstState
        {BT_RELEASE, VL_BORDER, 0,0,0},    // event, volume
        {GR_DONT_CARE, HL_DONT_CARE, TRUE, VL_BORDER,0},
        NULL
    },

    {
        57,VL_BORDER, VL_CORNER,    //index, srcState, dstState
        {BT_MOVE, VL_CORNER, 0,0,0},    // event, volume
        {GR_DONT_CARE, HL_HIT, TRUE, VL_CORNER,0},
        NULL
    },
    {
        58,VL_BORDER, VL_CORNER_SEL,    //index, srcState, dstState
        {BT_PRESS, VL_CORNER, 0,0,0},    // event, volume
        {GR_DONT_CARE, HL_DRAG, TRUE, VL_CORNER,0},
        NULL
    },
    {
        59,VL_BORDER, VL_CORNER,    //index, srcState, dstState
        {BT_RELEASE, VL_CORNER, 0,0,0},    // event, volume
        {GR_DONT_CARE, HL_HIT, TRUE, VL_CORNER,0},
        NULL
    },


    // border_sel -> ...
    {
        60,VL_BORDER_SEL, VL_BORDER_SEL,      //index, srcState, dstState
        {BT_MOVE, VL_OUTSIDE, 0,0,0},      // event, volume
        {GR_DONT_CARE, HL_DONT_CARE, TRUE, VL_BORDER,0},
        NULL
    },
    {
        61,VL_BORDER_SEL, VL_OUTSIDE,      //index, srcState, dstState
        {BT_PRESS, VL_OUTSIDE, 0,0,0}, // event, volume
        {GR_RELEASE, HL_NONE, TRUE, VL_OUTSIDE,0},
        NULL
    },
    {
        62,VL_BORDER_SEL, VL_OUTSIDE,      //index, srcState, dstState
        {BT_RELEASE, VL_OUTSIDE, 0,0,0},   // event, volume
        {GR_RELEASE, HL_HIT, TRUE, VL_OUTSIDE,0},
        NULL
    },

    {
        63,VL_BORDER_SEL, VL_BORDER_SEL,    //index, srcState, dstState
        {BT_MOVE, VL_TRANSFORM, 0,0,0},    // event, volume
        {GR_DONT_CARE, HL_DONT_CARE, TRUE, VL_BORDER,0},
        NULL
    },
    {
        64,VL_BORDER_SEL, VL_TRANSFORM,    //index, srcState, dstState
        {BT_PRESS, VL_TRANSFORM, 0,0,0},    // event, volume
        {GR_DONT_CARE, HL_HIT, TRUE, VL_TRANSFORM,0},
        NULL
    },
    {
        65,VL_BORDER_SEL, VL_TRANSFORM,    //index, srcState, dstState
        {BT_RELEASE, VL_TRANSFORM, 0,0,0},    // event, volume
        {GR_DONT_CARE, HL_HIT, TRUE, VL_TRANSFORM,0},
        NULL
    },

    {
        66,VL_BORDER_SEL, VL_BORDER_SEL,    //index, srcState, dstState
        {BT_MOVE, VL_CLIENT, 0,0,0},    // event, volume
        {GR_DONT_CARE, HL_DONT_CARE, TRUE, VL_BORDER,0},
        NULL
    },
    {
        67,VL_BORDER_SEL, VL_CLIENT,    //index, srcState, dstState
        {BT_PRESS, VL_CLIENT, 0,0,0},    // event, volume
        {GR_RELEASE, HL_HIT, FALSE, VL_CLIENT,0},
        NULL
    },
    {
        68,VL_BORDER_SEL, VL_CLIENT,    //index, srcState, dstState
        {BT_RELEASE, VL_CLIENT, 0,0,0},    // event, volume
        {GR_RELEASE, HL_HIT, FALSE, VL_CLIENT,0},
        NULL
    },

    {
        69,VL_BORDER_SEL, VL_BORDER_SEL,    //index, srcState, dstState
        {BT_MOVE, VL_BORDER, 0,0,0},    // event, volume
        {GR_DONT_CARE, HL_DONT_CARE, TRUE, VL_BORDER,0},
        NULL
    },
    {
        70,VL_BORDER_SEL, VL_BORDER_SEL,    //index, srcState, dstState
        {BT_PRESS, VL_BORDER, 0,0,0},    // event, volume
        {GR_DONT_CARE, HL_DONT_CARE, TRUE, VL_BORDER,0},
        NULL
    },
    {
        71,VL_BORDER_SEL, VL_BORDER,    //index, srcState, dstState
        {BT_RELEASE, VL_BORDER, 0,0,0},    // event, volume
        {GR_DONT_CARE, HL_HIT, TRUE, VL_BORDER,0},
        NULL
    },

    {
        72,VL_BORDER_SEL, VL_BORDER_SEL,    //index, srcState, dstState
        {BT_MOVE, VL_CORNER, 0,0,0},    // event, volume
        {GR_DONT_CARE, HL_DONT_CARE, TRUE, VL_BORDER,0},
        NULL
    },
    {
        73,VL_BORDER_SEL, VL_CORNER_SEL,    //index, srcState, dstState
        {BT_PRESS, VL_CORNER, 0,0,0},    // event, volume
        {GR_DONT_CARE, HL_DRAG, TRUE, VL_CORNER,0},
        NULL
    },
    {
        74,VL_BORDER_SEL, VL_CORNER,    //index, srcState, dstState
        {BT_RELEASE, VL_CORNER, 0,0,0},    // event, volume
        {GR_DONT_CARE, HL_HIT, TRUE, VL_CORNER,0},
        NULL
    },


    // corner -> ...
    {
        75,VL_CORNER, VL_OUTSIDE,      //index, srcState, dstState
        {BT_PRESS, VL_OUTSIDE, 0,0,0},      // event, volume
        {GR_RELEASE, HL_NONE, TRUE, VL_OUTSIDE,0},
        NULL
    },
    {
        76,VL_CORNER, VL_OUTSIDE,      //index, srcState, dstState
        {AUT_DONT_CARE, VL_OUTSIDE, 0,0,0}, // event, volume
        {GR_RELEASE, HL_HIT, TRUE, VL_OUTSIDE,0},
        NULL
    },
    {
        77,VL_CORNER, VL_OUTSIDE,      //index, srcState, dstState
        {AUT_DONT_CARE, VL_OUTSIDE, 0,0,0},   // event, volume
        {GR_RELEASE, HL_HIT, TRUE, VL_OUTSIDE,0},
        NULL
    },

    {
        78,VL_CORNER, VL_TRANSFORM,    //index, srcState, dstState
        {AUT_DONT_CARE, VL_TRANSFORM, 0,0,0},    // event, volume
        {GR_DONT_CARE, HL_HIT, TRUE, VL_TRANSFORM,0},
        NULL
    },
    {
        79,VL_CORNER, VL_TRANSFORM,    //index, srcState, dstState
        {AUT_DONT_CARE, VL_TRANSFORM, 0,0,0},    // event, volume
        {GR_DONT_CARE, HL_HIT, TRUE, VL_TRANSFORM,0},
        NULL
    },
    {
        80,VL_CORNER, VL_TRANSFORM,    //index, srcState, dstState
        {AUT_DONT_CARE, VL_TRANSFORM, 0,0,0},    // event, volume
        {GR_DONT_CARE, HL_HIT, TRUE, VL_TRANSFORM,0},
        NULL
    },

    {
        81,VL_CORNER, VL_CLIENT,    //index, srcState, dstState
        {AUT_DONT_CARE, VL_CLIENT, 0,0,0},    // event, volume
        {GR_RELEASE, HL_HIT, FALSE, VL_CLIENT,0},
        NULL
    },
    {
        82,VL_CORNER, VL_CLIENT,    //index, srcState, dstState
        {AUT_DONT_CARE, VL_CLIENT, 0,0,0},    // event, volume
        {GR_RELEASE, HL_HIT, FALSE, VL_CLIENT,0},
        NULL
    },
    {
        83,VL_CORNER, VL_CLIENT,    //index, srcState, dstState
        {AUT_DONT_CARE, VL_CLIENT, 0,0,0},    // event, volume
        {GR_RELEASE, HL_HIT, FALSE, VL_CLIENT,0},
        NULL
    },

    {
        84,VL_CORNER, VL_BORDER,    //index, srcState, dstState
        {BT_MOVE, VL_BORDER, 0,0,0},    // event, volume
        {GR_DONT_CARE, HL_HIT, TRUE, VL_BORDER,0},
        NULL
    },
    {
        85,VL_CORNER, VL_BORDER_SEL,    //index, srcState, dstState
        {BT_PRESS, VL_BORDER, 0,0,0},    // event, volume
        {GR_DONT_CARE, HL_DRAG, TRUE, VL_BORDER,0},
        NULL
    },
    {
        86,VL_CORNER, VL_BORDER,    //index, srcState, dstState
        {BT_RELEASE, VL_BORDER, 0,0,0},    // event, volume
        {GR_DONT_CARE, HL_HIT, TRUE, VL_BORDER,0},
        NULL
    },

    {
        87,VL_CORNER, VL_CORNER,    //index, srcState, dstState
        {BT_MOVE, VL_CORNER, 0,0,0},    // event, volume
        {GR_DONT_CARE, HL_DONT_CARE, TRUE, VL_CORNER,0},
        NULL
    },
    {
        88,VL_CORNER, VL_CORNER_SEL,    //index, srcState, dstState
        {BT_PRESS, VL_CORNER, 0,0,0},    // event, volume
        {GR_DONT_CARE, HL_DRAG, TRUE, VL_CORNER,0},
        NULL
    },
    {
        89,VL_CORNER, VL_CORNER,    //index, srcState, dstState
        {BT_RELEASE, VL_CORNER, 0,0,0},    // event, volume
        {GR_DONT_CARE, HL_DONT_CARE, TRUE, VL_CORNER,0},
        NULL
    },


    // corner_sel -> ...
    {
        90,VL_CORNER_SEL, VL_CORNER_SEL,      //index, srcState, dstState
        {BT_MOVE, VL_OUTSIDE, 0,0,0},      // event, volume
        {GR_DONT_CARE, HL_DONT_CARE, TRUE, VL_CORNER,0},
        NULL
    },
    {
        91,VL_CORNER_SEL, VL_OUTSIDE,      //index, srcState, dstState
        {BT_PRESS, VL_OUTSIDE, 0,0,0}, // event, volume
        {GR_RELEASE, HL_NONE, TRUE, VL_OUTSIDE,0},
        NULL
    },
    {
        92,VL_CORNER_SEL, VL_OUTSIDE,      //index, srcState, dstState
        {BT_RELEASE, VL_OUTSIDE, 0,0,0},   // event, volume
        {GR_RELEASE, HL_HIT, TRUE, VL_OUTSIDE,0},
        NULL
    },

    {
        93,VL_CORNER_SEL, VL_CORNER_SEL,    //index, srcState, dstState
        {BT_MOVE, VL_TRANSFORM, 0,0,0},    // event, volume
        {GR_DONT_CARE, HL_DONT_CARE, TRUE, VL_CORNER,0},
        NULL
    },
    {
        94,VL_CORNER_SEL, VL_TRANSFORM,    //index, srcState, dstState
        {BT_PRESS, VL_TRANSFORM, 0,0,0},    // event, volume
        {GR_DONT_CARE, HL_HIT, TRUE, VL_TRANSFORM,0},
        NULL
    },
    {
        95,VL_CORNER_SEL, VL_TRANSFORM,    //index, srcState, dstState
        {BT_RELEASE, VL_TRANSFORM, 0,0,0},    // event, volume
        {GR_DONT_CARE, HL_HIT, TRUE, VL_TRANSFORM,0},
        NULL
    },

    {
        96,VL_CORNER_SEL, VL_CORNER_SEL,    //index, srcState, dstState
        {BT_MOVE, VL_CLIENT, 0,0,0},    // event, volume
        {GR_DONT_CARE, HL_DONT_CARE, TRUE, VL_CORNER,0},
        NULL
    },
    {
        97,VL_CORNER_SEL, VL_CLIENT,    //index, srcState, dstState
        {BT_PRESS, VL_CLIENT, 0,0,0},    // event, volume
        {GR_RELEASE, HL_HIT, FALSE, VL_CLIENT,0},
        NULL
    },
    {
        98,VL_CORNER_SEL, VL_CLIENT,    //index, srcState, dstState
        {BT_RELEASE, VL_CLIENT, 0,0,0},    // event, volume
        {GR_RELEASE, HL_HIT, FALSE, VL_CLIENT,0},
        NULL
    },

    {
        99,VL_CORNER_SEL, VL_CORNER_SEL,    //index, srcState, dstState
        {BT_MOVE, VL_BORDER, 0,0,0},    // event, volume
        {GR_DONT_CARE, HL_DONT_CARE, TRUE, VL_CORNER,0},
        NULL
    },
    {
       100,VL_CORNER_SEL, VL_BORDER_SEL,    //index, srcState, dstState
        {BT_PRESS, VL_BORDER, 0,0,0},    // event, volume
        {GR_DONT_CARE, HL_DRAG, TRUE, VL_BORDER,0},
        NULL
    },
    {
       101,VL_CORNER_SEL, VL_BORDER,    //index, srcState, dstState
        {BT_RELEASE, VL_BORDER, 0,0,0},    // event, volume
        {GR_DONT_CARE, HL_HIT, TRUE, VL_BORDER,0},
        NULL
    },

    {
       102,VL_CORNER_SEL, VL_CORNER_SEL,    //index, srcState, dstState
        {BT_MOVE, VL_CORNER, 0,0,0},    // event, volume
        {GR_DONT_CARE, HL_DONT_CARE, TRUE, VL_CORNER,0},
        NULL
    },
    {
       103,VL_CORNER_SEL, VL_CORNER_SEL,    //index, srcState, dstState
        {BT_PRESS, VL_CORNER, 0,0,0},    // event, volume
        {GR_DONT_CARE, HL_DONT_CARE, TRUE, VL_CORNER,0},
        NULL
    },
    {
       104,VL_CORNER_SEL, VL_CORNER,    //index, srcState, dstState
        {BT_RELEASE, VL_CORNER, 0,0,0},    // event, volume
        {GR_DONT_CARE, HL_HIT, TRUE, VL_CORNER,0},
        NULL
    }
};


// #define __SO_WIN_KIT_DBG__
#ifdef __SO_WIN_KIT_DBG__
#define _PF_  printf("   ");printf
#define _PFN_ printf("SoWindowKit[@%x]",this);printf
#define _PFS_ printf("SoWindowKit[@%x]",((SoWindowKit *)data));printf
#endif

SO_KIT_SOURCE(SoWindowKit);
BASE3D_SOURCE(SoWindowKit);

#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoCallbackAction.h>

#include <Inventor/elements/SoMaterialBindingElement.h>
#include <Inventor/elements/SoLazyElement.h>

#include <stbapi/context/StudierstubeContext.h>
#include <stbapi/context/SoContextElement.h>

 /* ----------------------------------------------------------------------- */
/** process color change for individual user colors
  * @param data - user data for callback (windowkit)
  * @param action - action that triggered callback */
 /* ----------------------------------------------------------------------- */
void SoWindowKit::userColorChange(void *data, SoAction *action)
{
#ifdef __SO_WIN_KIT_DBG__
    _PFS_("::userColorChange\n");
#endif
    // return;
    SoWindowKit *wk = (SoWindowKit *)data;

      // don't change highlight of border and corner volumes
      // if any user ins in the xf-volumes
    if (VL_TRANSFORM <= wk->careState) { return; }

      // change color during rendering if neccessary
    if (action->isOfType(SoGLRenderAction::getClassTypeId()))
    {   
        SbBool oldnotify = wk->enableNotify(FALSE);
        SoState *state = action->getState();
        // try to get a DIV object from the context in the action state.
        // as windows are only used below contexts, this should always work.
        // it avoids the overhead of having several search actions per frame
        CDivMain * theDivObject = NULL;        
        int32_t appId = atoi(SoContextElement::get(state, StudierstubeContext::Application ).getString());
        SoDivGroup * group = SoContextManagerKit::getInstance()->getAppGroup( appId );

        if( group != NULL )
        {
            theDivObject = group->getDiv();
        }

        if(theDivObject)
        {
		  //#ifdef __SO_WIN_KIT_DBG__
		  // _PFS_(" disabled node %s with %p\n",wk->borderGeometry.getValue()->getName().getString(),theDivObject);            
		  //#endif
            theDivObject->disableNode(wk->borderGeometry.getValue());
			theDivObject->disableNode(wk->frontDrawStyle.getValue());
			theDivObject->disableNode(wk->backDrawStyle .getValue());
        }
        else
        {
		  //#ifdef __SO_WIN_KIT_DBG__
		  // _PFS_(" no divobj for %i\n", appId );
		  //#endif
        }
        // int index = StudierstubeContext::User;
        
        if (SoContextElement::isSet(state,StudierstubeContext::User))
        {
            int user = atoi(SoContextElement::get(state,StudierstubeContext::User).getString());
            SoUserManagerKit *uman;
            SoUserKit   *uk;

#ifdef __SO_WIN_KIT_DBG__
			_PFS_("Pass for user %i with action %x\n", user, action);
#endif
            uman = SoUserManagerKit::getInstance();
            if (-1 != user)
            {
                // set focused appearance
                uk = uman->getUserKit(user);
                if (uk->getFocus() == wk)
                {
                    wk->setFrameColor(user);
                    wk->setAppearance(true);
                }
                else
                {
                    wk->setFrameColor(wk->focus[wk->focus.getNum()-1]);
                    wk->setAppearance(false);
                }                           
            }
        }    
        if(theDivObject)
        {
			theDivObject->enableNode(wk->backDrawStyle .getValue());            
			theDivObject->enableNode(wk->frontDrawStyle.getValue());
			theDivObject->enableNode(wk->borderGeometry.getValue());            
        }
        wk->enableNotify(oldnotify);
    }
}


 /* ----------------------------------------------------------------------- */
/** constructor - initialize node
  * set up callbacks and sensors                                            */
 /* ----------------------------------------------------------------------- */
SoWindowKit::SoWindowKit() :
    careState(VL_OUTSIDE),
    bordersOff(true),
    grabberSet(0),
    eventAutomaton(7,105,2,4,transitions,VL_OUTSIDE),
    applicationID(-1)
{
    SO_KIT_CONSTRUCTOR(SoWindowKit);

	//--build up scene graph hierarchy
    // a dedicated callback node for user id tests
	SO_KIT_ADD_CATALOG_ENTRY(renderCallback, SoCallback, FALSE, this, \0, TRUE );
	
    SO_KIT_ADD_CATALOG_ENTRY(root          ,SoSeparator ,TRUE ,this ,\0,TRUE);

    SO_KIT_ADD_CATALOG_ENTRY(transform     ,SoTransform ,TRUE ,root ,\0,TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(visibleSwitch ,SoSwitch    ,TRUE ,root ,\0,TRUE);

        // border components
    SO_KIT_ADD_CATALOG_ENTRY(
        borderSwitch   ,SoSwitch                 ,TRUE,visibleSwitch,\0,TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(
        borderGeometry ,SoIndexedTriangleStripSet,TRUE,borderSwitch ,\0,TRUE);

        // frame components
    SO_KIT_ADD_CATALOG_ENTRY(frameSwitch    ,SoSwitch        ,TRUE ,visibleSwitch ,\0,TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(frameSeparator ,SoSeparator     ,TRUE ,frameSwitch   ,\0,TRUE);

    SO_KIT_ADD_CATALOG_ENTRY(frameNoLight   ,SoLightModel    ,TRUE ,frameSeparator,\0,TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(frameShapeHints,SoShapeHints    ,TRUE ,frameSeparator,\0,TRUE);

    SO_KIT_ADD_CATALOG_ENTRY(frameFrontSw   ,SoSwitch        ,TRUE ,frameSeparator,\0,TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(frontDrawStyle ,SoDrawStyle     ,TRUE ,frameFrontSw  ,\0,TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(frontGeometry  ,SoIndexedFaceSet,TRUE ,frameFrontSw  ,\0,TRUE);

    SO_KIT_ADD_CATALOG_ENTRY(frameBackSw    ,SoSwitch        ,TRUE ,frameSeparator,\0,TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(backDrawStyle  ,SoDrawStyle     ,TRUE ,frameBackSw   ,\0,TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(backGeometry   ,SoIndexedFaceSet,TRUE ,frameBackSw   ,\0,TRUE);

        //clipping plane components
    SO_KIT_ADD_CATALOG_ENTRY(clipSwitch ,SoSwitch,TRUE,visibleSwitch,\0,TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(clipPlaneX0,SoClipPlane,TRUE,clipSwitch,\0,TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(clipPlaneX1,SoClipPlane,TRUE,clipSwitch,\0,TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(clipPlaneY0,SoClipPlane,TRUE,clipSwitch,\0,TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(clipPlaneY1,SoClipPlane,TRUE,clipSwitch,\0,TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(clipPlaneZ0,SoClipPlane,TRUE,clipSwitch,\0,TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(clipPlaneZ1,SoClipPlane,TRUE,clipSwitch,\0,TRUE);

        // client area components
    SO_KIT_ADD_CATALOG_ENTRY(clientSwitch,SoSwitch     ,TRUE,visibleSwitch,\0,TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(clientCenter,SoTranslation,TRUE,clientSwitch ,\0,TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(clientVolume,SoSeparator  ,TRUE,clientSwitch ,\0,TRUE);

//--register enums
    SO_NODE_DEFINE_ENUM_VALUE(State,NORMAL);
    SO_NODE_DEFINE_ENUM_VALUE(State,MINIMIZED);
    SO_NODE_DEFINE_ENUM_VALUE(State,MAXIMIZED);

    SO_NODE_SET_SF_ENUM_TYPE(state,State);

    SO_NODE_DEFINE_ENUM_VALUE(Style,BORDER);
    SO_NODE_DEFINE_ENUM_VALUE(Style,CLIENTVOLUMEVISIBLE);
    SO_NODE_DEFINE_ENUM_VALUE(Style,TITLE);
    SO_NODE_DEFINE_ENUM_VALUE(Style,ROTATEABLE);
    SO_NODE_DEFINE_ENUM_VALUE(Style,TRANSLATEABLE);
    SO_NODE_DEFINE_ENUM_VALUE(Style,MOVEABLE);
    SO_NODE_DEFINE_ENUM_VALUE(Style,RESIZEABLE);
    SO_NODE_DEFINE_ENUM_VALUE(Style,CLIPPING);
    SO_NODE_DEFINE_ENUM_VALUE(Style,VISIBLE);
    SO_NODE_DEFINE_ENUM_VALUE(Style,DEFAULT);

    SO_NODE_SET_SF_ENUM_TYPE(style,Style);

    SO_NODE_DEFINE_ENUM_VALUE(FocusPolicy,CLICK_TO_FOCUS);
    SO_NODE_DEFINE_ENUM_VALUE(FocusPolicy,MOVE_TO_FOCUS);

    SO_NODE_SET_SF_ENUM_TYPE(focusPolicy,FocusPolicy);

    SO_NODE_DEFINE_ENUM_VALUE(AppearanceDetail,DEFAULT_WALL);
    SO_NODE_DEFINE_ENUM_VALUE(AppearanceDetail,BACK_WALL_SOLID_FOCUSED);
    SO_NODE_DEFINE_ENUM_VALUE(AppearanceDetail,BACK_WALL_SOLID_UNFOCUSED);
    SO_NODE_DEFINE_ENUM_VALUE(AppearanceDetail,BACK_WALL_SOLID);
    SO_NODE_DEFINE_ENUM_VALUE(AppearanceDetail,FRONT_WALL_FOCUSED);
    SO_NODE_DEFINE_ENUM_VALUE(AppearanceDetail,FRONT_WALL_UNFOCUSED);
    SO_NODE_DEFINE_ENUM_VALUE(AppearanceDetail,FRONT_WALL);

    SO_NODE_SET_SF_ENUM_TYPE(appearance,AppearanceDetail);

//--set default field values
    SO_NODE_ADD_FIELD(position   ,(0,0,0));
    SO_NODE_ADD_FIELD(orientation,(SbRotation::identity()));
    SO_NODE_ADD_FIELD(center     ,(0.5,0.5,0.5));
    SO_NODE_ADD_FIELD(thickness  ,(0.025));
    SO_NODE_ADD_FIELD(size       ,(1,1,1));
    SO_NODE_ADD_FIELD(minSize    ,(0,0,0));
    SO_NODE_ADD_FIELD(state      ,(NORMAL));
    SO_NODE_ADD_FIELD(style      ,(DEFAULT));
    SO_NODE_ADD_FIELD(title      ,(""));
    SO_NODE_ADD_FIELD(focusPolicy,(CLICK_TO_FOCUS));
    SO_NODE_ADD_FIELD(appearance ,(BACK_WALL_SOLID | FRONT_WALL));

    SO_NODE_ADD_FIELD(focus      ,(-1));

    SO_KIT_INIT_INSTANCE();
    BASE3D_CONSTRUCTOR;
	
//--set kit's scene graph properties
    setPart("root",new SoSeparator);
    setPart("transform",new SoTransform);

    setPart("visibleSwitch",new SoSwitch);

    setPart("borderSwitch"   ,new SoSwitch);
    setPart("frameSwitch"    ,new SoSwitch);
    setPart("frameSeparator" ,new SoSeparator);
    setPart("frameNoLight"   ,new SoLightModel);
    setPart("frameShapeHints",new SoShapeHints);
    setPart("frameFrontSw"   ,new SoSwitch);
    setPart("frontDrawStyle" ,new SoDrawStyle);
    setPart("frameBackSw"    ,new SoSwitch);
    setPart("backDrawStyle"  ,new SoDrawStyle);

    setPart("clipSwitch",new SoSwitch);
    set("clipPlaneX0","plane  1  0  0 -1 on TRUE");
    set("clipPlaneX1","plane -1  0  0 -1 on TRUE");
    set("clipPlaneY0","plane  0  1  0 -1 on TRUE");
    set("clipPlaneY1","plane  0 -1  0 -1 on TRUE");
    set("clipPlaneZ0","plane  0  0  1 -1 on TRUE");
    set("clipPlaneZ1","plane  0  0 -1 -1 on TRUE");

    set("clientCenter","translation 0 0 0");

        // (de-)active switches
    SO_GET_ANY_PART(this,"visibleSwitch",SoSwitch)->whichChild=SO_SWITCH_ALL;
    SO_GET_ANY_PART(this,"borderSwitch" ,SoSwitch)->whichChild=SO_SWITCH_NONE;
    SO_GET_ANY_PART(this,"frameSwitch"  ,SoSwitch)->whichChild=SO_SWITCH_ALL;
    SO_GET_ANY_PART(this,"frameFrontSw" ,SoSwitch)->whichChild=SO_SWITCH_ALL;
    SO_GET_ANY_PART(this,"frameBackSw"  ,SoSwitch)->whichChild=SO_SWITCH_ALL;
    SO_GET_ANY_PART(this,"clipSwitch"   ,SoSwitch)->whichChild=SO_SWITCH_ALL;
    SO_GET_ANY_PART(this,"clientSwitch" ,SoSwitch)->whichChild=SO_SWITCH_ALL;

    // read default geometry
    SoNode *result;
    SoInput in;

    SoVertexProperty *property;
    in.setBuffer(vertexProperty, strlen(vertexProperty));
    SoDB::read(&in, result);
    property = (SoVertexProperty *)result;

    SoIndexedTriangleStripSet *borderShape;
    in.setBuffer(triangleSet, strlen(triangleSet));
    SoDB::read(&in, result);
    borderShape = (SoIndexedTriangleStripSet *)result;
    borderShape->vertexProperty = property;
    setPart("borderGeometry", borderShape);

    SoIndexedFaceSet *frameShape;
    in.setBuffer(frontfaceSet, strlen(frontfaceSet));
    SoDB::read(&in, result);
    frameShape = (SoIndexedFaceSet *)result;
    frameShape->vertexProperty = property;
    setPart("frontGeometry"   , frameShape);

    in.setBuffer(backfaceSet, strlen(backfaceSet));
    SoDB::read(&in, result);
    frameShape = (SoIndexedFaceSet *)result;
    frameShape->vertexProperty = property;
    setPart("backGeometry", frameShape);

        // set frame light
    set("frameNoLight","model BASE_COLOR");

       // set frame shape hints (back walls)
    set("frameShapeHints","vertexOrdering COUNTERCLOCKWISE");
    set("frameShapeHints","shapeType SOLID");
    set("frameShapeHints","faceType CONVEX");

        // set ff draw style
    set("frontDrawStyle","style LINES");

        // set frame draw style
        // this value changes with different appearance details
    set("backDrawStyle","style FILLED");
        // callback node determines render color of user

    setColor(COL_INACTIVE,COL_INACTIVE,COL_INACTIVE);

//--set callbacks and sensors
    positionSensor    = new SoFieldSensor(positionCB   ,(void *)this);
    orientationSensor = new SoFieldSensor(orientationCB,(void *)this);
    centerSensor      = new SoFieldSensor(centerCB     ,(void *)this);
    thicknessSensor   = new SoFieldSensor(sizeCB       ,(void *)this);
    sizeSensor        = new SoFieldSensor(sizeCB       ,(void *)this);
    stateSensor       = new SoFieldSensor(stateCB      ,(void *)this);
    styleSensor       = new SoFieldSensor(styleCB      ,(void *)this);
    focusSensor       = new SoFieldSensor(focusCB      ,(void *)this);

    //size.setValue(1,1,1);
    //position.setValue(0,0,0);
    //center.setValue(0.5,0.5,0.5);
	
    setUpConnections( TRUE, TRUE );
}

SbBool SoWindowKit::setUpConnections(SbBool onOff, SbBool doItAlways)
{
    if (!doItAlways && connectionsSetUp == onOff)
        return onOff;
    if (onOff) {
        // We connect AFTER base class.
        SoBaseKit::setUpConnections(onOff, doItAlways);        

        // set a callback for color changes based on user ids
        SO_GET_PART(this,"renderCallback",SoCallback)->setCallback(userColorChange, (void *)this);

        // setup sensors
        positionSensor   -> attach(&position);
        orientationSensor-> attach(&orientation);
        centerSensor     -> attach(&center);
        thicknessSensor  -> attach(&thickness);
        sizeSensor       -> attach(&size);
        stateSensor      -> attach(&state);
        styleSensor      -> attach(&style);
        focusSensor      -> attach(&focus);

        //--set initial values to force sensor callbacks
		// only possible if clientCenter is not NULL (will be if there is a read error !)
		if (clientCenter.getValue() != NULL) 
		{
			positionCB(this, NULL );
			orientationCB(this, NULL );
			centerCB(this, NULL );
			sizeCB(this, NULL );
			stateCB(this, NULL );
			styleCB(this, NULL );
			focusCB(this, NULL );
		}
    }
    else {
        // We disconnect BEFORE base class.
        // Disconnect the field sensors.
        positionSensor   -> detach();
        orientationSensor-> detach();
        centerSensor     -> detach();
        thicknessSensor  -> detach();
        sizeSensor       -> detach();
        stateSensor      -> detach();
        styleSensor      -> detach();
        focusSensor      -> detach();

        // remove callbacks        
		SO_GET_PART(this,"renderCallback",SoCallback)->setCallback(NULL,NULL);

        SoBaseKit::setUpConnections(onOff, doItAlways);
    }
    return !(connectionsSetUp = onOff);
}

 /* ----------------------------------------------------------------------- */
/** destructor                                                              */
 /* ----------------------------------------------------------------------- */
SoWindowKit::~SoWindowKit()
{
    positionSensor   -> detach();
    orientationSensor-> detach();
    centerSensor     -> detach();
    thicknessSensor  -> detach();
    sizeSensor       -> detach();
    stateSensor      -> detach();
    styleSensor      -> detach();
    focusSensor      -> detach();
	delete positionSensor;
	delete orientationSensor;
	delete centerSensor;
	delete thicknessSensor;
	delete sizeSensor;
	delete stateSensor;
	delete styleSensor;
	delete focusSensor;
}

 /* ----------------------------------------------------------------------- */
/** static class initialization                                             */
 /* ----------------------------------------------------------------------- */
void
SoWindowKit::initClass(void)
{
    SO_KIT_INIT_CLASS(SoWindowKit, SoBaseKit, "BaseKit");
    BASE3D_INIT_CLASS(SoWindowKit);
}

 /* ----------------------------------------------------------------------- */
/** evaluate window's interest in given 3D event
  * @param h3a - event action containing 3d event in question
  * @return true if window is interested                                    */
 /* ----------------------------------------------------------------------- */
SbBool SoWindowKit::isInterested(SoHandle3DEventAction* h3a)
{
#ifdef __SO_WIN_KIT_DBG__
    _PFN_("::isInterested");
#endif
    // do not check bounding box if window is maximize
    // return true immediatly
    // @todo: also check visibility of window in maximized state??
    if (MAXIMIZED == state.getValue()) { return TRUE; }

    So3DEvent *event = (So3DEvent *)h3a->getEvent();
    SbViewportRegion vr;
    SoGetBoundingBoxAction bba(vr);

    SoFullPath *path = (SoFullPath *)h3a->getPathFromRoot();
    path->ref();
    bba.apply(path);

    float x,y,z, t;
    SbVec3f minV, maxV;

    SbBox3f box(bba.getBoundingBox());

    // expand bounding box if all users are outside
    //(!!bbox encloses only client volume if all users are outside, and
    //   border, corner and transform volumes would never be taken into
    //   account!!)
    if (bordersOff)
    {
        // thickness times sqrt of 2
        t = thickness.getValue() * 1.414214f;
        minV = box.getMin();
        minV.getValue(x,y,z);
        minV.setValue(x-t,y-t,z-t);

        maxV = box.getMax();
        maxV.getValue(x,y,z);
        maxV.setValue(x+t,y+t,z+t);

        box.setBounds(minV,maxV);
    }

    //check big bounding box around rotated window
    if (box.intersect(event->getTranslation()))
    {
        //check tight bounding box (window's own local coord. sys.)
        SbXfBox3f xfbox(bba.getXfBoundingBox());
        // expand bounding box by thickness if all users are outside
        //(!!bbox encloses only client volume if all users are outside!!)

        if (bordersOff)
        {
            size.getValue().getValue(x,y,z);
            t = thickness.getValue();
            xfbox.setBounds(-x-t,-y-t,-z-t, x+t, y+t, z+t);
        }

        if (xfbox.intersect(event->getTranslation())) {
            return TRUE;
        }
    }

        // hide borders if we are outside
    if (!bordersOff) { hideBorder(VL_OUTSIDE); }
    careState = VL_OUTSIDE;
    return FALSE;
}

 /* ----------------------------------------------------------------------- */
/** process 3d event
  * @param h3a - event action containing 3d event to be processed           */
 /* ----------------------------------------------------------------------- */
void SoWindowKit::handle3DEvent(SoHandle3DEventAction* h3a)
{
#ifdef __SO_WIN_KIT_DBG__
    _PFN_("::handle3DEvent\n");
#endif
    int input[STB_MAX_INPUT] = {0,0,0,0,0};
    int output[STB_MAX_OUTPUT] = {0,0,0,0,0};
    // compute event code
    So3DEvent *event = (So3DEvent *)h3a->getEvent();
    int evType;

    switch (event->getType())
    {
        case So3DEvent::ET_MOVE_EVENT:
            evType = BT_MOVE;
            break;
        case So3DEvent::ET_BUTTON0_EVENT:
            if (event->getButton(0)) { evType = BT_PRESS; }
            else { evType = BT_RELEASE; }
            break;
        default:
            // default = no event
            evType = -1;
    }

    // check new state
    input[IN_EVENT]  = evType;
    input[IN_VOLUME] = checkNewState(h3a);

        // if window is focused check appearance, grabber and callback
        // store care state to undo action if neccessary
    int resetState = careState;

        // execute automaton transition
    if (AUT_NO_TRANSITION == eventAutomaton.exec(input,output)) { return; }

        // get new state from automaton
    careState = eventAutomaton.getState();

        // switch borders on/off if necessary
    if (CLICK_TO_FOCUS == focusPolicy.getValue() ) { hideBorder(output[OUT_VOLUME]); }

    //@todo
      //user locking
    int user;
    SoUserKit   *uk;
    SoUserManagerKit *uman = SoUserManagerKit::getInstance();
    user = uman->getUserFromStation(event->getStation());
    /*
    if (-1 != user)
    {
        if (focus.getNum() != 0)
        {
              // if an user is forcing an event, but she/he is not
              // the last user who enters the window lock her/him.
            if (focus[focus.getNum()-1] != user)) { return; }
        }
    }
    */ 
    if (isFocused(h3a,input))
    {
            // moved outside window
        if (output[OUT_HIGHLIGHT] == HL_NONE)
        {
            if (-1 != user)
            {
                uk = uman->getUserKit(user);
                if (this == uk->getFocus())
                {
                      // set focus of user to nil
                      // remove her/him from focus user list
                    uk->setFocus(NULL);
                    removeUser(user);
                }
            }
        }
        //if (AUT_NO_TRANSITION == eventAutomaton.exec(input,output))
        //{
		//#ifdef __SO_WIN_KIT_DBG__
        //    _PF_("NO TRANSITION!!!\n");
		//#endif
        //    return;
        //}
            // get new state from automaton
        //careState = eventAutomaton.getState();
            // call application's callback routine
        callCallback(h3a);
            // switch borders on/off if necessary
        if (CLICK_TO_FOCUS != focusPolicy.getValue() ) { hideBorder(output[OUT_VOLUME]); }
            // set/release grabber if necessary
        handleGrabber(h3a,output[OUT_GRABBER]);
            // change highlighting on demand
        changeHighlight(output[OUT_HIGHLIGHT],output[OUT_VOLUME]);
            // handle event input (size, move,..)
        handleEventInput(evType,careState,h3a,event);
            // set 3d event handled
        if (output[OUT_HANDLED]) { h3a->setHandled(); }
    }
    else
    {
            // undo last action if this window will not get focused
        careState = resetState;
    }
}

// @todo callback triggering
/*
SoWindowKit::process(action,event)
{
  if(preCallback != NULL) 
  {
    handled = preCallback(event,window,action);
  }
  if(ignorePreCallbackResult || !handled)
  {
    if(inClientArea(event)) handled = clientArea->handle(event);
    else if(inBorder(event)) handled = handleBorder(event)...
  }
  if(ignoreInternalProcessingResult ||!handled && postCallback != NULL) {
     handled = postCallback(event,window,action);
  }
  //what to do with "handled" value here? nothing?
}
*/

 /* ----------------------------------------------------------------------- */
/** process state change
  * @param h3a - event action containing 3d event to be processed           */
 /* ----------------------------------------------------------------------- */
int SoWindowKit::checkNewState(SoHandle3DEventAction* h3a)
{
    // do not check bounding box if window is maximize
    // return client vlomue hit immediatly
    if (MAXIMIZED == state.getValue()) { return VL_CLIENT; }

    SbViewportRegion vr;
    SoGetBoundingBoxAction bba(vr);

    SoFullPath *path = (SoFullPath *)h3a->getPathFromRoot();
    path->ref();
    bba.apply(path);

    //we are now within the window volume, so we have to check
    // if we are
    //   inside the client volume
    //   inside a border's volume
    //   inside a corner's volume
    // to determine the correct response of handle3DEvent
    float x,y,z, t;
    SbVec3f vec;
    vec = size.getValue() * 0.5f;
    vec.getValue(x,y,z);
    t = thickness.getValue();

    SbXfBox3f xfbox(bba.getXfBoundingBox());

    //set bounds to the client volume and do nothing if we are inside
    vec = ((So3DEvent *)h3a->getEvent())->getTranslation();
    xfbox.setBounds(-x-t,-y-t,-z-t, x+t, y+t, z+t);

    /* 1.step               outside the window
     *  \\\\\\\\\\\\\\\\\\\\\\\\\\
     *   \\\\\\\\\\\\\\\\\\\\\\\\\\
     *    \\\\+--+---------+--+(x+t,y+t)
     *   \\\\\| t|         |  |\\\\
     *   \\\\\+--+---------+--+\\\\
     *  \\\\\\|  |    (x,y)|  |\\\
     *  \\\\\\|  |         |  |\\\
     *  \\\\\\|  |         |  |\\\
     *   \\\\\|  |(-x,-y)  |  |\\\\
     *   \\\\\+--+---------+--+\\\\
     *   \\\\\|  |         |  |\\\\
     *   \\\\\+--+---------+--+\\\\
     *(-x-t,-y-t)\\\\\\\\\\\\\\\\\\\
     */

    // check if we are outside of our window (size x,y,z and thickness t) ..
    if (!xfbox.intersect(vec))
    {
	  //#ifdef __SO_WIN_KIT_DBG__
        //_PF_("cns: %i %i !!outside!!\n",myID,grabberSet);
	  //#endif
        return VL_OUTSIDE;
    }

    // 2.step               inside the client volume
    //        +--+---------+--+(x+t,y+t)
    //        | t|         |  |
    //        +--+---------+--+
    //        |  |\\\\(x,y)|  |
    //        |  |\\\\\\\\\|  |
    //        |  |\\\\\\\\\|  |
    //        |  |(-x,-y)\\|  |
    //        +--+---------+--+
    //        |  |         |  |
    //        +--+---------+--+
    //(-x-t,-y-t)

    // ..no, so check internal volume (client volume size x,y,z)
    xfbox.setBounds(-x,-y,-z, x, y, z);
    if (xfbox.intersect(vec))
    {
	  //#ifdef __SO_WIN_KIT_DBG__
        //_PF_("cns: %i %i client volume hit\n",myID,grabberSet);
	  //#endif
        return VL_CLIENT;
    }

    // if we have made it trough, we can operate again with the internal
    // volume without being scared if the users is inside the client
    // volume or not. at this point we have decided whether (s)he is or not.

    // 3.+ 4.step               inside border or corner volumes
    //        +--+---------+--+(x+t,y+t)    +--+---------+--+(x+t,y+t)
    //        | t|\\\\\\\\\|  |             | t|         |  |
    //        +--+---------+--+             +--+---------+--+
    //        |  |\\\\(x,y)|  |             |\\|\\\\(x,y)|\\|
    //        |  |\\\\\\\\\|  |      +      |\\|\\\\\\\\\|\\|
    //        |  |\\\\\\\\\|  |             |\\|\\\\\\\\\|\\|
    //        |  |(-x,-y)\\|  |             |\\|(-x,-y)\\|\\|
    //        +--+---------+--+             +--+---------+--+
    //        |  |\\\\\\\\\|  |             |  |         |  |
    //        +--+---------+--+             +--+---------+--+
    //(-x-t,-y-t)                   (-x-t,-y-t)
    //
    //
    //set bounds to a cross without corners and borders (accumulate)
    // i.e. check intersection with the union of three cubes elongated along
    // an axis by the value of thickness
    // each of the cubes contains no border and no corner volumes
    SbBool intersectBorder;
    xfbox.setBounds(-x-t,-y  ,-z  , x+t, y  , z  ); //cube elongated in x dir
    intersectBorder  = xfbox.intersect(vec);

    xfbox.setBounds(-x  ,-y-t,-z  , x  , y+t, z  ); //cube elongated in y dir
    intersectBorder |= xfbox.intersect(vec);

    xfbox.setBounds(-x  ,-y  ,-z-t, x  , y  , z+t); //cube elongated in z dir
    intersectBorder |= xfbox.intersect(vec);

    // if we get an intersection we are neither in a border, nor in a corner
    // volume
    if (intersectBorder)
    {
	  //#ifdef __SO_WIN_KIT_DBG__
        //_PF_("cns: %i %i empty side\n",myID,grabberSet);
	  //#endif
        return VL_TRANSFORM;
    }
    else
    {
        //set bounds to a cube without corners (accumulate)
        // i.e. check intersection with the union of three cubes
        // with two components elongated by thickness
        // each of the cubes contains 4 border volumes and no corner volume
        xfbox.setBounds(-x-t,-y-t,-z  , x+t, y+t, z  );
        intersectBorder  = xfbox.intersect(vec);

        xfbox.setBounds(-x  ,-y-t,-z-t, x  , y+t, z+t);
        intersectBorder |= xfbox.intersect(vec);

        xfbox.setBounds(-x-t,-y  ,-z-t, x+t, y  , z+t);
        intersectBorder |= xfbox.intersect(vec);

        // if we get now an intersection we crossed an border volume,
        // if not we hit a corner volume
        // (i.e. we are neither outside the window,
        //       nor in the client volume, nor in the transform volume,
        //       nor in the border volumes)
        if (intersectBorder)
        {
		  //#ifdef __SO_WIN_KIT_DBG__
            //_PF_("cns: %i %i border hit\n",myID,grabberSet);
		  //#endif
            return VL_BORDER;
        }
        else
        {
		  //#ifdef __SO_WIN_KIT_DBG__
            //_PF_("cns: %i %i corner hit\n",myID,grabberSet);
		  //#endif
            return VL_CORNER;
        }
    }
}

 /* ----------------------------------------------------------------------- */
/** set highlight colors
  * @param line   - color of lines
  * @param border - color of borders
  * @param corner - color of corners                                        */
 /* ----------------------------------------------------------------------- */
void SoWindowKit::setColor(unsigned int line, unsigned int border, unsigned int corner)
{
#ifdef __SO_WIN_KIT_DBG__
    _PFN_("change highlight is writing\n");
#endif
    SoVertexProperty *vp = (SoVertexProperty *)
        ((SoIndexedTriangleStripSet *)borderGeometry.getValue())
            ->vertexProperty.getValue();

    //vp->orderedRGBA.set1Value(0,line);
    vp->orderedRGBA.set1Value(1,border);
    vp->orderedRGBA.set1Value(2,corner);
}

 /* ----------------------------------------------------------------------- */
/** callback for position changes
  * @param data - pointer to SoWindowKit instance
  * @param sensor - pointer to positionSensor */
 /* ----------------------------------------------------------------------- */
void SoWindowKit::positionCB(void* data, SoSensor* sensor)
{
#ifdef __SO_WIN_KIT_DBG__
    _PFS_("::positionCB\n");
#endif
    SoWindowKit *wk = (SoWindowKit *)data;
    if(wk->getBase3DMode()==Base3D::BASE3D_SLAVEMODE) { return; }

    SO_GET_ANY_PART(wk,"transform",SoTransform)
        ->translation.setValue(wk->position.getValue());
}

 /* ----------------------------------------------------------------------- */
/** callback for orientation changes
  * @param data - pointer to SoWindowKit instance
  * @param sensor - pointer to orientationSensor */
 /* ----------------------------------------------------------------------- */
void SoWindowKit::orientationCB(void* data, SoSensor* sensor)
{
#ifdef __SO_WIN_KIT_DBG__
    _PFS_("::orientationCB\n");
#endif
    SoWindowKit *wk = (SoWindowKit *)data;
    if(wk->getBase3DMode()==Base3D::BASE3D_SLAVEMODE) { return; }

    SO_GET_ANY_PART(wk,"transform",SoTransform)
        ->rotation.setValue(wk->orientation.getValue());
}

 /* ----------------------------------------------------------------------- */
/** callback for client volume center changes
  * @param data - pointer to SoWindowKit instance
  * @param sensor - pointer to centerSensor */
 /* ----------------------------------------------------------------------- */
void SoWindowKit::centerCB(void* data, SoSensor* sensor)
{
#ifdef __SO_WIN_KIT_DBG__
    _PFS_("SoWindowKit::centerCB\n");
#endif
    static const SbVec3f half(0.5,0.5,0.5);
    SbVec3f center, size;
    SoWindowKit *wk = (SoWindowKit *)data;
    if(wk->getBase3DMode()==Base3D::BASE3D_SLAVEMODE) { return; }

    // set center percent values range (e [0,1]) to a new range e [-1,1]
    // clamp values if necessary
    center = 2.0f * (wk->center.getValue() - half);
    if ( center[0] >  1.0 ) { center[0] =  1.0; }
    if ( center[1] >  1.0 ) { center[1] =  1.0; }
    if ( center[2] >  1.0 ) { center[2] =  1.0; }
    if ( center[0] < -1.0 ) { center[0] = -1.0; }
    if ( center[1] < -1.0 ) { center[1] = -1.0; }
    if ( center[2] < -1.0 ) { center[2] = -1.0; }

    // mulitply size and center componentwise to get the appropriate
    // translation of the client volume center
    size   = wk->size.getValue() * 0.5f;
    center[0] = center[0] * size[0];
    center[1] = center[1] * size[1];
    center[2] = center[2] * size[2];

    // set new center
    ((SoTranslation *)wk->clientCenter.getValue())->
        translation.setValue(center);
}

 /* ----------------------------------------------------------------------- */
/** callback for size and thickness changes
  * @param data - pointer to SoWindowKit instance
  * @param sensor - pointer to sizeSensor or thicknessSensor */
 /* ----------------------------------------------------------------------- */
void SoWindowKit::sizeCB(void* data, SoSensor* sensor)
{
#ifdef __SO_WIN_KIT_DBG__
    _PFS_("::sizeCB\n");
#endif
    SoWindowKit *wk = (SoWindowKit *)data;
    if(wk->getBase3DMode()==Base3D::BASE3D_SLAVEMODE) { return; }

    SoVertexProperty *property =
        (SoVertexProperty *)SO_GET_ANY_PART(wk,"borderGeometry",
            SoIndexedTriangleStripSet)->vertexProperty.getValue();

    int i;
    SbVec3f *vec;

    float x,y,z;
    float dx,dy,dz;

    SbVec3f size;
    size = wk->size.getValue() * 0.5f;
    size.getValue(x,y,z);
    dx = dy = dz = wk->thickness.getValue();       // border thickness

      // set new clipplane distances (normals keep the same)
    SbVec3f norm;
    SoClipPlane *clipPlane;

    #define SET_DISTANCE(clip, dist)                    \
    clipPlane = ((SoClipPlane *)wk->clip.getValue());   \
    norm = clipPlane->plane.getValue().getNormal();     \
    clipPlane->plane.setValue(SbPlane(norm,-dist));

    SET_DISTANCE(clipPlaneX0,x);
    SET_DISTANCE(clipPlaneX1,x);
    SET_DISTANCE(clipPlaneY0,y);
    SET_DISTANCE(clipPlaneY1,y);
    SET_DISTANCE(clipPlaneZ0,z);
    SET_DISTANCE(clipPlaneZ1,z);

    // calculate the geometry with the three values of size
    // divide space into 8 regions front/back, left/right, top/bottom
    // in every region we have to build a cube, which all together form
    // the geometry of the window which will be connected by
    // the frameGeometry (triangle strip set) or lineGeometry (line set).

    // think of a cube at position x,y,z and with width,height,length of
    // dx,dy,dz.

    // case 0: first we set the 8 vertices of the cube
    // case 1: now we mirror its vertices at the yz-plane
    // case 2: now mirror at the xz-plane

    // ... and so on, so we finally get the cubes vertices
    // ranging from 0..63 (8 cube a 8 vertices) in the following regions
    //    top: +y   right: +x   front: +z

    //    top    right front    index  0.. 7
    //    top    left  front    index  8..15
    //    bottom left  front    index 16..23
    //    bottom right front    index 24..31
    //    top    right back     index 32..39
    //    top    left  back     index 40..47
    //    bottom left  back     index 48..55
    //    bottom right back     index 56..63

    vec = property->vertex.startEditing();
      // generate 8 cubes and mirror them accordingly (see switch below)
    for (i = 0; i < 8; i++)
    {
          // mirror cases to fill all 8 regions with tiny cubes
        switch (i)
        {
        default:
        case 0: break;
        case 1: x = -x; dx = -dx; break;
        case 2: y = -y; dy = -dy; break;
        case 3: x = -x; dx = -dx; break;
        case 4: y = -y; z = -z; dy = -dy; dz= -dz; break;
        case 5: x = -x; dx = -dx; break;
        case 6: y = -y; dy = -dy; break;
        case 7: x = -x; dx = -dx; break;
        }
        // coordinates for a simple cube at position x,y,z
        // and with width, height, and length of dx, dy, and dz respectivly

        // keep in mind that vec is a pointer to a vertex array
        // so we change a value and jump right to the next vertex.
        // doing so, we go 8 times to build a cube and repeat this step
        // 8 times again to build all 8 corner cubes of the window
        vec->setValue(x    ,y    ,z   ); vec++; // coordinate index 0
        vec->setValue(x+dx ,y    ,z   ); vec++; // coordinate index 1

        vec->setValue(x+dx ,y+dy ,z   ); vec++; // coordinate index 2
        vec->setValue(x    ,y+dy ,z   ); vec++; // coordinate index 3

        vec->setValue(x    ,y    ,z+dz); vec++; // coordinate index 4
        vec->setValue(x+dx ,y    ,z+dz); vec++; // coordinate index 5

        vec->setValue(x+dx ,y+dy ,z+dz); vec++; // coordinate index 6
        vec->setValue(x    ,y+dy ,z+dz); vec++; // coordinate index 7
    }
    property->vertex.finishEditing();

    centerCB(data,sensor);
}

 /* ----------------------------------------------------------------------- */
/** callback for state changes
  * @param data - pointer to SoWindowKit instance
  * @param sensor - pointer to stateSensor */
 /* ----------------------------------------------------------------------- */
void SoWindowKit::stateCB(void* data, SoSensor* sensor)
{
#ifdef __SO_WIN_KIT_DBG__
    _PFS_("::stateCB\n");
#endif
    SoWindowKit *wk = (SoWindowKit *)data;
    if(wk->getBase3DMode()==Base3D::BASE3D_SLAVEMODE) { return; }

    switch (wk->state.getValue())
    {
        default:
        case NORMAL:     wk->normalize(); break;
        case MINIMIZED:  wk->minimize();  break;
        case MAXIMIZED:  wk->maximize();  break;
    }
}

 /* ----------------------------------------------------------------------- */
/** callback for style changes
  * @param data - pointer to SoWindowKit instance
  * @param sensor - pointer to styleSensor */
 /* ----------------------------------------------------------------------- */
void SoWindowKit::styleCB(void* data, SoSensor* sensor)
{
    SoWindowKit *wk = (SoWindowKit *)data;
#ifdef __SO_WIN_KIT_DBG__
    _PFS_("::styleCB  value:%4x\n",wk->style.getValue());
#endif
    if(wk->getBase3DMode()==Base3D::BASE3D_SLAVEMODE) { return; }

    // suppress visibility change if window is minimized!!
    int state = wk->state.getValue();
    if (MINIMIZED != state)
    {
        if (wk->hasStyle(VISIBLE))
        {
#ifdef __SO_WIN_KIT_DBG__
            _PF_("VISIBLE on\n");
#endif
            wk->setSwitch(wk->visibleSwitch);
        }
        else
        {
#ifdef __SO_WIN_KIT_DBG__
            _PF_("VISIBLE off\n");
#endif
            wk->clearSwitch(wk->visibleSwitch);
        }
    }

    // suppress border and clipping change if window is maximized!!
    if (MAXIMIZED != state)
    {
        if (wk->hasStyle(BORDER))
        {
#ifdef __SO_WIN_KIT_DBG__
            _PF_("BORDER on\n");
#endif
            wk->clearSwitch(wk->borderSwitch);
            wk->setSwitch(wk->frameSwitch);
        }
        else
        {
#ifdef __SO_WIN_KIT_DBG__
            _PF_("BORDER off\n");
#endif
            wk->clearSwitch(wk->borderSwitch);
            wk->clearSwitch(wk->frameSwitch);
        }

        if (wk->hasStyle(CLIPPING))
        {
#ifdef __SO_WIN_KIT_DBG__
            _PF_("CLIPPING on\n");
#endif
            wk->setSwitch(wk->clipSwitch);
        }
        else
        {
#ifdef __SO_WIN_KIT_DBG__
            _PF_("CLIPPING off\n");
#endif
            wk->clearSwitch(wk->clipSwitch);
        }
    }

    // all following switches can be handled immediatly without
    // care of the window state
    if (wk->hasStyle(CLIENTVOLUMEVISIBLE))
    {
#ifdef __SO_WIN_KIT_DBG__
        _PF_("CLIENTVOMLUMEVISIBLE on\n");
#endif
        wk->setSwitch(wk->clientSwitch);
    }
    else
    {
#ifdef __SO_WIN_KIT_DBG__
        _PF_("CLIENTVOMLUMEVISIBLE off\n");
#endif
        wk->clearSwitch(wk->clientSwitch);
    }

    if (wk->hasStyle(TITLE))
    {
#ifdef __SO_WIN_KIT_DBG__
        _PF_("TITLE on\n");
#endif
    }
    else
    {
#ifdef __SO_WIN_KIT_DBG__
        _PF_("TITLE off\n");
#endif
    }
}

 /* ----------------------------------------------------------------------- */
/** callback for focus changes
  * @param data - pointer to SoWindowKit instance
  * @param sensor - pointer to focusSensor */
 /* ----------------------------------------------------------------------- */
void SoWindowKit::focusCB(void* data, SoSensor* sensor)
{
#ifdef __SO_WIN_KIT_DBG__
    _PFS_("SoWindowKit::focusCB\n");
#endif
    SoWindowKit *wk = (SoWindowKit *)data;
    if(wk->getBase3DMode()==Base3D::BASE3D_SLAVEMODE) { return; }
}

 /* ----------------------------------------------------------------------- */
/** setAppearance appearance changes
  * @param data - pointer to SoWindowKit instance
  * @param sensor - pointer to appearanceSensor */
 /* ----------------------------------------------------------------------- */
void SoWindowKit::setAppearance(bool focused)
{
#ifdef __SO_WIN_KIT_DBG__
    _PFN_("SoWindowKit::setAppearance to %i\n",(int)focused);
#endif
    if (getBase3DMode()==Base3D::BASE3D_SLAVEMODE) { return; }

    SoDrawStyle *frontDS = (SoDrawStyle *)frontDrawStyle.getValue();
    SoDrawStyle *backDS  = (SoDrawStyle *)backDrawStyle .getValue();

    if (focused)
    {
        if(appearance.getValue() & BACK_WALL_SOLID_FOCUSED)
            { backDS->style = SoDrawStyle::FILLED; }
        else
            { backDS->style = SoDrawStyle::LINES; }

        if(appearance.getValue() & FRONT_WALL_FOCUSED)
            { frontDS->style = SoDrawStyle::LINES; }
        else
            { frontDS->style = SoDrawStyle::INVISIBLE; }
    }
    else
    {
        if(appearance.getValue() & BACK_WALL_SOLID_UNFOCUSED)
            { backDS->style = SoDrawStyle::FILLED; }
        else
            { backDS->style = SoDrawStyle::LINES; }

        if(appearance.getValue() & FRONT_WALL_UNFOCUSED)
            { frontDS->style = SoDrawStyle::LINES; }
        else
            { frontDS->style = SoDrawStyle::INVISIBLE; }
    }
}

 /* ----------------------------------------------------------------------- */
/** isFocused change focus of window, if state has changed and no other
  * window is in focus
  * (this routine also handles focus policy -
  *  like click to focus, or move to focus)
  * @param h3a      - action
  * @param input    - input state vector (may be changed)
  * @return true    - if window is focused */
 /* ----------------------------------------------------------------------- */
bool SoWindowKit::isFocused(SoHandle3DEventAction* h3a, int *input)
{
    int user;
    SoWindowKit *focused;   // focused window of user
    So3DEvent   *event = (So3DEvent *)h3a->getEvent();
    SoUserManagerKit *uman;
    SoUserKit   *uk = NULL;

    uman = SoUserManagerKit::getInstance();
    user = uman->getUserFromStation(event->getStation());
    if (-1 != user)
    {
        uk = uman->getUserKit(user);
        focused = uk->getFocus();
    }
    else 
	{ 
		focused = NULL; 
		return FALSE;
	}
    //printf("this: %i %x, focus: %i %x\n",myID,this, focused ? focused->myID : -1 ,focused);

#ifdef __SO_WIN_KIT_DBG__
    _PFN_("isFocused this %x  focus %x\n",this, focused);
#endif
    if (this == focused)
    {
            // nothing to do if window is focused and application id is set
        if (-1 != applicationID) { return true; } // succeed with event
        // else set application id ...
    }
    else
    {
            // if focused window exists, and foucsed window is not this
            // one and we didn't hit the transform volume of this window
            // check focused window's event interests (pen intersection)
        if (focused)
        {
            switch (focusPolicy.getValue() )
            {
                // behaviour of MOVE_TO_FOCUS only
            case MOVE_TO_FOCUS:
                    // never get focus if focused window is active
                if ((VL_OUTSIDE != focused->careState       ) &&
                    (MAXIMIZED  != focused->state.getValue()))
                { return false; }   // don't give focus to this window
                break;
                // behaviour of CLICK_TO_FOCUS only
            default:
            case CLICK_TO_FOCUS:
                    // get focus if focused window is maximized
                if (MAXIMIZED   != focused->state.getValue())
                {
                    // get focus:
                    // true-logic explanation of
                    // negated false logic underneath
                        // client volume clicked and
                        // pen outside focused window
                    //if (PRESS == event) &&
                    //   (CLIENT == volume) &&
                    //   (OUTSIDE == fcareState)
                    //{ do focus stuff; return true; }

                        // any transform volume clicked
                    //if (PRESS == event) && (TRANSFORM <= volume)
                    //{ do focus stuff; return true; }

                    if ( (
                           (BT_PRESS   != input[IN_EVENT]   ) ||
                           (VL_CLIENT  != input[IN_VOLUME]  ) ||
                           (VL_OUTSIDE != focused->careState)
                         ) && (
                           (BT_PRESS     != input[IN_EVENT] ) || 
                           (VL_TRANSFORM >  input[IN_VOLUME])
                       ) ) { return false; }
                    // if passed do focus stuff, return true
                }
                break;
            }
                // remove user from user list in old focused window
                // and add it to the new focused window
            focused->removeUser(user);
            this->addUser(user);
        }
        else
        {
            if (focusPolicy.getValue()==CLICK_TO_FOCUS)
            {
                if ( (
                       (BT_PRESS   != input[IN_EVENT]   ) ||
                       (VL_CLIENT  != input[IN_VOLUME]  )
                     ) && (
                       (BT_PRESS     != input[IN_EVENT] ) || 
                       (VL_TRANSFORM >  input[IN_VOLUME])
                   ) ) { return false; }
            }
            this->addUser(user);
        }
    }
        // change focus to this window, if the old focused one
        // is not longer interested (pen isn't in any volume of the latter)
        // or the latter is maximized
        // set application id if available
    if (-1 != user)
    {
        if (-1 == applicationID)    // search for application id
        {
            SoContextKit *ctx = SoContextManagerKit::getInstance()->
                getAnyContextFromNode(this);
            if (ctx)
            {
                applicationID = ctx->appID.getValue();
                uk->useResources(applicationID);
                uk->setFocus(this);
#ifdef __SO_WIN_KIT_DBG__
                _PF_("pipsheet changed %i %i\n",applicationID,user);
                _PF_("this %x  focus %x\n",this,focused);
#endif
            }
        }
        else
        {
            uk->useResources(applicationID);
            uk->setFocus(this);
#ifdef __SO_WIN_KIT_DBG__
            _PF_("pipsheet changed %i %i\n",applicationID,user);
            _PF_("this %x  focus %x\n",this,focused);
#endif
        }
    }
    else
    {
        focus.setNum(0);
#ifdef __SO_WIN_KIT_DBG__
        _PF_("no user available\n");
        _PF_("this %x  focus %x\n",this,focused);
#endif
    }
        // in the case of overlapping windows the state machine will
        // take a transition from clientvolume to clientvolume, which
        // will not result in a highlight change, so we have to reset
        // the input volume to outside to simulate a reentering into the
        // window.
    if (this != focused) { input[IN_VOLUME] = VL_OUTSIDE; }
    return true;
}

 /* ----------------------------------------------------------------------- */
/** callCallback call application callback if pen is in client area
  * @param h3a - current event action */
 /* ----------------------------------------------------------------------- */
void SoWindowKit::callCallback(SoHandle3DEventAction *h3a)
{
    if ((VL_CLIENT == careState) && (callback))
    {
        int user;
        SoPath *path;
        So3DEvent *event = (So3DEvent *)h3a->getEvent();

        path = (SoFullPath *)h3a->getPathFromRoot();
        path->ref();
        user = SoUserManagerKit::getInstance()->
            getUserFromStation(event->getStation());

        (*callback)(userData, WM_EVENT, this, user, event, path);
        // @todo simplify application callback routine
    }
//typedef SbBool WindowCB(int, SoWindowKit *, So3DEvent *, SoPath *);

//typedef SbBool WindowEventCB(void *, int, SoWindowKit *, int userID, 
//                               So3DEvent *, SoPath *);
}

 /* ----------------------------------------------------------------------- */
/** hideBorder hide borders if pen is outside window, or in client volume
  * @param volume - volume pen is in may switch borders off */
 /* ----------------------------------------------------------------------- */
void SoWindowKit::hideBorder(int volume)
{
        // don't care about borders if window is maximized
    if (MAXIMIZED == state.getValue()) { return; }

    bordersOff = (VL_OUTSIDE == volume) || (VL_CLIENT == volume) ||
                 (0 == style.getValue() & BORDER);
    if (hasStyle(BORDER))
    {
        if (bordersOff)
        {
            if (getSwitchValue(borderSwitch) != SO_SWITCH_NONE)
            {
#ifdef __SO_WIN_KIT_DBG__
                _PF_("hidee border is writing (hide borders)\n");
#endif
                clearSwitch(borderSwitch);
                setSwitch(frameSwitch);
            }
        }
        else
        {
            if (getSwitchValue(borderSwitch) != SO_SWITCH_ALL)
            {
#ifdef __SO_WIN_KIT_DBG__
                _PF_("hide border is writing (show borders)\n");
#endif
                setSwitch(borderSwitch);
                clearSwitch(frameSwitch);
            }
        }
    }
}


 /* ----------------------------------------------------------------------- */
/** handleGrabber handle grabber set/release events
  * @param h3a   - event to handle grabber state
  * @param state - grabber's new state */
 /* ----------------------------------------------------------------------- */
void SoWindowKit::handleGrabber(SoHandle3DEventAction* h3a, int state)
{
    switch (state)
    {
    case GR_SET:
        if (0 == grabberSet)
        {
            h3a->setGrabber(this);
            grabberSet = 1;         // @todo delete this line
        }
        else
        {
#ifdef __SO_WIN_KIT_DBG__
            _PFN_(" grabber set twice!!\n");
#endif
        }
        break;
    case GR_RELEASE:
        if (1 == grabberSet)
        {
            if (this == h3a->getGrabber())
            {
                h3a->releaseGrabber();
                grabberSet = 0;         // @todo delete this line
            }
        }
        else
        {
#ifdef __SO_WIN_KIT_DBG__
            _PFN_(" grabber released twice!!\n");
#endif
        }
        break;
    case GR_DONT_CARE:
    default:
        break;
    }
}

 /* ----------------------------------------------------------------------- */
/** changeHighlight highlights the specified volume
  * @param highlight - kind of highlighting
  * @param volume    - volume to highlight */
 /* ----------------------------------------------------------------------- */
void SoWindowKit::changeHighlight(int highlight, int volume)
{
    switch (highlight)
    {
            // none of the volumes are active
    case HL_NONE:
        setColor(COL_INACTIVE,COL_INACTIVE,COL_INACTIVE);
        break;
            // a volume is active (pen inside)
    case HL_HIT:
        switch (volume)
        {
        case VL_TRANSFORM:
            setColor(COL_INACTIVE,COL_ACTIVE,COL_ACTIVE);
            break;
        case VL_OUTSIDE:   //< part of lazy focus policy (keep clientvol active)
        case VL_CLIENT:
            setColor(COL_ACTIVE,COL_INACTIVE,COL_INACTIVE);
            break;
        case VL_BORDER:
            setColor(COL_INACTIVE,COL_HIT,COL_ACTIVE);
            break;
        case VL_CORNER:
            setColor(COL_INACTIVE,COL_ACTIVE,COL_HIT);
            break;
        }
        break;
            // a volume is selected (pen in border or corner vol.)
        case HL_DRAG:
        switch (volume)
        {
        case VL_BORDER:
            setColor(COL_INACTIVE,COL_SELECT,COL_ACTIVE);
            break;
        case VL_CORNER:
            setColor(COL_INACTIVE,COL_ACTIVE,COL_SELECT);
            break;
        }
        break;
        break;
    case HL_DONT_CARE:
    default:
        break;
    }
}


 /* ----------------------------------------------------------------------- */
/** handle event input (pen's movement and buttons)
  * @param type   - type of event (press, release, motion)
  * @param volume - current state of pen's position relative to window
  *                 (indside, outside, border,..)
  * @param action - 3D action
  * @param event  - 3D event                                                */
 /* ----------------------------------------------------------------------- */
void SoWindowKit::handleEventInput(int type, int volume,
                            SoHandle3DEventAction *action, So3DEvent *event)
{
  //#ifdef __SO_WIN_KIT_DBG__
    //_PFN_("::handleEventInput\n");
  //#endif
    if ((BT_PRESS == type) && (VL_BORDER <= volume))
    {
        startSize            = size.getValue();
        startPosition        = position.getValue();
        startOrientation     = orientation.getValue();
        startTranslationPenG = event->getTranslation();
        startRotationPenG    = event->getRotation();

        SbVec3f startTransL;
        SbViewportRegion vp;
        SoGetMatrixAction ma(vp);

        SoFullPath *path = (SoFullPath *)action->getPathFromRoot();
        path->ref();

        ma.apply(path);
        globalToLocal = ma.getInverse();

        path->unref();

        globalToLocal.multVecMatrix(startTranslationPenG,startTransL);

        pickOffset = startPosition-startTransL;
    }
    else if ((BT_RELEASE == type) && (VL_BORDER <= volume))
    {
        update(event->getTranslation(), event->getRotation());
    }
    else if (((VL_BORDER_SEL == volume) || (VL_CORNER_SEL == volume)) &&
             (BT_MOVE == type))
    {
        update(event->getTranslation(), event->getRotation());
    }
}

//-----------------------------------------------------------------------------
// @see SoDragKit function taken from SoDragKit::update
 /* ----------------------------------------------------------------------- */
/** update translation and rotation (pen's movement to window's pos and rot)
  * @param srcTranslation - endpoint of translation (world coordinate system)
  * @param srcRotation    - endspin of rotation (world coordinate system) */
 /* ----------------------------------------------------------------------- */
void
SoWindowKit::update(
        const SbVec3f &srcTranslation, const SbRotation &srcRotation)
{
#ifdef __SO_WIN_KIT_DBG__
    _PFN_("::update\n");
#endif
    SbVec3f    dstPickoffset(startPosition);
    SbVec3f    relTransG, tmp;
    SbVec3f    dstTranslation;
    SbRotation dstRotation;
    SbRotation relRotG,   relRotL;

    relTransG  = srcTranslation - startTranslationPenG;
    relRotG    = startRotationPenG.inverse() * srcRotation;

    SbMatrix   relRotGMatrix;
    SbMatrix   theMatrix;
    SbVec3f    t, s;
    SbRotation so;

    relRotG.getValue(relRotGMatrix);
    theMatrix = globalToLocal.inverse() * relRotGMatrix * globalToLocal;
    theMatrix.getTransform(t,relRotL,s,so);
    relRotL.multVec(pickOffset,tmp);

    tmp -= pickOffset;
    globalToLocal.multDirMatrix(relTransG,dstTranslation);
        // if careState is in corner or corner select mode (pen in corner vol)
    if (VL_CORNER <= careState)
    {
        // dstTranslation represent the size of window, so we have
        // to perform a rotation around window's orientation
        startOrientation.inverse().multVec(dstTranslation,tmp);
        dstTranslation = tmp;
    }
    else
    {
        // dstTranslation represent the new relative position of window
        // so add starting position and pickup offset position to get
        // the absolute position of the window
        dstPickoffset  += tmp;
        dstTranslation += dstPickoffset;
    }

    dstRotation = startOrientation * relRotL;

    writeBack(dstTranslation,dstRotation,dstPickoffset);
}

//-----------------------------------------------------------------------------
// @see SoDragKit function taken from SoDragKit::writeBack
 /* ----------------------------------------------------------------------- */
/** write back new position and orientation
  * @param translation - new position vector
  * @param rotation    - new rotation vector
  * @param pickoff     - new pickOffset vector */
 /* ----------------------------------------------------------------------- */
void
SoWindowKit::writeBack(
    const SbVec3f &trans, const SbRotation &rot, const SbVec3f &pickoff)
{
#ifdef __SO_WIN_KIT_DBG__
    _PFN_("::writeBack\n");
#endif

    int i;
      // new size, corner picked, half of translation, minimum window size
    SbVec3f resize, pick, half, min;

    switch (careState)
    {
    case VL_BORDER:
    case VL_BORDER_SEL:
        if (hasStyle(TRANSLATEABLE)) { position = trans; }
        else { position = pickoff; }
        if (hasStyle(ROTATEABLE)   ) { orientation = rot; }
        break;
    case VL_CORNER:
    case VL_CORNER_SEL:
        if (hasStyle(RESIZEABLE))
        {
              // get minimum size of window,
              // half translation of pen,
              // and coner through sign of pickOffset (locale coordinates)
            min  = minSize.getValue();
            half = trans;
            startOrientation.inverse().multVec(pickOffset,pick);

              // compute new values for window's size and position
              // all computation are done in window/locale coordinates
              // (window axis aligned).
            for (i = 0; i < 3; i++)
            {
                  // compute new size of window
                  // set resize to the size when a corner was picked
                  // minus half the translation of pen * sign(pick).
                resize[i] = startSize[i];
                if (pick[i] <0.f) { resize[i] += half[i]; }
                             else { resize[i] -= half[i]; }
                  // clamp resize and half of translation if window size will
                  // get smaller than the defined minimum size.
                  // the translation affects the position of the window, so
                  // we have to care to clamp 'half' either.
                if (resize[i] < min[i])
                {
                    resize[i] = min[i];
                    half[i] = startSize[i] - min[i];
                    if (pick[i] < 0.f) { half[i] *= -1.f; }
                }
            }
                // set new size of window (rem: force size callback)
            size.setValue(resize);

                // transform start position into locale coordinates,
                // add clamped value of translation,
                // and transform back to world coordinate system.
                // set new position (world coordinates)
            startOrientation.inverse().multVec(startPosition,pick);
            pick += half * 0.5f;
            startOrientation.multVec(pick,pick);
            position = pick;
        }
        break;
    }
}

 /* ----------------------------------------------------------------------- */
/** normalize window 
  * turn visibility, frame, border, clipping, client volume switch on,
  * if requested by style */
 /* ----------------------------------------------------------------------- */
void
SoWindowKit::normalize(void)
{
    style.setValue(style.getValue());
}

 /* ----------------------------------------------------------------------- */
/** maximize window
  * turn frame, border, clipping switch off
  * turn visibility, client volume switch on, if requested by style */
 /* ----------------------------------------------------------------------- */
void
SoWindowKit::maximize(void)
{
    if (hasStyle(VISIBLE)) { setSwitch(visibleSwitch); }
    clearSwitch(borderSwitch);
    clearSwitch(frameSwitch);
    clearSwitch(clipSwitch);
}

 /* ----------------------------------------------------------------------- */
/** minimize window
  * turn visibility switch on */
 /* ----------------------------------------------------------------------- */
void
SoWindowKit::minimize(void)
{
    clearSwitch(visibleSwitch);
}

 /* ----------------------------------------------------------------------- */
/** add user to window's user focused list
  * @param user - user's id */
 /* ----------------------------------------------------------------------- */
void
SoWindowKit::addUser(int user)
{
    if (focus.find(user) == -1) { focus.set1Value(focus.getNum(),user); }
    setFrameColor(user);
}

 /* ----------------------------------------------------------------------- */
/** remove user from window's user focused list
  * @param user - user's id */
 /* ----------------------------------------------------------------------- */
void
SoWindowKit::removeUser(int user)
{
    int num = focus.find(user);

    if (-1 != num) { focus.deleteValues(num,1); }

    setFrameColor(focus[focus.getNum()-1]);
}

 /* ----------------------------------------------------------------------- */
/** get user's color
  * @param user - user's id */
 /* ----------------------------------------------------------------------- */
const SbColor &SoWindowKit::getUserColor(int user)
{
    return SoUserManagerKit::getInstance()->getUserKit(user)->color.getValue();
}

 /* ----------------------------------------------------------------------- */
/** set frame component color to user's color or inactive if none user given
  * @param user - user's id */
 /* ----------------------------------------------------------------------- */
void SoWindowKit::setFrameColor(int user)
{
    SoVertexProperty *vp = (SoVertexProperty *)
        ((SoIndexedTriangleStripSet *)borderGeometry.getValue())
            ->vertexProperty.getValue();

    if (-1 != user)
    {
        vp->orderedRGBA.set1Value(0,getUserColor(user).getPackedValue());
        vp->orderedRGBA.set1Value(3,SbColor(getUserColor(user)*0.5f).
            getPackedValue());
    }
    else
    {
        vp->orderedRGBA.set1Value(0,COL_INACTIVE);
        vp->orderedRGBA.set1Value(3,COL_INASHADOW);
    }
}

 /* ----------------------------------------------------------------------- */
/** set application callback routine and data pointer */
 /* ----------------------------------------------------------------------- */
void SoWindowKit::setApplicationCallback(
        WindowEventCB *callback, void *userData)
{
    this->callback = callback;
    this->userData = userData;
}

 /* ----------------------------------------------------------------------- */
/** get separator of client volume */
 /* ----------------------------------------------------------------------- */
SoSeparator *SoWindowKit::getClientVolume()
{
    return SO_GET_PART(this,"clientVolume",SoSeparator);
}
