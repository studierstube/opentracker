 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** source file for the WrapperNode class.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/Attic/WrapperNode.cxx,v 1.1 2001/03/26 22:11:21 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "WrapperNode.h"

#include <dom/DOM_Element.hpp>

void WrapperNode::setParent( DOM_Element & parElement )
{
	Node::setParent( parElement );
	tagname = parent->getTagName().transcode();
}

