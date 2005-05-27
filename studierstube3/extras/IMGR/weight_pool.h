#ifndef WEIGHT_POOL_INCLUDED
#define WEIGHT_POOL_INCLUDED

#include <math.h>

#include "weight.h"
#include "feature_list_pool.h"
#include "dependence_pool.h"

#define _min_(x,y) (((x) < (y)) ? (x) : (y))
#define _max_(x,y) (((x) > (y)) ? (x) : (y))

#define original_function(X)   (_max_(0.0,_min_(98.0,20.0+(X   -20.0)*3.0)))
#define reflection_function(X) (_max_(0.0,_min_(98.0,20.0+(80.0-X   )*3.0)))

#define DEFINED_WEIGHT_TYPE	float

#define DEFINED_WEIGHT_CONTENT_TYPE weight<DEFINED_WEIGHT_TYPE,DEFINED_FEATURE_LIST_TYPE>::WEIGHT_CONTENT_TYPE

//dependence level 0-----------------------------------------------------------
#define	WEIGHT_STRAIGHTNESS			0

#define WEIGHT_HEIGHT_WIDTH_RATIO		1
#define WEIGHT_HEIGHT_DEPTH_RATIO		2
#define WEIGHT_DEPTH_WIDTH_RATIO		3

#define WEIGHT_HEIGHT_WIDTH_RATIO_R45		4
#define WEIGHT_HEIGHT_DEPTH_RATIO_R45		5
#define WEIGHT_DEPTH_WIDTH_RATIO_R45		6

#define WEIGHT_RELATIVE_START_X			7
#define WEIGHT_RELATIVE_START_Y			8
#define WEIGHT_RELATIVE_START_Z			9

#define WEIGHT_RELATIVE_END_X				10
#define WEIGHT_RELATIVE_END_Y				11
#define WEIGHT_RELATIVE_END_Z				12

#define WEIGHT_HORIZONTAL_MOVEMENTS			13
#define WEIGHT_VERTICAL_MOVEMENTS			14
#define WEIGHT_DEPTH_MOVEMENTS				15

#define WEIGHT_HORIZONTAL_MOVEMENTS_R45		16
#define WEIGHT_VERTICAL_MOVEMENTS_R45		17
#define WEIGHT_DEPTH_MOVEMENTS_R45			18

#define WEIGHT_START_ANGLE_X_Y			19
#define WEIGHT_START_ANGLE_X_Z			20
#define WEIGHT_START_ANGLE_Z_Y			21

#define WEIGHT_END_ANGLE_X_Y			22
#define WEIGHT_END_ANGLE_X_Z			23
#define WEIGHT_END_ANGLE_Z_Y			24

#define WEIGHT_RELATIVE_CG_X			25
#define WEIGHT_RELATIVE_CG_Y			26
#define WEIGHT_RELATIVE_CG_Z			27

#define	WEIGHT_ORIENTATION_STRAIGHTNESS				28

#define WEIGHT_ORIENTATION_HEIGHT_WIDTH_RATIO		29
#define WEIGHT_ORIENTATION_HEIGHT_DEPTH_RATIO		30
#define WEIGHT_ORIENTATION_DEPTH_WIDTH_RATIO		31

#define WEIGHT_ORIENTATION_HEIGHT_WIDTH_RATIO_R45		32
#define WEIGHT_ORIENTATION_HEIGHT_DEPTH_RATIO_R45		33
#define WEIGHT_ORIENTATION_DEPTH_WIDTH_RATIO_R45		34

#define WEIGHT_ORIENTATION_RELATIVE_START_AZIM			35
#define WEIGHT_ORIENTATION_RELATIVE_START_ELEV			36
#define WEIGHT_ORIENTATION_RELATIVE_START_ROLL			37

#define WEIGHT_ORIENTATION_RELATIVE_END_AZIM			38
#define WEIGHT_ORIENTATION_RELATIVE_END_ELEV			39
#define WEIGHT_ORIENTATION_RELATIVE_END_ROLL			40

#define WEIGHT_ORIENTATION_AZIMUTHAL_MOVEMENTS			41
#define WEIGHT_ORIENTATION_ELEVATION_MOVEMENTS			42
#define WEIGHT_ORIENTATION_ROLL_MOVEMENTS				43

#define WEIGHT_ORIENTATION_AZIMUTHAL_MOVEMENTS_R45		44
#define WEIGHT_ORIENTATION_ELEVATION_MOVEMENTS_R45		45
#define WEIGHT_ORIENTATION_ROLL_MOVEMENTS_R45			46

