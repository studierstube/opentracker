// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       voAppearanceElement.h
//  TYPE:       c++ header
//  PROJECT:    Studierstube MediDesk
//  CONTENT:    Demo application for volume rendering
//  VERSION:    1.3
// ===========================================================================
//  AUTHORS:    ww      Werner Wohlfahrter
// ===========================================================================
//  HISTORY:    2000 09 26  ww :) 
// ===========================================================================

#ifndef  _voAppearanceElement_h_
#define  _voAppearanceElement_h_

#include <vo/Appearance.h>
#include <Inventor/elements/SoReplacedElement.h>
#include "voAppearance.h"

//////////////////////////////////////////////////////////////////////////////
//
//  Class: voAppearanceElement
//
//  Element that stores the current volume appearance (i.e., texture) info.
//
//  Modified on 10/6/99 by dla to include a work-around for a CC 7.3
//  code generation bug.
//
//////////////////////////////////////////////////////////////////////////////

SoEXTENDER class voAppearanceElement : public SoReplacedElement {

  // Note: replaced the SO_ELEMENT_HEADER macro to get around CC bug.

  // *** Begin replaced portion of header *** 

public:                                                                     
   static SoType       getClassTypeId()        {
        // This is the function that was crashing under 7.3. We replace it
        // with something that puts the static SoType on the stack before
        // returning it.
        SoType temp = classTypeId;
        return temp;
   }       
   static int          getClassStackIndex()    { return classStackIndex; }   

protected:                                                                 
   static int          classStackIndex;                                      
   voAppearanceElement();

private:                                                                   
   static SoType       classTypeId;
   static void *createInstance();

public:
   virtual void    init(SoState *state);
   static void     set(SoState *state, SoNode *node, voAppearance *cdp);
   static voAppearance *get(SoState *state);

   // Returns the top (current) instance of the element in the state
   static const voAppearanceElement * getInstance(SoState *state);

   virtual SbBool matches(const SoElement *elt) const;
   virtual SoElement *copyMatchInfo() const;
   virtual void print(FILE *fp) const;

SoINTERNAL public:
   // Initializes the voAppearanceElement class
   static void     initClass();

protected:
   voAppearance *data;
   virtual ~voAppearanceElement() {}
};

#endif // _voAppearanceElement_h_

