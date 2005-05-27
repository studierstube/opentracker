 /* ========================================================================
  * (C) 2002 Vienna University of Technology
  * ========================================================================
  * PROJECT: Spray
  * ======================================================================== */ 
/** Headerfile of SoSprayingKit.
  * 
  *
  * @author  Andreas Zajic, Dieter Schmalstieg, Anton Fuhrmann, Hermann Wurnig
  *
  * $Id: SoSprayingKit.h 3426 2004-07-14 12:11:27Z tamer $
  * @file  SoSprayingKit.h                                                   */
 /* ======================================================================== */
#ifndef _SOSPRAYINGKIT_H_
#define _SOSPRAYINGKIT_H_

#include <stbapi/workspace/SoContextKit.h>

#include "spray.h"


#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class So3DButton;

/**
    SoSprayingKit class.
    SoSprayingKit implements a small multi user spraying application
    for the Studierstube AR System.
    @author Andreas Zajic, Dieter Schmalstieg, Anton Fuhrmann, Hermann Wurnig
*/
class SPRAY_API SoSprayingKit
:
    public SoContextKit
{
    SO_KIT_HEADER(SoSprayingKit);

public:
/**
    static class initialization
*/
    static void initClass();

/**
    NodeKit constructor.
    It handles standard OIV initializations of the application nodekit
    and its fields.
*/
    SoSprayingKit();

/**
    virtual constructor
*/
    SoContextKit* factory();

/**
    NodeKit destructor    
*/
    ~SoSprayingKit();

/** 
    Callback function attached to the Clear button.
    When triggered, it resets all sprayed contents and clears the window
*/
    static void clearButtonCB(
        void* data,
        So3DButton* button
        );

private:
/**
    In this implementation it assumed that the PIP Geometry that is
    passed with the loader file is correct and does not need further
    corrections here.
    There is also no need to do additional computations here.
*/
    SbBool checkPipGeometry();

/**
    Performs a check on the window geometry from the loader .iv-file.
    When no window is passed in the loader file, an empty window with
    default properties is created.
    It is assumed that the window definition in the loader file is correct
    to fit the needs of the application (there is no sophisticated error
    handling!)
*/
    SbBool checkWindowGeometry();

/**
    Attaches the callback to the Clear-button when application enters mastermode.
*/
    virtual void checkPipMasterMode(
        SoNode * pipSheetContents,
        SbBool masterMode);

/**
    This Callback is triggered when the window passes an event to the application.
    When the user moves the pen inside the window and klicks the button a voxel
    is sprayed in the window.
*/               
    virtual SbBool windowEventCB(      
        void* data,                    
        int messageId,
        SoWindowKit* win,
        int uid,
        So3DEvent* event, 
        SoPath* path
        );

/**
    do actual spraying in the window
    @param position the position where to drop the sprayed voxel
    @param uid UserID of the active user
*/    
    void spray(
        SbVec3f position,
        int uid
        );

/**
    add a single cubic "spray" droplet into window
    @param position Position where the voxel is placed
    @param color Colour of the sprayed voxel
    @param size Size of the Voxel
*/
    void drawVoxel(
        SbVec3f position,
        SbColor color,
        float size
        );

/**
    Convenience variable containing a pointer to the voxel mesh that stores
    the sprayed voxels.
*/
    SoIndexedTriangleStripSet* mesh;

/**
    Convenience variable containing a pointer to the property node
    of the voxel mesh.
*/
    SoVertexProperty* vp;

/**
    OIV Field storing the number of voxels currently in the mesh
*/
    SoSFInt32 voxelField;

/**
    OIV Field storing the template voxel.
    The template is passed to the application from the loader file.
*/
    SoSFNode templateMesh;
}; //class SoSprayingKit

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif//_SOSPRAYINGKIT_H_