#define WEIGHT_ORIENTATION_START_ANGLE_AZIM_ELEV		47
#define WEIGHT_ORIENTATION_START_ANGLE_AZIM_ROLL		48
#define WEIGHT_ORIENTATION_START_ANGLE_ROLL_ELEV		49

#define WEIGHT_ORIENTATION_END_ANGLE_AZIM_ELEV			50
#define WEIGHT_ORIENTATION_END_ANGLE_AZIM_ROLL			51
#define WEIGHT_ORIENTATION_END_ANGLE_ROLL_ELEV			52

#define WEIGHT_ORIENTATION_RELATIVE_CG_AZIM			53
#define WEIGHT_ORIENTATION_RELATIVE_CG_ELEV			54
#define WEIGHT_ORIENTATION_RELATIVE_CG_ROLL			55
//-----------------------------------------------------------------------------

//calculate section
//-----------------------------------------------------------------------------
void  WEIGHT_CALCULATE_STRAIGHTNESS (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{
 //nothing to do  
}
//-----------------------------------------------------------------------------
void  WEIGHT_CALCULATE_HEIGHT_WIDTH_RATIO (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{
 //nothing to do 
}
//-----------------------------------------------------------------------------
void  WEIGHT_CALCULATE_HEIGHT_DEPTH_RATIO (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{
 //nothing to do 
}
//-----------------------------------------------------------------------------
void  WEIGHT_CALCULATE_DEPTH_WIDTH_RATIO (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{
 //nothing to do  
}
//-----------------------------------------------------------------------------
void  WEIGHT_CALCULATE_HEIGHT_WIDTH_RATIO_R45 (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{
 //nothing to do 
}
//-----------------------------------------------------------------------------
void  WEIGHT_CALCULATE_HEIGHT_DEPTH_RATIO_R45 (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{
 //nothing to do
}
//-----------------------------------------------------------------------------
void  WEIGHT_CALCULATE_DEPTH_WIDTH_RATIO_R45 (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{
 //nothing to do  
}
//-----------------------------------------------------------------------------
void  WEIGHT_CALCULATE_RELATIVE_START_X (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{
 v.component.divident=(DEFINED_WEIGHT_TYPE)reflection_function(fl.look_up(FEATURE_HEIGHT_WIDTH_RATIO).get());  
}
//-----------------------------------------------------------------------------
void  WEIGHT_CALCULATE_RELATIVE_START_Y (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{
  v.component.divident=(DEFINED_WEIGHT_TYPE)original_function(fl.look_up(FEATURE_HEIGHT_DEPTH_RATIO).get()); 
}
//-----------------------------------------------------------------------------
void  WEIGHT_CALCULATE_RELATIVE_START_Z (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{
 v.component.divident=(DEFINED_WEIGHT_TYPE)original_function(fl.look_up(FEATURE_DEPTH_WIDTH_RATIO).get()); 
}
//-----------------------------------------------------------------------------
void  WEIGHT_CALCULATE_RELATIVE_END_X (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{
 v.component.divident=(DEFINED_WEIGHT_TYPE)reflection_function(fl.look_up(FEATURE_HEIGHT_WIDTH_RATIO).get()); 
}
//-----------------------------------------------------------------------------
void  WEIGHT_CALCULATE_RELATIVE_END_Y (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{
 v.component.divident=(DEFINED_WEIGHT_TYPE)original_function(fl.look_up(FEATURE_HEIGHT_DEPTH_RATIO).get());    
}
//-----------------------------------------------------------------------------
void  WEIGHT_CALCULATE_RELATIVE_END_Z (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{
 v.component.divident=(DEFINED_WEIGHT_TYPE)original_function(fl.look_up(FEATURE_DEPTH_WIDTH_RATIO).get());   
}
//-----------------------------------------------------------------------------
void  WEIGHT_CALCULATE_HORIZONTAL_MOVEMENTS (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{
 v.component.divident=(DEFINED_WEIGHT_TYPE)reflection_function(fl.look_up(FEATURE_HEIGHT_WIDTH_RATIO).get());  
}
//-----------------------------------------------------------------------------
void  WEIGHT_CALCULATE_VERTICAL_MOVEMENTS (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{
 v.component.divident=(DEFINED_WEIGHT_TYPE)original_function(fl.look_up(FEATURE_HEIGHT_DEPTH_RATIO).get());
}
//-----------------------------------------------------------------------------
void  WEIGHT_CALCULATE_DEPTH_MOVEMENTS (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{
 v.component.divident=(DEFINED_WEIGHT_TYPE)original_function(fl.look_up(FEATURE_DEPTH_WIDTH_RATIO).get());   
}
//-----------------------------------------------------------------------------
void  WEIGHT_CALCULATE_HORIZONTAL_MOVEMENTS_R45 (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{
 v.component.divident=(DEFINED_WEIGHT_TYPE)reflection_function(fl.look_up(FEATURE_HEIGHT_WIDTH_RATIO_R45).get());   
}
//-----------------------------------------------------------------------------
void  WEIGHT_CALCULATE_VERTICAL_MOVEMENTS_R45 (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{
 v.component.divident=(DEFINED_WEIGHT_TYPE)original_function(fl.look_up(FEATURE_HEIGHT_DEPTH_RATIO_R45).get());   
}
//-----------------------------------------------------------------------------
void  WEIGHT_CALCULATE_DEPTH_MOVEMENTS_R45 (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{
 v.component.divident=(DEFINED_WEIGHT_TYPE)original_function(fl.look_up(FEATURE_DEPTH_WIDTH_RATIO_R45).get());  
}
//-----------------------------------------------------------------------------
void  WEIGHT_CALCULATE_START_ANGLE_X_Y (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{
 //nothing to do  
}
//-----------------------------------------------------------------------------
void  WEIGHT_CALCULATE_START_ANGLE_X_Z (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{
 //nothing to do  
}
//-----------------------------------------------------------------------------
void  WEIGHT_CALCULATE_START_ANGLE_Z_Y (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{
 //nothing to do  
}
//-----------------------------------------------------------------------------
void  WEIGHT_CALCULATE_END_ANGLE_X_Y (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{
 //nothing to do  
}
//-----------------------------------------------------------------------------
void  WEIGHT_CALCULATE_END_ANGLE_X_Z (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{
 //nothing to do  
}
//-----------------------------------------------------------------------------
void  WEIGHT_CALCULATE_END_ANGLE_Z_Y (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{
 //nothing to do  
}
//-----------------------------------------------------------------------------
void  WEIGHT_CALCULATE_RELATIVE_CG_X (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{
 v.component.divident=(DEFINED_WEIGHT_TYPE)reflection_function(fl.look_up(FEATURE_HEIGHT_WIDTH_RATIO).get());
}
//-----------------------------------------------------------------------------
void  WEIGHT_CALCULATE_RELATIVE_CG_Y (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{
 v.component.divident=(DEFINED_WEIGHT_TYPE)original_function(fl.look_up(FEATURE_HEIGHT_DEPTH_RATIO).get());
}
//-----------------------------------------------------------------------------
void  WEIGHT_CALCULATE_RELATIVE_CG_Z (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{
 v.component.divident=(DEFINED_WEIGHT_TYPE)original_function(fl.look_up(FEATURE_DEPTH_WIDTH_RATIO).get()); 
}
//-----------------------------------------------------------------------------
void  WEIGHT_CALCULATE_ORIENTATION_STRAIGHTNESS (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{
 //nothing to do
}
//-----------------------------------------------------------------------------
void  WEIGHT_CALCULATE_ORIENTATION_HEIGHT_WIDTH_RATIO (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{
 //nothing to do  
}
//-----------------------------------------------------------------------------
void  WEIGHT_CALCULATE_ORIENTATION_HEIGHT_DEPTH_RATIO (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{
 //nothing to do
}
//-----------------------------------------------------------------------------
void  WEIGHT_CALCULATE_ORIENTATION_DEPTH_WIDTH_RATIO (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{
 //nothing to do
}
//-----------------------------------------------------------------------------
void  WEIGHT_CALCULATE_ORIENTATION_HEIGHT_WIDTH_RATIO_R45 (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{
 //nothing to do
}
//-----------------------------------------------------------------------------
void  WEIGHT_CALCULATE_ORIENTATION_HEIGHT_DEPTH_RATIO_R45 (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{
 //nothing to do
}
//-----------------------------------------------------------------------------
void  WEIGHT_CALCULATE_ORIENTATION_DEPTH_WIDTH_RATIO_R45 (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{
 //nothing to do
}
//-----------------------------------------------------------------------------
void  WEIGHT_CALCULATE_ORIENTATION_RELATIVE_START_AZIM (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{
 v.component.divident=(DEFINED_WEIGHT_TYPE)reflection_function(fl.look_up(FEATURE_ORIENTATION_HEIGHT_WIDTH_RATIO).get());
}
//-----------------------------------------------------------------------------
void  WEIGHT_CALCULATE_ORIENTATION_RELATIVE_START_ELEV (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{
 v.component.divident=(DEFINED_WEIGHT_TYPE)original_function(fl.look_up(FEATURE_ORIENTATION_HEIGHT_DEPTH_RATIO).get());  
}
//-----------------------------------------------------------------------------
void  WEIGHT_CALCULATE_ORIENTATION_RELATIVE_START_ROLL (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{
 v.component.divident=(DEFINED_WEIGHT_TYPE)original_function(fl.look_up(FEATURE_ORIENTATION_DEPTH_WIDTH_RATIO).get());  
}
//-----------------------------------------------------------------------------
void  WEIGHT_CALCULATE_ORIENTATION_RELATIVE_END_AZIM (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{
 v.component.divident=(DEFINED_WEIGHT_TYPE)reflection_function(fl.look_up(FEATURE_ORIENTATION_HEIGHT_WIDTH_RATIO).get());
}
//-----------------------------------------------------------------------------
void  WEIGHT_CALCULATE_ORIENTATION_RELATIVE_END_ELEV (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{
  v.component.divident=(DEFINED_WEIGHT_TYPE)original_function(fl.look_up(FEATURE_ORIENTATION_HEIGHT_DEPTH_RATIO).get());
}
//-----------------------------------------------------------------------------
void  WEIGHT_CALCULATE_ORIENTATION_RELATIVE_END_ROLL (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{
 v.component.divident=(DEFINED_WEIGHT_TYPE)original_function(fl.look_up(FEATURE_ORIENTATION_DEPTH_WIDTH_RATIO).get());
}
//-----------------------------------------------------------------------------
void  WEIGHT_CALCULATE_ORIENTATION_AZIMUTHAL_MOVEMENTS (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{
 v.component.divident=(DEFINED_WEIGHT_TYPE)reflection_function(fl.look_up(FEATURE_ORIENTATION_HEIGHT_WIDTH_RATIO).get());  
}
//-----------------------------------------------------------------------------
void  WEIGHT_CALCULATE_ORIENTATION_ELEVATION_MOVEMENTS (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{
 v.component.divident=(DEFINED_WEIGHT_TYPE)original_function(fl.look_up(FEATURE_ORIENTATION_HEIGHT_DEPTH_RATIO).get());  
}
//-----------------------------------------------------------------------------
void  WEIGHT_CALCULATE_ORIENTATION_ROLL_MOVEMENTS (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{
 v.component.divident=(DEFINED_WEIGHT_TYPE)original_function(fl.look_up(FEATURE_ORIENTATION_DEPTH_WIDTH_RATIO).get());  
}

//-----------------------------------------------------------------------------
void  WEIGHT_CALCULATE_ORIENTATION_AZIMUTHAL_MOVEMENTS_R45 (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{
 v.component.divident=(DEFINED_WEIGHT_TYPE)reflection_function(fl.look_up(FEATURE_ORIENTATION_HEIGHT_WIDTH_RATIO_R45).get());  
}
//-----------------------------------------------------------------------------
void  WEIGHT_CALCULATE_ORIENTATION_ELEVATION_MOVEMENTS_R45 (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{
 v.component.divident=(DEFINED_WEIGHT_TYPE)original_function(fl.look_up(FEATURE_ORIENTATION_HEIGHT_DEPTH_RATIO_R45).get());  
}
//-----------------------------------------------------------------------------
void  WEIGHT_CALCULATE_ORIENTATION_ROLL_MOVEMENTS_R45 (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{
 v.component.divident=(DEFINED_WEIGHT_TYPE)original_function(fl.look_up(FEATURE_ORIENTATION_DEPTH_WIDTH_RATIO_R45).get());   
}
//-----------------------------------------------------------------------------
void  WEIGHT_CALCULATE_ORIENTATION_START_ANGLE_AZIM_ELEV (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{
 //nothing to do  
}
//-----------------------------------------------------------------------------
void  WEIGHT_CALCULATE_ORIENTATION_START_ANGLE_AZIM_ROLL (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{
 //nothing to do  
}
//-----------------------------------------------------------------------------
void  WEIGHT_CALCULATE_ORIENTATION_START_ANGLE_ROLL_ELEV (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{
 //nothing to do  
}
//-----------------------------------------------------------------------------
void  WEIGHT_CALCULATE_ORIENTATION_END_ANGLE_AZIM_ELEV (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{
 //nothing to do  
}
//-----------------------------------------------------------------------------
void  WEIGHT_CALCULATE_ORIENTATION_END_ANGLE_AZIM_ROLL (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{
 //nothing to do  
}
//-----------------------------------------------------------------------------
void  WEIGHT_CALCULATE_ORIENTATION_END_ANGLE_ROLL_ELEV (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{
 //nothing to do 
}
//-----------------------------------------------------------------------------
void  WEIGHT_CALCULATE_ORIENTATION_RELATIVE_CG_AZIM (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{
 v.component.divident=(DEFINED_WEIGHT_TYPE)reflection_function(fl.look_up(FEATURE_ORIENTATION_HEIGHT_WIDTH_RATIO).get());  
}
//-----------------------------------------------------------------------------
void  WEIGHT_CALCULATE_ORIENTATION_RELATIVE_CG_ELEV (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{
  v.component.divident=(DEFINED_WEIGHT_TYPE)original_function(fl.look_up(FEATURE_ORIENTATION_HEIGHT_DEPTH_RATIO).get());
}
//-----------------------------------------------------------------------------
void  WEIGHT_CALCULATE_ORIENTATION_RELATIVE_CG_ROLL (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{
 v.component.divident=(DEFINED_WEIGHT_TYPE)original_function(fl.look_up(FEATURE_ORIENTATION_DEPTH_WIDTH_RATIO).get());  
}
//-----------------------------------------------------------------------------

// initialization section
//-----------------------------------------------------------------------------
void  WEIGHT_INITIALIZE_STRAIGHTNESS (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{v.component.divident=(DEFINED_WEIGHT_TYPE)5.0;v.component.divisor=(DEFINED_WEIGHT_TYPE)1.0;}
//-----------------------------------------------------------------------------
void  WEIGHT_INITIALIZE_HEIGHT_WIDTH_RATIO (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{v.component.divident=(DEFINED_WEIGHT_TYPE)1.0;v.component.divisor=(DEFINED_WEIGHT_TYPE)1.0;}
//-----------------------------------------------------------------------------
void  WEIGHT_INITIALIZE_HEIGHT_DEPTH_RATIO (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{v.component.divident=(DEFINED_WEIGHT_TYPE)1.0;v.component.divisor=(DEFINED_WEIGHT_TYPE)1.0;}
//-----------------------------------------------------------------------------
void  WEIGHT_INITIALIZE_DEPTH_WIDTH_RATIO (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{v.component.divident=(DEFINED_WEIGHT_TYPE)1.0;v.component.divisor=(DEFINED_WEIGHT_TYPE)1.0;}
//-----------------------------------------------------------------------------
void  WEIGHT_INITIALIZE_HEIGHT_WIDTH_RATIO_R45 (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{v.component.divident=(DEFINED_WEIGHT_TYPE)1.0;v.component.divisor=(DEFINED_WEIGHT_TYPE)1.0;}
//-----------------------------------------------------------------------------
void  WEIGHT_INITIALIZE_HEIGHT_DEPTH_RATIO_R45 (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{v.component.divident=(DEFINED_WEIGHT_TYPE)1.0;v.component.divisor=(DEFINED_WEIGHT_TYPE)1.0;}
//-----------------------------------------------------------------------------
void  WEIGHT_INITIALIZE_DEPTH_WIDTH_RATIO_R45 (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{v.component.divident=(DEFINED_WEIGHT_TYPE)1.0;v.component.divisor=(DEFINED_WEIGHT_TYPE)1.0;}
//-----------------------------------------------------------------------------
void  WEIGHT_INITIALIZE_RELATIVE_START_X (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{v.component.divident=(DEFINED_WEIGHT_TYPE)1.0;v.component.divisor=(DEFINED_WEIGHT_TYPE)20.0;}
//-----------------------------------------------------------------------------
void  WEIGHT_INITIALIZE_RELATIVE_START_Y (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{v.component.divident=(DEFINED_WEIGHT_TYPE)1.0;v.component.divisor=(DEFINED_WEIGHT_TYPE)20.0;}
//-----------------------------------------------------------------------------
void  WEIGHT_INITIALIZE_RELATIVE_START_Z (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{v.component.divident=(DEFINED_WEIGHT_TYPE)1.0;v.component.divisor=(DEFINED_WEIGHT_TYPE)20.0;}
//-----------------------------------------------------------------------------
void  WEIGHT_INITIALIZE_RELATIVE_END_X (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{v.component.divident=(DEFINED_WEIGHT_TYPE)1.0;v.component.divisor=(DEFINED_WEIGHT_TYPE)20.0;}
//-----------------------------------------------------------------------------
void  WEIGHT_INITIALIZE_RELATIVE_END_Y (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{v.component.divident=(DEFINED_WEIGHT_TYPE)1.0;v.component.divisor=(DEFINED_WEIGHT_TYPE)20.0;}
//-----------------------------------------------------------------------------
void  WEIGHT_INITIALIZE_RELATIVE_END_Z (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{v.component.divident=(DEFINED_WEIGHT_TYPE)1.0;v.component.divisor=(DEFINED_WEIGHT_TYPE)20.0;}
//-----------------------------------------------------------------------------
void  WEIGHT_INITIALIZE_HORIZONTAL_MOVEMENTS (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{v.component.divident=(DEFINED_WEIGHT_TYPE)3.0;v.component.divisor=(DEFINED_WEIGHT_TYPE)20.0;}
//-----------------------------------------------------------------------------
void  WEIGHT_INITIALIZE_VERTICAL_MOVEMENTS (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{v.component.divident=(DEFINED_WEIGHT_TYPE)3.0;v.component.divisor=(DEFINED_WEIGHT_TYPE)20.0;}
//-----------------------------------------------------------------------------
void  WEIGHT_INITIALIZE_DEPTH_MOVEMENTS (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{v.component.divident=(DEFINED_WEIGHT_TYPE)3.0;v.component.divisor=(DEFINED_WEIGHT_TYPE)20.0;}
//-----------------------------------------------------------------------------
void  WEIGHT_INITIALIZE_HORIZONTAL_MOVEMENTS_R45 (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{v.component.divident=(DEFINED_WEIGHT_TYPE)3.0;v.component.divisor=(DEFINED_WEIGHT_TYPE)20.0;}
//-----------------------------------------------------------------------------
void  WEIGHT_INITIALIZE_VERTICAL_MOVEMENTS_R45 (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{v.component.divident=(DEFINED_WEIGHT_TYPE)3.0;v.component.divisor=(DEFINED_WEIGHT_TYPE)20.0;}
//-----------------------------------------------------------------------------
void  WEIGHT_INITIALIZE_DEPTH_MOVEMENTS_R45 (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{v.component.divident=(DEFINED_WEIGHT_TYPE)3.0;v.component.divisor=(DEFINED_WEIGHT_TYPE)20.0;}
//-----------------------------------------------------------------------------
void  WEIGHT_INITIALIZE_START_ANGLE_X_Y (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{v.component.divident=(DEFINED_WEIGHT_TYPE)5.0;v.component.divisor=(DEFINED_WEIGHT_TYPE)1.0;}
//-----------------------------------------------------------------------------
void  WEIGHT_INITIALIZE_START_ANGLE_X_Z (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{v.component.divident=(DEFINED_WEIGHT_TYPE)5.0;v.component.divisor=(DEFINED_WEIGHT_TYPE)1.0;}
//-----------------------------------------------------------------------------
void  WEIGHT_INITIALIZE_START_ANGLE_Z_Y (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{v.component.divident=(DEFINED_WEIGHT_TYPE)5.0;v.component.divisor=(DEFINED_WEIGHT_TYPE)1.0;}
//-----------------------------------------------------------------------------
void  WEIGHT_INITIALIZE_END_ANGLE_X_Y (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{v.component.divident=(DEFINED_WEIGHT_TYPE)5.0;v.component.divisor=(DEFINED_WEIGHT_TYPE)1.0;}
//-----------------------------------------------------------------------------
void  WEIGHT_INITIALIZE_END_ANGLE_X_Z (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{v.component.divident=(DEFINED_WEIGHT_TYPE)5.0;v.component.divisor=(DEFINED_WEIGHT_TYPE)1.0;}
//-----------------------------------------------------------------------------
void  WEIGHT_INITIALIZE_END_ANGLE_Z_Y (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{v.component.divident=(DEFINED_WEIGHT_TYPE)5.0;v.component.divisor=(DEFINED_WEIGHT_TYPE)1.0;}
//-----------------------------------------------------------------------------
void  WEIGHT_INITIALIZE_RELATIVE_CG_X (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{v.component.divident=(DEFINED_WEIGHT_TYPE)1.0;v.component.divisor=(DEFINED_WEIGHT_TYPE)20.0;}
//-----------------------------------------------------------------------------
void  WEIGHT_INITIALIZE_RELATIVE_CG_Y (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{v.component.divident=(DEFINED_WEIGHT_TYPE)1.0;v.component.divisor=(DEFINED_WEIGHT_TYPE)20.0;}
//-----------------------------------------------------------------------------
void  WEIGHT_INITIALIZE_RELATIVE_CG_Z (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{v.component.divident=(DEFINED_WEIGHT_TYPE)1.0;v.component.divisor=(DEFINED_WEIGHT_TYPE)20.0;}
//-----------------------------------------------------------------------------
void  WEIGHT_INITIALIZE_ORIENTATION_STRAIGHTNESS (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{v.component.divident=(DEFINED_WEIGHT_TYPE)5.0;v.component.divisor=(DEFINED_WEIGHT_TYPE)1.0;}
//-----------------------------------------------------------------------------
void  WEIGHT_INITIALIZE_ORIENTATION_HEIGHT_WIDTH_RATIO (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{v.component.divident=(DEFINED_WEIGHT_TYPE)1.0;v.component.divisor=(DEFINED_WEIGHT_TYPE)1.0;}
//-----------------------------------------------------------------------------
void  WEIGHT_INITIALIZE_ORIENTATION_HEIGHT_DEPTH_RATIO (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{v.component.divident=(DEFINED_WEIGHT_TYPE)1.0;v.component.divisor=(DEFINED_WEIGHT_TYPE)1.0;}
//-----------------------------------------------------------------------------
void  WEIGHT_INITIALIZE_ORIENTATION_DEPTH_WIDTH_RATIO (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{v.component.divident=(DEFINED_WEIGHT_TYPE)1.0;v.component.divisor=(DEFINED_WEIGHT_TYPE)1.0;}
//-----------------------------------------------------------------------------
void  WEIGHT_INITIALIZE_ORIENTATION_HEIGHT_WIDTH_RATIO_R45 (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{v.component.divident=(DEFINED_WEIGHT_TYPE)1.0;v.component.divisor=(DEFINED_WEIGHT_TYPE)1.0;}
//-----------------------------------------------------------------------------
void  WEIGHT_INITIALIZE_ORIENTATION_HEIGHT_DEPTH_RATIO_R45 (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{v.component.divident=(DEFINED_WEIGHT_TYPE)1.0;v.component.divisor=(DEFINED_WEIGHT_TYPE)1.0;}
//-----------------------------------------------------------------------------
void  WEIGHT_INITIALIZE_ORIENTATION_DEPTH_WIDTH_RATIO_R45 (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{v.component.divident=(DEFINED_WEIGHT_TYPE)1.0;v.component.divisor=(DEFINED_WEIGHT_TYPE)1.0;}
//-----------------------------------------------------------------------------
void  WEIGHT_INITIALIZE_ORIENTATION_RELATIVE_START_AZIM (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{v.component.divident=(DEFINED_WEIGHT_TYPE)1.0;v.component.divisor=(DEFINED_WEIGHT_TYPE)20.0;}
//-----------------------------------------------------------------------------
void  WEIGHT_INITIALIZE_ORIENTATION_RELATIVE_START_ELEV (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{v.component.divident=(DEFINED_WEIGHT_TYPE)1.0;v.component.divisor=(DEFINED_WEIGHT_TYPE)20.0;}
//-----------------------------------------------------------------------------
void  WEIGHT_INITIALIZE_ORIENTATION_RELATIVE_START_ROLL (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{v.component.divident=(DEFINED_WEIGHT_TYPE)1.0;v.component.divisor=(DEFINED_WEIGHT_TYPE)20.0;}
//-----------------------------------------------------------------------------
void  WEIGHT_INITIALIZE_ORIENTATION_RELATIVE_END_AZIM (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{v.component.divident=(DEFINED_WEIGHT_TYPE)1.0;v.component.divisor=(DEFINED_WEIGHT_TYPE)20.0;}
//-----------------------------------------------------------------------------
void  WEIGHT_INITIALIZE_ORIENTATION_RELATIVE_END_ELEV (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{v.component.divident=(DEFINED_WEIGHT_TYPE)1.0;v.component.divisor=(DEFINED_WEIGHT_TYPE)20.0;}
//-----------------------------------------------------------------------------
void  WEIGHT_INITIALIZE_ORIENTATION_RELATIVE_END_ROLL (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{v.component.divident=(DEFINED_WEIGHT_TYPE)1.0;v.component.divisor=(DEFINED_WEIGHT_TYPE)20.0;}
//-----------------------------------------------------------------------------
void  WEIGHT_INITIALIZE_ORIENTATION_AZIMUTHAL_MOVEMENTS (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{v.component.divident=(DEFINED_WEIGHT_TYPE)3.0;v.component.divisor=(DEFINED_WEIGHT_TYPE)20.0;}
//-----------------------------------------------------------------------------
void  WEIGHT_INITIALIZE_ORIENTATION_ELEVATION_MOVEMENTS (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{v.component.divident=(DEFINED_WEIGHT_TYPE)3.0;v.component.divisor=(DEFINED_WEIGHT_TYPE)20.0;}
//-----------------------------------------------------------------------------
void  WEIGHT_INITIALIZE_ORIENTATION_ROLL_MOVEMENTS (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{v.component.divident=(DEFINED_WEIGHT_TYPE)3.0;v.component.divisor=(DEFINED_WEIGHT_TYPE)20.0;}
//-----------------------------------------------------------------------------
void  WEIGHT_INITIALIZE_ORIENTATION_AZIMUTHAL_MOVEMENTS_R45 (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{v.component.divident=(DEFINED_WEIGHT_TYPE)3.0;v.component.divisor=(DEFINED_WEIGHT_TYPE)20.0;}
//-----------------------------------------------------------------------------
void  WEIGHT_INITIALIZE_ORIENTATION_ELEVATION_MOVEMENTS_R45 (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{v.component.divident=(DEFINED_WEIGHT_TYPE)3.0;v.component.divisor=(DEFINED_WEIGHT_TYPE)20.0;}
//-----------------------------------------------------------------------------
void  WEIGHT_INITIALIZE_ORIENTATION_ROLL_MOVEMENTS_R45 (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{v.component.divident=(DEFINED_WEIGHT_TYPE)3.0;v.component.divisor=(DEFINED_WEIGHT_TYPE)20.0;}
//-----------------------------------------------------------------------------
void  WEIGHT_INITIALIZE_ORIENTATION_START_ANGLE_AZIM_ELEV (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{v.component.divident=(DEFINED_WEIGHT_TYPE)5.0;v.component.divisor=(DEFINED_WEIGHT_TYPE)1.0;}
//-----------------------------------------------------------------------------
void  WEIGHT_INITIALIZE_ORIENTATION_START_ANGLE_AZIM_ROLL (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{v.component.divident=(DEFINED_WEIGHT_TYPE)5.0;v.component.divisor=(DEFINED_WEIGHT_TYPE)1.0;}
//-----------------------------------------------------------------------------
void  WEIGHT_INITIALIZE_ORIENTATION_START_ANGLE_ROLL_ELEV (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{v.component.divident=(DEFINED_WEIGHT_TYPE)5.0;v.component.divisor=(DEFINED_WEIGHT_TYPE)1.0;}
//-----------------------------------------------------------------------------
void  WEIGHT_INITIALIZE_ORIENTATION_END_ANGLE_AZIM_ELEV (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{v.component.divident=(DEFINED_WEIGHT_TYPE)5.0;v.component.divisor=(DEFINED_WEIGHT_TYPE)1.0;}
//-----------------------------------------------------------------------------
void  WEIGHT_INITIALIZE_ORIENTATION_END_ANGLE_AZIM_ROLL (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{v.component.divident=(DEFINED_WEIGHT_TYPE)5.0;v.component.divisor=(DEFINED_WEIGHT_TYPE)1.0;}
//-----------------------------------------------------------------------------
void  WEIGHT_INITIALIZE_ORIENTATION_END_ANGLE_ROLL_ELEV (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{v.component.divident=(DEFINED_WEIGHT_TYPE)5.0;v.component.divisor=(DEFINED_WEIGHT_TYPE)1.0;}
//-----------------------------------------------------------------------------
void  WEIGHT_INITIALIZE_ORIENTATION_RELATIVE_CG_AZIM (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{v.component.divident=(DEFINED_WEIGHT_TYPE)1.0;v.component.divisor=(DEFINED_WEIGHT_TYPE)20.0;}
//-----------------------------------------------------------------------------
void  WEIGHT_INITIALIZE_ORIENTATION_RELATIVE_CG_ELEV (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{v.component.divident=(DEFINED_WEIGHT_TYPE)1.0;v.component.divisor=(DEFINED_WEIGHT_TYPE)20.0;}
//-----------------------------------------------------------------------------
void  WEIGHT_INITIALIZE_ORIENTATION_RELATIVE_CG_ROLL (DEFINED_WEIGHT_CONTENT_TYPE &v,DEFINED_FEATURE_LIST_TYPE fl)
{v.component.divident=(DEFINED_WEIGHT_TYPE)1.0;v.component.divisor=(DEFINED_WEIGHT_TYPE)20.0;}
//-----------------------------------------------------------------------------

#endif

