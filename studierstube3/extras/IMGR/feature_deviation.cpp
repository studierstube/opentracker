#include "feature_deviation.h"


template <class FEATURE_DEVIATION_TYPE,class FEATURE_TYPE, class PROPERTY_LIST_TYPE>
feature_deviation<FEATURE_DEVIATION_TYPE,FEATURE_TYPE,PROPERTY_LIST_TYPE>::feature_deviation(unsigned int i,FEATURE_DEVIATION_FUNCTION df)
:id(i), 
calculate_function(df), 
value(0), 
dependence_level(0)
{
}

template <class FEATURE_DEVIATION_TYPE,class FEATURE_TYPE, class PROPERTY_LIST_TYPE>
feature_deviation<FEATURE_DEVIATION_TYPE,FEATURE_TYPE,PROPERTY_LIST_TYPE>::feature_deviation()
:id(0), calculate_function(NULL), value(0), dependence_level(0)
{
}

template <class FEATURE_DEVIATION_TYPE,class FEATURE_TYPE, class PROPERTY_LIST_TYPE>
feature_deviation<FEATURE_DEVIATION_TYPE,FEATURE_TYPE,PROPERTY_LIST_TYPE>::~feature_deviation()
{
}


template <class FEATURE_DEVIATION_TYPE,class FEATURE_TYPE, class PROPERTY_LIST_TYPE>
FEATURE_DEVIATION_TYPE feature_deviation<FEATURE_DEVIATION_TYPE,FEATURE_TYPE,PROPERTY_LIST_TYPE>::get(void)
{
	return(value);
}

template <class FEATURE_DEVIATION_TYPE,class FEATURE_TYPE, class PROPERTY_LIST_TYPE>
unsigned int feature_deviation<FEATURE_DEVIATION_TYPE,FEATURE_TYPE,PROPERTY_LIST_TYPE>::identification(void)
{
	return(id);
}

template <class FEATURE_DEVIATION_TYPE,class FEATURE_TYPE, class PROPERTY_LIST_TYPE>
void feature_deviation<FEATURE_DEVIATION_TYPE,FEATURE_TYPE,PROPERTY_LIST_TYPE>::calculate (feature<FEATURE_TYPE,PROPERTY_LIST_TYPE> a,feature<FEATURE_TYPE,PROPERTY_LIST_TYPE> b)
{
	if (calculate_function!=NULL)
		value=calculate_function(a,b);
}

template <class FEATURE_DEVIATION_TYPE,class FEATURE_TYPE, class PROPERTY_LIST_TYPE>
void feature_deviation<FEATURE_DEVIATION_TYPE,FEATURE_TYPE,PROPERTY_LIST_TYPE>::define(unsigned int i,FEATURE_DEVIATION_FUNCTION df)
{
	id=i;
	calculate_function=df;
}

template <class FEATURE_DEVIATION_TYPE,class FEATURE_TYPE, class PROPERTY_LIST_TYPE>
void feature_deviation<FEATURE_DEVIATION_TYPE,FEATURE_TYPE,PROPERTY_LIST_TYPE>::set_dependence_level(unsigned int dl)
{
	dependence_level=dl;
}

template <class FEATURE_DEVIATION_TYPE,class FEATURE_TYPE, class PROPERTY_LIST_TYPE>
unsigned int feature_deviation<FEATURE_DEVIATION_TYPE,FEATURE_TYPE,PROPERTY_LIST_TYPE>::get_dependence_level(void)
{
	return(dependence_level);
}

