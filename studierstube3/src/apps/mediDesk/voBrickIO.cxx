// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       voBrickIO.cxx
//  TYPE:       c++ source code
//  PROJECT:    Studierstube MediDesk
//  CONTENT:    Demo application for volume rendering
//  VERSION:    1.3
// ===========================================================================
//  AUTHORS:    SGI Developers
//              ww      Werner Wohlfahrter
// ===========================================================================
//  HISTORY:    2000 09 26  ww :) 
// ===========================================================================

//-----------------------------------------------------------------------------
// The routines provided here implement simple file I/O on files containing
// voxel data.  The first group of routines uses the Image File Library (IFL)
// to read voxel data from a variety of file formats that IFL recognizes, for
// example 3D TIFF files.  The second group, uses standard system I/O routines
// to read voxel data from a list of 2D "raw" (i.e., texel stream) images.

#include <errno.h>
#include <stdio.h>
#include <assert.h>
#include <iostream.h>
#include <unistd.h>
#include <fcntl.h>


#include <ifl/iflError.h>
#include <ifl/iflFile.h>
#include <ifl/iflSize.h>

#include <vo/Volumizer.h>


//-----------------------------------------------------------------------------
// myReadBrickIfl() reads a 3D subvolume anchored at [xyz]BrickOrigin and 
// of size [xyz]BrickSize from a volume of size [xyz]VolumeSize using IFL.
// Bricks that extend beyond the volume are zero filled.

int myReadBrickIfl(char *fileName, void *data,
		int xBrickOrigin, int yBrickOrigin, int zBrickOrigin,
		int xBrickSize, int yBrickSize, int zBrickSize,
		int xVolumeSize, int yVolumeSize, int zVolumeSize)
{

   iflStatus sts;
   iflFile *file = iflFile::open(fileName, O_RDONLY, &sts);
   if (sts != iflOKAY) {
      cerr << "Cannot read in texture from " << fileName << " in myReadBrickIfl()\n";
      return -1;
   }
   iflSize dims;
   file->getDimensions(dims);
   iflConfig cfg(file->getDataType(), iflInterleaved);

   assert(xBrickOrigin >= 0 && xBrickOrigin < xVolumeSize);
   assert(yBrickOrigin >= 0 && yBrickOrigin < yVolumeSize);
   assert(zBrickOrigin >= 0 && zBrickOrigin < zVolumeSize);

   unsigned char *ptr = (unsigned char *) data;

   int bpp;  // bytes-per-pixel
   switch (file->getDataType()) {  // compute bytes-per-channel
   case iflChar:
   case iflUChar:
      bpp = 1;
      break;
   case iflShort:
   case iflUShort:
      bpp = 2;
      break;
   case iflFloat:
      bpp = 4;
      break;
   default:
      fprintf(stderr, "Unknown data type (%x) in file %s.\n", file->getDataType(), fileName);
   }

   // multiply bytes-per-channel by channels-num to get bytes-per-pixel
   bpp *= dims.c;  

   // is this a full brick?
   if ((xBrickOrigin + xBrickSize <= xVolumeSize) &&
       (yBrickOrigin + yBrickSize <= yVolumeSize) &&
       (zBrickOrigin + zBrickSize <= zVolumeSize)) {
      for (int iz = zBrickOrigin; iz < (zBrickOrigin + zBrickSize); iz++) {
         sts = file->getTile(
	    xBrickOrigin, yBrickOrigin, iz, xBrickSize, yBrickSize, 1, ptr, &cfg);
	 if (sts != iflOKAY) {
            fprintf(stderr, "%s(): Unable to read a tile.\n", "myReadBrickIfl");
            return -1;
	 }
	    ptr += xBrickSize * yBrickSize * bpp;
	 }			// iz

   } else {			// deal with partial bricks
      int xPartBrickSize, yPartBrickSize, zPartBrickSize;

      xPartBrickSize = xBrickOrigin + xBrickSize <= xVolumeSize ?
         xBrickSize : xVolumeSize - xBrickOrigin;
      yPartBrickSize = yBrickOrigin + yBrickSize <= yVolumeSize ?
         yBrickSize : yVolumeSize - yBrickOrigin;
      zPartBrickSize = zBrickOrigin + zBrickSize <= zVolumeSize ?
         zBrickSize : zVolumeSize - zBrickOrigin;

      unsigned char *buffer = new unsigned char[
         xBrickSize * yBrickSize * zBrickSize * sizeof(*buffer) * bpp];

      assert(buffer != NULL);

      ptr = buffer;

      fprintf(stderr, "|\b");

      for (int iz = zBrickOrigin; iz < (zBrickOrigin + zPartBrickSize); iz++) {
         sts = file->getTile(xBrickOrigin, yBrickOrigin, iz,
			     xPartBrickSize, yPartBrickSize, 1, ptr, &cfg);
	 if (sts != iflOKAY) {
            fprintf(stderr, "%s(): Unable to read a tile.\n", "myReadBrickIfl");
	    return -1;
	 }
	 ptr += xPartBrickSize * yPartBrickSize * bpp;
      }			// iz
      fprintf(stderr, "/\b");

      // KLUDGE does not set all the borders
      for (int j0 = 0; j0 < zPartBrickSize; j0++)
         for (int j1 = 0; j1 < yPartBrickSize; j1++) {
            memcpy((unsigned char *) data + \
               j0 * xBrickSize * yBrickSize + j1 * xBrickSize, \
               buffer + j0 * xPartBrickSize * yPartBrickSize + j1 * xPartBrickSize, \
               xPartBrickSize);
            memset(buffer + j0 * xPartBrickSize * yPartBrickSize + \
               j1 * xPartBrickSize + xPartBrickSize, \
               0, \
               xBrickSize - xPartBrickSize);
	 }

      delete[]buffer;
      fprintf(stderr, "-\b");
    }

    file->close();
    return 0;
}


