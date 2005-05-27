// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       voAppearanceElement.cxx
//  TYPE:       c++ source code
//  PROJECT:    Studierstube MediDesk
//  CONTENT:    Demo application for volume rendering
//  VERSION:    1.3
// ===========================================================================
//  AUTHORS:    ww      Werner Wohlfahrter
// ===========================================================================
//  HISTORY:    2000 09 26  ww :) 
// ===========================================================================

#include <iostream.h>
#include "voAppearanceElement.h"

//-----------------------------------------------------------------------------
// This class enables to pass the current voBrickSetCollection to and
// from the state for use by other nodes that need it (i.e., SimpleGeometry

SO_ELEMENT_SOURCE(voAppearanceElement);

//-----------------------------------------------------------------------------
// Initializes element.
// 
// Use: public

void voAppearanceElement::init(SoState *)
{
   //cout << "voAppearanceElement::init running ..." << endl;
   data = NULL;
}


//-----------------------------------------------------------------------------
// initClass

void voAppearanceElement::initClass()
{   
   //cout << "voAppearanceElement::initClass running..." << endl;

   SO_ELEMENT_INIT_CLASS(voAppearanceElement, SoReplacedElement);
}


//-----------------------------------------------------------------------------
// Overrides this method to return TRUE if the two elements match.
// Comparison is based on nodeId and the value of "data".
// Use: public

SbBool voAppearanceElement::matches(const SoElement *elt) const
{
   //cout << "voAppearanceElement::matches running ..." << endl;
   if (elt == NULL) return FALSE;

   if ((nodeId == ((const voAppearanceElement *) elt)->nodeId) &&
       (data == ((const voAppearanceElement *) elt)->data)) {
       return TRUE;
   }
   else {
       return FALSE;
   }
}


//-----------------------------------------------------------------------------
// Set element's value.
//
// Use: public, static

void voAppearanceElement::set(SoState *state, SoNode *node, voAppearance *value)
{
   //cout << "voAppearanceElement::set running ..." << endl;
   voAppearanceElement  *elt;

   // Get an instance we can change (pushing if necessary)
   elt = (voAppearanceElement *)getElement(state, classStackIndex, node);

   if (elt != NULL) elt->data = value;

}


//-----------------------------------------------------------------------------
// Returns value of a subclass from state.
//
// Use: public, static

voAppearance * voAppearanceElement::get(SoState *state)
{
   //cout << "voAppearanceElement::get running ..." << endl;
   const voAppearanceElement *elt;

   elt = (const voAppearanceElement *)getConstElement(state, classStackIndex);

   return elt->data;
}


//-----------------------------------------------------------------------------
// Returns the first (top) instance of the element in the state.
//
// Use: public, static
 
const voAppearanceElement *
voAppearanceElement::getInstance(SoState *state)
{  
   //cout << "voAppearanceElement::getInstance running ..." << endl;
   return (const voAppearanceElement *)
      getConstElement(state, classStackIndex);
}


//-----------------------------------------------------------------------------
// Create a copy of this instance suitable for calling matches() on.
//
// Use: protected

SoElement *
voAppearanceElement::copyMatchInfo() const
{
   //cout << "voAppearanceElement::copyMatchInfo running ..." << endl;
   voAppearanceElement *result =
      (voAppearanceElement *)getTypeId().createInstance();

   result->nodeId = nodeId;
   result->data = data;

   return result;
}


//-----------------------------------------------------------------------------
// Prints element for debugging.
//
// Use: public

#ifdef DEBUG
void
voAppearanceElement::print(FILE *fp) const
{
   //cout << "voAppearanceElement::print running ..." << endl;
   SoElement::print(fp);

   fprintf(fp, "node: \"%s\", pointer: %d\n", getTypeId().getName().getString(), data);
}
#else  /* DEBUG */
void
voAppearanceElement::print(FILE *) const
{
}
#endif /* DEBUG */
