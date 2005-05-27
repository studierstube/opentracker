/* 
   (C) 2000 Nemosoft Unv.    nemosoft@smcc.demon.nl
   
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/* 
 $Log: ccvt.h,v $
 Revision 1.1  2002/08/29 06:17:14  bornik
 *** empty log message ***

 Revision 1.8  2002/04/14 01:00:27  nemosoft
 Finishing touches: adding const, adding libs for 'show'

 Revision 1.7  2001/12/03 00:09:34  jvoosten
 *** empty log message ***

 Revision 1.6  2001/12/02 04:20:19  jvoosten
 Lotta changes... Only 420p_bgr32 at the moment, but with show and benchmark
 utilities
*/


#ifndef CCVT_H
#define CCVT_H

#ifdef __cplusplus
extern "C" {
#endif

/* Colour ConVerT: going from one colour space to another.
   ** NOTE: the set of available functions is far from complete! **

   Format descriptions:
   420i = "4:2:0 interlaced"
           YYYY UU YYYY UU   even lines
           YYYY VV YYYY VV   odd lines
           U/V data is subsampled by 2 both in horizontal 
           and vertical directions, and intermixed with the Y values.
   
   420p = "4:2:0 planar"
           YYYYYYYY      N lines
           UUUU          N/2 lines
           VVVV          N/2 lines
           U/V is again subsampled, but all the Ys, Us and Vs are placed 
           together in separate buffers. The buffers may be placed in
           one piece of contiguous memory though, with Y buffer first,
           followed by U, followed by V.

   yuyv = "4:2:2 interlaced"
           YUYV YUYV YUYV ...   N lines
           The U/V data is subsampled by 2 in horizontal direction only.

   bgr24 = 3 bytes per pixel, in the order Blue Green Red (whoever came up
           with that idea...)
   rgb24 = 3 bytes per pixel, in the order Red Green Blue (which is sensible)
   rgb32 = 4 bytes per pixel, in the order Red Green Blue Alpha, with 
           Alpha really being a filler byte (0)
   bgr32 = last but not least, 4 bytes per pixel, in the order Blue Green Red
           Alpha, Alpha again a filler byte (0)
 */

/* 4:2:0 YUV planar to RGB/BGR     */
void ccvt_420p_bgr24(int width, int height, const void *src, void *dst);
void ccvt_420p_rgb24(int width, int height, const void *src, void *dst);
void ccvt_420p_bgr32(int width, int height, const void *src, void *dst);
void ccvt_420p_rgb32(int width, int height, const void *src, void *dst);

/* 4:2:2 YUYV interlaced to RGB/BGR */
void ccvt_yuyv_rgb32(int width, int height, const void *src, void *dst);
void ccvt_yuyv_bgr32(int width, int height, const void *src, void *dst);

/* 4:2:2 YUYV interlaced to 4:2:0 YUV planar */
void ccvt_yuyv_420p(int width, int height, const void *src, void *dsty, void *dstu, void *dstv);

/* RGB/BGR to 4:2:0 YUV interlaced */

/* RGB/BGR to 4:2:0 YUV planar     */
void ccvt_rgb24_420p(int width, int height, const void *src, void *dsty, void *dstu, void *dstv);
void ccvt_bgr24_420p(int width, int height, const void *src, void *dsty, void *dstu, void *dstv);

#ifdef __cplusplus
}
#endif

#endif
