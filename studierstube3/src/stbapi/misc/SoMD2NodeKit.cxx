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
/** implementation SoMD2NodeKit
  *
  * A Node that displays an MD2 mesh
  *
  * @author  Daniel Wagner
  *
  * $Id: SoMD2NodeKit.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifdef WIN32
  #include <windows.h>
// we need this for defining min !!!??
#endif

#include <Inventor/SoDB.h>
#include <Inventor/fields/SoSFTime.h>
#include <Inventor/fields/SoSFNode.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoComplexity.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoEnvironment.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoLightModel.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoNormal.h>
#include <Inventor/nodes/SoNormalBinding.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/nodes/SoTextureCoordinate2.h>
#include <Inventor/nodes/SoTextureCoordinateBinding.h>
#include <Inventor/nodes/SoTransform.h>

#include <assert.h>
#include <string.h>
#include <algorithm>

#include "SoMD2NodeKit.h"

using namespace std;

SO_KIT_SOURCE(SoMD2NodeKit);

void
SoMD2NodeKit::initClass()
{
    SO_KIT_INIT_CLASS(SoMD2NodeKit, SoBaseKit, "BaseKit");
}


// Constructor
SoMD2NodeKit::SoMD2NodeKit()
{
    SO_KIT_CONSTRUCTOR(SoMD2NodeKit);
    isBuiltIn = TRUE;

    // setup sub nodes
    //
    SO_KIT_ADD_CATALOG_ENTRY(mainSep, SoSeparator, TRUE, this, \0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(transform, SoTransform, TRUE, mainSep, \0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(light1, SoDirectionalLight, TRUE, mainSep, \0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(light2, SoDirectionalLight, TRUE, mainSep, \0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(normals, SoNormal, TRUE, mainSep, \0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(coords, SoCoordinate3, TRUE, mainSep, \0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(texCoords, SoTextureCoordinate2, TRUE, mainSep, \0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(complexity, SoComplexity, TRUE, mainSep, \0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(texture2, SoTexture2, TRUE, mainSep, \0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(shapeHints, SoShapeHints, TRUE, mainSep, \0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(normalBinding, SoNormalBinding, TRUE, mainSep, \0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(texCoordBinding, SoTextureCoordinateBinding, TRUE, mainSep, \0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(indexedFaceSet, SoIndexedFaceSet, TRUE, mainSep, \0, FALSE);

    SO_KIT_INIT_INSTANCE();

    setPart("mainSep",new SoSeparator);
    setPart("transform",new SoTransform);
    setPart("light1"  ,new SoDirectionalLight);
    setPart("light2"  ,new SoDirectionalLight);
    setPart("normals"  ,new SoNormal);
    setPart("coords"  ,new SoCoordinate3);
    setPart("texCoords"  ,new SoTextureCoordinate2);
    setPart("texture2",new SoTexture2);
    setPart("complexity",new SoComplexity);
    setPart("shapeHints",new SoShapeHints);
    setPart("normalBinding"  ,new SoNormalBinding);
    setPart("texCoordBinding"  ,new SoTextureCoordinateBinding);
    setPart("indexedFaceSet"  ,new SoIndexedFaceSet);


    SO_GET_ANY_PART(this, "light1", SoDirectionalLight)->direction.setValue(0, 1, -1);
    SO_GET_ANY_PART(this, "light2", SoDirectionalLight)->direction.setValue(1, 0,  0);
    SO_GET_ANY_PART(this, "normalBinding", SoNormalBinding)->value = SoNormalBinding::PER_VERTEX_INDEXED;
    SO_GET_ANY_PART(this, "texCoordBinding", SoTextureCoordinateBinding)->value = SoTextureCoordinateBinding::PER_VERTEX_INDEXED;
    SO_GET_ANY_PART(this, "complexity", SoComplexity)->textureQuality = 1.0;



    // setup fields, sensors and callbacks
    //
    SO_NODE_ADD_FIELD(position,(0,0,0));
    SO_NODE_ADD_FIELD(rotation,(SbRotation::identity()));
    SO_NODE_ADD_FIELD(scaleFactor,(1,1,1));
    SO_NODE_ADD_FIELD(meshname,(""));
    SO_NODE_ADD_FIELD(skinname,(""));
    SO_NODE_ADD_FIELD(animname,(""));
    SO_NODE_ADD_FIELD(animtime, (0));

    timeSensor = new SoFieldSensor(timeChangedCB, this);
    positionSensor = new SoFieldSensor(positionCB, this);
    rotationSensor = new SoFieldSensor(rotationCB, this);
    scaleSensor = new SoFieldSensor(scaleCB, this);
    meshnameSensor = new SoFieldSensor(meshnameCB, this);
    meshnameSensor->setPriority(0);
    skinnameSensor = new SoFieldSensor(skinnameCB, this);
    skinnameSensor->setPriority(0);
    animnameSensor = new SoFieldSensor(animnameCB, this);
    animnameSensor->setPriority(0);
    animtimeSensor = new SoFieldSensor(animtimeCB, this);
    animtimeSensor->setPriority(0);

    positionSensor->attach(&position);
    rotationSensor->attach(&rotation);
    scaleSensor->attach(&scaleFactor);
    timeSensor->attach(SoDB::getGlobalField("realTime"));
    meshnameSensor->attach(&meshname);
    skinnameSensor->attach(&skinname);
    animnameSensor->attach(&animname);
    animtimeSensor->attach(&animtime);


    // setup MD2 info structures
    //
    vNorm = NULL;
    vPos = NULL;
    vTex = NULL;
    md2Frames = NULL;
    numTriangles = numVertices = numTexCoords = numFrames = 0;

    vertexIndices = NULL;
    textureIndices = NULL;

    playFrom = playTo = 0;
    playNumFrames = 1;
    isPlaying = false;
    curFrame = 0;
}


// Destructor
SoMD2NodeKit::~SoMD2NodeKit()
{
    positionSensor->detach();
    rotationSensor->detach();
    scaleSensor->detach();
    timeSensor->detach();
    meshnameSensor->detach();
    skinnameSensor->detach();
    animnameSensor->detach();
    animtimeSensor->detach();

    delete positionSensor;
    delete rotationSensor;
    delete scaleSensor;
    delete timeSensor;
    delete meshnameSensor;
    delete skinnameSensor;
    delete animnameSensor;
    delete animtimeSensor;
}


////////////////////////////////////////////////////////////////////////////////
//
//                               CALLBACK METHODS
//
void
SoMD2NodeKit::timeChangedCB(void* data, SoSensor* )
{
    assert(data);
    SoMD2NodeKit* self = (SoMD2NodeKit*)data;

    self->update();
}


void
SoMD2NodeKit::positionCB(void* data, SoSensor* sensor)
{
    SoMD2NodeKit* self = (SoMD2NodeKit*)data;
    SoTransform* trans = SO_GET_ANY_PART(self,"transform",SoTransform);
    assert(trans);

    trans->translation.setValue(self->position.getValue());
}


void
SoMD2NodeKit::rotationCB(void* data, SoSensor* sensor)
{
    SoMD2NodeKit* self = (SoMD2NodeKit*)data;
    SoTransform* trans = SO_GET_ANY_PART(self,"transform",SoTransform);
    assert(trans);

    trans->rotation.setValue(self->rotation.getValue());
}


void
SoMD2NodeKit::scaleCB(void* data, SoSensor* sensor)
{
    SoMD2NodeKit* self = (SoMD2NodeKit*)data;
    SoTransform* trans = SO_GET_ANY_PART(self,"transform",SoTransform);
    assert(trans);

    trans->scaleFactor.setValue(self->scaleFactor.getValue());
}


void
SoMD2NodeKit::meshnameCB(void* data, SoSensor* sensor)
{
    SoMD2NodeKit* self = (SoMD2NodeKit*)data;

    self->loadMD2(self->meshname.getValue().getString());
    self->applyNewMD2();
    self->curFrame = 0;
}


void
SoMD2NodeKit::skinnameCB(void* data, SoSensor* sensor)
{
    SoMD2NodeKit* self = (SoMD2NodeKit*)data;
    SoTexture2* texture = SO_GET_ANY_PART(self,"texture2",SoTexture2);
    assert(texture);

    texture->filename.setValue(self->skinname.getValue().getString());
}


void
SoMD2NodeKit::animnameCB(void* data, SoSensor* sensor)
{
    SoMD2NodeKit* self = (SoMD2NodeKit*)data;

    self->isPlaying = false;
}


void
SoMD2NodeKit::animtimeCB(void* data, SoSensor* sensor)
{
    SoMD2NodeKit* self = (SoMD2NodeKit*)data;

    self->isPlaying = false;
}



////////////////////////////////////////////////////////////////////////////////
//
//                               MEMBER METHODS
//
void
SoMD2NodeKit::update()
{
    if(animname.getValue()!="" && !isPlaying)
    {
        for(int i=0; i<numAnims; i++)
            if(anims[i].name==animname.getValue())
            {
                play(anims[i].playFrom, anims[i].playTo);
                break;
            }
    }

    if(!isPlaying)
        return;


    // get the real time as double in seconds
    //
    SoSFTime*       timeField = (SoSFTime*)SoDB::getGlobalField("realTime");
    const SbTime&   time = timeField->getValue();
    double          curTime = time.getValue();
    double          dt = curTime-startTime;

    while(dt>=animtime.getValue())
    {
        startTime += animtime.getValue();
        dt = curTime-startTime;
    }

     double relDt = dt / animtime.getValue();

    assert(relDt<1.0);

    curFrame = playFrom + relDt * playNumFrames;
    assert(curFrame>=0 && curFrame<numFrames);

    applyGeometry();
}

bool
SoMD2NodeKit::play(const char* nanimname, double nPlayTime)
{
    animname = nanimname;
    animtime =(float) nPlayTime;
    return true;
}


bool
SoMD2NodeKit::play(int nFrom, int nTo, double nPlayTime)
{
    if(!md2Frames)
        return false;

    if(nPlayTime!=0)
        animtime =(float) nPlayTime;
    playFrom = nFrom;
    playTo = nTo;
    playNumFrames = playTo-playFrom+1;
    startTime = ((SoSFTime*)SoDB::getGlobalField("realTime"))->getValue().getValue();
    isPlaying = true;

    return true;
}


bool
SoMD2NodeKit::stop()
{
    isPlaying = false;
    return true;
}


void
SoMD2NodeKit::setHeading(double nDir)
{
    rotation.setValue(SbVec3f(0, 1, 0),(float) nDir);
}


double
SoMD2NodeKit::getHeading() const
{
    SbVec3f axis(0, 1, 0);
    float   angle;

    rotation.getValue().getValue(axis, angle);
    return angle;
}


void
SoMD2NodeKit::applyGeometry()
{
    int i, fIdx0 = (int)(curFrame), fIdx1 = fIdx0+1;
    double fract = curFrame-fIdx0, fractR = 1-fract;

    if(fIdx1>playTo)
        fIdx1 = playFrom;

    const _md2vertex    *f0 = md2Frames[fIdx0],
                        *f1 = md2Frames[fIdx1];

    // prepare data by updating all vertex and normal data
    //
	for(i=0; i<numVertices; i++)
    {
        vPos[i].setValue((float)(fractR*f0[i].px + fract*f1[i].px),
                         (float)(fractR*f0[i].py + fract*f1[i].py),
                         (float)(fractR*f0[i].pz + fract*f1[i].pz));


        vNorm[i].setValue((float)(fractR*f0[i].nx + fract*f1[i].nx),
                          (float)(fractR*f0[i].ny + fract*f1[i].ny),
                          (float)(fractR*f0[i].nz + fract*f1[i].nz));
    }


    // set data
    //
    SoCoordinate3* _coords = SO_GET_ANY_PART(this, "coords", SoCoordinate3);
    SoNormal* _normals = SO_GET_ANY_PART(this, "normals", SoNormal);

    assert(_coords);
    assert(_normals);

    _coords->point.setValues(0, numVertices, vPos);
    _normals->vector.setValues(0, numVertices, vNorm);
}


void
SoMD2NodeKit::applyNewMD2()
{
    if(numTriangles<=0)
        return;

    // create vertex data arrays
    //
    if(vNorm)
        delete vNorm;
    if(vPos)
        delete vPos;
    //if(vTex)
    //    delete vTex;

    vNorm = new SbVec3f[3*numTriangles];
    vPos = new SbVec3f[3*numTriangles];


    SoTextureCoordinate2* _texcoords = SO_GET_ANY_PART(this, "texCoords", SoTextureCoordinate2);
    _texcoords->point.setValues(0, numTexCoords, vTex);


    // fill faceset data
    //
    SoIndexedFaceSet* _indexedFaceSet = SO_GET_ANY_PART(this, "indexedFaceSet", SoIndexedFaceSet);
    _indexedFaceSet->coordIndex.setValues(0, 4*numTriangles, vertexIndices);
    _indexedFaceSet->textureCoordIndex.setValues(0, 4*numTriangles, textureIndices);


    // fill geometry for the first time
    //
    applyGeometry();
}


/////////////////////////////////////////////////////////////////////////////////////////////
//
//                                  CORE MD2 DATA + METHODS
//

static float    md2Normals[162][3] = 
{
{-0.525731f, 0.000000f, 0.850651f}, 
{-0.442863f, 0.238856f, 0.864188f}, 
{-0.295242f, 0.000000f, 0.955423f}, 
{-0.309017f, 0.500000f, 0.809017f}, 
{-0.162460f, 0.262866f, 0.951056f}, 
{0.000000f, 0.000000f, 1.000000f}, 
{0.000000f, 0.850651f, 0.525731f}, 
{-0.147621f, 0.716567f, 0.681718f}, 
{0.147621f, 0.716567f, 0.681718f}, 
{0.000000f, 0.525731f, 0.850651f}, 
{0.309017f, 0.500000f, 0.809017f}, 
{0.525731f, 0.000000f, 0.850651f}, 
{0.295242f, 0.000000f, 0.955423f}, 
{0.442863f, 0.238856f, 0.864188f}, 
{0.162460f, 0.262866f, 0.951056f}, 
{-0.681718f, 0.147621f, 0.716567f}, 
{-0.809017f, 0.309017f, 0.500000f}, 
{-0.587785f, 0.425325f, 0.688191f}, 
{-0.850651f, 0.525731f, 0.000000f}, 
{-0.864188f, 0.442863f, 0.238856f}, 
{-0.716567f, 0.681718f, 0.147621f}, 
{-0.688191f, 0.587785f, 0.425325f}, 
{-0.500000f, 0.809017f, 0.309017f}, 
{-0.238856f, 0.864188f, 0.442863f}, 
{-0.425325f, 0.688191f, 0.587785f}, 
{-0.716567f, 0.681718f, -0.147621f}, 
{-0.500000f, 0.809017f, -0.309017f}, 
{-0.525731f, 0.850651f, 0.000000f}, 
{0.000000f, 0.850651f, -0.525731f}, 
{-0.238856f, 0.864188f, -0.442863f}, 
{0.000000f, 0.955423f, -0.295242f}, 
{-0.262866f, 0.951056f, -0.162460f}, 
{0.000000f, 1.000000f, 0.000000f}, 
{0.000000f, 0.955423f, 0.295242f}, 
{-0.262866f, 0.951056f, 0.162460f}, 
{0.238856f, 0.864188f, 0.442863f}, 
{0.262866f, 0.951056f, 0.162460f}, 
{0.500000f, 0.809017f, 0.309017f}, 
{0.238856f, 0.864188f, -0.442863f}, 
{0.262866f, 0.951056f, -0.162460f}, 
{0.500000f, 0.809017f, -0.309017f}, 
{0.850651f, 0.525731f, 0.000000f}, 
{0.716567f, 0.681718f, 0.147621f}, 
{0.716567f, 0.681718f, -0.147621f}, 
{0.525731f, 0.850651f, 0.000000f}, 
{0.425325f, 0.688191f, 0.587785f}, 
{0.864188f, 0.442863f, 0.238856f}, 
{0.688191f, 0.587785f, 0.425325f}, 
{0.809017f, 0.309017f, 0.500000f}, 
{0.681718f, 0.147621f, 0.716567f}, 
{0.587785f, 0.425325f, 0.688191f}, 
{0.955423f, 0.295242f, 0.000000f}, 
{1.000000f, 0.000000f, 0.000000f}, 
{0.951056f, 0.162460f, 0.262866f}, 
{0.850651f, -0.525731f, 0.000000f}, 
{0.955423f, -0.295242f, 0.000000f}, 
{0.864188f, -0.442863f, 0.238856f}, 
{0.951056f, -0.162460f, 0.262866f}, 
{0.809017f, -0.309017f, 0.500000f}, 
{0.681718f, -0.147621f, 0.716567f}, 
{0.850651f, 0.000000f, 0.525731f}, 
{0.864188f, 0.442863f, -0.238856f}, 
{0.809017f, 0.309017f, -0.500000f}, 
{0.951056f, 0.162460f, -0.262866f}, 
{0.525731f, 0.000000f, -0.850651f}, 
{0.681718f, 0.147621f, -0.716567f}, 
{0.681718f, -0.147621f, -0.716567f}, 
{0.850651f, 0.000000f, -0.525731f}, 
{0.809017f, -0.309017f, -0.500000f}, 
{0.864188f, -0.442863f, -0.238856f}, 
{0.951056f, -0.162460f, -0.262866f}, 
{0.147621f, 0.716567f, -0.681718f}, 
{0.309017f, 0.500000f, -0.809017f}, 
{0.425325f, 0.688191f, -0.587785f}, 
{0.442863f, 0.238856f, -0.864188f}, 
{0.587785f, 0.425325f, -0.688191f}, 
{0.688191f, 0.587785f, -0.425325f}, 
{-0.147621f, 0.716567f, -0.681718f}, 
{-0.309017f, 0.500000f, -0.809017f}, 
{0.000000f, 0.525731f, -0.850651f}, 
{-0.525731f, 0.000000f, -0.850651f}, 
{-0.442863f, 0.238856f, -0.864188f}, 
{-0.295242f, 0.000000f, -0.955423f}, 
{-0.162460f, 0.262866f, -0.951056f}, 
{0.000000f, 0.000000f, -1.000000f}, 
{0.295242f, 0.000000f, -0.955423f}, 
{0.162460f, 0.262866f, -0.951056f}, 
{-0.442863f, -0.238856f, -0.864188f}, 
{-0.309017f, -0.500000f, -0.809017f}, 
{-0.162460f, -0.262866f, -0.951056f}, 
{0.000000f, -0.850651f, -0.525731f}, 
{-0.147621f, -0.716567f, -0.681718f}, 
{0.147621f, -0.716567f, -0.681718f}, 
{0.000000f, -0.525731f, -0.850651f}, 
{0.309017f, -0.500000f, -0.809017f}, 
{0.442863f, -0.238856f, -0.864188f}, 
{0.162460f, -0.262866f, -0.951056f}, 
{0.238856f, -0.864188f, -0.442863f}, 
{0.500000f, -0.809017f, -0.309017f}, 
{0.425325f, -0.688191f, -0.587785f}, 
{0.716567f, -0.681718f, -0.147621f}, 
{0.688191f, -0.587785f, -0.425325f}, 
{0.587785f, -0.425325f, -0.688191f}, 
{0.000000f, -0.955423f, -0.295242f}, 
{0.000000f, -1.000000f, 0.000000f}, 
{0.262866f, -0.951056f, -0.162460f}, 
{0.000000f, -0.850651f, 0.525731f}, 
{0.000000f, -0.955423f, 0.295242f}, 
{0.238856f, -0.864188f, 0.442863f}, 
{0.262866f, -0.951056f, 0.162460f}, 
{0.500000f, -0.809017f, 0.309017f}, 
{0.716567f, -0.681718f, 0.147621f}, 
{0.525731f, -0.850651f, 0.000000f}, 
{-0.238856f, -0.864188f, -0.442863f}, 
{-0.500000f, -0.809017f, -0.309017f}, 
{-0.262866f, -0.951056f, -0.162460f}, 
{-0.850651f, -0.525731f, 0.000000f}, 
{-0.716567f, -0.681718f, -0.147621f}, 
{-0.716567f, -0.681718f, 0.147621f}, 
{-0.525731f, -0.850651f, 0.000000f}, 
{-0.500000f, -0.809017f, 0.309017f}, 
{-0.238856f, -0.864188f, 0.442863f}, 
{-0.262866f, -0.951056f, 0.162460f}, 
{-0.864188f, -0.442863f, 0.238856f}, 
{-0.809017f, -0.309017f, 0.500000f}, 
{-0.688191f, -0.587785f, 0.425325f}, 
{-0.681718f, -0.147621f, 0.716567f}, 
{-0.442863f, -0.238856f, 0.864188f}, 
{-0.587785f, -0.425325f, 0.688191f}, 
{-0.309017f, -0.500000f, 0.809017f}, 
{-0.147621f, -0.716567f, 0.681718f}, 
{-0.425325f, -0.688191f, 0.587785f}, 
{-0.162460f, -0.262866f, 0.951056f}, 
{0.442863f, -0.238856f, 0.864188f}, 
{0.162460f, -0.262866f, 0.951056f}, 
{0.309017f, -0.500000f, 0.809017f}, 
{0.147621f, -0.716567f, 0.681718f}, 
{0.000000f, -0.525731f, 0.850651f}, 
{0.425325f, -0.688191f, 0.587785f}, 
{0.587785f, -0.425325f, 0.688191f}, 
{0.688191f, -0.587785f, 0.425325f}, 
{-0.955423f, 0.295242f, 0.000000f}, 
{-0.951056f, 0.162460f, 0.262866f}, 
{-1.000000f, 0.000000f, 0.000000f}, 
{-0.850651f, 0.000000f, 0.525731f}, 
{-0.955423f, -0.295242f, 0.000000f}, 
{-0.951056f, -0.162460f, 0.262866f}, 
{-0.864188f, 0.442863f, -0.238856f}, 
{-0.951056f, 0.162460f, -0.262866f}, 
{-0.809017f, 0.309017f, -0.500000f}, 
{-0.864188f, -0.442863f, -0.238856f}, 
{-0.951056f, -0.162460f, -0.262866f}, 
{-0.809017f, -0.309017f, -0.500000f}, 
{-0.681718f, 0.147621f, -0.716567f}, 
{-0.681718f, -0.147621f, -0.716567f}, 
{-0.850651f, 0.000000f, -0.525731f}, 
{-0.688191f, 0.587785f, -0.425325f}, 
{-0.587785f, 0.425325f, -0.688191f}, 
{-0.425325f, 0.688191f, -0.587785f}, 
{-0.425325f, -0.688191f, -0.587785f}, 
{-0.587785f, -0.425325f, -0.688191f}, 
{-0.688191f, -0.587785f, -0.425325f}, 
};


typedef struct
{
   int magic;
   int version;
   int skinWidth;
   int skinHeight;
   int frameSize;
   int numSkins;
   int numVertices;
   int numTexCoords;
   int numTriangles;
   int numGlCommands;
   int numFrames;
   int offsetSkins;
   int offsetTexCoords;
   int offsetTriangles;
   int offsetFrames;
   int offsetGlCommands;
   int offsetEnd;
} model_t;


typedef struct
{
   unsigned char vertex[3];
   unsigned char lightNormalIndex;
} triangleVertex_t;


typedef struct
{
   float scale[3];
   float translate[3];
   char name[16];
   triangleVertex_t vertices[1];
} frame_t;


typedef struct
{
   short vertexIndices[3];
   short textureIndices[3];
} triangle_t;


typedef struct
{
   short s, t;
} textureCoordinate_t;



static int
calcLastDigits(const char* nString)
{
    int i, num=strlen(nString);

    for(i=num-1; i>=0; i--)
        if(nString[i]<'0' || nString[i]>'9')
            return num-i-1;

    return num-1;
}


static const frame_t*
getFrameFromRawFrames(const char* nRawFrames, int nIdx, int nNumVertices)
{
    return (frame_t*)(nRawFrames + nIdx * (40 + nNumVertices*sizeof(triangleVertex_t)));
}


static int
fillAnims(const char* nRawFrames, int nNumVertices, int nNumFrames, SoMD2NodeKit::_md2anim *anims)
{
    int         i, numAnims=-1;
    char        curAnimName[256], curFrameName[256];

    strcpy(curAnimName, "_");
    
    for(i=0; i<nNumFrames; i++)
    {
        const frame_t* f = getFrameFromRawFrames(nRawFrames, i, nNumVertices);
        strcpy(curFrameName, f->name);
        int numDig = calcLastDigits(f->name);
		if(numDig>2)
			numDig = 2;
        curFrameName[strlen(curFrameName)-numDig] = 0;

        if(strncmp(curAnimName, curFrameName, min(strlen(curAnimName), strlen(curFrameName))))
        {
            if(numAnims>=0)
                anims[numAnims].playTo = i-1;
            numAnims++;
            anims[numAnims].playFrom=i;
            anims[numAnims].name = curFrameName;
            strcpy(curAnimName, curFrameName);
        }
    }

    anims[numAnims].playTo = i-1;
    numAnims++;
    return numAnims;
}


bool
SoMD2NodeKit::loadMD2(const char* nMeshName, double nScale)
{
    int                     i, fnr,
                            rawSize, read;
    unsigned char*          rawData;

    FILE* fp = fopen(nMeshName, "rb");

    if(!fp)
    {
        printf("ERROR: cannot open md2 file '%s'\n", nMeshName);
        return false;
    }

    fseek(fp, 0, SEEK_END);
    rawSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    rawData = new unsigned char[rawSize];
    read = fread(rawData, 1, rawSize, fp);

    if(read != rawSize)
    {
        printf("ERROR: reading md2 file '%s' failed\n", nMeshName);
        delete rawData;
        return false;
    }


    // read through the raw data
    //
    model_t*                header = (model_t*)rawData;
    textureCoordinate_t*    texcoords = (textureCoordinate_t*)(rawData+header->offsetTexCoords);
    triangle_t*             triangles = (triangle_t*)(rawData+header->offsetTriangles);
    frame_t*                frames = (frame_t*)(rawData+header->offsetFrames);
    char*                   rawFrames = (char*)frames;

    float                   fSkinW = (float)header->skinWidth;
    float                   fSkinH = (float)header->skinHeight;

    float                   scaling = (float)(nScale * 0.008 * 0.4);

    numVertices = header->numVertices;
    numTexCoords = header->numTexCoords;
    numFrames = header->numFrames;
    numTriangles = header->numTriangles;
    md2Frames = new _md2vertexPtr[numFrames];


    // fill vertex + texcoord index lists
    //
    if(vertexIndices)
        delete vertexIndices;
    vertexIndices = new int[4*numTriangles];

    if(textureIndices)
        delete textureIndices;
    textureIndices = new int[4*numTriangles];

    int *v = vertexIndices, *t = textureIndices;

    for(i=0; i<numTriangles; i++)
    {
        *(v++) = triangles[i].vertexIndices[0];
        *(v++) = triangles[i].vertexIndices[1];
        *(v++) = triangles[i].vertexIndices[2];
        *(v++) = SO_END_FACE_INDEX;

        *(t++) = triangles[i].textureIndices[0];
        *(t++) = triangles[i].textureIndices[1];
        *(t++) = triangles[i].textureIndices[2];
        *(t++) = SO_END_FACE_INDEX;
    }


    // fill texture coord list (there is only one set of texture
    // coords since they don't change during the animation)
    //
    if(vTex)
        delete vTex;
    vTex = new SbVec2f[numTexCoords];

    for(i=0; i<numTexCoords; i++)
    {
        float   u = (float)(texcoords[i].s) / fSkinW;
        float   v = (float)(texcoords[i].t) / fSkinH;

        vTex[i].setValue(u,v);
    }


	// calculate the animation names plus start & end frame indices
	//
    numAnims = fillAnims(rawFrames, numVertices, numFrames, anims);


    // fill all vertices from all frames into md2Frames
    //
    for(fnr=0; fnr<numFrames; fnr++)
    {
        frame_t*            frame = (frame_t*)(rawFrames + fnr * (40 + numVertices*sizeof(triangleVertex_t)));
        triangleVertex_t*   vertices = frame->vertices;

        md2Frames[fnr] = new _md2vertex[numVertices];
        _md2vertex* f = md2Frames[fnr];

        // constant vertex scaling for this frame
        //
        float sx = frame->scale[0];
        float sy = frame->scale[1];
        float sz = frame->scale[2];

        // constant vertex translation for this frame
        //
        float dx = frame->translate[0];
        float dy = frame->translate[1];
        float dz = frame->translate[2];

        for(i=0; i<numVertices; i++)
        {
            // position
            //
            float x = scaling*(dx + vertices[i].vertex[0] * sx);
            float z = scaling*(dy + vertices[i].vertex[1] * sy);
            float y = scaling*(dz + vertices[i].vertex[2] * sz);

            // normal
            //
            int   n = vertices[i].lightNormalIndex;

            float nx = md2Normals[n][0];
            float ny = md2Normals[n][1];
            float nz = md2Normals[n][2];

            // store it together
            //
            f[i].set(x, y, z, nx, ny, nz);
        }

    }

    delete rawData;

    printf("INFO: MD2 read from file '%s'\n", nMeshName);
    return true;
}
