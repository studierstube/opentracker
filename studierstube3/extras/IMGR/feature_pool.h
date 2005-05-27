#ifndef FEATURE_POOL_INCLUDED
#define FEATURE_POOL_INCLUDED

#include <math.h>

#include "feature.h"
#include "property.h"
#include "property_pool.h"
#include "property_list_pool.h"
#include "dependence_pool.h"

#define DEFINED_FEATURE_TYPE float

//dependence level 0-----------------------------------------------------------
#define	FEATURE_STRAIGHTNESS			0

#define FEATURE_HEIGHT_WIDTH_RATIO		1
#define FEATURE_HEIGHT_DEPTH_RATIO		2
#define FEATURE_DEPTH_WIDTH_RATIO		3

#define FEATURE_HEIGHT_WIDTH_RATIO_R45		4
#define FEATURE_HEIGHT_DEPTH_RATIO_R45		5
#define FEATURE_DEPTH_WIDTH_RATIO_R45		6

#define FEATURE_RELATIVE_START_X			7
#define FEATURE_RELATIVE_START_Y			8
#define FEATURE_RELATIVE_START_Z			9

#define FEATURE_RELATIVE_END_X				10
#define FEATURE_RELATIVE_END_Y				11
#define FEATURE_RELATIVE_END_Z				12

#define FEATURE_HORIZONTAL_MOVEMENTS		13
#define FEATURE_VERTICAL_MOVEMENTS			14
#define FEATURE_DEPTH_MOVEMENTS				15

#define FEATURE_HORIZONTAL_MOVEMENTS_R45	16
#define FEATURE_VERTICAL_MOVEMENTS_R45		17
#define FEATURE_DEPTH_MOVEMENTS_R45			18

#define FEATURE_START_ANGLE_X_Y			19
#define FEATURE_START_ANGLE_X_Z			20
#define FEATURE_START_ANGLE_Z_Y			21

#define FEATURE_END_ANGLE_X_Y			22
#define FEATURE_END_ANGLE_X_Z			23
#define FEATURE_END_ANGLE_Z_Y			24

#define FEATURE_RELATIVE_CG_X			25
#define FEATURE_RELATIVE_CG_Y			26
#define FEATURE_RELATIVE_CG_Z			27

#define	FEATURE_ORIENTATION_STRAIGHTNESS			28

#define FEATURE_ORIENTATION_HEIGHT_WIDTH_RATIO		29
#define FEATURE_ORIENTATION_HEIGHT_DEPTH_RATIO		30
#define FEATURE_ORIENTATION_DEPTH_WIDTH_RATIO		31

#define FEATURE_ORIENTATION_HEIGHT_WIDTH_RATIO_R45		32
#define FEATURE_ORIENTATION_HEIGHT_DEPTH_RATIO_R45		33
#define FEATURE_ORIENTATION_DEPTH_WIDTH_RATIO_R45		34

#define FEATURE_ORIENTATION_RELATIVE_START_AZIM			35
#define FEATURE_ORIENTATION_RELATIVE_START_ELEV			36
#define FEATURE_ORIENTATION_RELATIVE_START_ROLL			37

#define FEATURE_ORIENTATION_RELATIVE_END_AZIM			38
#define FEATURE_ORIENTATION_RELATIVE_END_ELEV			39
#define FEATURE_ORIENTATION_RELATIVE_END_ROLL			40

#define FEATURE_ORIENTATION_AZIMUTHAL_MOVEMENTS			41
#define FEATURE_ORIENTATION_ELEVATION_MOVEMENTS			42
#define FEATURE_ORIENTATION_ROLL_MOVEMENTS				43

#define FEATURE_ORIENTATION_AZIMUTHAL_MOVEMENTS_R45		44
#define FEATURE_ORIENTATION_ELEVATION_MOVEMENTS_R45		45
#define FEATURE_ORIENTATION_ROLL_MOVEMENTS_R45			46

#define FEATURE_ORIENTATION_START_ANGLE_AZIM_ELEV		47
#define FEATURE_ORIENTATION_START_ANGLE_AZIM_ROLL		48
#define FEATURE_ORIENTATION_START_ANGLE_ROLL_ELEV		49

#define FEATURE_ORIENTATION_END_ANGLE_AZIM_ELEV			50
#define FEATURE_ORIENTATION_END_ANGLE_AZIM_ROLL			51
#define FEATURE_ORIENTATION_END_ANGLE_ROLL_ELEV			52

#define FEATURE_ORIENTATION_RELATIVE_CG_AZIM			53
#define FEATURE_ORIENTATION_RELATIVE_CG_ELEV			54
#define FEATURE_ORIENTATION_RELATIVE_CG_ROLL			55
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
DEFINED_FEATURE_TYPE angle(DEFINED_PROPERTY_TYPE x,DEFINED_PROPERTY_TYPE y)
{ double a,b;				//angle from sine and cosine
                                     
  a=(double)hypot(y,x);

  if(a==0)return(0);
  b=(double)acos(x/a);
  if(y<0)b=(double)M_PI*2-b;
  return((DEFINED_FEATURE_TYPE)(b/M_PI*50));             // normalize :
}														 // range 0-100
//-----------------------------------------------------------------------------

