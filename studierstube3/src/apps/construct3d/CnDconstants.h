// CnDconstants.h

#ifndef _CNDCONSTANTS_H_
#define _CNDCONSTANTS_H_

#define MAX(i, j) ((i) >= (j) ? (i) : (j))
#define MIN(i, j) ((i) >= (j) ? (j) : (i))
#define ABS(i) ((i) >= (0) ? (i) : (i*(-1)))

// number of Buttons on the PIP
const int buttonNum = 42;
// number of Buttons with construction functions, where preview can be shown
const int constructButtons = 28;
// maximum number of layers
const int usedLayers = 6;
// maximum distance between objects in meters
const float maxDistance = 100.;  //100 m 
// If distance of Pen to all objects is bigger, 
// than all objects get deselected
const float deselectDist = 0.3f; // 30cm
// if the distance between 2 lines is bigger, they do not intersect
// (by our definition)
const float LineIntersectionTolerance = 0.001f;  // 1mm
// used by Cylinder-Line Intersection procedure
const float outOfSightDistance = 10.0f; // 10m
// size of the fully transparent bounding box around a Point, that allows dragging
// for beginners 5cm, for advanced users 2-3cm
const float pointDraggingBoxSize = 0.04f; 
// If a button is pressed and the pen slightly moved between press and release
// the movement must not be bigger than dragMoveTolerance - otherwise the object
// will be dragged
const float	dragMoveTolerance = 0.02f; // now 2cm //0.006f;  // 6mm

// default size of points
const float pSize = 0.005f; // former box size 0.007f;
// If distance to a Point is smaller than this value, then it is equal to 0
//const float PointPriorityEpsilon = 0.008f; // 8mm
const float PointPriorityEpsilon = 0.03f; // now 3cm - to make point selection easier, former 1cm
// If distance to an Intersection Curve is smaller than this value, then it is equal to 0
const float IntersectCurvePriorityEpsilon = 0.004f; // 4mm
// epsilon for line/line interesection tests
const float eps = 1.0e-4f;
// default size of text
const float defaultTextSize = 0.02f;//26; //30 = font size
// default thickness of grid lines
const float gridLineWidth = 1.5f;
// default if a distance is 0
const float EPS = 0.001f;
// default thickness of linear borders of polygons
const float polygonLineThickness = 3.0f;
// default size of tangentplane
const float tangentplaneSize = 0.2f;
// default length of a line Normal to a Plane in both directions
const float normalLineLength = 0.15f;

static float minPlaneSize = 0.2f;                 //set the min size of a plane
static float distancePlanepointToEdge = 0.05f;     //set the distance for Point of Plane to Edge of Plane

const  int   maxNumofCurvePoints = 2000; //set the max number of curve points
const  int   maxNumofPoints = 200; //set the max number of controlpoints
const  int   maxDegree = 50; //set the max value of degree
const  int   maxNumofSurfacePoints = 100; //set the max number of controlpoints

// colors now specified in MaterialConstants
// static float textDefaultColor[] = {1.0f, 1.0f, 1.0f};  // base color
static float gridColor[] = {1.0f, 1.0f, 1.0f};  // color of x,y,z grid

//static float boolObjectDefaultTransparency = 0.0f;

//static int boolObjectLayer = 2;
//static int boolBaseObjectLayer = 1;
//static int surfaceObjectLayer = 2;
//static int surfaceBaseObjectLayer = 1;
//static int intersectionObjectLayer = 0;

enum enumBoolOp // bool operation definition
{
  BOOL_UNINITIALIZED = 0, // not initialized
  BOOL_CUT = 1,           // Subtract obj2 from obj1 (cut)
  BOOL_UNION = 2,         // Union of obj1 and obj2
  BOOL_INTERSECTION = 3   // Intersection of obj1 and obj2
};

enum enumCurveOp //Curve def
{
  CURVE_BEZIER = 1,
  CIRCLE = 2,
  ELLIPSE_CURVE = 3,
  CURVE_POINTS_ON = 4,
  CURVE_INTERSECTION = 5
};

enum enumSurfaceOp //Surface def
{
  SURFACE_B_SPLINE = 1,
  SURFACE_POINTS_ON = 2,
  SURFACE_SWEEP = 3
};

enum enumIntersectionType //Intersection Types
{
  OBJECT_OBJECT = 1,
  LINE_OBJECT = 2,
  CURVE_OBJECT = 3,
  PLANE_PLANE =4,
  EDGE_EDGE =5,
  BODY_PLANE =6,
  SLICE_BODY = 7
};


static float surface_low = 30;
static float surface_high = 5;

static float sweepTolerance = 0;

static float bool_low = 30;
static float bool_high = 10;

static float maxPointDistance = 100.0f; //needed in Intersection

// modify to change color schemes of users
// 0 = blue, 1 = orange, 2 = green, 3 = red
static int user1Color = 0;
static int user2Color = 3;
static int user3Color = 2;
static int user4Color = 1;

#endif//_CNDCONSTANTS_H_
