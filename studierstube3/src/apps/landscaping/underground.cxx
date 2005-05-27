/* ===========================================================================
    (C) 2000 Vienna University of Technology
   ===========================================================================
    NAME:       underground
    TYPE:       c++ code
    PROJECT:    Studierstube
    CONTENT:    Implementation of underground cable wiring draw tool u
    VERSION:    2.2
   ===========================================================================
    AUTHORS:    ds      Dieter Schmalstieg
                gh      Gerd Hesina
                jp      Jan Prikryl
   ===========================================================================
    HISTORY:

    14-Dec-00 11:35:53  jp      last modification
    17-sep-98 21:47:52  ds      created
=========================================================================== */

#include <stbapi/stbapi.h>
#include <stdio.h>
#ifndef USE_SOQT_LIB
#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/viewers/SoXtExaminerViewer.h>
#else
#include <Inventor/Qt/SoQt.h>
#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>
#endif
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoMaterial.h>

#include <stbapi/util/math2.h>
#include <stbapi/interaction/So3DSensor.h>
#include <stbapi/misc/wstudierstube.h>
#include "underground.h"
#include "piputil.h"

//-----------------------------------------------------------------------------
// globals
//-----------------------------------------------------------------------------

class Underground;

const float spacing = 100;      //spacing of underground lines
const float cursorOffset = 10;  //how many grid lines is grid from origin
Underground* u;                 //Underground object
Underground* uhi;               //Underground highlight object
SoTranslation* uCursorT;        //translation of underground cursor
SoTranslation* uCursorTExact;   //translation of underground cursor
SoCube* waterCube;              //sensor geometry = water
So3DSensor* usensor;            //sensor for drawing
SoMaterial* uCursorColor;       //cursor color
SbVec3f uStart, uStop;          //position of start/stop of line

//-----------------------------------------------------------------------------
// class Underground implementation
//-----------------------------------------------------------------------------

// class underground is a kind of very simple drawing program for a regular
// grid in Inventor. It is used to draw the cable TV lines for the underground
// seams tool in the landscaping application

class Underground
{
public:
    Underground(              //constructor
        int aDim,             //size of grid
        float aSpacing        //grid spacing
        );
    ~Underground(             //destructor
        );
    SoNode* createSceneGraph( //create initial empty grid
        );
    void drawPolyLine(        //draw a poly line (two axis aligned lines)
        int x1,               //start point
        int y1,
        int x2,               //end point
        int y2,
        SbColor color         //color of line
        );
    void setColor(            //set whole grid to one color
        SbColor color
        );
    void clear(               //set whole grid to default color
        );

private:
    void drawLine(            //draw one (horizontal or vertical) line
        int x1,               //start point
        int y1,
        int x2,               //end point
        int y2,
        SbColor color         //color of line
        );

    int dim;                  //size of grid
    int size;                 //dim*dim
    float spacing;            //lenth of one grid cell
    SoLineSet* lines;         //inventor line set
    SoVertexProperty* vp;     //vertex property for line set

};

//-----------------------------------------------------------------------------
// constructor

Underground::Underground(int aDim, float aSpacing)
:
    dim(aDim), size(dim*dim), spacing(aSpacing)
{}

//-----------------------------------------------------------------------------
// destructor

Underground::~Underground() {}

//-----------------------------------------------------------------------------
// fill all lines with given color
// color: color to use

void
Underground::setColor(SbColor color)
{
    int32_t c = color.getPackedValue();
    uint32_t* oc = vp->orderedRGBA.startEditing();
    for(int i=0; i<2*size; i++) oc[i] = c;
    vp->orderedRGBA.finishEditing();
}

//-----------------------------------------------------------------------------
// fill all lines with black

void
Underground::clear()
{
    uint32_t* oc = vp->orderedRGBA.startEditing();
    for(int i=0; i<2*size; i++) oc[i] = 0x00000000; //transparent
    vp->orderedRGBA.finishEditing();
}

//-----------------------------------------------------------------------------
// make the scene graph for the line grid
// returns: pointer to scene graph

SoNode*
Underground::createSceneGraph()
{
    int i,j;
    lines = new SoLineSet;
    vp = new SoVertexProperty;
    vp->materialBinding = SoVertexProperty::PER_PART;
    lines->vertexProperty = vp;

    lines->numVertices.setNum(2*dim);
    int32_t* nv = lines->numVertices.startEditing();
    for(i=0; i<2*dim; i++) nv[i] = dim;
    lines->numVertices.finishEditing();

    vp->vertex.setNum(2*size);
    vp->orderedRGBA.setNum(2*size);
    SbVec3f* vx = vp->vertex.startEditing();
    uint32_t* oc = vp->orderedRGBA.startEditing();
    for(i=0; i<dim; i++) for(j=0; j<dim; j++)
    {
        int offset = j+i*dim;
        vx[offset]      = SbVec3f(j*spacing,0,i*spacing);
        vx[offset+size] = SbVec3f(i*spacing,0,j*spacing);
        oc[offset]      = 0x222222FF; //make everything grey
        oc[offset+size] = 0x222222FF;
    }
    vp->vertex.finishEditing();
    vp->orderedRGBA.finishEditing();

    return lines;
}

//-----------------------------------------------------------------------------
// draw one line: must be horizontal or vertical
// x1, y1: start point
// x2, y2: end point
// color: color of line