//calculate section
//-----------------------------------------------------------------------------
void  FEATURE_CALCULATE_STRAIGHTNESS (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
   DEFINED_PROPERTY_TYPE width,height,depth;

	
   width=pl.look_up(PROPERTY_BOUNDING_BOX_RIGHT).get()-pl.look_up(PROPERTY_BOUNDING_BOX_LEFT).get()+1;				
   height=pl.look_up(PROPERTY_BOUNDING_BOX_TOP).get()-pl.look_up(PROPERTY_BOUNDING_BOX_BOTTOM).get()+1;            
   depth=pl.look_up(PROPERTY_BOUNDING_BOX_FRONT).get()-pl.look_up(PROPERTY_BOUNDING_BOX_BACK).get()+1;				
	
   v=(DEFINED_FEATURE_TYPE)(sqrt(width*width+height*height+depth*depth)*98/pl.look_up(PROPERTY_TOTAL_LENGTH).get()+1);

}
//-----------------------------------------------------------------------------
void  FEATURE_CALCULATE_HEIGHT_WIDTH_RATIO (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
   DEFINED_PROPERTY_TYPE width,height;

	
   width=pl.look_up(PROPERTY_BOUNDING_BOX_RIGHT).get()-pl.look_up(PROPERTY_BOUNDING_BOX_LEFT).get()+1;				
   height=pl.look_up(PROPERTY_BOUNDING_BOX_TOP).get()-pl.look_up(PROPERTY_BOUNDING_BOX_BOTTOM).get()+1;            
   
	
   v=(DEFINED_FEATURE_TYPE)(height*98/(height+width)+1);

}
//-----------------------------------------------------------------------------
void  FEATURE_CALCULATE_HEIGHT_DEPTH_RATIO (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
   DEFINED_PROPERTY_TYPE depth,height;

	
   depth=pl.look_up(PROPERTY_BOUNDING_BOX_FRONT).get()-pl.look_up(PROPERTY_BOUNDING_BOX_BACK).get()+1;				
   height=pl.look_up(PROPERTY_BOUNDING_BOX_TOP).get()-pl.look_up(PROPERTY_BOUNDING_BOX_BOTTOM).get()+1;            
   
	
   v=(DEFINED_FEATURE_TYPE)(height*98/(height+depth)+1);

}
//-----------------------------------------------------------------------------
void  FEATURE_CALCULATE_DEPTH_WIDTH_RATIO (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
   DEFINED_PROPERTY_TYPE depth,width;

	
   depth=pl.look_up(PROPERTY_BOUNDING_BOX_FRONT).get()-pl.look_up(PROPERTY_BOUNDING_BOX_BACK).get()+1;				
   width=pl.look_up(PROPERTY_BOUNDING_BOX_RIGHT).get()-pl.look_up(PROPERTY_BOUNDING_BOX_LEFT).get()+1;				           
   
	
   v=(DEFINED_FEATURE_TYPE)(depth*98/(depth+width)+1);

}
//-----------------------------------------------------------------------------
void  FEATURE_CALCULATE_HEIGHT_WIDTH_RATIO_R45 (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
   DEFINED_PROPERTY_TYPE rwidth,rheight;

	
   rwidth=pl.look_up(PROPERTY_BOUNDING_BOX_RIGHT_R45).get()-pl.look_up(PROPERTY_BOUNDING_BOX_LEFT_R45).get()+1;				
   rheight=pl.look_up(PROPERTY_BOUNDING_BOX_TOP_R45).get()-pl.look_up(PROPERTY_BOUNDING_BOX_BOTTOM_R45).get()+1;            
   
	
   v=(DEFINED_FEATURE_TYPE)(rheight*98/(rheight+rwidth)+1);

}
//-----------------------------------------------------------------------------
void  FEATURE_CALCULATE_HEIGHT_DEPTH_RATIO_R45 (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
   DEFINED_PROPERTY_TYPE rdepth,rheight;

	
   rdepth=pl.look_up(PROPERTY_BOUNDING_BOX_FRONT_R45).get()-pl.look_up(PROPERTY_BOUNDING_BOX_BACK_R45).get()+1;				
   rheight=pl.look_up(PROPERTY_BOUNDING_BOX_TOP_R45).get()-pl.look_up(PROPERTY_BOUNDING_BOX_BOTTOM_R45).get()+1;            
   
	
   v=(DEFINED_FEATURE_TYPE)(rheight*98/(rheight+rdepth)+1);

}
//-----------------------------------------------------------------------------
void  FEATURE_CALCULATE_DEPTH_WIDTH_RATIO_R45 (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
   DEFINED_PROPERTY_TYPE rdepth,rwidth;

	
   rdepth=pl.look_up(PROPERTY_BOUNDING_BOX_FRONT_R45).get()-pl.look_up(PROPERTY_BOUNDING_BOX_BACK_R45).get()+1;				
   rwidth=pl.look_up(PROPERTY_BOUNDING_BOX_RIGHT_R45).get()-pl.look_up(PROPERTY_BOUNDING_BOX_LEFT_R45).get()+1;				           
   
	
   v=(DEFINED_FEATURE_TYPE)(rdepth*98/(rdepth+rwidth)+1);

}
//-----------------------------------------------------------------------------
void  FEATURE_CALCULATE_RELATIVE_START_X (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
   DEFINED_PROPERTY_TYPE width;
			
   width=pl.look_up(PROPERTY_BOUNDING_BOX_RIGHT).get()-pl.look_up(PROPERTY_BOUNDING_BOX_LEFT).get()+1;	

   v=(DEFINED_FEATURE_TYPE)((pl.look_up(PROPERTY_START_X).get()-pl.look_up(PROPERTY_BOUNDING_BOX_LEFT).get())*98/(width)+1);

}
//-----------------------------------------------------------------------------
void  FEATURE_CALCULATE_RELATIVE_START_Y (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
   
   DEFINED_PROPERTY_TYPE height;

				
   height=pl.look_up(PROPERTY_BOUNDING_BOX_TOP).get()-pl.look_up(PROPERTY_BOUNDING_BOX_BOTTOM).get()+1;			
   
   v=(DEFINED_FEATURE_TYPE)((pl.look_up(PROPERTY_START_Y).get()-pl.look_up(PROPERTY_BOUNDING_BOX_BOTTOM).get())*98/(height)+1);

}
//-----------------------------------------------------------------------------
void  FEATURE_CALCULATE_RELATIVE_START_Z (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
   DEFINED_PROPERTY_TYPE depth;

	
   depth=pl.look_up(PROPERTY_BOUNDING_BOX_FRONT).get()-pl.look_up(PROPERTY_BOUNDING_BOX_BACK).get()+1;				
   
   v=(DEFINED_FEATURE_TYPE)((pl.look_up(PROPERTY_START_Z).get()-pl.look_up(PROPERTY_BOUNDING_BOX_BACK).get())*98/(depth)+1);

}
//-----------------------------------------------------------------------------
void  FEATURE_CALCULATE_RELATIVE_END_X (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
   DEFINED_PROPERTY_TYPE width;
			
   width=pl.look_up(PROPERTY_BOUNDING_BOX_RIGHT).get()-pl.look_up(PROPERTY_BOUNDING_BOX_LEFT).get()+1;	

   v=(DEFINED_FEATURE_TYPE)((pl.look_up(PROPERTY_END_X).get()-pl.look_up(PROPERTY_BOUNDING_BOX_LEFT).get())*98/(width)+1);

}
//-----------------------------------------------------------------------------
void  FEATURE_CALCULATE_RELATIVE_END_Y (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
   
   DEFINED_PROPERTY_TYPE height;

				
   height=pl.look_up(PROPERTY_BOUNDING_BOX_TOP).get()-pl.look_up(PROPERTY_BOUNDING_BOX_BOTTOM).get()+1;			
   
   v=(DEFINED_FEATURE_TYPE)((pl.look_up(PROPERTY_END_Y).get()-pl.look_up(PROPERTY_BOUNDING_BOX_BOTTOM).get())*98/(height)+1);

}
//-----------------------------------------------------------------------------
void  FEATURE_CALCULATE_RELATIVE_END_Z (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
   DEFINED_PROPERTY_TYPE depth;

	
   depth=pl.look_up(PROPERTY_BOUNDING_BOX_FRONT).get()-pl.look_up(PROPERTY_BOUNDING_BOX_BACK).get()+1;				
   
   v=(DEFINED_FEATURE_TYPE)((pl.look_up(PROPERTY_END_Z).get()-pl.look_up(PROPERTY_BOUNDING_BOX_BACK).get())*98/(depth)+1);

}
//-----------------------------------------------------------------------------
void  FEATURE_CALCULATE_HORIZONTAL_MOVEMENTS (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
   DEFINED_PROPERTY_TYPE width;
			
   width=pl.look_up(PROPERTY_BOUNDING_BOX_RIGHT).get()-pl.look_up(PROPERTY_BOUNDING_BOX_LEFT).get()+1;	
   

   v=(DEFINED_FEATURE_TYPE)(width*98/pl.look_up(PROPERTY_HORIZONTAL_MOVEMENTS).get()+1);
}
//-----------------------------------------------------------------------------
void  FEATURE_CALCULATE_VERTICAL_MOVEMENTS (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
   DEFINED_PROPERTY_TYPE height;

				
   height=pl.look_up(PROPERTY_BOUNDING_BOX_TOP).get()-pl.look_up(PROPERTY_BOUNDING_BOX_BOTTOM).get()+1;

   v=(DEFINED_FEATURE_TYPE)(height*98/pl.look_up(PROPERTY_VERTICAL_MOVEMENTS).get()+1);
}
//-----------------------------------------------------------------------------
void  FEATURE_CALCULATE_DEPTH_MOVEMENTS (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
   DEFINED_PROPERTY_TYPE depth;

	
   depth=pl.look_up(PROPERTY_BOUNDING_BOX_FRONT).get()-pl.look_up(PROPERTY_BOUNDING_BOX_BACK).get()+1;

   v=(DEFINED_FEATURE_TYPE)(depth*98/pl.look_up(PROPERTY_DEPTH_MOVEMENTS).get()+1);
}
//-----------------------------------------------------------------------------
void  FEATURE_CALCULATE_HORIZONTAL_MOVEMENTS_R45 (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
   DEFINED_PROPERTY_TYPE rwidth;
			
   rwidth=pl.look_up(PROPERTY_BOUNDING_BOX_RIGHT_R45).get()-pl.look_up(PROPERTY_BOUNDING_BOX_LEFT_R45).get()+1;	

   v=(DEFINED_FEATURE_TYPE)(rwidth*98/pl.look_up(PROPERTY_HORIZONTAL_MOVEMENTS_R45).get()+1);
}
//-----------------------------------------------------------------------------
void  FEATURE_CALCULATE_VERTICAL_MOVEMENTS_R45 (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
   DEFINED_PROPERTY_TYPE rheight;

				
   rheight=pl.look_up(PROPERTY_BOUNDING_BOX_TOP_R45).get()-pl.look_up(PROPERTY_BOUNDING_BOX_BOTTOM_R45).get()+1;

   v=(DEFINED_FEATURE_TYPE)(rheight*98/pl.look_up(PROPERTY_VERTICAL_MOVEMENTS_R45).get()+1);
}
//-----------------------------------------------------------------------------
void  FEATURE_CALCULATE_DEPTH_MOVEMENTS_R45 (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
   DEFINED_PROPERTY_TYPE rdepth;

	
   rdepth=pl.look_up(PROPERTY_BOUNDING_BOX_FRONT_R45).get()-pl.look_up(PROPERTY_BOUNDING_BOX_BACK_R45).get()+1;
   
   v=(DEFINED_FEATURE_TYPE)(rdepth*98/pl.look_up(PROPERTY_DEPTH_MOVEMENTS_R45).get()+1);
}
//-----------------------------------------------------------------------------
void  FEATURE_CALCULATE_START_ANGLE_X_Y (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
   v=(DEFINED_FEATURE_TYPE)angle(pl.look_up(PROPERTY_ANGLE_COEFF_SAX).get(),pl.look_up(PROPERTY_ANGLE_COEFF_SAY).get());
}
//-----------------------------------------------------------------------------
void  FEATURE_CALCULATE_START_ANGLE_X_Z (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
   v=(DEFINED_FEATURE_TYPE)angle(pl.look_up(PROPERTY_ANGLE_COEFF_SAX).get(),pl.look_up(PROPERTY_ANGLE_COEFF_SAZ).get());
}
//-----------------------------------------------------------------------------
void  FEATURE_CALCULATE_START_ANGLE_Z_Y (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
   v=(DEFINED_FEATURE_TYPE)angle(pl.look_up(PROPERTY_ANGLE_COEFF_SAZ).get(),pl.look_up(PROPERTY_ANGLE_COEFF_SAY).get());
}
//-----------------------------------------------------------------------------
void  FEATURE_CALCULATE_END_ANGLE_X_Y (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
   v=(DEFINED_FEATURE_TYPE)angle(pl.look_up(PROPERTY_ANGLE_COEFF_EAX).get(),pl.look_up(PROPERTY_ANGLE_COEFF_EAY).get());
}
//-----------------------------------------------------------------------------
void  FEATURE_CALCULATE_END_ANGLE_X_Z (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
   v=(DEFINED_FEATURE_TYPE)angle(pl.look_up(PROPERTY_ANGLE_COEFF_EAX).get(),pl.look_up(PROPERTY_ANGLE_COEFF_EAZ).get());
}
//-----------------------------------------------------------------------------
void  FEATURE_CALCULATE_END_ANGLE_Z_Y (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
   v=(DEFINED_FEATURE_TYPE)angle(pl.look_up(PROPERTY_ANGLE_COEFF_EAZ).get(),pl.look_up(PROPERTY_ANGLE_COEFF_EAY).get());
}
//-----------------------------------------------------------------------------
void  FEATURE_CALCULATE_RELATIVE_CG_X (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
   DEFINED_PROPERTY_TYPE width;
   double cgx;

   cgx=(DEFINED_PROPERTY_TYPE)((double)pl.look_up(PROPERTY_SUM_X).get()/(double)pl.look_up(PROPERTY_NUMBER_OF_SAMPLES).get());	
   width=pl.look_up(PROPERTY_BOUNDING_BOX_RIGHT).get()-pl.look_up(PROPERTY_BOUNDING_BOX_LEFT).get()+1;

   v=(DEFINED_FEATURE_TYPE)((cgx-pl.look_up(PROPERTY_BOUNDING_BOX_LEFT).get())*98/(width)+1);
}
//-----------------------------------------------------------------------------
void  FEATURE_CALCULATE_RELATIVE_CG_Y (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
   DEFINED_PROPERTY_TYPE height;
   double cgy;

   cgy=(DEFINED_PROPERTY_TYPE)((double)pl.look_up(PROPERTY_SUM_Y).get()/(double)pl.look_up(PROPERTY_NUMBER_OF_SAMPLES).get());	
   height=pl.look_up(PROPERTY_BOUNDING_BOX_TOP).get()-pl.look_up(PROPERTY_BOUNDING_BOX_BOTTOM).get()+1;

   v=(DEFINED_FEATURE_TYPE)((cgy-pl.look_up(PROPERTY_BOUNDING_BOX_BOTTOM).get())*98/(height)+1);
}
//-----------------------------------------------------------------------------
void  FEATURE_CALCULATE_RELATIVE_CG_Z (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
   DEFINED_PROPERTY_TYPE depth;
   double cgz;

   cgz=(DEFINED_PROPERTY_TYPE)((double)pl.look_up(PROPERTY_SUM_Z).get()/(double)pl.look_up(PROPERTY_NUMBER_OF_SAMPLES).get());	
   depth=pl.look_up(PROPERTY_BOUNDING_BOX_FRONT).get()-pl.look_up(PROPERTY_BOUNDING_BOX_BACK).get()+1;				

   v=(DEFINED_FEATURE_TYPE)((cgz-pl.look_up(PROPERTY_BOUNDING_BOX_BACK).get())*98/(depth)+1);
}
//-----------------------------------------------------------------------------
void  FEATURE_CALCULATE_ORIENTATION_STRAIGHTNESS (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
   DEFINED_PROPERTY_TYPE width,height,depth;

	
   width=pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_RIGHT).get()-pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_LEFT).get()+1;				
   height=pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_TOP).get()-pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_BOTTOM).get()+1;            
   depth=pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_FRONT).get()-pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_BACK).get()+1;				
	
   v=(DEFINED_FEATURE_TYPE)(sqrt(width*width+height*height+depth*depth)*98/pl.look_up(PROPERTY_ORIENTATION_TOTAL_LENGTH).get()+1);

}
//-----------------------------------------------------------------------------
void  FEATURE_CALCULATE_ORIENTATION_HEIGHT_WIDTH_RATIO (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
   DEFINED_PROPERTY_TYPE width,height;

	
   width=pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_RIGHT).get()-pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_LEFT).get()+1;				
   height=pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_TOP).get()-pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_BOTTOM).get()+1;            
   
	
   v=(DEFINED_FEATURE_TYPE)(height*98/(height+width)+1);

}
//-----------------------------------------------------------------------------
void  FEATURE_CALCULATE_ORIENTATION_HEIGHT_DEPTH_RATIO (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
   DEFINED_PROPERTY_TYPE depth,height;

	
   depth=pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_FRONT).get()-pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_BACK).get()+1;				
   height=pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_TOP).get()-pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_BOTTOM).get()+1;            
   
	
   v=(DEFINED_FEATURE_TYPE)(height*98/(height+depth)+1);

}
//-----------------------------------------------------------------------------
void  FEATURE_CALCULATE_ORIENTATION_DEPTH_WIDTH_RATIO (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
   DEFINED_PROPERTY_TYPE depth,width;

	
   depth=pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_FRONT).get()-pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_BACK).get()+1;				
   width=pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_RIGHT).get()-pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_LEFT).get()+1;				           
   
	
   v=(DEFINED_FEATURE_TYPE)(depth*98/(depth+width)+1);

}
//-----------------------------------------------------------------------------
void  FEATURE_CALCULATE_ORIENTATION_HEIGHT_WIDTH_RATIO_R45 (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
   DEFINED_PROPERTY_TYPE rwidth,rheight;

	
   rwidth=pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_RIGHT_R45).get()-pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_LEFT_R45).get()+1;				
   rheight=pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_TOP_R45).get()-pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_BOTTOM_R45).get()+1;            
   
	
   v=(DEFINED_FEATURE_TYPE)(rheight*98/(rheight+rwidth)+1);

}
//-----------------------------------------------------------------------------
void  FEATURE_CALCULATE_ORIENTATION_HEIGHT_DEPTH_RATIO_R45 (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
   DEFINED_PROPERTY_TYPE rdepth,rheight;

	
   rdepth=pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_FRONT_R45).get()-pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_BACK_R45).get()+1;				
   rheight=pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_TOP_R45).get()-pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_BOTTOM_R45).get()+1;            
   
	
   v=(DEFINED_FEATURE_TYPE)(rheight*98/(rheight+rdepth)+1);

}
//-----------------------------------------------------------------------------
void  FEATURE_CALCULATE_ORIENTATION_DEPTH_WIDTH_RATIO_R45 (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
   DEFINED_PROPERTY_TYPE rdepth,rwidth;

	
   rdepth=pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_FRONT_R45).get()-pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_BACK_R45).get()+1;				
   rwidth=pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_RIGHT_R45).get()-pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_LEFT_R45).get()+1;				           
   
	
   v=(DEFINED_FEATURE_TYPE)(rdepth*98/(rdepth+rwidth)+1);

}
//-----------------------------------------------------------------------------
void  FEATURE_CALCULATE_ORIENTATION_RELATIVE_START_AZIM (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
   DEFINED_PROPERTY_TYPE width;
			
   width=pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_RIGHT).get()-pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_LEFT).get()+1;	

   v=(DEFINED_FEATURE_TYPE)((pl.look_up(PROPERTY_ORIENTATION_START_AZIM).get()-pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_LEFT).get())*98/(width)+1);

}
//-----------------------------------------------------------------------------
void  FEATURE_CALCULATE_ORIENTATION_RELATIVE_START_ELEV (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
   
   DEFINED_PROPERTY_TYPE height;

				
   height=pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_TOP).get()-pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_BOTTOM).get()+1;			
   
   v=(DEFINED_FEATURE_TYPE)((pl.look_up(PROPERTY_ORIENTATION_START_ELEV).get()-pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_BOTTOM).get())*98/(height)+1);

}
//-----------------------------------------------------------------------------
void  FEATURE_CALCULATE_ORIENTATION_RELATIVE_START_ROLL (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
   DEFINED_PROPERTY_TYPE depth;

	
   depth=pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_FRONT).get()-pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_BACK).get()+1;				
   
   v=(DEFINED_FEATURE_TYPE)((pl.look_up(PROPERTY_ORIENTATION_START_ROLL).get()-pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_BACK).get())*98/(depth)+1);

}
//-----------------------------------------------------------------------------
void  FEATURE_CALCULATE_ORIENTATION_RELATIVE_END_AZIM (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
   DEFINED_PROPERTY_TYPE width;
			
   width=pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_RIGHT).get()-pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_LEFT).get()+1;	

   v=(DEFINED_FEATURE_TYPE)((pl.look_up(PROPERTY_ORIENTATION_END_AZIM).get()-pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_LEFT).get())*98/(width)+1);

}
//-----------------------------------------------------------------------------
void  FEATURE_CALCULATE_ORIENTATION_RELATIVE_END_ELEV (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
   
   DEFINED_PROPERTY_TYPE height;

				
   height=pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_TOP).get()-pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_BOTTOM).get()+1;			
   
   v=(DEFINED_FEATURE_TYPE)((pl.look_up(PROPERTY_ORIENTATION_END_ELEV).get()-pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_BOTTOM).get())*98/(height)+1);

}
//-----------------------------------------------------------------------------
void  FEATURE_CALCULATE_ORIENTATION_RELATIVE_END_ROLL (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
   DEFINED_PROPERTY_TYPE depth;

	
   depth=pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_FRONT).get()-pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_BACK).get()+1;				
   
   v=(DEFINED_FEATURE_TYPE)((pl.look_up(PROPERTY_ORIENTATION_END_ROLL).get()-pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_BACK).get())*98/(depth)+1);

}
//-----------------------------------------------------------------------------
void  FEATURE_CALCULATE_ORIENTATION_AZIMUTHAL_MOVEMENTS (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
   DEFINED_PROPERTY_TYPE width;
			
   width=pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_RIGHT).get()-pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_LEFT).get()+1;	
   
   v=(DEFINED_FEATURE_TYPE)(width*98/pl.look_up(PROPERTY_ORIENTATION_AZIMUTHAL_MOVEMENTS).get()+1);
}
//-----------------------------------------------------------------------------
void  FEATURE_CALCULATE_ORIENTATION_ELEVATION_MOVEMENTS (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
   DEFINED_PROPERTY_TYPE height;

				
   height=pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_TOP).get()-pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_BOTTOM).get()+1;

   v=(DEFINED_FEATURE_TYPE)(height*98/pl.look_up(PROPERTY_ORIENTATION_ELEVATION_MOVEMENTS).get()+1);
}
//-----------------------------------------------------------------------------
void  FEATURE_CALCULATE_ORIENTATION_ROLL_MOVEMENTS (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
   DEFINED_PROPERTY_TYPE depth;

	
   depth=pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_FRONT).get()-pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_BACK).get()+1;

   v=(DEFINED_FEATURE_TYPE)(depth*98/pl.look_up(PROPERTY_ORIENTATION_ROLL_MOVEMENTS).get()+1);
}

//-----------------------------------------------------------------------------
void  FEATURE_CALCULATE_ORIENTATION_AZIMUTHAL_MOVEMENTS_R45 (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
   DEFINED_PROPERTY_TYPE rwidth;
			
   rwidth=pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_RIGHT_R45).get()-pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_LEFT_R45).get()+1;	

   v=(DEFINED_FEATURE_TYPE)(rwidth*98/pl.look_up(PROPERTY_ORIENTATION_AZIMUTHAL_MOVEMENTS_R45).get()+1);
}
//-----------------------------------------------------------------------------
void  FEATURE_CALCULATE_ORIENTATION_ELEVATION_MOVEMENTS_R45 (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
   DEFINED_PROPERTY_TYPE rheight;

				
   rheight=pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_TOP_R45).get()-pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_BOTTOM_R45).get()+1;

   v=(DEFINED_FEATURE_TYPE)(rheight*98/pl.look_up(PROPERTY_ORIENTATION_ELEVATION_MOVEMENTS_R45).get()+1);
}
//-----------------------------------------------------------------------------
void  FEATURE_CALCULATE_ORIENTATION_ROLL_MOVEMENTS_R45 (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
   DEFINED_PROPERTY_TYPE rdepth;

	
   rdepth=pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_FRONT_R45).get()-pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_BACK_R45).get()+1;
   
   v=(DEFINED_FEATURE_TYPE)(rdepth*98/pl.look_up(PROPERTY_ORIENTATION_ROLL_MOVEMENTS_R45).get()+1);
}
//-----------------------------------------------------------------------------
void  FEATURE_CALCULATE_ORIENTATION_START_ANGLE_AZIM_ELEV (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
   v=(DEFINED_FEATURE_TYPE)angle(pl.look_up(PROPERTY_ORIENTATION_ANGLE_COEFF_SAAZIM).get(),pl.look_up(PROPERTY_ORIENTATION_ANGLE_COEFF_SAELEV).get());
}
//-----------------------------------------------------------------------------
void  FEATURE_CALCULATE_ORIENTATION_START_ANGLE_AZIM_ROLL (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
   v=(DEFINED_FEATURE_TYPE)angle(pl.look_up(PROPERTY_ORIENTATION_ANGLE_COEFF_SAAZIM).get(),pl.look_up(PROPERTY_ORIENTATION_ANGLE_COEFF_SAROLL).get());
}
//-----------------------------------------------------------------------------
void  FEATURE_CALCULATE_ORIENTATION_START_ANGLE_ROLL_ELEV (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
   v=(DEFINED_FEATURE_TYPE)angle(pl.look_up(PROPERTY_ORIENTATION_ANGLE_COEFF_SAROLL).get(),pl.look_up(PROPERTY_ORIENTATION_ANGLE_COEFF_SAELEV).get());
}
//-----------------------------------------------------------------------------
void  FEATURE_CALCULATE_ORIENTATION_END_ANGLE_AZIM_ELEV (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
   v=(DEFINED_FEATURE_TYPE)angle(pl.look_up(PROPERTY_ORIENTATION_ANGLE_COEFF_EAAZIM).get(),pl.look_up(PROPERTY_ORIENTATION_ANGLE_COEFF_EAELEV).get());
}
//-----------------------------------------------------------------------------
void  FEATURE_CALCULATE_ORIENTATION_END_ANGLE_AZIM_ROLL (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
   v=(DEFINED_FEATURE_TYPE)angle(pl.look_up(PROPERTY_ORIENTATION_ANGLE_COEFF_EAAZIM).get(),pl.look_up(PROPERTY_ORIENTATION_ANGLE_COEFF_EAROLL).get());
}
//-----------------------------------------------------------------------------
void  FEATURE_CALCULATE_ORIENTATION_END_ANGLE_ROLL_ELEV (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
   v=(DEFINED_FEATURE_TYPE)angle(pl.look_up(PROPERTY_ORIENTATION_ANGLE_COEFF_EAROLL).get(),pl.look_up(PROPERTY_ORIENTATION_ANGLE_COEFF_EAELEV).get());
}
//-----------------------------------------------------------------------------
void  FEATURE_CALCULATE_ORIENTATION_RELATIVE_CG_AZIM (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
   DEFINED_PROPERTY_TYPE width;
   double cgazim;

   cgazim=(DEFINED_PROPERTY_TYPE)((double)pl.look_up(PROPERTY_ORIENTATION_SUM_AZIM).get()/(double)pl.look_up(PROPERTY_NUMBER_OF_SAMPLES).get());	
   width=pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_RIGHT).get()-pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_LEFT).get()+1;

   v=(DEFINED_FEATURE_TYPE)((cgazim-pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_LEFT).get())*98/(width)+1);
}
//-----------------------------------------------------------------------------
void  FEATURE_CALCULATE_ORIENTATION_RELATIVE_CG_ELEV (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
   DEFINED_PROPERTY_TYPE height;
   double cgelev;

   cgelev=(DEFINED_PROPERTY_TYPE)((double)pl.look_up(PROPERTY_ORIENTATION_SUM_ELEV).get()/(double)pl.look_up(PROPERTY_NUMBER_OF_SAMPLES).get());	
   height=pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_TOP).get()-pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_BOTTOM).get()+1;

   v=(DEFINED_FEATURE_TYPE)((cgelev-pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_BOTTOM).get())*98/(height)+1);
}
//-----------------------------------------------------------------------------
void  FEATURE_CALCULATE_ORIENTATION_RELATIVE_CG_ROLL (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
   DEFINED_PROPERTY_TYPE depth;
   double cgroll;

   cgroll=(DEFINED_PROPERTY_TYPE)((double)pl.look_up(PROPERTY_ORIENTATION_SUM_ROLL).get()/(double)pl.look_up(PROPERTY_NUMBER_OF_SAMPLES).get());	
   depth=pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_FRONT).get()-pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_BACK).get()+1;				

   v=(DEFINED_FEATURE_TYPE)((cgroll-pl.look_up(PROPERTY_ORIENTATION_BOUNDING_BOX_BACK).get())*98/(depth)+1);
}
//-----------------------------------------------------------------------------

