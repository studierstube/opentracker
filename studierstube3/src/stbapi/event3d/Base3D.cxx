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
/** Implementation of class Base3D
  *
  * @author   Andreas Zajic, Markus Krutz, Gerd Hesina
  *
  * $Id: Base3D.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <Inventor/actions/SoGetBoundingBoxAction.h>

#include <stbapi/event3d/Base3D.h>

Base3D::Base3D() {
    base3DMode=BASE3D_NORMALMODE;
    blocked = TRUE;
}

Base3D::~Base3D() {
}

//
// function that states the possible interest of the nodekit in the event
// called by the action during traversal
//

SbBool Base3D::isInterested(SoHandle3DEventAction *h3a) {
	So3DEvent *ev = (So3DEvent*)h3a->getEvent();
	const SoPath *currPath = h3a->getPathFromRoot();
	static SbViewportRegion vp;
	static SoGetBoundingBoxAction *bba=new SoGetBoundingBoxAction(vp);

	bba->apply((SoFullPath*)currPath);
	
	SbBox3f box=bba->getBoundingBox();
	if (box.intersect(ev->getTranslation())==FALSE) {
		return FALSE;
	}
	
	SbXfBox3f xfbox=bba->getXfBoundingBox();

	return(xfbox.intersect(ev->getTranslation()));
}

// 
// function that is called if the nodekit is interested in the event
//

void Base3D::handle3DEvent(SoHandle3DEventAction *) {

}

void Base3D::callHandle3DEvent(SoAction *,SoNode *) {

}
