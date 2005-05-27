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
/** class definition SoMD2NodeKit
  *
  * A Node that displays an MD2 mesh
  *
  * @author  Daniel Wagner
  *
  * $Id: SoMD2NodeKit.h 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFRotation.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/sensors/SoFieldSensor.h>


// applications that don't want to use Studierstube should
// define NO_STAPI in order to compile this...
#ifdef NO_STAPI
#define STBAPI_API
#else
#include <stbapi/stbapi.h>
#endif


#ifndef __SOMD2NODEKIT_HEADERFILE__
#define __SOMD2NODEKIT_HEADERFILE__


/// SoMD2NodeKit displays and animated md2 quake2 model files
/**
 *  SoMD2NodeKit allows to import and display md2 files. md2 is
 *  the model file format for quake2 characters (player models)
 *  A large number of free models can be found at
 *  http://www.planetquake.com/polycount
 *
 *  An md2 model consists of a number of frames, each having
 *  a name. Frames with the same name having a different number
 *  added at the end of the name string form an animation.
 *  Typical animations are 'run', 'stand', 'jump'
 *
 *  In order to preview model files you should get an md2 viewer
 *  such as 'MD2 Viewer'
 * @ingroup util
 */
class STBAPI_API SoMD2NodeKit : public SoBaseKit
{
    SO_KIT_HEADER(SoMD2NodeKit);

    // Defines fields for the new parts in the catalog
    SO_KIT_CATALOG_ENTRY_HEADER(mainSep);
    SO_KIT_CATALOG_ENTRY_HEADER(indexedFaceSet);
    SO_KIT_CATALOG_ENTRY_HEADER(normals);
    SO_KIT_CATALOG_ENTRY_HEADER(coords);
    SO_KIT_CATALOG_ENTRY_HEADER(texCoords);
	SO_KIT_CATALOG_ENTRY_HEADER(texture2);
    SO_KIT_CATALOG_ENTRY_HEADER(complexity);

    SO_KIT_CATALOG_ENTRY_HEADER(shapeHints);
    SO_KIT_CATALOG_ENTRY_HEADER(normalBinding);
    SO_KIT_CATALOG_ENTRY_HEADER(texCoordBinding);

    SO_KIT_CATALOG_ENTRY_HEADER(light2);
    SO_KIT_CATALOG_ENTRY_HEADER(light1);
    SO_KIT_CATALOG_ENTRY_HEADER(transform);

public:
    enum {  MAX_ANIMS = 32  };

    // Constructor
    //
    SoMD2NodeKit();


	//
    // attributes
    //

	/// Allows to change the models (relative) position
    SoSFVec3f       position;

	/// Allows to change the models (relative) rotation
    SoSFRotation    rotation;

	/// Allows to set a scaling factor
    SoSFVec3f       scaleFactor;

	/// Sets the name of the md2 file to use
    SoSFString      meshname;

	/// Sets the name of the texture to use
	/**
	 *  Unfortunatelly OI currently does not support
	 *  TGA files which are usually supplied for md2 skins
	 *  Convert the skin to jpg or gif. If the skin does not
	 *  look correctly try flipping it horizontaly
	 */
    SoSFString      skinname;

	/// Specifies the name of the animation to play.
	/**
	 *  Animations currently switch immediatelly.
	 */
    SoSFString      animname;

	/// Sets the number of seconds the current animation takes
	/**
	 *  After the time is over the animation is looped.
	 */
    SoSFFloat       animtime;


    // animation playing methods
    //

	/// Plays an animation
    bool play(const char* nAnimName, double nPlayTime);

	/// Plays an animation from frame nFrom to nTo
    bool play(int nFrom, int nTo, double nFrameTime=0);

	/// Stops playing animations
    bool stop();

	/// Returns the number of animations of the md2
	int getNumAnimations() const  {  return numAnims;  }

	/// Returns the name of specific animaion
	/**
	 *  nIndex must be in the range [0..getNumAnimations()]
	 */
	const SbString& getAnimName(int nIndex) const  {  return anims[nIndex].name;  }


	/// Sets the heading of the rotation component
    /**
	 *  This methods assume that the local coordinate system uses only 
     *  a roation around axis [0 1 0] (as quake models usually do...)
	 */
    void setHeading(double nDir);

	/// Returns the heading of the rotation component
    double getHeading() const;


SoINTERNAL public:
    static void initClass();

protected:
    bool loadMD2(const char* nMeshName, double nScale=1.0);
    void applyNewMD2();

    void applyGeometry();
    void update();


private:
    // Destructor
    virtual ~SoMD2NodeKit();


    // Sensors plus callbacks
    //
    SoFieldSensor* positionSensor;
    SoFieldSensor* rotationSensor;
    SoFieldSensor* scaleSensor;
    SoFieldSensor* timeSensor;
    SoFieldSensor* meshnameSensor;
    SoFieldSensor* skinnameSensor;
    SoFieldSensor* animnameSensor;
    SoFieldSensor* animtimeSensor;

    static void positionCB(void* data, SoSensor* sensor);
    static void rotationCB(void* data, SoSensor* sensor);
    static void scaleCB(void* data, SoSensor* sensor);
    static void timeChangedCB(void *data, SoSensor* nSensor);
    static void meshnameCB(void* data, SoSensor* sensor);
    static void skinnameCB(void* data, SoSensor* sensor);
    static void animnameCB(void* data, SoSensor* sensor);
    static void animtimeCB(void* data, SoSensor* sensor);


    // MD2 core stuff
    //
	struct _md2vertex
    {
        float   px,py,pz,
                nx,ny,nz;

        void set(float nPx, float nPy, float nPz, float nNx, float nNy, float nNz)
        {  px = nPx;  py = nPy;  pz = nPz;  nx = nNx;  ny = nNy;  nz = nNz;  }
    };

    typedef _md2vertex* _md2vertexPtr;

public:
    struct _md2anim
    {
        SbString    name;
        int         playFrom, playTo;
    };

protected:
    int                     numFrames, numTriangles, numVertices, numTexCoords;
    _md2vertexPtr*          md2Frames;

    _md2anim                anims[MAX_ANIMS];
    int                     numAnims;


	int						*vertexIndices,
							*textureIndices;

    SbVec3f*                vPos;
    SbVec3f*                vNorm;
    SbVec2f*                vTex;


    // animation handling stuff
    //
    double                  curFrame;
    bool                    isPlaying;
    int                     playFrom, playTo, playNumFrames;
    double                  startTime;
};

#endif //__SOMD2NODEKIT_HEADERFILE__
