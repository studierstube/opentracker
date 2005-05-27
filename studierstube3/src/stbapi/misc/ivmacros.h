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
//  CONTENT:    collection of Inventor macros
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    ds      Dieter Schmalstieg
//              gh      Gerd Hesina
// ===========================================================================
//  HISTORY:
//
//  27-aug-99 17:03:54  gh      last modification
// ===========================================================================

#ifndef _IVMACROS_H_
#define _IVMACROS_H_

//-----------------------------------------------------------------------------
//macros
//-----------------------------------------------------------------------------

//--get a node by name, with variable declaration

#define GETNODE(var,type,name) \
    type* var=(type*)SoNode::getByName(name)

//--get a node by name and assert it exists, with variable declaration 

#define AGETNODE(var,type,name) \
    type* var=(type*)SoNode::getByName(name); \
    assert(var)

//--get a node by name, without variable declaration

#define VGETNODE(var,type,name) \
    var=(type*)SoNode::getByName(name)

//--get a node by name and assert it exists, without variable declaration

#define AVGETNODE(var,type,name) \
    var=(type*)SoNode::getByName(name); \
    assert(var)

//--create a new node of a given type
#define NEWNODE(var,type) \
    type* var = new type

//--get the child with given index from a parent

#define GETCHILD(var,type,parent,idx) \
    type* var = (type*)parent->getChild(idx)

//--get the child with given index from a parent (assertive)

#define AGETCHILD(var,type,parent,idx) \
    type* var=(type*)parent->getChild(idx);\
    assert(var); \
    assert(var->isOfType(type::getClassTypeId()))

//--get the child with given index from a parent w/o variable declaration

#define VGETCHILD(var,type,parent,idx) \
    var = (type*)parent->getChild(idx)

//--get the child with given index from a parent (assertive), w/o variable
#define AVGETCHILD(var,type,parent,idx) \
    var=(type*)parent->getChild(idx);\
    assert(var); \
    assert(var->isOfType(type::getClassTypeId()))

#endif//_IVMACROS_H_

