#include "feature.h"


template <class FEATURE_TYPE, class PROPERTY_LIST_TYPE>
feature<FEATURE_TYPE,PROPERTY_LIST_TYPE>::feature(unsigned int i,FEATURE_FUNCTION cf,FEATURE_FUNCTION inf , FEATURE_TYPE v)
:id(i), calculate_function(cf), initialize_function (inf), value(v), dependence_level(0)
{
}

template <class FEATURE_TYPE, class PROPERTY_LIST_TYPE>
feature<FEATURE_TYPE,PROPERTY_LIST_TYPE>::feature()
:id(0), calculate_function(NULL), initialize_function(NULL), value(0), dependence_level(0)
{
}

template <class FEATURE_TYPE, class PROPERTY_LIST_TYPE>
feature<FEATURE_TYPE,PROPERTY_LIST_TYPE>::~feature()
{
}

template <class FEATURE_TYPE, class PROPERTY_LIST_TYPE>
void feature<FEATURE_TYPE,PROPERTY_LIST_TYPE>::set(FEATURE_TYPE v)
{
	value=v;
}

template <class FEATURE_TYPE, class PROPERTY_LIST_TYPE>
void feature<FEATURE_TYPE,PROPERTY_LIST_TYPE>::set(unsigned int i,FEATURE_TYPE v)
{
	value=v;
	id=i;
}

template <class FEATURE_TYPE, class PROPERTY_LIST_TYPE>
FEATURE_TYPE feature<FEATURE_TYPE,PROPERTY_LIST_TYPE>::get(void)
{
	return(value);
}

template <class FEATURE_TYPE, class PROPERTY_LIST_TYPE>
unsigned int feature<FEATURE_TYPE,PROPERTY_LIST_TYPE>::identification(void)
{
	return(id);
}

template <class FEATURE_TYPE, class PROPERTY_LIST_TYPE>
void feature<FEATURE_TYPE,PROPERTY_LIST_TYPE>::calculate (PROPERTY_LIST_TYPE pl)
{
	if (calculate_function!=NULL)
		calculate_function(value,pl);
}

template <class FEATURE_TYPE, class PROPERTY_LIST_TYPE>
void feature<FEATURE_TYPE,PROPERTY_LIST_TYPE>::define(unsigned int i,FEATURE_FUNCTION cf,FEATURE_FUNCTION inf )
{
	id=i;
	calculate_function=cf;
	initialize_function=inf;
}

template <class FEATURE_TYPE, class PROPERTY_LIST_TYPE>
void feature<FEATURE_TYPE,PROPERTY_LIST_TYPE>::initialize (PROPERTY_LIST_TYPE pl)
{
	if (initialize_function!=NULL)
		initialize_function(value,pl);
}

template <class FEATURE_TYPE, class PROPERTY_LIST_TYPE>
void feature<FEATURE_TYPE,PROPERTY_LIST_TYPE>::set_dependence_level(unsigned int dl)
{
	dependence_level=dl;
}

template <class FEATURE_TYPE, class PROPERTY_LIST_TYPE>
unsigned int feature<FEATURE_TYPE,PROPERTY_LIST_TYPE>::get_dependence_level(void)
{
	return(dependence_level);
}
