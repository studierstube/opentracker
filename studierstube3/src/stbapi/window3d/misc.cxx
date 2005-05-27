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
/** misc code for studierstube
  *
  * @author Markus Krutz, Gerd Hesina
  *
  * $Id: misc.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include "misc.h"
#include "dbg.h"

////////////////////////////////////////////////////////////////////////
// misc convenience functions
////////////////////////////////////////////////////////////////////////
void WriteGraph(SoSeparator *s) {
	SoWriteAction myAction;
	myAction.getOutput()->openFile("scene.iv");
	myAction.getOutput()->setBinary(FALSE);
	myAction.apply(s);
	myAction.getOutput()->closeFile();
}

void printRotation(const char *text, const SbRotation& r) {
	SbVec3f axis;
	float angle;
	r.getValue(axis, angle);
	dbgout("%s: (%f, %f, %f), %f\n", text, axis[0], axis[1], axis[2], angle);
}

void printVector(const char *text, const SbVec3f& v) {
	dbgout("%s: (%f, %f, %f)\n", text, v[0], v[1], v[2]);
}

void printMatrix(const char *text, const SbMatrix& m, SbBool b) {
	SbVec3f t, s, r1, so1, c;
	SbRotation r, so;
	float r2, so2;

	if (b)
		m.getTransform(t, r, s, so, c);
	else
		m.getTransform(t, r, s, so);
	r.getValue(r1, r2);
	so.getValue(so1, so2);
	printf("%s\n Translation: (%5.2f, %5.2f, %5.2f)\n Rotation: ((%5.2f, %5.2f, %5.2f), %5.2f)\n ScaleFactor: (%5.2f, %5.2f, %5.2f)\n ScaleOrientation: ((%5.2f, %5.2f, %5.2f), %5.2f)\n", \
		text, t[0], t[1], t[2], r1[0], r1[1], r1[2], r2, s[0], s[1], s[2], so1[0], so1[1], so1[2], so2);
	if (b)
		printf(" Center: (%5.2f, %5.2f, %5.2f)\n", c[0], c[1], c[2]);
}
