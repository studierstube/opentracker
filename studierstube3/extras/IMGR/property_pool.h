#ifndef PROPERTY_POOL_INCLUDED
#define PROPERTY_POOL_INCLUDED

#include <math.h>
#include <time.h>
#include <sys/timeb.h>

#include "property.h"
#include "sample.h"
#include "sample_pool.h"
#include "dependence_pool.h"

#define DEFINED_PROPERTY_TYPE	float

//#define M_PI 3.141592654  // commented out because already defined in math.h  BAS
#define SC45 0.7071067
#define TIME_MAX_SECONDS 511 //max. duration (sec.) of a gesture (!binary!)

#define _min_(x,y) (((x) < (y)) ? (x) : (y))
#define _max_(x,y) (((x) > (y)) ? (x) : (y))

//dependence level 0-----------------------------------------------------------
#define PROPERTY_BOUNDING_BOX_LEFT		0
#define PROPERTY_BOUNDING_BOX_RIGHT		1
#define PROPERTY_BOUNDING_BOX_FRONT		2
#define PROPERTY_BOUNDING_BOX_BACK		3
#define PROPERTY_BOUNDING_BOX_TOP		4
#define PROPERTY_BOUNDING_BOX_BOTTOM	5

#define PROPERTY_BOUNDING_BOX_LEFT_R45		6
#define PROPERTY_BOUNDING_BOX_RIGHT_R45		7
#define PROPERTY_BOUNDING_BOX_FRONT_R45		8
#define PROPERTY_BOUNDING_BOX_BACK_R45		9
#define PROPERTY_BOUNDING_BOX_TOP_R45		10
#define PROPERTY_BOUNDING_BOX_BOTTOM_R45	11

#define PROPERTY_TOTAL_LENGTH				12

#define PROPERTY_ANGLE_COEFF_SAX			13
#define PROPERTY_ANGLE_COEFF_SAY			14
#define PROPERTY_ANGLE_COEFF_SAZ			15
#define PROPERTY_ANGLE_COEFF_EAX			16
#define PROPERTY_ANGLE_COEFF_EAY			17
#define PROPERTY_ANGLE_COEFF_EAZ			18

#define PROPERTY_SUM_X						19
#define PROPERTY_SUM_Y						20
#define PROPERTY_SUM_Z						21

#define PROPERTY_HORIZONTAL_MOVEMENTS		22
#define PROPERTY_VERTICAL_MOVEMENTS			23
#define PROPERTY_DEPTH_MOVEMENTS			24

#define PROPERTY_HORIZONTAL_MOVEMENTS_R45	25
#define PROPERTY_VERTICAL_MOVEMENTS_R45		26
#define PROPERTY_DEPTH_MOVEMENTS_R45		27

#define PROPERTY_ORIENTATION_BOUNDING_BOX_LEFT		28
#define PROPERTY_ORIENTATION_BOUNDING_BOX_RIGHT		29
#define PROPERTY_ORIENTATION_BOUNDING_BOX_FRONT		30
#define PROPERTY_ORIENTATION_BOUNDING_BOX_BACK		31
#define PROPERTY_ORIENTATION_BOUNDING_BOX_TOP		32
#define PROPERTY_ORIENTATION_BOUNDING_BOX_BOTTOM	33

#define PROPERTY_ORIENTATION_BOUNDING_BOX_LEFT_R45		34
#define PROPERTY_ORIENTATION_BOUNDING_BOX_RIGHT_R45		35
#define PROPERTY_ORIENTATION_BOUNDING_BOX_FRONT_R45		36
#define PROPERTY_ORIENTATION_BOUNDING_BOX_BACK_R45		37
#define PROPERTY_ORIENTATION_BOUNDING_BOX_TOP_R45		38
#define PROPERTY_ORIENTATION_BOUNDING_BOX_BOTTOM_R45	39

#define PROPERTY_ORIENTATION_TOTAL_LENGTH				40

#define PROPERTY_ORIENTATION_ANGLE_COEFF_SAAZIM			41
#define PROPERTY_ORIENTATION_ANGLE_COEFF_SAELEV			42
#define PROPERTY_ORIENTATION_ANGLE_COEFF_SAROLL			43
#define PROPERTY_ORIENTATION_ANGLE_COEFF_EAAZIM			44
#define PROPERTY_ORIENTATION_ANGLE_COEFF_EAELEV			45
#define PROPERTY_ORIENTATION_ANGLE_COEFF_EAROLL			46

#define PROPERTY_ORIENTATION_SUM_AZIM					47
#define PROPERTY_ORIENTATION_SUM_ELEV					48
#define PROPERTY_ORIENTATION_SUM_ROLL					49

#define PROPERTY_ORIENTATION_AZIMUTHAL_MOVEMENTS		50
#define PROPERTY_ORIENTATION_ELEVATION_MOVEMENTS		51
#define PROPERTY_ORIENTATION_ROLL_MOVEMENTS				52

#define PROPERTY_ORIENTATION_AZIMUTHAL_MOVEMENTS_R45	53
#define PROPERTY_ORIENTATION_ELEVATION_MOVEMENTS_R45	54
#define PROPERTY_ORIENTATION_ROLL_MOVEMENTS_R45			55

#define PROPERTY_START_X								56
#define PROPERTY_START_Y								57
#define PROPERTY_START_Z								58

#define PROPERTY_END_X									59
#define PROPERTY_END_Y									60
#define PROPERTY_END_Z									61

#define PROPERTY_ORIENTATION_START_AZIM					62
#define PROPERTY_ORIENTATION_START_ELEV					63
#define PROPERTY_ORIENTATION_START_ROLL					64

#define PROPERTY_ORIENTATION_END_AZIM					65
#define PROPERTY_ORIENTATION_END_ELEV					66
#define PROPERTY_ORIENTATION_END_ROLL					67

#define PROPERTY_START_TIME								68
#define PROPERTY_END_TIME								69

#define PROPERTY_NUMBER_OF_SAMPLES						70
//-----------------------------------------------------------------------------
//dependence level 1-----------------------------------------------------------
#define PROPERTY_MIN_CG_2_CURVE_X						71
#define PROPERTY_MIN_CG_2_CURVE_Y						72
#define PROPERTY_MIN_CG_2_CURVE_Z						73

#define PROPERTY_MAX_CG_2_CURVE_X						74
#define PROPERTY_MAX_CG_2_CURVE_Y						75
#define PROPERTY_MAX_CG_2_CURVE_Z						76
//-----------------------------------------------------------------------------

