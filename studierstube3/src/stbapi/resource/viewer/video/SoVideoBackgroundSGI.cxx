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
/** implementation of the SGI video background class.
 *
 * @author Jan Prikryl, Hannes Kaufmann, Gerhard Reitmayr, Tamer Fahmy
 *
 * $Id: SoVideoBackgroundSGI.cxx 3425 2004-07-14 12:09:17Z tamer $
 * @file                                                                   */
/* ======================================================================= */


#include <stbapi/util/GLtools.h>

#include "SoVideoBackgroundSGI.h"

SO_NODE_SOURCE(SoVideoBackgroundSGI);

void
SoVideoBackgroundSGI::initClass()
{ 
   SO_NODE_INIT_CLASS(SoVideoBackgroundSGI, SoVideoBackground, "SoVideoBackground");
}

SoVideoBackgroundSGI::SoVideoBackgroundSGI()
{
   SO_NODE_CONSTRUCTOR(SoVideoBackgroundSGI);
}

#if defined(SGI_VIDEO) && defined(_SGI_SOURCE)

// #include "McastNet.h"

// Camera *VideoStuff::camera;
int VideoStuff::sysFlag = VideoStuff::SYS_O2;
VLServer VideoStuff::vlServer = NULL;
int VideoStuff::vlSourceNodeNum = 1;
int VideoStuff::vlNBuffers = 4;

void
VideoStuff::error(char *s)
{
   fprintf(stderr, "%s\n%s", s, vlStrError(vlGetErrno()));
   exit(-1);
}

void
VideoStuff::warn(char *s)
{
   fprintf(stderr, "%s\n%s", s, vlStrError(vlGetErrno()));
}

VideoStuff::~VideoStuff()
{
   vlEndTransfer(vlServer, vlPath);
   vlDeregisterBuffer(vlServer, vlPath, vlDrain, vlBuffer);
   vlDestroyBuffer(vlServer, vlBuffer);
   vlDestroyPath(vlServer, vlPath);
   glDeleteTextures(1, &glTexName);
}


void
VideoStuff::cameraAutoGain(char flag)
{
   VLControlValue val;
  
   val.boolVal=flag;
   if(vlSetControl(vlServer, vlPath, vlSource, 
                   VL_CAMERA_AUTO_GAIN_CONTROL, &val)== -1)
   {
      warn("note: camera auto gain could not be set.");
   }
}

