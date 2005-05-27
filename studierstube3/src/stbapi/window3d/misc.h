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
//  CONTENT:    misc - implementation
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    mk      Markus Krutz
//              gh      Gerd Hesina
// ===========================================================================
//  HISTORY:
//
//  27-aug-99 17:12:36  gh      last modification
// ===========================================================================
#ifndef _MISC_H_
#define _MISC_H

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/SbLinear.h>

void WriteGraph(SoSeparator *s);

void printRotation(const char *text, const SbRotation& r);
void printVector(const char *text, const SbVec3f& v);
void printMatrix(const char *text, const SbMatrix& m, SbBool b=FALSE);
#endif
