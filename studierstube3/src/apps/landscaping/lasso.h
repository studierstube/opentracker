// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       lasso
//  TYPE:       c++ header
//  PROJECT:    Studierstube
//  CONTENT:    Function prototypes for the lasso tool for the pip
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    ds      Dieter Schmalstieg
//              gh      Gerd Hesina
// ===========================================================================
//  HISTORY:
//
//  27-aug-99 17:17:57  gh      last modification
//  17-sep-98 21:47:52  ds      created
// ===========================================================================


#ifndef _LASSO_H_
#define _LASSO_H_

// the lasso tool allows to select artifacts by circling them with a freehand
// line on the pip. just point the pip towards the object and draw a line
// that circles the artifact(s). the line is automatically closed, and any
// artifact inside the line (according to a non-zero winding number rule on
// the centroid point of the artifact) is selected. the selection can be 
// repeated as desired.

void setupLasso(    //setup lasso stuff
    );
void resetLasso(    //reset lasso to start a new selection
    );

#endif//_LASSO_H_