void
VideoStuff::createVideoPath(int sizeX, int sizeY, int num, int denom, int packing)
{
   VLControlValue val;
   int i;

   fprintf(stderr, "create video path:\n"
           "%s\n"
           "tgt size %i x %i\n"
           "tgt zoom %.2f %%",
           sysFlag==SYS_O2?"using O2 video specs":"using OCTANE video specs",
           sizeX, sizeY, 100.0*num/denom);

   if(vlServer==NULL)
   {
      vlServer=vlOpenVideo(NULL);
      if(vlServer==NULL) error("vlOpenVideo failed\n");
   }

   vlSource=vlGetNode(vlServer, VL_SRC, VL_VIDEO, vlSourceNodeNum);
   vlDrain=vlGetNode(vlServer, VL_DRN, VL_MEM, VL_ANY);
   if((vlSource== -1) || (vlDrain== -1))
      error("vlGetNode failed");

   vlPath=vlCreatePath(vlServer, VL_ANY, vlSource, vlDrain);
   if(vlPath== -1)
      error("vlCreatePath failed");

   i=vlSetupPaths(vlServer, (VLPathList)&vlPath, 1, VL_SHARE, VL_SHARE);
   if(i== -1)
      error("vlSetupPaths failed");

   memset(&val, 0, sizeof(VLControlValue));
   vlGetControl(vlServer, vlPath, vlSource, VL_CAMERA_SHUTTER, &val);
   val.intVal=VL_CAMERA_SHUTTER_100;
   vlSetControl(vlServer, vlPath, vlSource, VL_CAMERA_SHUTTER, &val);
   // fail silently -- we might be using a different source!

   if(vlGetControl(vlServer, vlPath, vlDrain, VL_SIZE, &val)== -1)
      error("vlGetControl(VL_SIZE) failed");
   fprintf(stderr, "src size: %i x %i", val.xyVal.x, val.xyVal.y);
  
   val.intVal = packing;
   if(vlSetControl(vlServer, vlPath, vlDrain, VL_PACKING, &val)== -1)
      error("vlSetControl(VL_PACKING) failed");

   val.fractVal.numerator=num;
   val.fractVal.denominator=denom;
   if(vlSetControl(vlServer, vlPath, vlDrain, VL_ZOOM, &val)== -1)
      warn("vlSetControl(VL_ZOOM) failed");

   val.fractVal.numerator=1;
   val.fractVal.denominator=1;
   if(vlSetControl(vlServer, vlPath, vlDrain, VL_ASPECT, &val)== -1)
      warn("vlSetControl(VL_ASPECT) failed");

   val.xyVal.x=sizeX; val.xyVal.y=sizeY;
   if(vlSetControl(vlServer, vlPath, vlDrain, VL_SIZE, &val)== -1)
   {
      warn("vlSetControl(VL_SIZE) failed");
      fprintf(stderr, " size was: %i %i", sizeX, sizeY);
   }

   if(vlGetControl(vlServer, vlPath, vlDrain, VL_SIZE, &val)== -1)
      error("vlGetControl(VL_SIZE) failed");
   vlCapSizeX=val.xyVal.x;
   vlCapSizeY=val.xyVal.y;
  
   vlBuffer=vlCreateBuffer(vlServer, vlPath, vlDrain, vlNBuffers);
   if(vlBuffer==NULL)
      error("vlCreateBuffer failed");

   if(vlRegisterBuffer(vlServer, vlPath, vlDrain, vlBuffer)== -1)
      error("vlRegisterBuffer failed");

   vlBeginTransfer(vlServer, vlPath, 0, NULL);
}

SbBool 
SoVideoBackgroundSGI::initVideoBackground(int width, int height)
{
   printf("SGI_VIDEO InitOverlay()\n");
   VideoStuff::sysFlag = VideoStuff::SYS_O2; // or SYS_OCTANE
   VideoStuff::vlSourceNodeNum = 3; // 1=O2cam, 2=composite 3=SVHS

   if (olStuff.sysFlag == VideoStuff::SYS_OCTANE)
   {
      olStuff.createVideoPath(sizeX, sizeY, 1.0, 1.0, VL_PACKING_ABGR_8);
      olStuff.glTexWidth  = 1024;
      olStuff.glTexHeight = 1024;
   }
   else
   {
      olStuff.createVideoPath(sizeX, sizeY, 1.0, 1.0, VL_PACKING_ABGR_8);
      //       olStuff.glTexWidth =512;
      //       olStuff.glTexHeight=512;
      olStuff.glTexWidth  = 1024;
      olStuff.glTexHeight = 1024;
   }
   
   fprintf(stderr, "InitOverlay:\ntxt size %i x %i\ngl size %i x %i",
           sizeX, sizeY,
           olStuff.glTexWidth, olStuff.glTexHeight);
   
   olStuff.glTexBuffer = new GLubyte[olStuff.glTexHeight*olStuff.glTexWidth*4];
   
   checkGLErrors();
   
   for(int y=0; y < olStuff.glTexHeight; y++)
   {
      for(int x=0; x < olStuff.glTexWidth; x++)
      {
         olStuff.glTexBuffer[4*(olStuff.glTexWidth*y+x)  ] = x % 256;
         olStuff.glTexBuffer[4*(olStuff.glTexWidth*y+x)+1] = (x+y) % 256;
         olStuff.glTexBuffer[4*(olStuff.glTexWidth*y+x)+2] = y % 256;
         olStuff.glTexBuffer[4*(olStuff.glTexWidth*y+x)+3] = (x-y) % 256;
      }
   }
   
   checkGLErrors();
   /*
     glEnable(GL_TEXTURE_2D);
     glGenTextures(1, &(olStuff.glTexName));
     glBindTexture(GL_TEXTURE_2D, olStuff.glTexName);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 
     olStuff.glTexWidth, olStuff.glTexHeight, 
     0, GL_RGBA, GL_UNSIGNED_BYTE, olStuff.glTexBuffer);
     checkGLErrors();
   */

   return TRUE;
}

