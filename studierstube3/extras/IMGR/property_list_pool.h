#ifndef PROPERTY_LIST_POOL_INCLUDED
#define PROPERTY_LIST_POOL_INCLUDED

#include "property_list.h"
#include "property_pool.h"

typedef property_list<DEFINED_PROPERTY_TYPE,DEFINED_SAMPLE_TYPE,30> PROPERTY_LIST_2D;
typedef property_list<DEFINED_PROPERTY_TYPE,DEFINED_SAMPLE_TYPE,43> PROPERTY_LIST_3D;
typedef property_list<DEFINED_PROPERTY_TYPE,DEFINED_SAMPLE_TYPE,77> PROPERTY_LIST_6D;

#define DEFINED_PROPERTY_LIST_TYPE	PROPERTY_LIST_2D


//----------------------------------------------------------------------------
#if DEFINED_PROPERTY_LIST_TYPE==PROPERTY_LIST_2D
void build_property_list(PROPERTY_LIST_2D &pl)
{
	property<DEFINED_PROPERTY_TYPE,DEFINED_SAMPLE_TYPE> p;
	
	p.define(PROPERTY_BOUNDING_BOX_LEFT,PROPERTY_UPDATE_BOUNDING_BOX_LEFT,PROPERTY_INITIALIZE_BOUNDING_BOX_LEFT);
	pl.set(0,p);
	p.define(PROPERTY_BOUNDING_BOX_RIGHT,PROPERTY_UPDATE_BOUNDING_BOX_RIGHT,PROPERTY_INITIALIZE_BOUNDING_BOX_RIGHT);
	pl.set(1,p);
    p.define(PROPERTY_BOUNDING_BOX_TOP,PROPERTY_UPDATE_BOUNDING_BOX_TOP,PROPERTY_INITIALIZE_BOUNDING_BOX_TOP);
	pl.set(2,p);
	p.define(PROPERTY_BOUNDING_BOX_BOTTOM,PROPERTY_UPDATE_BOUNDING_BOX_BOTTOM,PROPERTY_INITIALIZE_BOUNDING_BOX_BOTTOM);
	pl.set(3,p);

	p.define(PROPERTY_BOUNDING_BOX_LEFT_R45,PROPERTY_UPDATE_BOUNDING_BOX_LEFT_R45,PROPERTY_INITIALIZE_BOUNDING_BOX_LEFT_R45);
	pl.set(4,p);
	p.define(PROPERTY_BOUNDING_BOX_RIGHT_R45,PROPERTY_UPDATE_BOUNDING_BOX_RIGHT_R45,PROPERTY_INITIALIZE_BOUNDING_BOX_RIGHT_R45);
	pl.set(5,p);
	p.define(PROPERTY_BOUNDING_BOX_TOP_R45,PROPERTY_UPDATE_BOUNDING_BOX_TOP_R45,PROPERTY_INITIALIZE_BOUNDING_BOX_TOP_R45);
	pl.set(6,p);
	p.define(PROPERTY_BOUNDING_BOX_BOTTOM_R45,PROPERTY_UPDATE_BOUNDING_BOX_BOTTOM_R45,PROPERTY_INITIALIZE_BOUNDING_BOX_BOTTOM_R45);
	pl.set(7,p);

	p.define(PROPERTY_TOTAL_LENGTH,PROPERTY_UPDATE_TOTAL_LENGTH,PROPERTY_INITIALIZE_TOTAL_LENGTH);
	pl.set(8,p);

	p.define(PROPERTY_ANGLE_COEFF_SAX,PROPERTY_UPDATE_ANGLE_COEFF_SAX,PROPERTY_INITIALIZE_ANGLE_COEFF_SAX);
	pl.set(9,p);
	p.define(PROPERTY_ANGLE_COEFF_SAY,PROPERTY_UPDATE_ANGLE_COEFF_SAY,PROPERTY_INITIALIZE_ANGLE_COEFF_SAY);
	pl.set(10,p);
	p.define(PROPERTY_ANGLE_COEFF_EAX,PROPERTY_UPDATE_ANGLE_COEFF_EAX,PROPERTY_INITIALIZE_ANGLE_COEFF_EAX);
	pl.set(11,p);
	p.define(PROPERTY_ANGLE_COEFF_EAY,PROPERTY_UPDATE_ANGLE_COEFF_EAY,PROPERTY_INITIALIZE_ANGLE_COEFF_EAY);
	pl.set(12,p);
	
	p.define(PROPERTY_SUM_X,PROPERTY_UPDATE_SUM_X,PROPERTY_INITIALIZE_SUM_X);
	pl.set(13,p);
	p.define(PROPERTY_SUM_Y,PROPERTY_UPDATE_SUM_Y,PROPERTY_INITIALIZE_SUM_Y);
	pl.set(14,p);
	
	p.define(PROPERTY_HORIZONTAL_MOVEMENTS,PROPERTY_UPDATE_HORIZONTAL_MOVEMENTS,PROPERTY_INITIALIZE_HORIZONTAL_MOVEMENTS);
	pl.set(15,p);
	p.define(PROPERTY_VERTICAL_MOVEMENTS,PROPERTY_UPDATE_VERTICAL_MOVEMENTS,PROPERTY_INITIALIZE_VERTICAL_MOVEMENTS);
	pl.set(16,p);
	
	p.define(PROPERTY_HORIZONTAL_MOVEMENTS_R45,PROPERTY_UPDATE_HORIZONTAL_MOVEMENTS_R45,PROPERTY_INITIALIZE_HORIZONTAL_MOVEMENTS_R45);
	pl.set(17,p);
	p.define(PROPERTY_VERTICAL_MOVEMENTS_R45,PROPERTY_UPDATE_VERTICAL_MOVEMENTS_R45,PROPERTY_INITIALIZE_VERTICAL_MOVEMENTS_R45);
	pl.set(18,p);

	p.define(PROPERTY_START_X,PROPERTY_UPDATE_START_X,PROPERTY_INITIALIZE_START_X);
	pl.set(19,p);
	p.define(PROPERTY_START_Y,PROPERTY_UPDATE_START_Y,PROPERTY_INITIALIZE_START_Y);
	pl.set(20,p);
    
	p.define(PROPERTY_END_X,PROPERTY_UPDATE_END_X,PROPERTY_INITIALIZE_END_X);
	pl.set(21,p);
	p.define(PROPERTY_END_Y,PROPERTY_UPDATE_END_Y,PROPERTY_INITIALIZE_END_Y);
	pl.set(22,p);

	p.define(PROPERTY_NUMBER_OF_SAMPLES,PROPERTY_UPDATE_NUMBER_OF_SAMPLES,PROPERTY_INITIALIZE_NUMBER_OF_SAMPLES);
	pl.set(23,p);

    p.define(PROPERTY_START_TIME,PROPERTY_UPDATE_START_TIME,PROPERTY_INITIALIZE_START_TIME);
	pl.set(24,p);
	p.define(PROPERTY_END_TIME,PROPERTY_UPDATE_END_TIME,PROPERTY_INITIALIZE_END_TIME);
	pl.set(25,p);

    p.define(PROPERTY_MIN_CG_2_CURVE_X,PROPERTY_UPDATE_MIN_CG_2_CURVE_X,PROPERTY_INITIALIZE_MIN_CG_2_CURVE_X);
    p.set_dependence_level(1);
	pl.set(26,p);
	p.define(PROPERTY_MIN_CG_2_CURVE_Y,PROPERTY_UPDATE_MIN_CG_2_CURVE_Y,PROPERTY_INITIALIZE_MIN_CG_2_CURVE_Y);
	p.set_dependence_level(1);
	pl.set(27,p);
	p.define(PROPERTY_MAX_CG_2_CURVE_X,PROPERTY_UPDATE_MAX_CG_2_CURVE_X,PROPERTY_INITIALIZE_MAX_CG_2_CURVE_X);
	p.set_dependence_level(1);
	pl.set(28,p);
	p.define(PROPERTY_MAX_CG_2_CURVE_Y,PROPERTY_UPDATE_MAX_CG_2_CURVE_Y,PROPERTY_INITIALIZE_MAX_CG_2_CURVE_Y);
	p.set_dependence_level(1);
	pl.set(29,p);
}
#endif
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
#if DEFINED_PROPERTY_LIST_TYPE==PROPERTY_LIST_3D
void build_property_list(PROPERTY_LIST_3D &pl)
{
	property<DEFINED_PROPERTY_TYPE,DEFINED_SAMPLE_TYPE> p;
	
	p.define(PROPERTY_BOUNDING_BOX_LEFT,PROPERTY_UPDATE_BOUNDING_BOX_LEFT,PROPERTY_INITIALIZE_BOUNDING_BOX_LEFT);
	pl.set(0,p);
	p.define(PROPERTY_BOUNDING_BOX_RIGHT,PROPERTY_UPDATE_BOUNDING_BOX_RIGHT,PROPERTY_INITIALIZE_BOUNDING_BOX_RIGHT);
	pl.set(1,p);
    p.define(PROPERTY_BOUNDING_BOX_FRONT,PROPERTY_UPDATE_BOUNDING_BOX_FRONT,PROPERTY_INITIALIZE_BOUNDING_BOX_FRONT);
	pl.set(2,p);
    p.define(PROPERTY_BOUNDING_BOX_BACK,PROPERTY_UPDATE_BOUNDING_BOX_BACK,PROPERTY_INITIALIZE_BOUNDING_BOX_BACK);
	pl.set(3,p);
	p.define(PROPERTY_BOUNDING_BOX_TOP,PROPERTY_UPDATE_BOUNDING_BOX_TOP,PROPERTY_INITIALIZE_BOUNDING_BOX_TOP);
	pl.set(4,p);
	p.define(PROPERTY_BOUNDING_BOX_BOTTOM,PROPERTY_UPDATE_BOUNDING_BOX_BOTTOM,PROPERTY_INITIALIZE_BOUNDING_BOX_BOTTOM);
	pl.set(5,p);

	p.define(PROPERTY_BOUNDING_BOX_LEFT_R45,PROPERTY_UPDATE_BOUNDING_BOX_LEFT_R45,PROPERTY_INITIALIZE_BOUNDING_BOX_LEFT_R45);
	pl.set(6,p);
	p.define(PROPERTY_BOUNDING_BOX_RIGHT_R45,PROPERTY_UPDATE_BOUNDING_BOX_RIGHT_R45,PROPERTY_INITIALIZE_BOUNDING_BOX_RIGHT_R45);
	pl.set(7,p);
	p.define(PROPERTY_BOUNDING_BOX_FRONT_R45,PROPERTY_UPDATE_BOUNDING_BOX_FRONT_R45,PROPERTY_INITIALIZE_BOUNDING_BOX_FRONT_R45);
	pl.set(8,p);
	p.define(PROPERTY_BOUNDING_BOX_BACK_R45,PROPERTY_UPDATE_BOUNDING_BOX_BACK_R45,PROPERTY_INITIALIZE_BOUNDING_BOX_BACK_R45);
	pl.set(9,p);
	p.define(PROPERTY_BOUNDING_BOX_TOP_R45,PROPERTY_UPDATE_BOUNDING_BOX_TOP_R45,PROPERTY_INITIALIZE_BOUNDING_BOX_TOP_R45);
	pl.set(10,p);
	p.define(PROPERTY_BOUNDING_BOX_BOTTOM_R45,PROPERTY_UPDATE_BOUNDING_BOX_BOTTOM_R45,PROPERTY_INITIALIZE_BOUNDING_BOX_BOTTOM_R45);
	pl.set(11,p);

	p.define(PROPERTY_TOTAL_LENGTH,PROPERTY_UPDATE_TOTAL_LENGTH,PROPERTY_INITIALIZE_TOTAL_LENGTH);
	pl.set(12,p);

	p.define(PROPERTY_ANGLE_COEFF_SAX,PROPERTY_UPDATE_ANGLE_COEFF_SAX,PROPERTY_INITIALIZE_ANGLE_COEFF_SAX);
	pl.set(13,p);
	p.define(PROPERTY_ANGLE_COEFF_SAY,PROPERTY_UPDATE_ANGLE_COEFF_SAY,PROPERTY_INITIALIZE_ANGLE_COEFF_SAY);
	pl.set(14,p);
	p.define(PROPERTY_ANGLE_COEFF_SAZ,PROPERTY_UPDATE_ANGLE_COEFF_SAZ,PROPERTY_INITIALIZE_ANGLE_COEFF_SAZ);
	pl.set(15,p);
	p.define(PROPERTY_ANGLE_COEFF_EAX,PROPERTY_UPDATE_ANGLE_COEFF_EAX,PROPERTY_INITIALIZE_ANGLE_COEFF_EAX);
	pl.set(16,p);
	p.define(PROPERTY_ANGLE_COEFF_EAY,PROPERTY_UPDATE_ANGLE_COEFF_EAY,PROPERTY_INITIALIZE_ANGLE_COEFF_EAY);
	pl.set(17,p);
	p.define(PROPERTY_ANGLE_COEFF_EAZ,PROPERTY_UPDATE_ANGLE_COEFF_EAZ,PROPERTY_INITIALIZE_ANGLE_COEFF_EAZ);
	pl.set(18,p);
	
	p.define(PROPERTY_SUM_X,PROPERTY_UPDATE_SUM_X,PROPERTY_INITIALIZE_SUM_X);
	pl.set(19,p);
	p.define(PROPERTY_SUM_Y,PROPERTY_UPDATE_SUM_Y,PROPERTY_INITIALIZE_SUM_Y);
	pl.set(20,p);
	p.define(PROPERTY_SUM_Z,PROPERTY_UPDATE_SUM_Z,PROPERTY_INITIALIZE_SUM_Z);
	pl.set(21,p);

	p.define(PROPERTY_HORIZONTAL_MOVEMENTS,PROPERTY_UPDATE_HORIZONTAL_MOVEMENTS,PROPERTY_INITIALIZE_HORIZONTAL_MOVEMENTS);
	pl.set(22,p);
	p.define(PROPERTY_VERTICAL_MOVEMENTS,PROPERTY_UPDATE_VERTICAL_MOVEMENTS,PROPERTY_INITIALIZE_VERTICAL_MOVEMENTS);
	pl.set(23,p);
	p.define(PROPERTY_DEPTH_MOVEMENTS,PROPERTY_UPDATE_DEPTH_MOVEMENTS,PROPERTY_INITIALIZE_DEPTH_MOVEMENTS);
	pl.set(24,p);

	p.define(PROPERTY_HORIZONTAL_MOVEMENTS_R45,PROPERTY_UPDATE_HORIZONTAL_MOVEMENTS_R45,PROPERTY_INITIALIZE_HORIZONTAL_MOVEMENTS_R45);
	pl.set(25,p);
	p.define(PROPERTY_VERTICAL_MOVEMENTS_R45,PROPERTY_UPDATE_VERTICAL_MOVEMENTS_R45,PROPERTY_INITIALIZE_VERTICAL_MOVEMENTS_R45);
	pl.set(26,p);
	p.define(PROPERTY_DEPTH_MOVEMENTS_R45,PROPERTY_UPDATE_DEPTH_MOVEMENTS_R45,PROPERTY_INITIALIZE_DEPTH_MOVEMENTS_R45);
	pl.set(27,p);

	p.define(PROPERTY_START_X,PROPERTY_UPDATE_START_X,PROPERTY_INITIALIZE_START_X);
	pl.set(28,p);
	p.define(PROPERTY_START_Y,PROPERTY_UPDATE_START_Y,PROPERTY_INITIALIZE_START_Y);
	pl.set(29,p);
	p.define(PROPERTY_START_Z,PROPERTY_UPDATE_START_Z,PROPERTY_INITIALIZE_START_Z);
	pl.set(30,p);

	p.define(PROPERTY_END_X,PROPERTY_UPDATE_END_X,PROPERTY_INITIALIZE_END_X);
	pl.set(31,p);
	p.define(PROPERTY_END_Y,PROPERTY_UPDATE_END_Y,PROPERTY_INITIALIZE_END_Y);
	pl.set(32,p);
	p.define(PROPERTY_END_Z,PROPERTY_UPDATE_END_Z,PROPERTY_INITIALIZE_END_Z);
	pl.set(33,p);

	p.define(PROPERTY_NUMBER_OF_SAMPLES,PROPERTY_UPDATE_NUMBER_OF_SAMPLES,PROPERTY_INITIALIZE_NUMBER_OF_SAMPLES);
	pl.set(34,p);

	p.define(PROPERTY_START_TIME,PROPERTY_UPDATE_START_TIME,PROPERTY_INITIALIZE_START_TIME);
	pl.set(35,p);
	p.define(PROPERTY_END_TIME,PROPERTY_UPDATE_END_TIME,PROPERTY_INITIALIZE_END_TIME);
	pl.set(36,p);

	p.define(PROPERTY_MIN_CG_2_CURVE_X,PROPERTY_UPDATE_MIN_CG_2_CURVE_X,PROPERTY_INITIALIZE_MIN_CG_2_CURVE_X);
    p.set_dependence_level(1);
	pl.set(37,p);
	p.define(PROPERTY_MIN_CG_2_CURVE_Y,PROPERTY_UPDATE_MIN_CG_2_CURVE_Y,PROPERTY_INITIALIZE_MIN_CG_2_CURVE_Y);
	p.set_dependence_level(1);
	pl.set(38,p);
	p.define(PROPERTY_MIN_CG_2_CURVE_Z,PROPERTY_UPDATE_MIN_CG_2_CURVE_Z,PROPERTY_INITIALIZE_MIN_CG_2_CURVE_Z);
	p.set_dependence_level(1);
	pl.set(39,p);
	p.define(PROPERTY_MAX_CG_2_CURVE_X,PROPERTY_UPDATE_MAX_CG_2_CURVE_X,PROPERTY_INITIALIZE_MAX_CG_2_CURVE_X);
	p.set_dependence_level(1);
	pl.set(40,p);
	p.define(PROPERTY_MAX_CG_2_CURVE_Y,PROPERTY_UPDATE_MAX_CG_2_CURVE_Y,PROPERTY_INITIALIZE_MAX_CG_2_CURVE_Y);
	p.set_dependence_level(1);
	pl.set(41,p);
	p.define(PROPERTY_MAX_CG_2_CURVE_Z,PROPERTY_UPDATE_MAX_CG_2_CURVE_Z,PROPERTY_INITIALIZE_MAX_CG_2_CURVE_Z);
	p.set_dependence_level(1);
	pl.set(42,p);
}
#endif
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
#if DEFINED_PROPERTY_LIST_TYPE==PROPERTY_LIST_6D
void build_property_list(PROPERTY_LIST_6D &pl)
{
	property<DEFINED_PROPERTY_TYPE,DEFINED_SAMPLE_TYPE> p;
	
	p.define(PROPERTY_BOUNDING_BOX_LEFT,PROPERTY_UPDATE_BOUNDING_BOX_LEFT,PROPERTY_INITIALIZE_BOUNDING_BOX_LEFT);
	pl.set(0,p);
	p.define(PROPERTY_BOUNDING_BOX_RIGHT,PROPERTY_UPDATE_BOUNDING_BOX_RIGHT,PROPERTY_INITIALIZE_BOUNDING_BOX_RIGHT);
	pl.set(1,p);
    p.define(PROPERTY_BOUNDING_BOX_FRONT,PROPERTY_UPDATE_BOUNDING_BOX_FRONT,PROPERTY_INITIALIZE_BOUNDING_BOX_FRONT);
	pl.set(2,p);
    p.define(PROPERTY_BOUNDING_BOX_BACK,PROPERTY_UPDATE_BOUNDING_BOX_BACK,PROPERTY_INITIALIZE_BOUNDING_BOX_BACK);
	pl.set(3,p);
	p.define(PROPERTY_BOUNDING_BOX_TOP,PROPERTY_UPDATE_BOUNDING_BOX_TOP,PROPERTY_INITIALIZE_BOUNDING_BOX_TOP);
	pl.set(4,p);
	p.define(PROPERTY_BOUNDING_BOX_BOTTOM,PROPERTY_UPDATE_BOUNDING_BOX_BOTTOM,PROPERTY_INITIALIZE_BOUNDING_BOX_BOTTOM);
	pl.set(5,p);

	p.define(PROPERTY_BOUNDING_BOX_LEFT_R45,PROPERTY_UPDATE_BOUNDING_BOX_LEFT_R45,PROPERTY_INITIALIZE_BOUNDING_BOX_LEFT_R45);
	pl.set(6,p);
	p.define(PROPERTY_BOUNDING_BOX_RIGHT_R45,PROPERTY_UPDATE_BOUNDING_BOX_RIGHT_R45,PROPERTY_INITIALIZE_BOUNDING_BOX_RIGHT_R45);
	pl.set(7,p);
	p.define(PROPERTY_BOUNDING_BOX_FRONT_R45,PROPERTY_UPDATE_BOUNDING_BOX_FRONT_R45,PROPERTY_INITIALIZE_BOUNDING_BOX_FRONT_R45);
	pl.set(8,p);
	p.define(PROPERTY_BOUNDING_BOX_BACK_R45,PROPERTY_UPDATE_BOUNDING_BOX_BACK_R45,PROPERTY_INITIALIZE_BOUNDING_BOX_BACK_R45);
	pl.set(9,p);
	p.define(PROPERTY_BOUNDING_BOX_TOP_R45,PROPERTY_UPDATE_BOUNDING_BOX_TOP_R45,PROPERTY_INITIALIZE_BOUNDING_BOX_TOP_R45);
	pl.set(10,p);
	p.define(PROPERTY_BOUNDING_BOX_BOTTOM_R45,PROPERTY_UPDATE_BOUNDING_BOX_BOTTOM_R45,PROPERTY_INITIALIZE_BOUNDING_BOX_BOTTOM_R45);
	pl.set(11,p);

	p.define(PROPERTY_TOTAL_LENGTH,PROPERTY_UPDATE_TOTAL_LENGTH,PROPERTY_INITIALIZE_TOTAL_LENGTH);
	pl.set(12,p);

	p.define(PROPERTY_ANGLE_COEFF_SAX,PROPERTY_UPDATE_ANGLE_COEFF_SAX,PROPERTY_INITIALIZE_ANGLE_COEFF_SAX);
	pl.set(13,p);
	p.define(PROPERTY_ANGLE_COEFF_SAY,PROPERTY_UPDATE_ANGLE_COEFF_SAY,PROPERTY_INITIALIZE_ANGLE_COEFF_SAY);
	pl.set(14,p);
	p.define(PROPERTY_ANGLE_COEFF_SAZ,PROPERTY_UPDATE_ANGLE_COEFF_SAZ,PROPERTY_INITIALIZE_ANGLE_COEFF_SAZ);
	pl.set(15,p);
	p.define(PROPERTY_ANGLE_COEFF_EAX,PROPERTY_UPDATE_ANGLE_COEFF_EAX,PROPERTY_INITIALIZE_ANGLE_COEFF_EAX);
	pl.set(16,p);
	p.define(PROPERTY_ANGLE_COEFF_EAY,PROPERTY_UPDATE_ANGLE_COEFF_EAY,PROPERTY_INITIALIZE_ANGLE_COEFF_EAY);
	pl.set(17,p);
	p.define(PROPERTY_ANGLE_COEFF_EAZ,PROPERTY_UPDATE_ANGLE_COEFF_EAZ,PROPERTY_INITIALIZE_ANGLE_COEFF_EAZ);
	pl.set(18,p);
	
	p.define(PROPERTY_SUM_X,PROPERTY_UPDATE_SUM_X,PROPERTY_INITIALIZE_SUM_X);
	pl.set(19,p);
	p.define(PROPERTY_SUM_Y,PROPERTY_UPDATE_SUM_Y,PROPERTY_INITIALIZE_SUM_Y);
	pl.set(20,p);
	p.define(PROPERTY_SUM_Z,PROPERTY_UPDATE_SUM_Z,PROPERTY_INITIALIZE_SUM_Z);
	pl.set(21,p);

	p.define(PROPERTY_HORIZONTAL_MOVEMENTS,PROPERTY_UPDATE_HORIZONTAL_MOVEMENTS,PROPERTY_INITIALIZE_HORIZONTAL_MOVEMENTS);
	pl.set(22,p);
	p.define(PROPERTY_VERTICAL_MOVEMENTS,PROPERTY_UPDATE_VERTICAL_MOVEMENTS,PROPERTY_INITIALIZE_VERTICAL_MOVEMENTS);
	pl.set(23,p);
	p.define(PROPERTY_DEPTH_MOVEMENTS,PROPERTY_UPDATE_DEPTH_MOVEMENTS,PROPERTY_INITIALIZE_DEPTH_MOVEMENTS);
	pl.set(24,p);

	p.define(PROPERTY_HORIZONTAL_MOVEMENTS_R45,PROPERTY_UPDATE_HORIZONTAL_MOVEMENTS_R45,PROPERTY_INITIALIZE_HORIZONTAL_MOVEMENTS_R45);
	pl.set(25,p);
	p.define(PROPERTY_VERTICAL_MOVEMENTS_R45,PROPERTY_UPDATE_VERTICAL_MOVEMENTS_R45,PROPERTY_INITIALIZE_VERTICAL_MOVEMENTS_R45);
	pl.set(26,p);
	p.define(PROPERTY_DEPTH_MOVEMENTS_R45,PROPERTY_UPDATE_DEPTH_MOVEMENTS_R45,PROPERTY_INITIALIZE_DEPTH_MOVEMENTS_R45);
	pl.set(27,p);


	p.define(PROPERTY_ORIENTATION_BOUNDING_BOX_LEFT,PROPERTY_UPDATE_ORIENTATION_BOUNDING_BOX_LEFT,PROPERTY_INITIALIZE_ORIENTATION_BOUNDING_BOX_LEFT);
	pl.set(28,p);
	p.define(PROPERTY_ORIENTATION_BOUNDING_BOX_RIGHT,PROPERTY_UPDATE_ORIENTATION_BOUNDING_BOX_RIGHT,PROPERTY_INITIALIZE_ORIENTATION_BOUNDING_BOX_RIGHT);
	pl.set(29,p);
    p.define(PROPERTY_ORIENTATION_BOUNDING_BOX_FRONT,PROPERTY_UPDATE_ORIENTATION_BOUNDING_BOX_FRONT,PROPERTY_INITIALIZE_ORIENTATION_BOUNDING_BOX_FRONT);
	pl.set(30,p);
    p.define(PROPERTY_ORIENTATION_BOUNDING_BOX_BACK,PROPERTY_UPDATE_ORIENTATION_BOUNDING_BOX_BACK,PROPERTY_INITIALIZE_ORIENTATION_BOUNDING_BOX_BACK);
	pl.set(31,p);
	p.define(PROPERTY_ORIENTATION_BOUNDING_BOX_TOP,PROPERTY_UPDATE_ORIENTATION_BOUNDING_BOX_TOP,PROPERTY_INITIALIZE_ORIENTATION_BOUNDING_BOX_TOP);
	pl.set(32,p);
	p.define(PROPERTY_ORIENTATION_BOUNDING_BOX_BOTTOM,PROPERTY_UPDATE_ORIENTATION_BOUNDING_BOX_BOTTOM,PROPERTY_INITIALIZE_ORIENTATION_BOUNDING_BOX_BOTTOM);
	pl.set(33,p);

	p.define(PROPERTY_ORIENTATION_BOUNDING_BOX_LEFT_R45,PROPERTY_UPDATE_ORIENTATION_BOUNDING_BOX_LEFT_R45,PROPERTY_INITIALIZE_ORIENTATION_BOUNDING_BOX_LEFT_R45);
	pl.set(34,p);
	p.define(PROPERTY_ORIENTATION_BOUNDING_BOX_RIGHT_R45,PROPERTY_UPDATE_ORIENTATION_BOUNDING_BOX_RIGHT_R45,PROPERTY_INITIALIZE_ORIENTATION_BOUNDING_BOX_RIGHT_R45);
	pl.set(35,p);
	p.define(PROPERTY_ORIENTATION_BOUNDING_BOX_FRONT_R45,PROPERTY_UPDATE_ORIENTATION_BOUNDING_BOX_FRONT_R45,PROPERTY_INITIALIZE_ORIENTATION_BOUNDING_BOX_FRONT_R45);
	pl.set(36,p);
	p.define(PROPERTY_ORIENTATION_BOUNDING_BOX_BACK_R45,PROPERTY_UPDATE_ORIENTATION_BOUNDING_BOX_BACK_R45,PROPERTY_INITIALIZE_ORIENTATION_BOUNDING_BOX_BACK_R45);
	pl.set(37,p);
	p.define(PROPERTY_ORIENTATION_BOUNDING_BOX_TOP_R45,PROPERTY_UPDATE_ORIENTATION_BOUNDING_BOX_TOP_R45,PROPERTY_INITIALIZE_ORIENTATION_BOUNDING_BOX_TOP_R45);
	pl.set(38,p);
	p.define(PROPERTY_ORIENTATION_BOUNDING_BOX_BOTTOM_R45,PROPERTY_UPDATE_ORIENTATION_BOUNDING_BOX_BOTTOM_R45,PROPERTY_INITIALIZE_ORIENTATION_BOUNDING_BOX_BOTTOM_R45);
	pl.set(39,p);

	p.define(PROPERTY_ORIENTATION_TOTAL_LENGTH,PROPERTY_UPDATE_ORIENTATION_TOTAL_LENGTH,PROPERTY_INITIALIZE_ORIENTATION_TOTAL_LENGTH);
	pl.set(40,p);

	p.define(PROPERTY_ORIENTATION_ANGLE_COEFF_SAAZIM,PROPERTY_UPDATE_ORIENTATION_ANGLE_COEFF_SAAZIM,PROPERTY_INITIALIZE_ORIENTATION_ANGLE_COEFF_SAAZIM);
	pl.set(41,p);
	p.define(PROPERTY_ORIENTATION_ANGLE_COEFF_SAELEV,PROPERTY_UPDATE_ORIENTATION_ANGLE_COEFF_SAELEV,PROPERTY_INITIALIZE_ORIENTATION_ANGLE_COEFF_SAELEV);
	pl.set(42,p);
	p.define(PROPERTY_ORIENTATION_ANGLE_COEFF_SAROLL,PROPERTY_UPDATE_ORIENTATION_ANGLE_COEFF_SAROLL,PROPERTY_INITIALIZE_ORIENTATION_ANGLE_COEFF_SAROLL);
	pl.set(43,p);
	p.define(PROPERTY_ORIENTATION_ANGLE_COEFF_EAAZIM,PROPERTY_UPDATE_ORIENTATION_ANGLE_COEFF_EAAZIM,PROPERTY_INITIALIZE_ORIENTATION_ANGLE_COEFF_EAAZIM);
	pl.set(44,p);
	p.define(PROPERTY_ORIENTATION_ANGLE_COEFF_EAELEV,PROPERTY_UPDATE_ORIENTATION_ANGLE_COEFF_EAELEV,PROPERTY_INITIALIZE_ORIENTATION_ANGLE_COEFF_EAELEV);
	pl.set(45,p);
	p.define(PROPERTY_ORIENTATION_ANGLE_COEFF_EAROLL,PROPERTY_UPDATE_ORIENTATION_ANGLE_COEFF_EAROLL,PROPERTY_INITIALIZE_ORIENTATION_ANGLE_COEFF_EAROLL);
	pl.set(46,p);
	
	p.define(PROPERTY_ORIENTATION_SUM_AZIM,PROPERTY_UPDATE_ORIENTATION_SUM_AZIM,PROPERTY_INITIALIZE_ORIENTATION_SUM_AZIM);
	pl.set(47,p);
	p.define(PROPERTY_ORIENTATION_SUM_ELEV,PROPERTY_UPDATE_ORIENTATION_SUM_ELEV,PROPERTY_INITIALIZE_ORIENTATION_SUM_ELEV);
	pl.set(48,p);
	p.define(PROPERTY_ORIENTATION_SUM_ROLL,PROPERTY_UPDATE_ORIENTATION_SUM_ROLL,PROPERTY_INITIALIZE_ORIENTATION_SUM_ROLL);
	pl.set(49,p);

	p.define(PROPERTY_ORIENTATION_AZIMUTHAL_MOVEMENTS,PROPERTY_UPDATE_ORIENTATION_AZIMUTHAL_MOVEMENTS,PROPERTY_INITIALIZE_ORIENTATION_AZIMUTHAL_MOVEMENTS);
	pl.set(50,p);
	p.define(PROPERTY_ORIENTATION_ELEVATION_MOVEMENTS,PROPERTY_UPDATE_ORIENTATION_ELEVATION_MOVEMENTS,PROPERTY_INITIALIZE_ORIENTATION_ELEVATION_MOVEMENTS);
	pl.set(51,p);
	p.define(PROPERTY_ORIENTATION_ROLL_MOVEMENTS,PROPERTY_UPDATE_ORIENTATION_ROLL_MOVEMENTS,PROPERTY_INITIALIZE_ORIENTATION_ROLL_MOVEMENTS);
	pl.set(52,p);

	p.define(PROPERTY_ORIENTATION_AZIMUTHAL_MOVEMENTS_R45,PROPERTY_UPDATE_ORIENTATION_AZIMUTHAL_MOVEMENTS_R45,PROPERTY_INITIALIZE_ORIENTATION_AZIMUTHAL_MOVEMENTS_R45);
	pl.set(53,p);
	p.define(PROPERTY_ORIENTATION_ELEVATION_MOVEMENTS_R45,PROPERTY_UPDATE_ORIENTATION_ELEVATION_MOVEMENTS_R45,PROPERTY_INITIALIZE_ORIENTATION_ELEVATION_MOVEMENTS_R45);
	pl.set(54,p);
	p.define(PROPERTY_ORIENTATION_ROLL_MOVEMENTS_R45,PROPERTY_UPDATE_ORIENTATION_ROLL_MOVEMENTS_R45,PROPERTY_INITIALIZE_ORIENTATION_ROLL_MOVEMENTS_R45);
	pl.set(55,p);

	p.define(PROPERTY_START_X,PROPERTY_UPDATE_START_X,PROPERTY_INITIALIZE_START_X);
	pl.set(56,p);
	p.define(PROPERTY_START_Y,PROPERTY_UPDATE_START_Y,PROPERTY_INITIALIZE_START_Y);
	pl.set(57,p);
	p.define(PROPERTY_START_Z,PROPERTY_UPDATE_START_Z,PROPERTY_INITIALIZE_START_Z);
	pl.set(58,p);
	p.define(PROPERTY_ORIENTATION_START_AZIM,PROPERTY_UPDATE_ORIENTATION_START_AZIM,PROPERTY_INITIALIZE_ORIENTATION_START_AZIM);
	pl.set(59,p);
	p.define(PROPERTY_ORIENTATION_START_ELEV,PROPERTY_UPDATE_ORIENTATION_START_ELEV,PROPERTY_INITIALIZE_ORIENTATION_START_ELEV);
	pl.set(60,p);
	p.define(PROPERTY_ORIENTATION_START_ROLL,PROPERTY_UPDATE_ORIENTATION_START_ROLL,PROPERTY_INITIALIZE_ORIENTATION_START_ROLL);
	pl.set(61,p);

	p.define(PROPERTY_END_X,PROPERTY_UPDATE_END_X,PROPERTY_INITIALIZE_END_X);
	pl.set(62,p);
	p.define(PROPERTY_END_Y,PROPERTY_UPDATE_END_Y,PROPERTY_INITIALIZE_END_Y);
	pl.set(63,p);
	p.define(PROPERTY_END_Z,PROPERTY_UPDATE_END_Z,PROPERTY_INITIALIZE_END_Z);
	pl.set(64,p);
	p.define(PROPERTY_ORIENTATION_END_AZIM,PROPERTY_UPDATE_ORIENTATION_END_AZIM,PROPERTY_INITIALIZE_ORIENTATION_END_AZIM);
	pl.set(65,p);
	p.define(PROPERTY_ORIENTATION_END_ELEV,PROPERTY_UPDATE_ORIENTATION_END_ELEV,PROPERTY_INITIALIZE_ORIENTATION_END_ELEV);
	pl.set(66,p);
	p.define(PROPERTY_ORIENTATION_END_ROLL,PROPERTY_UPDATE_ORIENTATION_END_ROLL,PROPERTY_INITIALIZE_ORIENTATION_END_ROLL);
	pl.set(67,p);

	p.define(PROPERTY_NUMBER_OF_SAMPLES,PROPERTY_UPDATE_NUMBER_OF_SAMPLES,PROPERTY_INITIALIZE_NUMBER_OF_SAMPLES);
	pl.set(68,p);

	p.define(PROPERTY_START_TIME,PROPERTY_UPDATE_START_TIME,PROPERTY_INITIALIZE_START_TIME);
	pl.set(69,p);
	p.define(PROPERTY_END_TIME,PROPERTY_UPDATE_END_TIME,PROPERTY_INITIALIZE_END_TIME);
	pl.set(70,p);

	p.define(PROPERTY_MIN_CG_2_CURVE_X,PROPERTY_UPDATE_MIN_CG_2_CURVE_X,PROPERTY_INITIALIZE_MIN_CG_2_CURVE_X);
    p.set_dependence_level(1);
	pl.set(71,p);
	p.define(PROPERTY_MIN_CG_2_CURVE_Y,PROPERTY_UPDATE_MIN_CG_2_CURVE_Y,PROPERTY_INITIALIZE_MIN_CG_2_CURVE_Y);
	p.set_dependence_level(1);
	pl.set(72,p);
	p.define(PROPERTY_MIN_CG_2_CURVE_Z,PROPERTY_UPDATE_MIN_CG_2_CURVE_Z,PROPERTY_INITIALIZE_MIN_CG_2_CURVE_Z);
	p.set_dependence_level(1);
	pl.set(73,p);
	p.define(PROPERTY_MAX_CG_2_CURVE_X,PROPERTY_UPDATE_MAX_CG_2_CURVE_X,PROPERTY_INITIALIZE_MAX_CG_2_CURVE_X);
	p.set_dependence_level(1);
	pl.set(74,p);
	p.define(PROPERTY_MAX_CG_2_CURVE_Y,PROPERTY_UPDATE_MAX_CG_2_CURVE_Y,PROPERTY_INITIALIZE_MAX_CG_2_CURVE_Y);
	p.set_dependence_level(1);
	pl.set(75,p);
	p.define(PROPERTY_MAX_CG_2_CURVE_Z,PROPERTY_UPDATE_MAX_CG_2_CURVE_Z,PROPERTY_INITIALIZE_MAX_CG_2_CURVE_Z);
	p.set_dependence_level(1);
	pl.set(76,p);
}
#endif
//----------------------------------------------------------------------------

#endif