//-----------------------------------------------------------------------------
// myGetVolumeSizesIfl() returns volume sizes and format for a file
// in IFL-readable format.

int myGetVolumeSizesIfl(char *fileName,
			int &xVolumeSize, int &yVolumeSize, int &zVolumeSize,
			voExternalFormatType & diskDataFormat,
			voDataType & dataType)
{

   iflStatus sts;
   iflFile *file = iflFile::open(fileName, O_RDONLY, &sts);
   if (sts != iflOKAY) {
      cerr << "myGetVolumeSizesIfl(): Cannot open texture in file '" << fileName << "'\n";
      return -1;
   }
   iflSize dims;
   file->getDimensions(dims);

   file->close();

   xVolumeSize = dims.x;
   yVolumeSize = dims.y;
   zVolumeSize = dims.z;

   switch (dims.c) {
   case 1:
      diskDataFormat = voExternalFormatTypeScope::LUMINANCE;
      break;
   case 4:
      diskDataFormat = voExternalFormatTypeScope::RGBA;
      break;
   default:
      fprintf(stderr, "Unsupported data format (%x) in file %s.\n", dims.c, fileName);
   }

   switch (file->getDataType()) {
   case iflChar:
      dataType = voDataTypeScope::BYTE;
      break;
   case iflUChar:
      dataType = voDataTypeScope::UNSIGNED_BYTE;
      break;
   case iflShort:
      dataType = voDataTypeScope::SHORT;
      break;
   case iflUShort:
      dataType = voDataTypeScope::UNSIGNED_SHORT;
      break;
   case iflFloat:
      dataType = voDataTypeScope::FLOAT;
      break;
   default:
      fprintf(stderr, "Unknown data type (%x) in file %s.\n", file->getDataType(), fileName);
   }
   return 0;
}


//-----------------------------------------------------------------------------
// Raw image I/O.
// A "raw" 2D image is a voxel stream in row major order possibly proceeded 
// with a fixed header that is discarded in the process.


//-----------------------------------------------------------------------------
// getTile() reads a 2D subregion of voxels from a 2D raw image, given
// the header length and the number of bytes per voxel.

int getTile(char *fileName, void *data,
	    int xBrickOrigin, int yBrickOrigin,
	    int xBrickSize, int yBrickSize,
	    int xVolumeSize, int /* yVolumeSize */,
	    int headerLength, int bytesPerVoxel)
{
   int fd = open(fileName, O_RDONLY);
   if (fd < 0) {
      fprintf(stderr, "%s():Cannot open file %s for reading.\n", "getTile", fileName);
      return -1;
   }
   char *ptr = (char *) data;	/* DataType */

   for (int j1 = yBrickOrigin; j1 < yBrickOrigin + yBrickSize; j1++) {
      int pos = headerLength + (j1 * xVolumeSize + xBrickOrigin) * bytesPerVoxel;

      if (lseek(fd, pos, SEEK_SET) != pos) {
         fprintf(stderr, "%s():Cannont position file %s.\n", "getTile", fileName);
         close(fd);
         return -2;
      }
      if (read(fd, ptr, bytesPerVoxel * xBrickSize) != bytesPerVoxel * xBrickSize) {
         fprintf(stderr, "%s():Short read in file %s.\n", "getTile", fileName);
         close(fd);
         return -3;
      }
      ptr += xBrickSize * bytesPerVoxel;
   }

   close(fd);
   return 0;
}


