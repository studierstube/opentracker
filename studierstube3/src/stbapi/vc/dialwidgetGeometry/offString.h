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

char *SoVCDialWidgetKit::widgetOffString=
"DEF geomSep Separator {"
"           DEF geomTransp TransparencyType { value SORTED_OBJECT_SORTED_TRIANGLE_BLEND }"
"           DEF geomFile File { name ../../src/stbapi/vc/dialwidgetGeometry/halfsphere_off.iv }"
" DEF indicatorRoot Separator {"   
" Separator {"
"  Transform { rotation 1 0 0 1.57079"
"              scaleFactor 0.044 0.044 0.044"
"            }"
"  Material { "
"    ambientColor  0.6 0.05 0.05"
"    diffuseColor  0.7  0.3 0.3"
"    transparency  0.0"
"}"
"  DrawStyle { lineWidth 1.5 }"
"  Coordinate4 { point [ 10.0     0.0     0.0   1.0,"
"                         7.071   7.071   0.0   0.7071,"
"		         0.0    10.0     0.0   1.0,"
"		        -7.071   7.071   0.0   0.7071,"
"		       -10.0     0.0     0.0   1.0,"
"		        -7.071  -7.071   0.0   0.7071,"
"		         0.0   -10.0     0.0   1.0,"
"			 7.071  -7.071   0.0   0.7071,"
"			10.0     0.0     0.0   1.0]"
"	      }"
"  NurbsCurve {"
"   numControlPoints 9"
"   knotVector [ 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 4 ]"
"   }"
"}"
"   DEF indicatorTransform Transform {"
"    center 0 0 0"
"    rotation 0 1 0 0"
"   }"
"   DEF indicatorTranslation Translation {"
"    translation 0 0 -0.425"
"   }"
"   DEF indicatorScale Scale {"
"    scaleFactor 0.15 0.15 0.15"
"   }"
"   DEF indicatorMaterial Material {"
"    ambientColor  0.6 0.05 0.05"
"    diffuseColor  0.7  0.3 0.3"
"    specularColor 0.5  0.5 0.5"
"    shininess     0.4"
"    transparency  0.0"
"   }"
"   DEF indicatorGeometry Sphere { radius 0.5 }"
" }      "
"}";