void
SoVideoBackgroundSGI::updateTexture()
{
   VLInfoPtr frame;
   char *data;
   checkGLErrors();

   glBindTexture(GL_TEXTURE_2D, olStuff.glTexName);

   checkGLErrors();

   frame=vlGetLatestValid(olStuff.vlServer, olStuff.vlBuffer);
   if (frame != NULL)
   {
      data = (char*)vlGetActiveRegion(olStuff.vlServer, 
                                      olStuff.vlBuffer, frame);
      checkGLErrors();

      glTexSubImage2DEXT(GL_TEXTURE_2D, 0, 0,0,
                         olStuff.vlCapSizeX, 
                         olStuff.vlCapSizeY, GL_RGBA, 
                         GL_UNSIGNED_BYTE, data);
      if(checkGLErrors())
      {
         fprintf(stderr, "glTexSubImage2D parameters:\n"
                 "capSize %i x %i\ndata at %p",
                 olStuff.vlCapSizeX,
                 olStuff.vlCapSizeY,
                 data);
      }
      vlPutFree(olStuff.vlServer, olStuff.vlBuffer);
   }
   //   else
   //     fprintf(stderr,"video overlay: no news!\n");
}

void
SoVideoBackgroundSGI::blitOverlay(int eye)
{
   VLInfoPtr frame;
   static char *data = NULL;

   preGLCalls();
    
   float m = mirrorMode ? -1.0F : 1.0F;
    
   glPixelZoom (m, -1.0);
   glRasterPos2f (-m,1);
    
   glPixelTransferf (GL_RED_SCALE, 1.0);
   glPixelTransferf (GL_GREEN_SCALE, 1.0);
   glPixelTransferf (GL_BLUE_SCALE, 1.0);
    
   glPixelTransferi (GL_MAP_COLOR, GL_FALSE);
    
   glColor3f (1.0F, 0.1F, 1.0F);

   checkGLErrors();

   // This is probably not necessary 
   // updateTexture ();
   // checkGLErrors ();
   
   frame = vlGetLatestValid(olStuff.vlServer, olStuff.vlBuffer);
   if(frame != NULL)
   {
      if (data != NULL) vlPutFree(olStuff.vlServer, olStuff.vlBuffer);
      data = (char*)vlGetActiveRegion(olStuff.vlServer, 
                                      olStuff.vlBuffer, frame);
      // memset (data, 255, 80000);
      checkGLErrors();

   }

   // glPixelStorei(GL_PACK_LSB_FIRST, FALSE);
   glDrawPixels(olStuff.vlCapSizeX, 
                olStuff.vlCapSizeY, GL_RGBA, 
                GL_UNSIGNED_BYTE, data);
   
   if (checkGLErrors())
   {
      fprintf(stderr, "glDrawPixels parameters:\n"
              "capSize %i x %i\ndata at %p",
              olStuff.vlCapSizeX,
              olStuff.vlCapSizeY,
              data);
   }
   
   // vlPutFree(olStuff.vlServer, olStuff.vlBuffer);
   

   /*
     if(frame!=NULL)
     {
     //     vlPutFree(olStuff.vlServer, olStuff.vlBuffer);
     }
   */   
   //   else
   //     fprintf(stderr,"video overlay: no news!\n");

   postGLCalls();
}

#endif //defined(SGI_VIDEO) && defined(_SGI_SOURCE)