void
Underground::drawLine(int x1, int y1, int x2, int y2, SbColor color)
{
    int i;
    uint32_t c = color.getPackedValue();
    for(i=x1; i<x2; i++) vp->orderedRGBA.set1Value(y1*(dim-1)+i,c);
    for(i=y1; i<y2; i++) vp->orderedRGBA.set1Value(size+x2*(dim-1)+i-dim,c);
}

//-----------------------------------------------------------------------------
// draw a poly line (two axis aligned lines)
// x1, y1: start point
// x2, y2: end point
// color: color of line

void
Underground::drawPolyLine(int x1, int y1, int x2, int y2, SbColor c)
{
    int sector = (x2<x1)+2*(y2<y1);
    switch(sector)
    {
        case 0: drawLine(x1,y1,x2,y1,c); drawLine(x2,y1,x2,y2,c); break;
        case 1: drawLine(x2,y1,x1,y1,c); drawLine(x2,y1,x2,y2,c); break;
        case 2: drawLine(x1,y1,x2,y1,c); drawLine(x2,y2,x2,y1,c); break;
        case 3: drawLine(x2,y1,x1,y1,c); drawLine(x2,y2,x2,y1,c); break;
    }
}

//-----------------------------------------------------------------------------
// underground module functions
//-----------------------------------------------------------------------------

// compute position of underground cursor from pen position
// returns: position of underground cursor in underground space

SbVec3f
moveUndergroundCursor()
{
    SbVec3f p, q, r;
    p = getPenPosition();
    SbMatrix matrix;
    assert(getMatrix(waterCube,matrix));
    matrix.inverse().multVecMatrix(p,q); //take pen positon to underg. space

    //draw the exact cursor
    q[1] = 0.0;
    uCursorTExact->translation = q;

    //draw the cursor that snaps to grid
#ifdef _SGI_SOURCE
    r[0] = q[0]-fmod(q[0]+spacing*0.5,spacing);
#else
    r[0] = q[0]-fmod(double(q[0]+spacing*0.5),double(spacing));
#endif
    r[1] = 0.0;
#ifdef _SGI_SOURCE
    r[2] = q[2]-fmod(q[2]+spacing*0.5,spacing);
#else
    r[2] = q[2]-fmod(double(q[2]+spacing*0.5),double(spacing));
#endif
    uCursorT->translation = r;

    //return integer grid coordinates
    return SbVec3f(r[0]/spacing+cursorOffset,0,r[2]/spacing+cursorOffset);
}

//-----------------------------------------------------------------------------
// callback for button pressed - line start

void
cbUndergroundPress(void*, So3DSensor*)
{
    if(pipSide == 0) return; //only active when pip backside visible
    uStart = moveUndergroundCursor();
}

//-----------------------------------------------------------------------------
// callback while dragging pen - update highlight line

void
cbUndergroundMove(void*, So3DSensor* sensor)
{
    if(pipSide == 0) return;
    uStop = moveUndergroundCursor();
    uhi->clear();
    uhi->drawPolyLine((int)uStart[0], (int)uStart[2],
                      (int)uStop[0] , (int)uStop[2] , SbColor(1,0.7,0));
}

//-----------------------------------------------------------------------------
// callback when pen released - draw permanent line

void
cbUndergroundRelease(void*, So3DSensor*)
{
    if(pipSide == 0) return;
    uStop = moveUndergroundCursor();
    //add permanent line here
    u->drawPolyLine((int)uStart[0], (int)uStart[2],
		    (int)uStop[0] , (int)uStop[2] , SbColor(0.4,0.4,0.7));
}

//-----------------------------------------------------------------------------
// setup the underground stuff

void
goUnderground()
{
    // create line grid and highlight line grid
    u = new Underground(20,spacing);
    SoNode* undergroundGraph = u->createSceneGraph();
    u->clear();

    uhi = new Underground(20,spacing);
    SoNode* undergroundGraphHi = uhi->createSceneGraph();
    uhi->clear();

    // assign misc. nodes
    AGETNODE(usep,SoSeparator,"U_SEP");
    AVGETNODE(uCursorT,SoTranslation,"U_CURSOR_T");
    AVGETNODE(uCursorTExact,SoTranslation,"U_CURSOR_T_EXACT");
    AVGETNODE(uCursorColor,SoMaterial,"U_CURSOR_COLOR");
    AVGETNODE(waterCube,SoCube,"WATER_CUBE");

    // add underground lines to seam's secondary world
    NEWNODE(ds1,SoDrawStyle);
    ds1->lineWidth = 8;
    NEWNODE(ds2,SoDrawStyle);
    ds2->lineWidth = 2;
    usep->addChild(ds1);
    usep->addChild(undergroundGraph);
    usep->addChild(ds2);
    usep->addChild(undergroundGraphHi);

    // setup underground sensor associated to water block
    AVGETNODE(usensor,So3DSensor,"U_SENSOR");
    usensor->setPressCallback(cbUndergroundPress);
    usensor->setMoveCallback(cbUndergroundMove);
    usensor->setReleaseCallback(cbUndergroundRelease);

    // make a pen cursor for secondary world
//    AGETNODE(ps,SoSeparator,"PEN_SEP");
//    AGETNODE(sps,SoGroup,"SECONDARY_PEN");
//    sps->addChild(ps);
}

//-----------------------------------------------------------------------------
//eof
