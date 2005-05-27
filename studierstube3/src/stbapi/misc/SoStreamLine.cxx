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
/** Implementation of SoStreamLine class
  *
  * @author Clemens Pecinovsky, Jan Prikryl
  *
  * $Id: SoStreamLine.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <stbapi/misc/SoStreamLine.h>
#include <Inventor/actions/SoWriteAction.h>

//prevent multiple initClass()
SbBool SoStreamLine::baseInitFlag = FALSE;  

SO_KIT_SOURCE(SoStreamLine);

//-----------------------------------------------------------------------------
// static class initialization. call before first use
void SoStreamLine::initClass(void) 
{
    if ( baseInitFlag ) return;
  
    baseInitFlag = TRUE;
    //SoFieldSensor::initClass();

    SO_KIT_INIT_CLASS(SoStreamLine, SoBaseKit, "BaseKit");
}

//-----------------------------------------------------------------------------
// constructor. initializes al parts and fields.
SoStreamLine::SoStreamLine() 
{
    SO_KIT_CONSTRUCTOR(SoStreamLine);

    //--kit parts
    SO_KIT_ADD_CATALOG_ENTRY(stripSet, SoIndexedTriangleStripSet, 
                             FALSE, this, \0x0 , FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(vp, SoVertexProperty, FALSE,this,\0x0 , FALSE);

    SO_KIT_ADD_FIELD(numOfSides, (3));
    SO_KIT_ADD_FIELD(withTexCoord, (FALSE) );
    SO_KIT_ADD_FIELD(withColor, (FALSE) );
    SO_KIT_ADD_FIELD(vertex, (0,0,0) );
    SO_KIT_ADD_FIELD(radius, (0.0) );
    SO_KIT_ADD_FIELD(RGBA, (0,0,0) );
    SO_KIT_ADD_FIELD(generateVTexCoord, (TRUE));
    SO_KIT_ADD_FIELD(texCoords, (0,0) );
    SO_KIT_ADD_FIELD(generateStripSet, (TRUE) );

    SO_KIT_INIT_INSTANCE();

    //init part fields
    ((SoVertexProperty*)(vp.getValue()))->materialBinding.setValue(SoVertexProperty::OVERALL);
    ((SoVertexProperty*)(vp.getValue()))->normalBinding.setValue(SoVertexProperty::OVERALL);
    ((SoIndexedTriangleStripSet*)stripSet.getValue())->vertexProperty.setValue((SoVertexProperty*)(vp.getValue()));

    //SoFieldSensor *dummy=new SoFieldSensor();
    //SoFieldSensor *dummy2=new SoFieldSensor();
    
    // attach sensors to fields
    //numOfSlidesSensor = new SoFieldSensor();
    //numOfSlidesSensor->setData((void*)this);
    //numOfSlidesSensor->setFunction(regenerate);
    
    
    //withTexCoordSensor = new SoFieldSensor();
    //withTexCoordSensor->setData((void*)this);
    //withTexCoordSensor->setFunction(regenerate);
    
    //withColorSensor = new SoFieldSensor();
    //withColorSensor->setData((void*)this);
    //withColorSensor->setFunction(regenerate);
    
    //vertexSensor = new SoFieldSensor();
    //vertexSensor->setData((void*)this);
    //vertexSensor->setFunction(regenerate);
    
    //radiusSensor = new SoFieldSensor();
    //radiusSensor->setData((void*)this);
    //radiusSensor->setFunction(regenerate);
    
    //RGBASensor = new SoFieldSensor();
    //RGBASensor->setData((void*)this);
    //RGBASensor->setFunction(regenerate);
    
    //generateVTexCoordSensor = new SoFieldSensor();
    //generateVTexCoordSensor->setData((void*)this);
    //generateVTexCoordSensor->setFunction(regenerate);
    
    //texCoordsSensor = new SoFieldSensor();
    //texCoordsSensor->setData((void*)this);
    //texCoordsSensor->setFunction(regenerate);
    
    //generateStripSetSensor = new SoFieldSensor();
    //generateStripSetSensor->setData((void*)this);
    //generateStripSetSensor->setFunction(regenerate);
    
    numOfSlidesSensor       = new SoFieldSensor(regenerate, (void*)this);
    withTexCoordSensor      = new SoFieldSensor(regenerate, (void*)this);
    withColorSensor         = new SoFieldSensor(regenerate, (void*)this);
    vertexSensor            = new SoFieldSensor(regenerate, (void*)this);
    radiusSensor            = new SoFieldSensor(regenerate, (void*)this);
    RGBASensor              = new SoFieldSensor(regenerate, (void*)this);
    generateVTexCoordSensor = new SoFieldSensor(regenerate, (void*)this);
    texCoordsSensor         = new SoFieldSensor(regenerate, (void*)this);
    generateStripSetSensor  = new SoFieldSensor(regenerate, (void*)this);

    numOfSlidesSensor->attach(&numOfSides);
    withTexCoordSensor->attach(&withTexCoord);
    withColorSensor->attach(&withColor);
    vertexSensor->attach(&vertex);
    radiusSensor->attach(&radius);
    RGBASensor->attach(&RGBA);
    generateVTexCoordSensor->attach(&generateVTexCoord);
    texCoordsSensor->attach(&texCoords);
    generateStripSetSensor->attach(&generateStripSet);
}


//-----------------------------------------------------------------------------
// destructor. detaches and frees the sensors
SoStreamLine::~SoStreamLine() 
{ 
    numOfSlidesSensor->detach();
    withTexCoordSensor->detach();
    withColorSensor->detach();
    vertexSensor->detach();
    radiusSensor->detach();
    RGBASensor->detach();
    generateVTexCoordSensor->detach();
    texCoordsSensor->detach();
    generateStripSetSensor->detach();
    
    delete numOfSlidesSensor;
    delete withTexCoordSensor;
    delete withColorSensor;
    delete vertexSensor;
    delete radiusSensor;
    delete RGBASensor;
    delete generateVTexCoordSensor;
    delete texCoordsSensor;
    delete generateStripSetSensor; 
}


//-----------------------------------------------------------------------------
// addRing generates a "ring" of vertices with "radius" around the point P
void SoStreamLine::addRing(int num,int steps, float radius,SbVec3f P,
        SbVec3f start, SbVec3f normal, SbColor *col,SbVec2f *texcoord, 
        SbBool generateVTexCoord, SoVertexProperty *VP)
{
    float x, y, da, alpha=0.0;  
    
    start.normalize(); start*=radius;
    normal.normalize(); normal*=radius;

    da=(float)(M_PI*2.0f/steps);

    for (int i=0; i<steps; i++) 
    {
        x=(float)sin(alpha);
		y=(float)cos(alpha);
		alpha+=da;

        if (col!=NULL)
            VP->orderedRGBA.set1Value(num+i, col->getPackedValue());
        
        if (texcoord!=NULL)
        {
            if ((generateVTexCoord) || (steps<2))
                VP->texCoord.set1Value(num+i, (*texcoord)[0],(*texcoord)[1]);
            else
                VP->texCoord.set1Value(num+i, ((float)i)/((float) (steps-1)), 
                                       (*texcoord)[1]);
        }
        VP->vertex.set1Value(num+i, (P+(start*x)+(normal*y)));
        VP->normal.set1Value(num+i, ((start*x)+(normal*y)));
    }
}

//-----------------------------------------------------------------------------
// callback function called from all the fieldsensors
// calls the addRing methode and generates the Trianglestripset.
void SoStreamLine::regenerate(void* data, SoSensor* sensor)
{
    SoStreamLine* self = (SoStreamLine*)data;
    
    int i,j,num;
    SbColor *col;
    SbVec2f *txt;
    SbVec3f h, A, B, C, AB, BC, normal, oldnorm, sym;
    int numVertices=self->vertex.getNum();

    if (numVertices<2) return;

    self->numOfSlidesSensor->detach();
    self->withTexCoordSensor->detach();
    self->withColorSensor->detach();
    self->vertexSensor->detach();
    self->radiusSensor->detach();
    self->RGBASensor->detach();
    self->generateVTexCoordSensor->detach();
    self->texCoordsSensor->detach();
    self->generateStripSetSensor->detach();

    if (self->generateStripSet.getValue()==FALSE) return;

    self->generateStripSetSensor->detach();

    // first ring has normal parallel to next normal
    A=self->vertex[0];
    B=self->vertex[1];
    C=self->vertex[2];
    
    if (self->withColor.getValue())
        col=(SbColor *)&(self->RGBA[0]);
    else
        col=NULL;
    
    if (self->withTexCoord.getValue())
        txt=(SbVec2f*)&(self->texCoords[0]);
    else
        txt=NULL;
    
    AB=B-A; BC=B-C;
    normal=AB.cross(BC); 
    addRing(0, self->numOfSides.getValue(), self->radius.getValue(),
            A, normal, normal.cross(AB), col, txt,
            self->generateVTexCoord.getValue(),
            (SoVertexProperty *)self->vp.getValue());	
    oldnorm=normal;

    // now generate intermediate rings (till N-1)
    for (i=1; i< numVertices-1; i++) 
    {
        A=self->vertex[i-1];
        B=self->vertex[i];
        C=self->vertex[i+1];
        
        AB=B-A; BC=C-B; //AB.normalize(); BC.normalize();
        sym=AB+BC; sym.normalize();
        h=sym*sym.dot(oldnorm);
        normal=oldnorm-h;
        if (col!=NULL)
            col=(SbColor *)&(self->RGBA[i]);
        if (txt!=NULL)
            txt=(SbVec2f*)&(self->texCoords[i]);
        
        addRing(i*self->numOfSides.getValue(), self->numOfSides.getValue(),
                self->radius.getValue(), B, normal, normal.cross(sym), col,
                txt, self->generateVTexCoord.getValue(),
                (SoVertexProperty *)self->vp.getValue());	
        oldnorm=normal;
    } 
    
    // last ring like first
    A=self->vertex[numVertices-1];
    B=self->vertex[numVertices-2];
    C=self->vertex[numVertices-3];
    AB=B-A; BC=B-C;
    normal=AB.cross(BC); //normal.normalize();
    if (col!=NULL)
        col=(SbColor *)&(self->RGBA[numVertices-1]);
    if (txt!=NULL)
        txt=(SbVec2f*)&(self->texCoords[i]);
    
    addRing(i*self->numOfSides.getValue(),
            self->numOfSides.getValue(), 
            self->radius.getValue(),
            A, normal, normal.cross(AB), col, txt, 
            self->generateVTexCoord.getValue(),
            (SoVertexProperty *)self->vp.getValue());	
 
    // generate faces of streamline ------------
    num = 0;
    for (j=0; j<self->numOfSides.getValue()-1; j++) 
    {
        for (i=0; i<numVertices; i++) 
        {
            ((SoIndexedTriangleStripSet*)(self->stripSet.getValue()))->coordIndex.set1Value(num++, i*self->numOfSides.getValue()+j);
            ((SoIndexedTriangleStripSet*)(self->stripSet.getValue()))->coordIndex.set1Value(num++, i*self->numOfSides.getValue()+j+1);
        }
        ((SoIndexedTriangleStripSet*)(self->stripSet.getValue()))->coordIndex.set1Value(num++, SO_END_STRIP_INDEX);
    }
    
    for (i=0; i<numVertices; i++) 
    {
        ((SoIndexedTriangleStripSet*)(self->stripSet.getValue()))->coordIndex.set1Value(num++, i*self->numOfSides.getValue()+j);
        ((SoIndexedTriangleStripSet*)(self->stripSet.getValue()))->coordIndex.set1Value(num++, i*self->numOfSides.getValue());
    }
    
    ((SoIndexedTriangleStripSet*)(self->stripSet.getValue()))->coordIndex.set1Value(num++, SO_END_STRIP_INDEX);
    
    ((SoVertexProperty*)(self->vp.getValue()))->materialBinding.setValue(SoVertexProperty::PER_VERTEX_INDEXED);
    ((SoVertexProperty*)(self->vp.getValue()))->normalBinding.setValue(SoVertexProperty::PER_VERTEX_INDEXED);
    
    self->numOfSlidesSensor->attach(&(self->numOfSides));
    self->withTexCoordSensor->attach(&(self->withTexCoord));
    self->withColorSensor->attach(&(self->withColor));
    self->vertexSensor->attach(&(self->vertex));
    self->radiusSensor->attach(&(self->radius));
    self->RGBASensor->attach(&(self->RGBA));
    self->generateVTexCoordSensor->attach(&(self->generateVTexCoord));
    self->texCoordsSensor->attach(&(self->texCoords));
    self->generateStripSetSensor->attach(&(self->generateStripSet));
}

/* ===========================================================================
    End of SoStreamLine.cxx
   ===========================================================================
    Automatic Emacs configuration follows.
    Local Variables:
    mode:c++
    c-basic-offset: 4
    eval: (c-set-offset 'substatement-open 0)
    eval: (c-set-offset 'case-label '+)
    eval: (c-set-offset 'statement 'c-lineup-runin-statements)
    eval: (setq indent-tabs-mode nil)
    End:
=========================================================================== */