//-----------------------------------------------------------------------------
// myReadBrickRaw() reads a 3D subvolume of voxels with origin at
// [xyz]BrickOrigin and of size [xyz]BrickSize from a volume of size
// [xyz]VolumeSize represented as a stack of 2D images.  Application
// needs to specify the list of file names, header length and the number
// of bytes per pixel.

int myReadBrickRaw(char **fileNames, void *data,	/* DataType */
		   int xBrickOrigin, int yBrickOrigin, int zBrickOrigin,
		   int xBrickSize, int yBrickSize, int zBrickSize,
		   int xVolumeSize, int yVolumeSize, int zVolumeSize,
		   int headerLength, int bytesPerVoxel)
{

   assert(xBrickOrigin >= 0 && xBrickOrigin < xVolumeSize);
   assert(yBrickOrigin >= 0 && yBrickOrigin < yVolumeSize);
   assert(zBrickOrigin >= 0 && zBrickOrigin < zVolumeSize);

   char *ptr = (char *) data;	/* DataType */

   // is this a full brick?
   if ((xBrickOrigin + xBrickSize <= xVolumeSize) &&
       (yBrickOrigin + yBrickSize <= yVolumeSize) &&
       (zBrickOrigin + zBrickSize <= zVolumeSize)) {
      for (int iz = zBrickOrigin; iz < (zBrickOrigin + zBrickSize); iz++) {
         if (getTile(fileNames[iz], ptr, xBrickOrigin, yBrickOrigin,
            xBrickSize, yBrickSize, xVolumeSize, yVolumeSize,
            headerLength, bytesPerVoxel) != 0) {
            fprintf(stderr, "ERROR: %s(): Unable to read a tile.\n", "myReadBrickRaw");
            return -1;
         }
         ptr += xBrickSize * yBrickSize * bytesPerVoxel;

      }			// iz

   } else {			// deal with partial bricks
      int xPartBrickSize, yPartBrickSize, zPartBrickSize;

      xPartBrickSize = xBrickOrigin + xBrickSize <= xVolumeSize ?
         xBrickSize : xVolumeSize - xBrickOrigin;
      yPartBrickSize = yBrickOrigin + yBrickSize <= yVolumeSize ?
         yBrickSize : yVolumeSize - yBrickOrigin;
      zPartBrickSize = zBrickOrigin + zBrickSize <= zVolumeSize ?
         zBrickSize : zVolumeSize - zBrickOrigin;

      char *buffer =		/* DataType */
         new char[xBrickSize * yBrickSize * zBrickSize * bytesPerVoxel];

      assert(buffer != NULL);

      ptr = buffer;

      fprintf(stderr, "|\b");
      for (int iz = zBrickOrigin; iz < (zBrickOrigin + zPartBrickSize); iz++) {
         if (getTile(fileNames[iz], ptr, xBrickOrigin, yBrickOrigin,
            xPartBrickSize, yPartBrickSize, xVolumeSize, yVolumeSize,
            headerLength, bytesPerVoxel) != 0) {
            fprintf(stderr, "%s(): Unable to read a tile.\n", "myReadBrickRaw");
            return -1;
         }
         ptr += xPartBrickSize * yPartBrickSize * bytesPerVoxel;
      }			// iz

      fprintf(stderr, "/\b");

      // KLUDGE does not set all the borders
      for (int j0 = 0; j0 < zPartBrickSize; j0++)
         for (int j1 = 0; j1 < yPartBrickSize; j1++) {
            memcpy((char *)data + (j0 * xBrickSize * yBrickSize + j1 * xBrickSize) * \
               bytesPerVoxel, \
               buffer + (j0 * xPartBrickSize * yPartBrickSize + j1 * xPartBrickSize) * \
               bytesPerVoxel, xPartBrickSize);
            memset(buffer + (j0 * xPartBrickSize * yPartBrickSize + \
               j1 * xPartBrickSize + xPartBrickSize) * bytesPerVoxel, \
               0, (xBrickSize - xPartBrickSize) * bytesPerVoxel);
         }

      delete[]buffer;
      fprintf(stderr, "-\b");

   }
   return 0;
}