//update section
//-----------------------------------------------------------------------------
void  PROPERTY_UPDATE_BOUNDING_BOX_LEFT (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	DEFINED_PROPERTY_TYPE tmp;
	
	tmp=v;
    v=_min_(s.get(0),tmp);
}
//-----------------------------------------------------------------------------
void  PROPERTY_UPDATE_BOUNDING_BOX_RIGHT (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	DEFINED_PROPERTY_TYPE tmp;
	
	tmp=v;
    v=_max_(s.get(0),tmp);
}
//-----------------------------------------------------------------------------
void  PROPERTY_UPDATE_BOUNDING_BOX_FRONT (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	DEFINED_PROPERTY_TYPE tmp;
	
	tmp=v;
    v=_max_(s.get(2),tmp);
}
//-----------------------------------------------------------------------------
void  PROPERTY_UPDATE_BOUNDING_BOX_BACK (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	DEFINED_PROPERTY_TYPE tmp;
	
	tmp=v;
    v=_min_(s.get(2),tmp);
}
//-----------------------------------------------------------------------------
void  PROPERTY_UPDATE_BOUNDING_BOX_TOP (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	DEFINED_PROPERTY_TYPE tmp;
	
	tmp=v;
    v=_max_(s.get(1),tmp);
}
//-----------------------------------------------------------------------------
void  PROPERTY_UPDATE_BOUNDING_BOX_BOTTOM (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	DEFINED_PROPERTY_TYPE tmp;
	
	tmp=v;
    v=_min_(s.get(1),tmp);
}
//-----------------------------------------------------------------------------
void  PROPERTY_UPDATE_BOUNDING_BOX_LEFT_R45 (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	DEFINED_PROPERTY_TYPE tmp,r45x;

	r45x=(DEFINED_PROPERTY_TYPE)(SC45*(double)(SC45*(double)(s.get(0)+SC45*(double)(s.get(1)+s.get(2)))
		-SC45*(double)(s.get(1)-s.get(2))));
	tmp=v;
    v=_min_(r45x,tmp);
}
//-----------------------------------------------------------------------------
void  PROPERTY_UPDATE_BOUNDING_BOX_RIGHT_R45 (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	DEFINED_PROPERTY_TYPE tmp,r45x;
	
	r45x=(DEFINED_PROPERTY_TYPE)(SC45*(double)(SC45*(double)(s.get(0)+SC45*(double)(s.get(1)+s.get(2)))
		-SC45*(double)(s.get(1)-s.get(2))));
	tmp=v;
    v=_max_(r45x,tmp);
}
//-----------------------------------------------------------------------------
void  PROPERTY_UPDATE_BOUNDING_BOX_FRONT_R45 (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	DEFINED_PROPERTY_TYPE tmp,r45z;
	
	r45z=(DEFINED_PROPERTY_TYPE)(SC45*(double)(SC45*(double)(s.get(1)+s.get(2))-s.get(0)));
	
	tmp=v;
    v=_max_(r45z,tmp);
}
//-----------------------------------------------------------------------------
void  PROPERTY_UPDATE_BOUNDING_BOX_BACK_R45 (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	DEFINED_PROPERTY_TYPE tmp,r45z;
	
	r45z=(DEFINED_PROPERTY_TYPE)(SC45*(double)(SC45*(double)(s.get(1)+s.get(2))-s.get(0)));
	
	tmp=v;
    v=_min_(r45z,tmp);
}
//-----------------------------------------------------------------------------
void  PROPERTY_UPDATE_BOUNDING_BOX_TOP_R45 (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	DEFINED_PROPERTY_TYPE tmp,r45y;
	
	r45y=(DEFINED_PROPERTY_TYPE)(SC45*(double)(SC45*(double)(s.get(0)+SC45*(double)(s.get(1)+s.get(2)))
		+SC45*(double)(s.get(1)-s.get(2))));
	tmp=v;
    v=_max_(r45y,tmp);
}
//-----------------------------------------------------------------------------
void  PROPERTY_UPDATE_BOUNDING_BOX_BOTTOM_R45 (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	DEFINED_PROPERTY_TYPE tmp,r45y;
	
	r45y=(DEFINED_PROPERTY_TYPE)(SC45*(double)(SC45*(double)(s.get(0)+SC45*(double)(s.get(1)+s.get(2)))
		+SC45*(double)(s.get(1)-s.get(2))));

	tmp=v;
    v=_min_(r45y,tmp);
}
//-----------------------------------------------------------------------------
void PROPERTY_UPDATE_TOTAL_LENGTH (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	DEFINED_PROPERTY_TYPE dx,dy,dz;

	dx=s.get(0)-s.get_previous(0);
	dy=s.get(1)-s.get_previous(1);
	dz=s.get(2)-s.get_previous(2);			
    v+=(DEFINED_PROPERTY_TYPE)sqrt(dx*dx+dy*dy+dz*dz);			
}
//-----------------------------------------------------------------------------
void PROPERTY_UPDATE_ANGLE_COEFF_SAX (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	DEFINED_PROPERTY_TYPE dx,dy,dz;
	static DEFINED_PROPERTY_TYPE l;
    
	if (s.number_of_samples()<=2) 
	 l=(DEFINED_PROPERTY_TYPE)1.0;

	dx=s.get(0)-s.get_previous(0);
	dy=s.get(1)-s.get_previous(1);
	dz=s.get(2)-s.get_previous(2);			
    l+=(DEFINED_PROPERTY_TYPE)sqrt(dx*dx+dy*dy+dz*dz);
    
	v+=dx/l/l;		
}
//-----------------------------------------------------------------------------
void PROPERTY_UPDATE_ANGLE_COEFF_SAY (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	DEFINED_PROPERTY_TYPE dx,dy,dz;
	static DEFINED_PROPERTY_TYPE l;
    
	if (s.number_of_samples()<=2) 
	 l=(DEFINED_PROPERTY_TYPE)1.0;
    
	dx=s.get(0)-s.get_previous(0);
	dy=s.get(1)-s.get_previous(1);
	dz=s.get(2)-s.get_previous(2);			
    l+=(DEFINED_PROPERTY_TYPE)sqrt(dx*dx+dy*dy+dz*dz);

	v+=dy/l/l;		
}
//-----------------------------------------------------------------------------
void PROPERTY_UPDATE_ANGLE_COEFF_SAZ (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	DEFINED_PROPERTY_TYPE dx,dy,dz;
	static DEFINED_PROPERTY_TYPE l;
    
	if (s.number_of_samples()<=2) 
	 l=(DEFINED_PROPERTY_TYPE)1.0;
    
	dx=s.get(0)-s.get_previous(0);
	dy=s.get(1)-s.get_previous(1);
	dz=s.get(2)-s.get_previous(2);			
    l+=(DEFINED_PROPERTY_TYPE)sqrt(dx*dx+dy*dy+dz*dz);

	v+=dz/l/l;		
}
//-----------------------------------------------------------------------------
void PROPERTY_UPDATE_ANGLE_COEFF_EAX (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	DEFINED_PROPERTY_TYPE dx,dy,dz;
	static DEFINED_PROPERTY_TYPE l;
    
	if (s.number_of_samples()<=2) 
	 l=(DEFINED_PROPERTY_TYPE)1.0;
    
	dx=s.get(0)-s.get_previous(0);
	dy=s.get(1)-s.get_previous(1);
	dz=s.get(2)-s.get_previous(2);			
    l+=(DEFINED_PROPERTY_TYPE)sqrt(dx*dx+dy*dy+dz*dz);

	v+=dx*l*l;		
}
//-----------------------------------------------------------------------------
void PROPERTY_UPDATE_ANGLE_COEFF_EAY (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	DEFINED_PROPERTY_TYPE dx,dy,dz;
	static DEFINED_PROPERTY_TYPE l;
    
	if (s.number_of_samples()<=2) 
	 l=(DEFINED_PROPERTY_TYPE)1.0;
    
	dx=s.get(0)-s.get_previous(0);
	dy=s.get(1)-s.get_previous(1);
	dz=s.get(2)-s.get_previous(2);			
    l+=(DEFINED_PROPERTY_TYPE)sqrt(dx*dx+dy*dy+dz*dz);

	v+=dy*l*l;		
}
//-----------------------------------------------------------------------------
void PROPERTY_UPDATE_ANGLE_COEFF_EAZ (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	DEFINED_PROPERTY_TYPE dx,dy,dz;
	static DEFINED_PROPERTY_TYPE l;
    
	if (s.number_of_samples()<=2) 
	 l=(DEFINED_PROPERTY_TYPE)1.0;
    
	dx=s.get(0)-s.get_previous(0);
	dy=s.get(1)-s.get_previous(1);
	dz=s.get(2)-s.get_previous(2);			
    l+=(DEFINED_PROPERTY_TYPE)sqrt(dx*dx+dy*dy+dz*dz);

	v+=dz*l*l;		
}
//-----------------------------------------------------------------------------
void PROPERTY_UPDATE_SUM_X (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	v+=s.get(0);		
}
//-----------------------------------------------------------------------------
void PROPERTY_UPDATE_SUM_Y (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	v+=s.get(1);		
}
//-----------------------------------------------------------------------------
void PROPERTY_UPDATE_SUM_Z (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	v+=s.get(2);		
}
//-----------------------------------------------------------------------------
void PROPERTY_UPDATE_HORIZONTAL_MOVEMENTS (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	DEFINED_PROPERTY_TYPE dx;

	dx=s.get(0)-s.get_previous(0);
	v+=(DEFINED_PROPERTY_TYPE)fabs(dx);		
}
//-----------------------------------------------------------------------------
void PROPERTY_UPDATE_VERTICAL_MOVEMENTS (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	DEFINED_PROPERTY_TYPE dy;

	dy=s.get(1)-s.get_previous(1);
	v+=(DEFINED_PROPERTY_TYPE)fabs(dy);		
}
//-----------------------------------------------------------------------------
void PROPERTY_UPDATE_DEPTH_MOVEMENTS (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	DEFINED_PROPERTY_TYPE dz;

	dz=s.get(2)-s.get_previous(2);
	v+=(DEFINED_PROPERTY_TYPE)fabs(dz);		
}
//-----------------------------------------------------------------------------
void PROPERTY_UPDATE_HORIZONTAL_MOVEMENTS_R45 (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	DEFINED_PROPERTY_TYPE r45dx,r45x,r45xp;

	r45x=(DEFINED_PROPERTY_TYPE)(SC45*(double)(SC45*(double)(s.get(0)+SC45*(double)(s.get(1)+s.get(2)))
		-SC45*(double)(s.get(1)-s.get(2))));

    r45xp=(DEFINED_PROPERTY_TYPE)(SC45*(double)(SC45*(double)(s.get_previous(0)
		+SC45*(double)(s.get_previous(1)+s.get_previous(2)))
		-SC45*(double)(s.get_previous(1)-s.get_previous(2))));
	
	r45dx=r45x-r45xp;
	v+=(DEFINED_PROPERTY_TYPE)fabs(r45dx);		
}
//-----------------------------------------------------------------------------
void PROPERTY_UPDATE_VERTICAL_MOVEMENTS_R45 (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	DEFINED_PROPERTY_TYPE r45dy,r45y,r45yp;
    
	r45y=(DEFINED_PROPERTY_TYPE)(SC45*(double)(SC45*(double)(s.get(0)+SC45*(double)(s.get(1)+s.get(2)))
		+SC45*(double)(s.get(1)-s.get(2))));

	r45yp=(DEFINED_PROPERTY_TYPE)(SC45*(double)(SC45*(double)(s.get_previous(0)
		+SC45*(double)(s.get_previous(1)+s.get_previous(2)))
		+SC45*(double)(s.get_previous(1)-s.get_previous(2))));

	r45dy=r45y-r45yp;
	v+=(DEFINED_PROPERTY_TYPE)fabs(r45dy);		
}
//-----------------------------------------------------------------------------
void PROPERTY_UPDATE_DEPTH_MOVEMENTS_R45 (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	DEFINED_PROPERTY_TYPE r45dz,r45z,r45zp;

	r45z=(DEFINED_PROPERTY_TYPE)(SC45*(double)(SC45*(double)(s.get(1)+s.get(2))-s.get(0)));

	r45zp=(DEFINED_PROPERTY_TYPE)(SC45*(double)(SC45*(double)(s.get_previous(1)
		+s.get_previous(2))-s.get_previous(0)));

	r45dz=r45z-r45zp;
	v+=(DEFINED_PROPERTY_TYPE)fabs(r45dz);		
}
//-----------------------------------------------------------------------------
void PROPERTY_UPDATE_START_X (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{//nothing to do
}
//-----------------------------------------------------------------------------
void PROPERTY_UPDATE_START_Y (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{//nothing to do
}
//-----------------------------------------------------------------------------
void PROPERTY_UPDATE_START_Z (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{//nothing to do
}
//-----------------------------------------------------------------------------
void PROPERTY_UPDATE_END_X (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=s.get(0);}
//-----------------------------------------------------------------------------
void PROPERTY_UPDATE_END_Y (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=s.get(1);}
//-----------------------------------------------------------------------------
void PROPERTY_UPDATE_END_Z (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=s.get(2);}
//-----------------------------------------------------------------------------
void PROPERTY_UPDATE_NUMBER_OF_SAMPLES (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=s.number_of_samples();}
//-----------------------------------------------------------------------------
void  PROPERTY_UPDATE_ORIENTATION_BOUNDING_BOX_LEFT (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	DEFINED_PROPERTY_TYPE tmp;
	
	tmp=v;
    v=_min_(s.get(3),tmp);
}
//-----------------------------------------------------------------------------
void  PROPERTY_UPDATE_ORIENTATION_BOUNDING_BOX_RIGHT (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	DEFINED_PROPERTY_TYPE tmp;
	
	tmp=v;
    v=_max_(s.get(3),tmp);
}
//-----------------------------------------------------------------------------
void  PROPERTY_UPDATE_ORIENTATION_BOUNDING_BOX_FRONT (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	DEFINED_PROPERTY_TYPE tmp;
	
	tmp=v;
    v=_max_(s.get(5),tmp);
}
//-----------------------------------------------------------------------------
void  PROPERTY_UPDATE_ORIENTATION_BOUNDING_BOX_BACK (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	DEFINED_PROPERTY_TYPE tmp;
	
	tmp=v;
    v=_min_(s.get(5),tmp);
}
//-----------------------------------------------------------------------------
void  PROPERTY_UPDATE_ORIENTATION_BOUNDING_BOX_TOP (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	DEFINED_PROPERTY_TYPE tmp;
	
	tmp=v;
    v=_max_(s.get(4),tmp);
}
//-----------------------------------------------------------------------------
void  PROPERTY_UPDATE_ORIENTATION_BOUNDING_BOX_BOTTOM (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	DEFINED_PROPERTY_TYPE tmp;
	
	tmp=v;
    v=_min_(s.get(4),tmp);
}
//-----------------------------------------------------------------------------
void  PROPERTY_UPDATE_ORIENTATION_BOUNDING_BOX_LEFT_R45 (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	DEFINED_PROPERTY_TYPE tmp,r45azim;

	r45azim=(DEFINED_PROPERTY_TYPE)(SC45*(double)(SC45*(double)(s.get(3)+SC45*(double)(s.get(4)+s.get(5)))
		-SC45*(double)(s.get(4)-s.get(5))));
	tmp=v;
    v=_min_(r45azim,tmp);
}
//-----------------------------------------------------------------------------
void  PROPERTY_UPDATE_ORIENTATION_BOUNDING_BOX_RIGHT_R45 (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	DEFINED_PROPERTY_TYPE tmp,r45azim;
	
	r45azim=(DEFINED_PROPERTY_TYPE)(SC45*(double)(SC45*(double)(s.get(3)+SC45*(double)(s.get(4)+s.get(5)))
		-SC45*(double)(s.get(4)-s.get(5))));
	tmp=v;
    v=_max_(r45azim,tmp);
}
//-----------------------------------------------------------------------------
void  PROPERTY_UPDATE_ORIENTATION_BOUNDING_BOX_FRONT_R45 (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	DEFINED_PROPERTY_TYPE tmp,r45roll;
	
	r45roll=(DEFINED_PROPERTY_TYPE)(SC45*(double)(SC45*(double)(s.get(4)+s.get(5))-s.get(3)));
	
	tmp=v;
    v=_max_(r45roll,tmp);
}
//-----------------------------------------------------------------------------
void  PROPERTY_UPDATE_ORIENTATION_BOUNDING_BOX_BACK_R45 (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	DEFINED_PROPERTY_TYPE tmp,r45roll;
	
	r45roll=(DEFINED_PROPERTY_TYPE)(SC45*(double)(SC45*(double)(s.get(4)+s.get(5))-s.get(3)));

	tmp=v;
    v=_min_(r45roll,tmp);
}
//-----------------------------------------------------------------------------
void  PROPERTY_UPDATE_ORIENTATION_BOUNDING_BOX_TOP_R45 (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	DEFINED_PROPERTY_TYPE tmp,r45elev;
	
	r45elev=(DEFINED_PROPERTY_TYPE)(SC45*(double)(SC45*(double)(s.get(3)+SC45*(double)(s.get(4)+s.get(5)))
		+SC45*(double)(s.get(4)-s.get(5))));
	
	tmp=v;
    v=_max_(r45elev,tmp);
}
//-----------------------------------------------------------------------------
void  PROPERTY_UPDATE_ORIENTATION_BOUNDING_BOX_BOTTOM_R45 (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	DEFINED_PROPERTY_TYPE tmp,r45elev;
	
	r45elev=(DEFINED_PROPERTY_TYPE)(SC45*(double)(SC45*(double)(s.get(3)+SC45*(double)(s.get(4)+s.get(5)))
		+SC45*(double)(s.get(4)-s.get(5))));
	
	tmp=v;
    v=_min_(r45elev,tmp);
}
//-----------------------------------------------------------------------------
void PROPERTY_UPDATE_ORIENTATION_TOTAL_LENGTH (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	DEFINED_PROPERTY_TYPE dazim,delev,droll;

	dazim=s.get(3)-s.get_previous(3);
	delev=s.get(4)-s.get_previous(4);
	droll=s.get(5)-s.get_previous(5);
	
    v+=(DEFINED_PROPERTY_TYPE)sqrt(dazim*dazim+delev*delev+droll*droll);			
}
//-----------------------------------------------------------------------------
void PROPERTY_UPDATE_ORIENTATION_ANGLE_COEFF_SAAZIM (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	DEFINED_PROPERTY_TYPE dazim,delev,droll;
	static DEFINED_PROPERTY_TYPE l;
    
	if (s.number_of_samples()<=2) 
	 l=(DEFINED_PROPERTY_TYPE)1.0;
    
	dazim=s.get(3)-s.get_previous(3);
	delev=s.get(4)-s.get_previous(4);
	droll=s.get(5)-s.get_previous(5);			
    l+=(DEFINED_PROPERTY_TYPE)sqrt(dazim*dazim+delev*delev+droll*droll);

	v+=dazim/l/l;		
}
//-----------------------------------------------------------------------------
void PROPERTY_UPDATE_ORIENTATION_ANGLE_COEFF_SAELEV (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	DEFINED_PROPERTY_TYPE dazim,delev,droll;
	static DEFINED_PROPERTY_TYPE l;
    
	if (s.number_of_samples()<=2) 
	 l=(DEFINED_PROPERTY_TYPE)1.0;
    
	dazim=s.get(3)-s.get_previous(3);
	delev=s.get(4)-s.get_previous(4);
	droll=s.get(5)-s.get_previous(5);			
    l+=(DEFINED_PROPERTY_TYPE)sqrt(dazim*dazim+delev*delev+droll*droll);

	v+=delev/l/l;		
}
//-----------------------------------------------------------------------------
void PROPERTY_UPDATE_ORIENTATION_ANGLE_COEFF_SAROLL (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	DEFINED_PROPERTY_TYPE dazim,delev,droll;
	static DEFINED_PROPERTY_TYPE l;
    
	if (s.number_of_samples()<=2) 
	 l=(DEFINED_PROPERTY_TYPE)1.0;
    
	dazim=s.get(3)-s.get_previous(3);
	delev=s.get(4)-s.get_previous(4);
	droll=s.get(5)-s.get_previous(5);			
    l+=(DEFINED_PROPERTY_TYPE)sqrt(dazim*dazim+delev*delev+droll*droll);

	v+=droll/l/l;		
}
//-----------------------------------------------------------------------------
void PROPERTY_UPDATE_ORIENTATION_ANGLE_COEFF_EAAZIM (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	DEFINED_PROPERTY_TYPE dazim,delev,droll;
	static DEFINED_PROPERTY_TYPE l;
    
	if (s.number_of_samples()<=2) 
	 l=(DEFINED_PROPERTY_TYPE)1.0;
    
	dazim=s.get(3)-s.get_previous(3);
	delev=s.get(4)-s.get_previous(4);
	droll=s.get(5)-s.get_previous(5);			
    l+=(DEFINED_PROPERTY_TYPE)sqrt(dazim*dazim+delev*delev+droll*droll);

	v+=dazim*l*l;		
}
//-----------------------------------------------------------------------------
void PROPERTY_UPDATE_ORIENTATION_ANGLE_COEFF_EAELEV (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	DEFINED_PROPERTY_TYPE dazim,delev,droll;
    static DEFINED_PROPERTY_TYPE l;
    
	if (s.number_of_samples()<=2) 
	 l=(DEFINED_PROPERTY_TYPE)1.0;

	dazim=s.get(3)-s.get_previous(3);
	delev=s.get(4)-s.get_previous(4);
	droll=s.get(5)-s.get_previous(5);			
    l+=(DEFINED_PROPERTY_TYPE)sqrt(dazim*dazim+delev*delev+droll*droll);

	v+=delev*l*l;		
}
//-----------------------------------------------------------------------------
void PROPERTY_UPDATE_ORIENTATION_ANGLE_COEFF_EAROLL (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	DEFINED_PROPERTY_TYPE dazim,delev,droll;
	static DEFINED_PROPERTY_TYPE l;
    
	if (s.number_of_samples()<=2) 
	 l=(DEFINED_PROPERTY_TYPE)1.0;
    
	dazim=s.get(3)-s.get_previous(3);
	delev=s.get(4)-s.get_previous(4);
	droll=s.get(5)-s.get_previous(5);			
    l+=(DEFINED_PROPERTY_TYPE)sqrt(dazim*dazim+delev*delev+droll*droll);

	v+=droll*l*l;		
}
//-----------------------------------------------------------------------------
void PROPERTY_UPDATE_ORIENTATION_SUM_AZIM (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	v+=s.get(3);		
}
//-----------------------------------------------------------------------------
void PROPERTY_UPDATE_ORIENTATION_SUM_ELEV (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	v+=s.get(4);		
}
//-----------------------------------------------------------------------------
void PROPERTY_UPDATE_ORIENTATION_SUM_ROLL (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	v+=s.get(5);		
}
//-----------------------------------------------------------------------------
void PROPERTY_UPDATE_ORIENTATION_AZIMUTHAL_MOVEMENTS (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	DEFINED_PROPERTY_TYPE dazim;

	dazim=s.get(3)-s.get_previous(3);
	v+=(DEFINED_PROPERTY_TYPE)fabs(dazim);		
}
//-----------------------------------------------------------------------------
void PROPERTY_UPDATE_ORIENTATION_ELEVATION_MOVEMENTS (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	DEFINED_PROPERTY_TYPE delev;

	delev=s.get(4)-s.get_previous(4);
	v+=(DEFINED_PROPERTY_TYPE)fabs(delev);		
}
//-----------------------------------------------------------------------------
void PROPERTY_UPDATE_ORIENTATION_ROLL_MOVEMENTS (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	DEFINED_PROPERTY_TYPE droll;

	droll=s.get(5)-s.get_previous(5);
	v+=(DEFINED_PROPERTY_TYPE)fabs(droll);		
}
//-----------------------------------------------------------------------------
void PROPERTY_UPDATE_ORIENTATION_AZIMUTHAL_MOVEMENTS_R45 (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	DEFINED_PROPERTY_TYPE r45dazim,r45azim,r45azimp;

	r45azim=(DEFINED_PROPERTY_TYPE)(SC45*(double)(SC45*(double)(s.get(3)+SC45*(double)(s.get(4)+s.get(5)))
		-SC45*(double)(s.get(4)-s.get(5))));

    r45azimp=(DEFINED_PROPERTY_TYPE)(SC45*(double)(SC45*(double)(s.get_previous(3)
		+SC45*(double)(s.get_previous(4)+s.get_previous(5)))
		-SC45*(double)(s.get_previous(4)-s.get_previous(5))));
	
	r45dazim=r45azim-r45azimp;
	v+=(DEFINED_PROPERTY_TYPE)fabs(r45dazim);		
}
//-----------------------------------------------------------------------------
void PROPERTY_UPDATE_ORIENTATION_ELEVATION_MOVEMENTS_R45 (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	DEFINED_PROPERTY_TYPE r45delev,r45elev,r45elevp;
    
	r45elev=(DEFINED_PROPERTY_TYPE)(SC45*(double)(SC45*(double)(s.get(3)+SC45*(double)(s.get(4)+s.get(5)))
		+SC45*(double)(s.get(4)-s.get(5))));

	r45elevp=(DEFINED_PROPERTY_TYPE)(SC45*(double)(SC45*(double)(s.get_previous(3)
		+SC45*(double)(s.get_previous(4)+s.get_previous(5)))
		+SC45*(double)(s.get_previous(4)-s.get_previous(5))));

	r45delev=r45elev-r45elevp;
	v+=(DEFINED_PROPERTY_TYPE)fabs(r45delev);		
}
//-----------------------------------------------------------------------------
void PROPERTY_UPDATE_ORIENTATION_ROLL_MOVEMENTS_R45 (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	DEFINED_PROPERTY_TYPE r45droll,r45roll,r45rollp;

	r45roll=(DEFINED_PROPERTY_TYPE)(SC45*(double)(SC45*(double)(s.get(4)+s.get(5))-s.get(3)));

	r45rollp=(DEFINED_PROPERTY_TYPE)(SC45*(double)(SC45*(double)(s.get_previous(4)
		+s.get_previous(5))-s.get_previous(3)));

	r45droll=r45roll-r45rollp;
	v+=(DEFINED_PROPERTY_TYPE)fabs(r45droll);		
}
//-----------------------------------------------------------------------------
void PROPERTY_UPDATE_ORIENTATION_START_AZIM (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{//nothing to do
}
//-----------------------------------------------------------------------------
void PROPERTY_UPDATE_ORIENTATION_START_ELEV (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{//nothing to do
}
//-----------------------------------------------------------------------------
void PROPERTY_UPDATE_ORIENTATION_START_ROLL (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{//nothing to do
}
//-----------------------------------------------------------------------------
void PROPERTY_UPDATE_ORIENTATION_END_AZIM (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=s.get(3);}
//-----------------------------------------------------------------------------
void PROPERTY_UPDATE_ORIENTATION_END_ELEV (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=s.get(4);}
//-----------------------------------------------------------------------------
void PROPERTY_UPDATE_ORIENTATION_END_ROLL (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=s.get(5);}
//-----------------------------------------------------------------------------
void PROPERTY_UPDATE_START_TIME (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{//nothing to do
}
//-----------------------------------------------------------------------------
void PROPERTY_UPDATE_END_TIME (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
  struct timeb timebuffer;
  ftime( &timebuffer );
  v=(DEFINED_PROPERTY_TYPE) (timebuffer.time&TIME_MAX_SECONDS)*1000+timebuffer.millitm; 
}
//-----------------------------------------------------------------------------
void PROPERTY_UPDATE_MIN_CG_2_CURVE_X (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	double min_distance, current_distance;
    DEFINED_PROPERTY_TYPE min_x,min_y,min_z;
    DEFINED_PROPERTY_TYPE cg_x,cg_y,cg_z;

	min_x=DEPENDENCE_PROPERTY_LIST->look_up(PROPERTY_MIN_CG_2_CURVE_X).get();
	min_y=DEPENDENCE_PROPERTY_LIST->look_up(PROPERTY_MIN_CG_2_CURVE_Y).get();
	min_z=DEPENDENCE_PROPERTY_LIST->look_up(PROPERTY_MIN_CG_2_CURVE_Z).get();

	cg_x=(DEFINED_PROPERTY_TYPE)((double)DEPENDENCE_PROPERTY_LIST->look_up(PROPERTY_SUM_X).get()/
	 	 (double)DEPENDENCE_PROPERTY_LIST->look_up(PROPERTY_NUMBER_OF_SAMPLES).get());
	cg_y=(DEFINED_PROPERTY_TYPE)((double)DEPENDENCE_PROPERTY_LIST->look_up(PROPERTY_SUM_Y).get()/
	 	 (double)DEPENDENCE_PROPERTY_LIST->look_up(PROPERTY_NUMBER_OF_SAMPLES).get());
	cg_z=(DEFINED_PROPERTY_TYPE)((double)DEPENDENCE_PROPERTY_LIST->look_up(PROPERTY_SUM_Z).get()/
	 	 (double)DEPENDENCE_PROPERTY_LIST->look_up(PROPERTY_NUMBER_OF_SAMPLES).get());

	min_distance=sqrt((min_x-cg_x)*(min_x-cg_x)+(min_y-cg_y)*(min_y-cg_y)+(min_z-cg_z)*(min_z-cg_z));
    current_distance=sqrt((s.get(0)-cg_x)*(s.get(0)-cg_x)+(s.get(1)-cg_y)*(s.get(1)-cg_y)+(s.get(2)-cg_z)*(s.get(2)-cg_z));

	if (current_distance<min_distance)
	{
	    DEPENDENCE_PROPERTY_LIST->store_value_to(PROPERTY_MIN_CG_2_CURVE_X,s.get(0));
		DEPENDENCE_PROPERTY_LIST->store_value_to(PROPERTY_MIN_CG_2_CURVE_Y,s.get(1));
		DEPENDENCE_PROPERTY_LIST->store_value_to(PROPERTY_MIN_CG_2_CURVE_Z,s.get(2));
		v=(DEFINED_PROPERTY_TYPE)s.get(0);
	}
}
//-----------------------------------------------------------------------------
void PROPERTY_UPDATE_MIN_CG_2_CURVE_Y (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	double min_distance, current_distance;
    DEFINED_PROPERTY_TYPE min_x,min_y,min_z;
    DEFINED_PROPERTY_TYPE cg_x,cg_y,cg_z;

	min_x=DEPENDENCE_PROPERTY_LIST->look_up(PROPERTY_MIN_CG_2_CURVE_X).get();
	min_y=DEPENDENCE_PROPERTY_LIST->look_up(PROPERTY_MIN_CG_2_CURVE_Y).get();
	min_z=DEPENDENCE_PROPERTY_LIST->look_up(PROPERTY_MIN_CG_2_CURVE_Z).get();

	cg_x=(DEFINED_PROPERTY_TYPE)((double)DEPENDENCE_PROPERTY_LIST->look_up(PROPERTY_SUM_X).get()/
	 	 (double)DEPENDENCE_PROPERTY_LIST->look_up(PROPERTY_NUMBER_OF_SAMPLES).get());
	cg_y=(DEFINED_PROPERTY_TYPE)((double)DEPENDENCE_PROPERTY_LIST->look_up(PROPERTY_SUM_Y).get()/
	 	 (double)DEPENDENCE_PROPERTY_LIST->look_up(PROPERTY_NUMBER_OF_SAMPLES).get());
	cg_z=(DEFINED_PROPERTY_TYPE)((double)DEPENDENCE_PROPERTY_LIST->look_up(PROPERTY_SUM_Z).get()/
	 	 (double)DEPENDENCE_PROPERTY_LIST->look_up(PROPERTY_NUMBER_OF_SAMPLES).get());

	min_distance=sqrt((min_x-cg_x)*(min_x-cg_x)+(min_y-cg_y)*(min_y-cg_y)+(min_z-cg_z)*(min_z-cg_z));
    current_distance=sqrt((s.get(0)-cg_x)*(s.get(0)-cg_x)+(s.get(1)-cg_y)*(s.get(1)-cg_y)+(s.get(2)-cg_z)*(s.get(2)-cg_z));

	if (current_distance<min_distance)
	{
	    DEPENDENCE_PROPERTY_LIST->store_value_to(PROPERTY_MIN_CG_2_CURVE_X,s.get(0));
		DEPENDENCE_PROPERTY_LIST->store_value_to(PROPERTY_MIN_CG_2_CURVE_Y,s.get(1));
		DEPENDENCE_PROPERTY_LIST->store_value_to(PROPERTY_MIN_CG_2_CURVE_Z,s.get(2));
		v=(DEFINED_PROPERTY_TYPE)s.get(1);
	}
}
//-----------------------------------------------------------------------------
void PROPERTY_UPDATE_MIN_CG_2_CURVE_Z (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	double min_distance, current_distance;
    DEFINED_PROPERTY_TYPE min_x,min_y,min_z;
    DEFINED_PROPERTY_TYPE cg_x,cg_y,cg_z;

	min_x=DEPENDENCE_PROPERTY_LIST->look_up(PROPERTY_MIN_CG_2_CURVE_X).get();
	min_y=DEPENDENCE_PROPERTY_LIST->look_up(PROPERTY_MIN_CG_2_CURVE_Y).get();
	min_z=DEPENDENCE_PROPERTY_LIST->look_up(PROPERTY_MIN_CG_2_CURVE_Z).get();

	cg_x=(DEFINED_PROPERTY_TYPE)((double)DEPENDENCE_PROPERTY_LIST->look_up(PROPERTY_SUM_X).get()/
	 	 (double)DEPENDENCE_PROPERTY_LIST->look_up(PROPERTY_NUMBER_OF_SAMPLES).get());
	cg_y=(DEFINED_PROPERTY_TYPE)((double)DEPENDENCE_PROPERTY_LIST->look_up(PROPERTY_SUM_Y).get()/
	 	 (double)DEPENDENCE_PROPERTY_LIST->look_up(PROPERTY_NUMBER_OF_SAMPLES).get());
	cg_z=(DEFINED_PROPERTY_TYPE)((double)DEPENDENCE_PROPERTY_LIST->look_up(PROPERTY_SUM_Z).get()/
	 	 (double)DEPENDENCE_PROPERTY_LIST->look_up(PROPERTY_NUMBER_OF_SAMPLES).get());

	min_distance=sqrt((min_x-cg_x)*(min_x-cg_x)+(min_y-cg_y)*(min_y-cg_y)+(min_z-cg_z)*(min_z-cg_z));
    current_distance=sqrt((s.get(0)-cg_x)*(s.get(0)-cg_x)+(s.get(1)-cg_y)*(s.get(1)-cg_y)+(s.get(2)-cg_z)*(s.get(2)-cg_z));

	if (current_distance<min_distance)
	{
	    DEPENDENCE_PROPERTY_LIST->store_value_to(PROPERTY_MIN_CG_2_CURVE_X,s.get(0));
		DEPENDENCE_PROPERTY_LIST->store_value_to(PROPERTY_MIN_CG_2_CURVE_Y,s.get(1));
		DEPENDENCE_PROPERTY_LIST->store_value_to(PROPERTY_MIN_CG_2_CURVE_Z,s.get(2));
		v=(DEFINED_PROPERTY_TYPE)s.get(2);
	}
}
//-----------------------------------------------------------------------------
void PROPERTY_UPDATE_MAX_CG_2_CURVE_X (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	double max_distance, current_distance;
    DEFINED_PROPERTY_TYPE max_x,max_y,max_z;
    DEFINED_PROPERTY_TYPE cg_x,cg_y,cg_z;

	max_x=DEPENDENCE_PROPERTY_LIST->look_up(PROPERTY_MAX_CG_2_CURVE_X).get();
	max_y=DEPENDENCE_PROPERTY_LIST->look_up(PROPERTY_MAX_CG_2_CURVE_Y).get();
	max_z=DEPENDENCE_PROPERTY_LIST->look_up(PROPERTY_MAX_CG_2_CURVE_Z).get();

	cg_x=(DEFINED_PROPERTY_TYPE)((double)DEPENDENCE_PROPERTY_LIST->look_up(PROPERTY_SUM_X).get()/
	 	 (double)DEPENDENCE_PROPERTY_LIST->look_up(PROPERTY_NUMBER_OF_SAMPLES).get());
	cg_y=(DEFINED_PROPERTY_TYPE)((double)DEPENDENCE_PROPERTY_LIST->look_up(PROPERTY_SUM_Y).get()/
	 	 (double)DEPENDENCE_PROPERTY_LIST->look_up(PROPERTY_NUMBER_OF_SAMPLES).get());
	cg_z=(DEFINED_PROPERTY_TYPE)((double)DEPENDENCE_PROPERTY_LIST->look_up(PROPERTY_SUM_Z).get()/
	 	 (double)DEPENDENCE_PROPERTY_LIST->look_up(PROPERTY_NUMBER_OF_SAMPLES).get());

	max_distance=sqrt((max_x-cg_x)*(max_x-cg_x)+(max_y-cg_y)*(max_y-cg_y)+(max_z-cg_z)*(max_z-cg_z));
    current_distance=sqrt((s.get(0)-cg_x)*(s.get(0)-cg_x)+(s.get(1)-cg_y)*(s.get(1)-cg_y)+(s.get(2)-cg_z)*(s.get(2)-cg_z));

	if (current_distance>max_distance)
	{
	    DEPENDENCE_PROPERTY_LIST->store_value_to(PROPERTY_MAX_CG_2_CURVE_X,s.get(0));
		DEPENDENCE_PROPERTY_LIST->store_value_to(PROPERTY_MAX_CG_2_CURVE_Y,s.get(1));
		DEPENDENCE_PROPERTY_LIST->store_value_to(PROPERTY_MAX_CG_2_CURVE_Z,s.get(2));
		v=(DEFINED_PROPERTY_TYPE)s.get(0);
	}
}
//-----------------------------------------------------------------------------
void PROPERTY_UPDATE_MAX_CG_2_CURVE_Y (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	double max_distance, current_distance;
    DEFINED_PROPERTY_TYPE max_x,max_y,max_z;
    DEFINED_PROPERTY_TYPE cg_x,cg_y,cg_z;

	max_x=DEPENDENCE_PROPERTY_LIST->look_up(PROPERTY_MAX_CG_2_CURVE_X).get();
	max_y=DEPENDENCE_PROPERTY_LIST->look_up(PROPERTY_MAX_CG_2_CURVE_Y).get();
	max_z=DEPENDENCE_PROPERTY_LIST->look_up(PROPERTY_MAX_CG_2_CURVE_Z).get();

	cg_x=(DEFINED_PROPERTY_TYPE)((double)DEPENDENCE_PROPERTY_LIST->look_up(PROPERTY_SUM_X).get()/
	 	 (double)DEPENDENCE_PROPERTY_LIST->look_up(PROPERTY_NUMBER_OF_SAMPLES).get());
	cg_y=(DEFINED_PROPERTY_TYPE)((double)DEPENDENCE_PROPERTY_LIST->look_up(PROPERTY_SUM_Y).get()/
	 	 (double)DEPENDENCE_PROPERTY_LIST->look_up(PROPERTY_NUMBER_OF_SAMPLES).get());
	cg_z=(DEFINED_PROPERTY_TYPE)((double)DEPENDENCE_PROPERTY_LIST->look_up(PROPERTY_SUM_Z).get()/
	 	 (double)DEPENDENCE_PROPERTY_LIST->look_up(PROPERTY_NUMBER_OF_SAMPLES).get());

	max_distance=sqrt((max_x-cg_x)*(max_x-cg_x)+(max_y-cg_y)*(max_y-cg_y)+(max_z-cg_z)*(max_z-cg_z));
    current_distance=sqrt((s.get(0)-cg_x)*(s.get(0)-cg_x)+(s.get(1)-cg_y)*(s.get(1)-cg_y)+(s.get(2)-cg_z)*(s.get(2)-cg_z));

	if (current_distance>max_distance)
	{
	    DEPENDENCE_PROPERTY_LIST->store_value_to(PROPERTY_MAX_CG_2_CURVE_X,s.get(0));
		DEPENDENCE_PROPERTY_LIST->store_value_to(PROPERTY_MAX_CG_2_CURVE_Y,s.get(1));
		DEPENDENCE_PROPERTY_LIST->store_value_to(PROPERTY_MAX_CG_2_CURVE_Z,s.get(2));
		v=(DEFINED_PROPERTY_TYPE)s.get(1);
	}
}
//-----------------------------------------------------------------------------
void PROPERTY_UPDATE_MAX_CG_2_CURVE_Z (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
	double max_distance, current_distance;
    DEFINED_PROPERTY_TYPE max_x,max_y,max_z;
    DEFINED_PROPERTY_TYPE cg_x,cg_y,cg_z;

	max_x=DEPENDENCE_PROPERTY_LIST->look_up(PROPERTY_MAX_CG_2_CURVE_X).get();
	max_y=DEPENDENCE_PROPERTY_LIST->look_up(PROPERTY_MAX_CG_2_CURVE_Y).get();
	max_z=DEPENDENCE_PROPERTY_LIST->look_up(PROPERTY_MAX_CG_2_CURVE_Z).get();

	cg_x=(DEFINED_PROPERTY_TYPE)((double)DEPENDENCE_PROPERTY_LIST->look_up(PROPERTY_SUM_X).get()/
	 	 (double)DEPENDENCE_PROPERTY_LIST->look_up(PROPERTY_NUMBER_OF_SAMPLES).get());
	cg_y=(DEFINED_PROPERTY_TYPE)((double)DEPENDENCE_PROPERTY_LIST->look_up(PROPERTY_SUM_Y).get()/
	 	 (double)DEPENDENCE_PROPERTY_LIST->look_up(PROPERTY_NUMBER_OF_SAMPLES).get());
	cg_z=(DEFINED_PROPERTY_TYPE)((double)DEPENDENCE_PROPERTY_LIST->look_up(PROPERTY_SUM_Z).get()/
	 	 (double)DEPENDENCE_PROPERTY_LIST->look_up(PROPERTY_NUMBER_OF_SAMPLES).get());

	max_distance=sqrt((max_x-cg_x)*(max_x-cg_x)+(max_y-cg_y)*(max_y-cg_y)+(max_z-cg_z)*(max_z-cg_z));
    current_distance=sqrt((s.get(0)-cg_x)*(s.get(0)-cg_x)+(s.get(1)-cg_y)*(s.get(1)-cg_y)+(s.get(2)-cg_z)*(s.get(2)-cg_z));

	if (current_distance>max_distance)
	{
	    DEPENDENCE_PROPERTY_LIST->store_value_to(PROPERTY_MAX_CG_2_CURVE_X,s.get(0));
		DEPENDENCE_PROPERTY_LIST->store_value_to(PROPERTY_MAX_CG_2_CURVE_Y,s.get(1));
		DEPENDENCE_PROPERTY_LIST->store_value_to(PROPERTY_MAX_CG_2_CURVE_Z,s.get(2));
		v=(DEFINED_PROPERTY_TYPE)s.get(3);
	}
}
//-----------------------------------------------------------------------------

//initialize section
//-----------------------------------------------------------------------------
void  PROPERTY_INITIALIZE_BOUNDING_BOX_LEFT (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=s.get(0);}
//-----------------------------------------------------------------------------
void  PROPERTY_INITIALIZE_BOUNDING_BOX_RIGHT (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=s.get(0);}
//-----------------------------------------------------------------------------
void  PROPERTY_INITIALIZE_BOUNDING_BOX_FRONT (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=s.get(2);}
//-----------------------------------------------------------------------------
void  PROPERTY_INITIALIZE_BOUNDING_BOX_BACK (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=s.get(2);}
//-----------------------------------------------------------------------------
void  PROPERTY_INITIALIZE_BOUNDING_BOX_TOP (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=s.get(1);}
//-----------------------------------------------------------------------------
void  PROPERTY_INITIALIZE_BOUNDING_BOX_BOTTOM (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=s.get(1);}
//-----------------------------------------------------------------------------
void  PROPERTY_INITIALIZE_BOUNDING_BOX_LEFT_R45 (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=(DEFINED_PROPERTY_TYPE)(SC45*(double)(SC45*(double)(s.get(0)+SC45*(double)(s.get(1)+s.get(2)))
		-SC45*(double)(s.get(1)-s.get(2))));}
//-----------------------------------------------------------------------------
void  PROPERTY_INITIALIZE_BOUNDING_BOX_RIGHT_R45 (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=(DEFINED_PROPERTY_TYPE)(SC45*(double)(SC45*(double)(s.get(0)+SC45*(double)(s.get(1)+s.get(2)))
		-SC45*(double)(s.get(1)-s.get(2))));}
//-----------------------------------------------------------------------------
void  PROPERTY_INITIALIZE_BOUNDING_BOX_FRONT_R45 (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=(DEFINED_PROPERTY_TYPE)(SC45*(double)(SC45*(double)(s.get(1)+s.get(2))-s.get(0)));}
//-----------------------------------------------------------------------------
void  PROPERTY_INITIALIZE_BOUNDING_BOX_BACK_R45 (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=(DEFINED_PROPERTY_TYPE)(SC45*(double)(SC45*(double)(s.get(1)+s.get(2))-s.get(0)));}
//-----------------------------------------------------------------------------
void  PROPERTY_INITIALIZE_BOUNDING_BOX_TOP_R45 (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=(DEFINED_PROPERTY_TYPE)(SC45*(double)(SC45*(double)(s.get(0)+SC45*(double)(s.get(1)+s.get(2)))
		+SC45*(double)(s.get(1)-s.get(2))));}
//-----------------------------------------------------------------------------
void  PROPERTY_INITIALIZE_BOUNDING_BOX_BOTTOM_R45 (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=(DEFINED_PROPERTY_TYPE)(SC45*(double)(SC45*(double)(s.get(0)+SC45*(double)(s.get(1)+s.get(2)))
		+SC45*(double)(s.get(1)-s.get(2))));}
//-----------------------------------------------------------------------------
void PROPERTY_INITIALIZE_TOTAL_LENGTH (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=(DEFINED_PROPERTY_TYPE)1.0;}
//-----------------------------------------------------------------------------
void PROPERTY_INITIALIZE_ANGLE_COEFF_SAX (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=(DEFINED_PROPERTY_TYPE)0.0;}
//-----------------------------------------------------------------------------
void PROPERTY_INITIALIZE_ANGLE_COEFF_SAY (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=(DEFINED_PROPERTY_TYPE)0.0;}
//-----------------------------------------------------------------------------
void PROPERTY_INITIALIZE_ANGLE_COEFF_SAZ (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=(DEFINED_PROPERTY_TYPE)0.0;}
//-----------------------------------------------------------------------------
void PROPERTY_INITIALIZE_ANGLE_COEFF_EAX (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=(DEFINED_PROPERTY_TYPE)0.0;}
//-----------------------------------------------------------------------------
void PROPERTY_INITIALIZE_ANGLE_COEFF_EAY (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=(DEFINED_PROPERTY_TYPE)0.0;}
//-----------------------------------------------------------------------------
void PROPERTY_INITIALIZE_ANGLE_COEFF_EAZ (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=(DEFINED_PROPERTY_TYPE)0.0;}
//-----------------------------------------------------------------------------
void PROPERTY_INITIALIZE_SUM_X (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=s.get(0);}
//-----------------------------------------------------------------------------
void PROPERTY_INITIALIZE_SUM_Y (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=s.get(1);}
//-----------------------------------------------------------------------------
void PROPERTY_INITIALIZE_SUM_Z (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=s.get(2);}
//-----------------------------------------------------------------------------
void PROPERTY_INITIALIZE_HORIZONTAL_MOVEMENTS (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=(DEFINED_PROPERTY_TYPE)1.0;}
//-----------------------------------------------------------------------------
void PROPERTY_INITIALIZE_VERTICAL_MOVEMENTS (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=(DEFINED_PROPERTY_TYPE)1.0;}
//-----------------------------------------------------------------------------
void PROPERTY_INITIALIZE_DEPTH_MOVEMENTS (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=(DEFINED_PROPERTY_TYPE)1.0;}
//-----------------------------------------------------------------------------
void PROPERTY_INITIALIZE_HORIZONTAL_MOVEMENTS_R45 (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=(DEFINED_PROPERTY_TYPE)1.0;}
//-----------------------------------------------------------------------------
void PROPERTY_INITIALIZE_VERTICAL_MOVEMENTS_R45 (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=(DEFINED_PROPERTY_TYPE)1.0;}
//-----------------------------------------------------------------------------
void PROPERTY_INITIALIZE_DEPTH_MOVEMENTS_R45 (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=(DEFINED_PROPERTY_TYPE)1.0;}
//-----------------------------------------------------------------------------
void PROPERTY_INITIALIZE_START_X (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=s.get(0);}
//-----------------------------------------------------------------------------
void PROPERTY_INITIALIZE_START_Y (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=s.get(1);}
//-----------------------------------------------------------------------------
void PROPERTY_INITIALIZE_START_Z (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=s.get(2);}
//-----------------------------------------------------------------------------
void PROPERTY_INITIALIZE_END_X (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=s.get(0);}
//-----------------------------------------------------------------------------
void PROPERTY_INITIALIZE_END_Y (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=s.get(1);}
//-----------------------------------------------------------------------------
void PROPERTY_INITIALIZE_END_Z (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=s.get(2);}
//-----------------------------------------------------------------------------
void  PROPERTY_INITIALIZE_ORIENTATION_BOUNDING_BOX_LEFT (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=s.get(3);}
//-----------------------------------------------------------------------------
void  PROPERTY_INITIALIZE_ORIENTATION_BOUNDING_BOX_RIGHT (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=s.get(3);}
//-----------------------------------------------------------------------------
void  PROPERTY_INITIALIZE_ORIENTATION_BOUNDING_BOX_FRONT (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=s.get(5);}
//-----------------------------------------------------------------------------
void  PROPERTY_INITIALIZE_ORIENTATION_BOUNDING_BOX_BACK (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=s.get(5);}
//-----------------------------------------------------------------------------
void  PROPERTY_INITIALIZE_ORIENTATION_BOUNDING_BOX_TOP (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=s.get(4);}
//-----------------------------------------------------------------------------
void  PROPERTY_INITIALIZE_ORIENTATION_BOUNDING_BOX_BOTTOM (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=s.get(4);}
//-----------------------------------------------------------------------------
void  PROPERTY_INITIALIZE_ORIENTATION_BOUNDING_BOX_LEFT_R45 (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=(DEFINED_PROPERTY_TYPE)(SC45*(double)(SC45*(double)(s.get(3)+SC45*(double)(s.get(4)+s.get(5)))
		-SC45*(double)(s.get(4)-s.get(5))));}
//-----------------------------------------------------------------------------
void  PROPERTY_INITIALIZE_ORIENTATION_BOUNDING_BOX_RIGHT_R45 (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=(DEFINED_PROPERTY_TYPE)(SC45*(double)(SC45*(double)(s.get(3)+SC45*(double)(s.get(4)+s.get(5)))
		-SC45*(double)(s.get(4)-s.get(5))));}
//-----------------------------------------------------------------------------
void  PROPERTY_INITIALIZE_ORIENTATION_BOUNDING_BOX_FRONT_R45 (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=(DEFINED_PROPERTY_TYPE)(SC45*(double)(SC45*(double)(s.get(4)+s.get(5))-s.get(3)));}
//-----------------------------------------------------------------------------
void  PROPERTY_INITIALIZE_ORIENTATION_BOUNDING_BOX_BACK_R45 (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=(DEFINED_PROPERTY_TYPE)(SC45*(double)(SC45*(double)(s.get(4)+s.get(5))-s.get(3)));}
//-----------------------------------------------------------------------------
void  PROPERTY_INITIALIZE_ORIENTATION_BOUNDING_BOX_TOP_R45 (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=(DEFINED_PROPERTY_TYPE)(SC45*(double)(SC45*(double)(s.get(3)+SC45*(double)(s.get(4)+s.get(5)))
		+SC45*(double)(s.get(4)-s.get(5))));}
//-----------------------------------------------------------------------------
void  PROPERTY_INITIALIZE_ORIENTATION_BOUNDING_BOX_BOTTOM_R45 (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=(DEFINED_PROPERTY_TYPE)(SC45*(double)(SC45*(double)(s.get(3)+SC45*(double)(s.get(4)+s.get(5)))
		+SC45*(double)(s.get(4)-s.get(5))));}
//-----------------------------------------------------------------------------
void PROPERTY_INITIALIZE_ORIENTATION_TOTAL_LENGTH (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=(DEFINED_PROPERTY_TYPE)1.0;}
//-----------------------------------------------------------------------------
void PROPERTY_INITIALIZE_ORIENTATION_ANGLE_COEFF_SAAZIM (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=(DEFINED_PROPERTY_TYPE)0.0;}
//-----------------------------------------------------------------------------
void PROPERTY_INITIALIZE_ORIENTATION_ANGLE_COEFF_SAELEV (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=(DEFINED_PROPERTY_TYPE)0.0;}
//-----------------------------------------------------------------------------
void PROPERTY_INITIALIZE_ORIENTATION_ANGLE_COEFF_SAROLL (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=(DEFINED_PROPERTY_TYPE)0.0;}
//-----------------------------------------------------------------------------
void PROPERTY_INITIALIZE_ORIENTATION_ANGLE_COEFF_EAAZIM (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=(DEFINED_PROPERTY_TYPE)0.0;}
//-----------------------------------------------------------------------------
void PROPERTY_INITIALIZE_ORIENTATION_ANGLE_COEFF_EAELEV (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=(DEFINED_PROPERTY_TYPE)0.0;}
//-----------------------------------------------------------------------------
void PROPERTY_INITIALIZE_ORIENTATION_ANGLE_COEFF_EAROLL (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=(DEFINED_PROPERTY_TYPE)0.0;}
//-----------------------------------------------------------------------------
void PROPERTY_INITIALIZE_ORIENTATION_SUM_AZIM (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=s.get(3);}
//-----------------------------------------------------------------------------
void PROPERTY_INITIALIZE_ORIENTATION_SUM_ELEV (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=s.get(4);}
//-----------------------------------------------------------------------------
void PROPERTY_INITIALIZE_ORIENTATION_SUM_ROLL (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=s.get(5);}
//-----------------------------------------------------------------------------
void PROPERTY_INITIALIZE_ORIENTATION_AZIMUTHAL_MOVEMENTS (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=(DEFINED_PROPERTY_TYPE)1.0;}
//-----------------------------------------------------------------------------
void PROPERTY_INITIALIZE_ORIENTATION_ELEVATION_MOVEMENTS (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=(DEFINED_PROPERTY_TYPE)1.0;}
//-----------------------------------------------------------------------------
void PROPERTY_INITIALIZE_ORIENTATION_ROLL_MOVEMENTS (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=(DEFINED_PROPERTY_TYPE)1.0;}
//-----------------------------------------------------------------------------
void PROPERTY_INITIALIZE_ORIENTATION_AZIMUTHAL_MOVEMENTS_R45 (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=(DEFINED_PROPERTY_TYPE)1.0;}
//-----------------------------------------------------------------------------
void PROPERTY_INITIALIZE_ORIENTATION_ELEVATION_MOVEMENTS_R45 (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=(DEFINED_PROPERTY_TYPE)1.0;}
//-----------------------------------------------------------------------------
void PROPERTY_INITIALIZE_ORIENTATION_ROLL_MOVEMENTS_R45 (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=(DEFINED_PROPERTY_TYPE)1.0;}
//-----------------------------------------------------------------------------
void PROPERTY_INITIALIZE_ORIENTATION_START_AZIM (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=s.get(3);}
//-----------------------------------------------------------------------------
void PROPERTY_INITIALIZE_ORIENTATION_START_ELEV (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=s.get(4);}
//-----------------------------------------------------------------------------
void PROPERTY_INITIALIZE_ORIENTATION_START_ROLL (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=s.get(5);}
//-----------------------------------------------------------------------------
void PROPERTY_INITIALIZE_ORIENTATION_END_AZIM (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=s.get(3);}
//-----------------------------------------------------------------------------
void PROPERTY_INITIALIZE_ORIENTATION_END_ELEV (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=s.get(4);}
//-----------------------------------------------------------------------------
void PROPERTY_INITIALIZE_ORIENTATION_END_ROLL (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=s.get(5);}
//-----------------------------------------------------------------------------
void PROPERTY_INITIALIZE_NUMBER_OF_SAMPLES (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=s.number_of_samples();}
//-----------------------------------------------------------------------------
void PROPERTY_INITIALIZE_START_TIME (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
  struct timeb timebuffer;
  ftime( &timebuffer );
  v=(DEFINED_PROPERTY_TYPE) (timebuffer.time&TIME_MAX_SECONDS)*1000+timebuffer.millitm; 
}
//-----------------------------------------------------------------------------
void PROPERTY_INITIALIZE_END_TIME (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{
 struct timeb timebuffer;
  ftime( &timebuffer );
  v=(DEFINED_PROPERTY_TYPE) (timebuffer.time&TIME_MAX_SECONDS)*1000+timebuffer.millitm; 
}
//-----------------------------------------------------------------------------
void PROPERTY_INITIALIZE_MIN_CG_2_CURVE_X (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=s.get(0);}
//-----------------------------------------------------------------------------
void PROPERTY_INITIALIZE_MIN_CG_2_CURVE_Y (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=s.get(1);}
//-----------------------------------------------------------------------------
void PROPERTY_INITIALIZE_MIN_CG_2_CURVE_Z (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=s.get(2);}
//-----------------------------------------------------------------------------
void PROPERTY_INITIALIZE_MAX_CG_2_CURVE_X (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=s.get(0);}
//-----------------------------------------------------------------------------
void PROPERTY_INITIALIZE_MAX_CG_2_CURVE_Y (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=s.get(1);}
//-----------------------------------------------------------------------------
void PROPERTY_INITIALIZE_MAX_CG_2_CURVE_Z (DEFINED_PROPERTY_TYPE &v,DEFINED_SAMPLE_TYPE s)
{v=s.get(2);}
//-----------------------------------------------------------------------------
#endif
