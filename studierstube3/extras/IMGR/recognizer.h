#ifndef RECOGNIZER_INCLUDED
#define RECOGNIZER_INCLUDED

#include "feature_list.h"
#include "feature_space.h"

#define NOTHING_RECOGNIZED 9999999

template <class PROPERTY_LIST_TYPE,class FEATURE_TYPE,class FEATURE_LIST_TYPE>
class recognizer
{
public :
	
	virtual unsigned int recognize(FEATURE_LIST_TYPE input_features)=0;
	virtual void learn(unsigned int id, FEATURE_LIST_TYPE input_features)=0;
	void set_feature_space(feature_space<PROPERTY_LIST_TYPE,FEATURE_TYPE,FEATURE_LIST_TYPE> d)
	{data=d;}
	void get_feature_space(feature_space<PROPERTY_LIST_TYPE,FEATURE_TYPE,FEATURE_LIST_TYPE> &d)
	{d=data;}

protected :
    
	feature_space<PROPERTY_LIST_TYPE,FEATURE_TYPE,FEATURE_LIST_TYPE> data;	

};


#endif

