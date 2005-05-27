#ifndef FEATURE_DEVIATION_LIST_POOL_INCLUDED
#define FEATURE_DEVIATION_LIST_POOL_INCLUDED

#include "feature_deviation_pool.h"

typedef feature_deviation_list<DEFINED_FEATURE_DEVIATION_TYPE,DEFINED_FEATURE_TYPE,DEFINED_PROPERTY_LIST_TYPE,15> FEATURE_DEVIATION_LIST_2D;
typedef feature_deviation_list<DEFINED_FEATURE_DEVIATION_TYPE,DEFINED_FEATURE_TYPE,DEFINED_PROPERTY_LIST_TYPE,28> FEATURE_DEVIATION_LIST_3D;
typedef feature_deviation_list<DEFINED_FEATURE_DEVIATION_TYPE,DEFINED_FEATURE_TYPE,DEFINED_PROPERTY_LIST_TYPE,56> FEATURE_DEVIATION_LIST_6D;

#define DEFINED_FEATURE_DEVIATION_LIST_TYPE	FEATURE_DEVIATION_LIST_2D


//----------------------------------------------------------------------------
#if DEFINED_FEATURE_DEVIATION_LIST_TYPE==FEATURE_DEVIATION_LIST_2D
void build_feature_deviation_list(FEATURE_DEVIATION_LIST_2D &fdl)
{
	feature_deviation<DEFINED_FEATURE_DEVIATION_TYPE,DEFINED_FEATURE_TYPE,DEFINED_PROPERTY_LIST_TYPE> fd;
	
	fd.define(FEATURE_DEVIATION_STRAIGHTNESS,FEATURE_DEVIATION_CALCULATE_MIN_MAX_RATIO);
	fdl.set(0,fd);
    
	fd.define(FEATURE_DEVIATION_HEIGHT_WIDTH_RATIO,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(1,fd);
    
	fd.define(FEATURE_DEVIATION_HEIGHT_WIDTH_RATIO_R45,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(2,fd);

    fd.define(FEATURE_DEVIATION_RELATIVE_START_X,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(3,fd);
    fd.define(FEATURE_DEVIATION_RELATIVE_START_Y,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(4,fd);
    
 
	fd.define(FEATURE_DEVIATION_RELATIVE_END_X,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(5,fd);
    fd.define(FEATURE_DEVIATION_RELATIVE_END_Y,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(6,fd);
    

    fd.define(FEATURE_DEVIATION_HORIZONTAL_MOVEMENTS,FEATURE_DEVIATION_CALCULATE_MIN_MAX_RATIO);
	fdl.set(7,fd);
    fd.define(FEATURE_DEVIATION_VERTICAL_MOVEMENTS,FEATURE_DEVIATION_CALCULATE_MIN_MAX_RATIO);
	fdl.set(8,fd);


    fd.define(FEATURE_DEVIATION_HORIZONTAL_MOVEMENTS_R45,FEATURE_DEVIATION_CALCULATE_MIN_MAX_RATIO);
	fdl.set(9,fd);
    fd.define(FEATURE_DEVIATION_VERTICAL_MOVEMENTS_R45,FEATURE_DEVIATION_CALCULATE_MIN_MAX_RATIO);
	fdl.set(10,fd);
   

    fd.define(FEATURE_DEVIATION_START_ANGLE_X_Y,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(11,fd);
    

	fd.define(FEATURE_DEVIATION_END_ANGLE_X_Y,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(12,fd);
   

    fd.define(FEATURE_DEVIATION_RELATIVE_CG_X,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(13,fd);
    fd.define(FEATURE_DEVIATION_RELATIVE_CG_Y,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(14,fd);
}
#endif
//----------------------------------------------------------------------------
#if DEFINED_FEATURE_DEVIATION_LIST_TYPE==FEATURE_DEVIATION_LIST_3D
void build_feature_deviation_list(FEATURE_DEVIATION_LIST_3D &fdl)
{
	feature_deviation<DEFINED_FEATURE_DEVIATION_TYPE,DEFINED_FEATURE_TYPE,DEFINED_PROPERTY_LIST_TYPE> fd;
	
	fd.define(FEATURE_DEVIATION_STRAIGHTNESS,FEATURE_DEVIATION_CALCULATE_MIN_MAX_RATIO);
	fdl.set(0,fd);
    
	fd.define(FEATURE_DEVIATION_HEIGHT_WIDTH_RATIO,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(1,fd);
	fd.define(FEATURE_DEVIATION_HEIGHT_DEPTH_RATIO,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(2,fd);
    fd.define(FEATURE_DEVIATION_DEPTH_WIDTH_RATIO,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(3,fd);
    
	fd.define(FEATURE_DEVIATION_HEIGHT_WIDTH_RATIO_R45,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(4,fd);
    fd.define(FEATURE_DEVIATION_HEIGHT_DEPTH_RATIO_R45,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(5,fd);
    fd.define(FEATURE_DEVIATION_DEPTH_WIDTH_RATIO_R45,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(6,fd);

    fd.define(FEATURE_DEVIATION_RELATIVE_START_X,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(7,fd);
    fd.define(FEATURE_DEVIATION_RELATIVE_START_Y,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(8,fd);
    fd.define(FEATURE_DEVIATION_RELATIVE_START_Z,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(9,fd);
 
	fd.define(FEATURE_DEVIATION_RELATIVE_END_X,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(10,fd);
    fd.define(FEATURE_DEVIATION_RELATIVE_END_Y,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(11,fd);
    fd.define(FEATURE_DEVIATION_RELATIVE_END_Z,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(12,fd);

    fd.define(FEATURE_DEVIATION_HORIZONTAL_MOVEMENTS,FEATURE_DEVIATION_CALCULATE_MIN_MAX_RATIO);
	fdl.set(13,fd);
    fd.define(FEATURE_DEVIATION_VERTICAL_MOVEMENTS,FEATURE_DEVIATION_CALCULATE_MIN_MAX_RATIO);
	fdl.set(14,fd);
    fd.define(FEATURE_DEVIATION_DEPTH_MOVEMENTS,FEATURE_DEVIATION_CALCULATE_MIN_MAX_RATIO);
	fdl.set(15,fd);


    fd.define(FEATURE_DEVIATION_HORIZONTAL_MOVEMENTS_R45,FEATURE_DEVIATION_CALCULATE_MIN_MAX_RATIO);
	fdl.set(16,fd);
    fd.define(FEATURE_DEVIATION_VERTICAL_MOVEMENTS_R45,FEATURE_DEVIATION_CALCULATE_MIN_MAX_RATIO);
	fdl.set(17,fd);
    fd.define(FEATURE_DEVIATION_DEPTH_MOVEMENTS_R45,FEATURE_DEVIATION_CALCULATE_MIN_MAX_RATIO);
	fdl.set(18,fd);

    fd.define(FEATURE_DEVIATION_START_ANGLE_X_Y,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(19,fd);
    fd.define(FEATURE_DEVIATION_START_ANGLE_X_Z,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(20,fd);
    fd.define(FEATURE_DEVIATION_START_ANGLE_Z_Y,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(21,fd);

	fd.define(FEATURE_DEVIATION_END_ANGLE_X_Y,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(22,fd);
    fd.define(FEATURE_DEVIATION_END_ANGLE_X_Z,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(23,fd);
    fd.define(FEATURE_DEVIATION_END_ANGLE_Z_Y,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(24,fd);

    fd.define(FEATURE_DEVIATION_RELATIVE_CG_X,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(25,fd);
    fd.define(FEATURE_DEVIATION_RELATIVE_CG_Y,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(26,fd);
    fd.define(FEATURE_DEVIATION_RELATIVE_CG_Z,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(27,fd);
}
#endif
//----------------------------------------------------------------------------
#if DEFINED_FEATURE_DEVIATION_LIST_TYPE==FEATURE_DEVIATION_LIST_6D
void build_feature_deviation_list(FEATURE_DEVIATION_LIST_6D &fdl)
{
	feature_deviation<DEFINED_FEATURE_DEVIATION_TYPE,DEFINED_FEATURE_TYPE,DEFINED_PROPERTY_LIST_TYPE> fd;
	
	fd.define(FEATURE_DEVIATION_STRAIGHTNESS,FEATURE_DEVIATION_CALCULATE_MIN_MAX_RATIO);
	fdl.set(0,fd);
    
	fd.define(FEATURE_DEVIATION_HEIGHT_WIDTH_RATIO,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(1,fd);
	fd.define(FEATURE_DEVIATION_HEIGHT_DEPTH_RATIO,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(2,fd);
    fd.define(FEATURE_DEVIATION_DEPTH_WIDTH_RATIO,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(3,fd);
    
	fd.define(FEATURE_DEVIATION_HEIGHT_WIDTH_RATIO_R45,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(4,fd);
    fd.define(FEATURE_DEVIATION_HEIGHT_DEPTH_RATIO_R45,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(5,fd);
    fd.define(FEATURE_DEVIATION_DEPTH_WIDTH_RATIO_R45,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(6,fd);

    fd.define(FEATURE_DEVIATION_RELATIVE_START_X,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(7,fd);
    fd.define(FEATURE_DEVIATION_RELATIVE_START_Y,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(8,fd);
    fd.define(FEATURE_DEVIATION_RELATIVE_START_Z,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(9,fd);
 
	fd.define(FEATURE_DEVIATION_RELATIVE_END_X,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(10,fd);
    fd.define(FEATURE_DEVIATION_RELATIVE_END_Y,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(11,fd);
    fd.define(FEATURE_DEVIATION_RELATIVE_END_Z,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(12,fd);

    fd.define(FEATURE_DEVIATION_HORIZONTAL_MOVEMENTS,FEATURE_DEVIATION_CALCULATE_MIN_MAX_RATIO);
	fdl.set(13,fd);
    fd.define(FEATURE_DEVIATION_VERTICAL_MOVEMENTS,FEATURE_DEVIATION_CALCULATE_MIN_MAX_RATIO);
	fdl.set(14,fd);
    fd.define(FEATURE_DEVIATION_DEPTH_MOVEMENTS,FEATURE_DEVIATION_CALCULATE_MIN_MAX_RATIO);
	fdl.set(15,fd);


    fd.define(FEATURE_DEVIATION_HORIZONTAL_MOVEMENTS_R45,FEATURE_DEVIATION_CALCULATE_MIN_MAX_RATIO);
	fdl.set(16,fd);
    fd.define(FEATURE_DEVIATION_VERTICAL_MOVEMENTS_R45,FEATURE_DEVIATION_CALCULATE_MIN_MAX_RATIO);
	fdl.set(17,fd);
    fd.define(FEATURE_DEVIATION_DEPTH_MOVEMENTS_R45,FEATURE_DEVIATION_CALCULATE_MIN_MAX_RATIO);
	fdl.set(18,fd);

    fd.define(FEATURE_DEVIATION_START_ANGLE_X_Y,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(19,fd);
    fd.define(FEATURE_DEVIATION_START_ANGLE_X_Z,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(20,fd);
    fd.define(FEATURE_DEVIATION_START_ANGLE_Z_Y,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(21,fd);

	fd.define(FEATURE_DEVIATION_END_ANGLE_X_Y,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(22,fd);
    fd.define(FEATURE_DEVIATION_END_ANGLE_X_Z,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(23,fd);
    fd.define(FEATURE_DEVIATION_END_ANGLE_Z_Y,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(24,fd);

    fd.define(FEATURE_DEVIATION_RELATIVE_CG_X,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(25,fd);
    fd.define(FEATURE_DEVIATION_RELATIVE_CG_Y,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(26,fd);
    fd.define(FEATURE_DEVIATION_RELATIVE_CG_Z,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(27,fd);


	fd.define(FEATURE_DEVIATION_ORIENTATION_STRAIGHTNESS,FEATURE_DEVIATION_CALCULATE_MIN_MAX_RATIO);
	fdl.set(28,fd);
    
	fd.define(FEATURE_DEVIATION_ORIENTATION_HEIGHT_WIDTH_RATIO,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(29,fd);
	fd.define(FEATURE_DEVIATION_ORIENTATION_HEIGHT_DEPTH_RATIO,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(30,fd);
    fd.define(FEATURE_DEVIATION_ORIENTATION_DEPTH_WIDTH_RATIO,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(31,fd);
    
	fd.define(FEATURE_DEVIATION_ORIENTATION_HEIGHT_WIDTH_RATIO_R45,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(32,fd);
    fd.define(FEATURE_DEVIATION_ORIENTATION_HEIGHT_DEPTH_RATIO_R45,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(33,fd);
    fd.define(FEATURE_DEVIATION_ORIENTATION_DEPTH_WIDTH_RATIO_R45,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(34,fd);

    fd.define(FEATURE_DEVIATION_ORIENTATION_RELATIVE_START_AZIM,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(35,fd);
    fd.define(FEATURE_DEVIATION_ORIENTATION_RELATIVE_START_ELEV,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(36,fd);
    fd.define(FEATURE_DEVIATION_ORIENTATION_RELATIVE_START_ROLL,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(37,fd);
 
	fd.define(FEATURE_DEVIATION_ORIENTATION_RELATIVE_END_AZIM,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(38,fd);
    fd.define(FEATURE_DEVIATION_ORIENTATION_RELATIVE_END_ELEV,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(39,fd);
    fd.define(FEATURE_DEVIATION_ORIENTATION_RELATIVE_END_ROLL,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(40,fd);

    fd.define(FEATURE_DEVIATION_ORIENTATION_AZIMUTHAL_MOVEMENTS,FEATURE_DEVIATION_CALCULATE_MIN_MAX_RATIO);
	fdl.set(41,fd);
    fd.define(FEATURE_DEVIATION_ORIENTATION_ELEVATION_MOVEMENTS,FEATURE_DEVIATION_CALCULATE_MIN_MAX_RATIO);
	fdl.set(42,fd);
    fd.define(FEATURE_DEVIATION_ORIENTATION_ROLL_MOVEMENTS,FEATURE_DEVIATION_CALCULATE_MIN_MAX_RATIO);
	fdl.set(43,fd);


    fd.define(FEATURE_DEVIATION_ORIENTATION_AZIMUTHAL_MOVEMENTS_R45,FEATURE_DEVIATION_CALCULATE_MIN_MAX_RATIO);
	fdl.set(44,fd);
    fd.define(FEATURE_DEVIATION_ORIENTATION_ELEVATION_MOVEMENTS_R45,FEATURE_DEVIATION_CALCULATE_MIN_MAX_RATIO);
	fdl.set(45,fd);
    fd.define(FEATURE_DEVIATION_ORIENTATION_ROLL_MOVEMENTS_R45,FEATURE_DEVIATION_CALCULATE_MIN_MAX_RATIO);
	fdl.set(46,fd);

    fd.define(FEATURE_DEVIATION_ORIENTATION_START_ANGLE_AZIM_ELEV,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(47,fd);
    fd.define(FEATURE_DEVIATION_ORIENTATION_START_ANGLE_AZIM_ROLL,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(48,fd);
    fd.define(FEATURE_DEVIATION_ORIENTATION_START_ANGLE_ROLL_ELEV,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(49,fd);

	fd.define(FEATURE_DEVIATION_ORIENTATION_END_ANGLE_AZIM_ELEV,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(50,fd);
    fd.define(FEATURE_DEVIATION_ORIENTATION_END_ANGLE_AZIM_ROLL,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(51,fd);
    fd.define(FEATURE_DEVIATION_ORIENTATION_END_ANGLE_ROLL_ELEV,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(52,fd);

    fd.define(FEATURE_DEVIATION_ORIENTATION_RELATIVE_CG_AZIM,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(53,fd);
    fd.define(FEATURE_DEVIATION_ORIENTATION_RELATIVE_CG_ELEV,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(54,fd);
    fd.define(FEATURE_DEVIATION_ORIENTATION_RELATIVE_CG_ROLL,FEATURE_DEVIATION_CALCULATE_ABS_DIFFERENCE);
	fdl.set(55,fd);
}
#endif
//----------------------------------------------------------------------------

#endif

