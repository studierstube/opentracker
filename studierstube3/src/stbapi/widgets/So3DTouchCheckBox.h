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
//  PROJECT:    Studierstube
//  CONTENT:    So3DTouchCheckBox - implementation
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    jp      Jan Prikryl
// ===========================================================================
//  HISTORY:
//
//  19-Sep-00 17:59:33 jp last modification
//  19-Sep-00 15:49:43 jp copied template from So3DCheckBox
// ===========================================================================
#ifndef __So3DTouchCheckBox_H__
#define __So3DTouchCheckBox_H__

#include <stbapi/widgets/SoBase3DButton.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class So3DTouchCheckBox;

typedef void So3DTouchCheckBoxCB(void *userData, So3DTouchCheckBox *cbox);

////////////////////////////////////////////////////////////////////
//
//  Class: So3DTouchCheckBox
//
//  Contains all functions for handling a 3D-Button. Acts like
//  a SoSeparator-Node. 
//
////////////////////////////////////////////////////////////////////
/**
 * @ingroup oldwidgets
 * @ingroup deprecated
 */
class STBAPI_API So3DTouchCheckBox : public SoBase3DButton {
    
    SO_KIT_HEADER(So3DTouchCheckBox);
    
public:
    // member-variables
    // member-methods
    
    static void initClass();    
    void initFields();  
    
    So3DTouchCheckBox();                
    So3DTouchCheckBox(float width, float height, float depth);
    So3DTouchCheckBox(float width, float height, float depth, 
                      SoTexture2 *buttonTexture, float xDist = 0,
                      float yDist = 0);
    So3DTouchCheckBox(float width, float height, float depth, 
        char *fontName, int fontSize, char *buttonText,
        float textHeight, SbColor textColor);

    virtual SbBool isInterested(SoHandle3DEventAction *h3a);
    virtual void handle3DEvent(SoHandle3DEventAction *h3a);

    void   addPressCallback(So3DTouchCheckBoxCB *funcCB, void *userData=NULL);
    SbBool removePressCallback(So3DTouchCheckBoxCB *funcCB);

    void   addReleaseCallback(So3DTouchCheckBoxCB *funcCB, void *userData=NULL);
    SbBool removeReleaseCallback(So3DTouchCheckBoxCB *funcCB);

protected:
    SbPList    *pressCBList;         // list of callback-function *
    SbPList    *releaseCBList;         // list of callback-function *
    static int classInitAttempts;
    // member-variables
private:
    // member-methods
    virtual    ~So3DTouchCheckBox();
    int        wasPressedUponEnter;
    
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif

