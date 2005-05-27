#include "fuzzy_logic_recognizer.h"

template<class PROPERTY_LIST_TYPE,class FEATURE_TYPE,class FEATURE_LIST_TYPE>
fuzzy_logic_recognizer<PROPERTY_LIST_TYPE, FEATURE_TYPE, FEATURE_LIST_TYPE>::fuzzy_logic_recognizer()
{
	build_weight_list(wl);
	build_feature_deviation_list(fdl);
}

template<class PROPERTY_LIST_TYPE,class FEATURE_TYPE,class FEATURE_LIST_TYPE>
fuzzy_logic_recognizer<PROPERTY_LIST_TYPE, FEATURE_TYPE, FEATURE_LIST_TYPE>::~fuzzy_logic_recognizer()
{
}

template<class PROPERTY_LIST_TYPE,class FEATURE_TYPE,class FEATURE_LIST_TYPE>
unsigned int fuzzy_logic_recognizer<PROPERTY_LIST_TYPE, FEATURE_TYPE, FEATURE_LIST_TYPE>::recognize(FEATURE_LIST_TYPE input_features)
{
	unsigned int closest_neighbour,fid,gid;
	FEATURE_LIST_TYPE fl;
	bool more;
	DEFINED_FEATURE_DEVIATION_TYPE deviation,smallest_deviation;
	double SOW;

	wl.initialize(input_features);
	wl.calculate(input_features);
    
	smallest_deviation=(DEFINED_FEATURE_DEVIATION_TYPE)MAX_DEVIATION;
    more=data.get_first(gid,fl);
	closest_neighbour=NOTHING_RECOGNIZED;
	while(more==true)//over all learned gestures
	 {	  
	  fdl.calculate(input_features,fl);// single feature deviations : input gesture - current learned gesture  
	  deviation=(DEFINED_FEATURE_DEVIATION_TYPE)0.0;
	  SOW=0.0;//sum of weights for total deviation normalization
	  for(fid=0;fid<fl.get_dim();fid++) //over all features
	  {
               // cout << input_features.look_up(fid).get() << " ";	
    	        deviation+=(DEFINED_FEATURE_DEVIATION_TYPE)pow((fdl.look_up(fid).get() * wl.look_up(fid).get_divident() / wl.look_up(fid).get_divisor()),2.0);
		SOW+=pow((double)wl.look_up(fid).get_divident()/(double)wl.look_up(fid).get_divisor(),2.0); //sum of weights for total deviation normalization 
	  }
          //cout << "\n";
	  if ((deviation<smallest_deviation)&&(sqrt((double)deviation/SOW)<=DEVIATION_TRESHOLD))
	  {smallest_deviation=deviation;closest_neighbour=gid;}
	  more=data.get_next(gid,fl); 
	}

	return closest_neighbour;
}

template<class PROPERTY_LIST_TYPE,class FEATURE_TYPE,class FEATURE_LIST_TYPE>
void fuzzy_logic_recognizer<PROPERTY_LIST_TYPE, FEATURE_TYPE, FEATURE_LIST_TYPE>::learn(unsigned int id,FEATURE_LIST_TYPE input_features)
{
 data.add(id,input_features);
}