// initialization section
//-----------------------------------------------------------------------------
void  FEATURE_INITIALIZE_STRAIGHTNESS (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
	v=(DEFINED_FEATURE_TYPE)0;
}
//-----------------------------------------------------------------------------
void  FEATURE_INITIALIZE_HEIGHT_WIDTH_RATIO (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
	v=(DEFINED_FEATURE_TYPE)0;
}
//-----------------------------------------------------------------------------
void  FEATURE_INITIALIZE_HEIGHT_DEPTH_RATIO (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
	v=(DEFINED_FEATURE_TYPE)0;
}
//-----------------------------------------------------------------------------
void  FEATURE_INITIALIZE_DEPTH_WIDTH_RATIO (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
	v=(DEFINED_FEATURE_TYPE)0;
}
//-----------------------------------------------------------------------------
void  FEATURE_INITIALIZE_HEIGHT_WIDTH_RATIO_R45 (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
	v=(DEFINED_FEATURE_TYPE)0;
}
//-----------------------------------------------------------------------------
void  FEATURE_INITIALIZE_HEIGHT_DEPTH_RATIO_R45 (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
	v=(DEFINED_FEATURE_TYPE)0;
}
//-----------------------------------------------------------------------------
void  FEATURE_INITIALIZE_DEPTH_WIDTH_RATIO_R45 (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
	v=(DEFINED_FEATURE_TYPE)0;
}
//-----------------------------------------------------------------------------
void  FEATURE_INITIALIZE_RELATIVE_START_X (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
	v=(DEFINED_FEATURE_TYPE)0;
}
//-----------------------------------------------------------------------------
void  FEATURE_INITIALIZE_RELATIVE_START_Y (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
	v=(DEFINED_FEATURE_TYPE)0;
}
//-----------------------------------------------------------------------------
void  FEATURE_INITIALIZE_RELATIVE_START_Z (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
	v=(DEFINED_FEATURE_TYPE)0;
}
//-----------------------------------------------------------------------------
void  FEATURE_INITIALIZE_RELATIVE_END_X (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
	v=(DEFINED_FEATURE_TYPE)0;
}
//-----------------------------------------------------------------------------
void  FEATURE_INITIALIZE_RELATIVE_END_Y (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
	v=(DEFINED_FEATURE_TYPE)0;
}
//-----------------------------------------------------------------------------
void  FEATURE_INITIALIZE_RELATIVE_END_Z (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
	v=(DEFINED_FEATURE_TYPE)0;
}
//-----------------------------------------------------------------------------
void  FEATURE_INITIALIZE_HORIZONTAL_MOVEMENTS (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
	v=(DEFINED_FEATURE_TYPE)0;
}
//-----------------------------------------------------------------------------
void  FEATURE_INITIALIZE_VERTICAL_MOVEMENTS (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
	v=(DEFINED_FEATURE_TYPE)0;
}
//-----------------------------------------------------------------------------
void  FEATURE_INITIALIZE_DEPTH_MOVEMENTS (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
	v=(DEFINED_FEATURE_TYPE)0;
}
//-----------------------------------------------------------------------------
void  FEATURE_INITIALIZE_HORIZONTAL_MOVEMENTS_R45 (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
	v=(DEFINED_FEATURE_TYPE)0;
}
//-----------------------------------------------------------------------------
void  FEATURE_INITIALIZE_VERTICAL_MOVEMENTS_R45 (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
	v=(DEFINED_FEATURE_TYPE)0;
}
//-----------------------------------------------------------------------------
void  FEATURE_INITIALIZE_DEPTH_MOVEMENTS_R45 (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
	v=(DEFINED_FEATURE_TYPE)0;
}
//-----------------------------------------------------------------------------
void  FEATURE_INITIALIZE_START_ANGLE_X_Y (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
	v=(DEFINED_FEATURE_TYPE)0;
}
//-----------------------------------------------------------------------------
void  FEATURE_INITIALIZE_START_ANGLE_X_Z (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
	v=(DEFINED_FEATURE_TYPE)0;
}
//-----------------------------------------------------------------------------
void  FEATURE_INITIALIZE_START_ANGLE_Z_Y (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
	v=(DEFINED_FEATURE_TYPE)0;
}
//-----------------------------------------------------------------------------
void  FEATURE_INITIALIZE_END_ANGLE_X_Y (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
	v=(DEFINED_FEATURE_TYPE)0;
}
//-----------------------------------------------------------------------------
void  FEATURE_INITIALIZE_END_ANGLE_X_Z (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
	v=(DEFINED_FEATURE_TYPE)0;
}
//-----------------------------------------------------------------------------
void  FEATURE_INITIALIZE_END_ANGLE_Z_Y (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
	v=(DEFINED_FEATURE_TYPE)0;
}
//-----------------------------------------------------------------------------
void  FEATURE_INITIALIZE_RELATIVE_CG_X (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
	v=(DEFINED_FEATURE_TYPE)0;
}
//-----------------------------------------------------------------------------
void  FEATURE_INITIALIZE_RELATIVE_CG_Y (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
	v=(DEFINED_FEATURE_TYPE)0;
}
//-----------------------------------------------------------------------------
void  FEATURE_INITIALIZE_RELATIVE_CG_Z (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
	v=(DEFINED_FEATURE_TYPE)0;
}
//-----------------------------------------------------------------------------
void  FEATURE_INITIALIZE_ORIENTATION_STRAIGHTNESS (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
	v=(DEFINED_FEATURE_TYPE)0;
}
//-----------------------------------------------------------------------------
void  FEATURE_INITIALIZE_ORIENTATION_HEIGHT_WIDTH_RATIO (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
	v=(DEFINED_FEATURE_TYPE)0;
}
//-----------------------------------------------------------------------------
void  FEATURE_INITIALIZE_ORIENTATION_HEIGHT_DEPTH_RATIO (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
	v=(DEFINED_FEATURE_TYPE)0;
}
//-----------------------------------------------------------------------------
void  FEATURE_INITIALIZE_ORIENTATION_DEPTH_WIDTH_RATIO (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
	v=(DEFINED_FEATURE_TYPE)0;
}
//-----------------------------------------------------------------------------
void  FEATURE_INITIALIZE_ORIENTATION_HEIGHT_WIDTH_RATIO_R45 (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
	v=(DEFINED_FEATURE_TYPE)0;
}
//-----------------------------------------------------------------------------
void  FEATURE_INITIALIZE_ORIENTATION_HEIGHT_DEPTH_RATIO_R45 (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
	v=(DEFINED_FEATURE_TYPE)0;
}
//-----------------------------------------------------------------------------
void  FEATURE_INITIALIZE_ORIENTATION_DEPTH_WIDTH_RATIO_R45 (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
	v=(DEFINED_FEATURE_TYPE)0;
}
//-----------------------------------------------------------------------------
void  FEATURE_INITIALIZE_ORIENTATION_RELATIVE_START_AZIM (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
	v=(DEFINED_FEATURE_TYPE)0;
}
//-----------------------------------------------------------------------------
void  FEATURE_INITIALIZE_ORIENTATION_RELATIVE_START_ELEV (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
	v=(DEFINED_FEATURE_TYPE)0;
}
//-----------------------------------------------------------------------------
void  FEATURE_INITIALIZE_ORIENTATION_RELATIVE_START_ROLL (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
	v=(DEFINED_FEATURE_TYPE)0;
}
//-----------------------------------------------------------------------------
void  FEATURE_INITIALIZE_ORIENTATION_RELATIVE_END_AZIM (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
	v=(DEFINED_FEATURE_TYPE)0;
}
//-----------------------------------------------------------------------------
void  FEATURE_INITIALIZE_ORIENTATION_RELATIVE_END_ELEV (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
	v=(DEFINED_FEATURE_TYPE)0;
}
//-----------------------------------------------------------------------------
void  FEATURE_INITIALIZE_ORIENTATION_RELATIVE_END_ROLL (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
	v=(DEFINED_FEATURE_TYPE)0;
}
//-----------------------------------------------------------------------------
void  FEATURE_INITIALIZE_ORIENTATION_AZIMUTHAL_MOVEMENTS (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
	v=(DEFINED_FEATURE_TYPE)0;
}
//-----------------------------------------------------------------------------
void  FEATURE_INITIALIZE_ORIENTATION_ELEVATION_MOVEMENTS (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
	v=(DEFINED_FEATURE_TYPE)0;
}
//-----------------------------------------------------------------------------
void  FEATURE_INITIALIZE_ORIENTATION_ROLL_MOVEMENTS (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
	v=(DEFINED_FEATURE_TYPE)0;
}
//-----------------------------------------------------------------------------
void  FEATURE_INITIALIZE_ORIENTATION_AZIMUTHAL_MOVEMENTS_R45 (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
	v=(DEFINED_FEATURE_TYPE)0;
}
//-----------------------------------------------------------------------------
void  FEATURE_INITIALIZE_ORIENTATION_ELEVATION_MOVEMENTS_R45 (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
	v=(DEFINED_FEATURE_TYPE)0;
}
//-----------------------------------------------------------------------------
void  FEATURE_INITIALIZE_ORIENTATION_ROLL_MOVEMENTS_R45 (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
	v=(DEFINED_FEATURE_TYPE)0;
}
//-----------------------------------------------------------------------------
void  FEATURE_INITIALIZE_ORIENTATION_START_ANGLE_AZIM_ELEV (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
	v=(DEFINED_FEATURE_TYPE)0;
}
//-----------------------------------------------------------------------------
void  FEATURE_INITIALIZE_ORIENTATION_START_ANGLE_AZIM_ROLL (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
	v=(DEFINED_FEATURE_TYPE)0;
}
//-----------------------------------------------------------------------------
void  FEATURE_INITIALIZE_ORIENTATION_START_ANGLE_ROLL_ELEV (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
	v=(DEFINED_FEATURE_TYPE)0;
}
//-----------------------------------------------------------------------------
void  FEATURE_INITIALIZE_ORIENTATION_END_ANGLE_AZIM_ELEV (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
	v=(DEFINED_FEATURE_TYPE)0;
}
//-----------------------------------------------------------------------------
void  FEATURE_INITIALIZE_ORIENTATION_END_ANGLE_AZIM_ROLL (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
	v=(DEFINED_FEATURE_TYPE)0;
}
//-----------------------------------------------------------------------------
void  FEATURE_INITIALIZE_ORIENTATION_END_ANGLE_ROLL_ELEV (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
	v=(DEFINED_FEATURE_TYPE)0;
}
//-----------------------------------------------------------------------------
void  FEATURE_INITIALIZE_ORIENTATION_RELATIVE_CG_AZIM (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
	v=(DEFINED_FEATURE_TYPE)0;
}
//-----------------------------------------------------------------------------
void  FEATURE_INITIALIZE_ORIENTATION_RELATIVE_CG_ELEV (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
	v=(DEFINED_FEATURE_TYPE)0;
}
//-----------------------------------------------------------------------------
void  FEATURE_INITIALIZE_ORIENTATION_RELATIVE_CG_ROLL (DEFINED_FEATURE_TYPE &v,DEFINED_PROPERTY_LIST_TYPE pl)
{
	v=(DEFINED_FEATURE_TYPE)0;
}
//-----------------------------------------------------------------------------
#endif

