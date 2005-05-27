#include "weight.h"

template <class WEIGHT_TYPE,class FEATURE_LIST_TYPE>
weight<WEIGHT_TYPE,FEATURE_LIST_TYPE>::weight(unsigned int i,WEIGHT_FUNCTION cf,WEIGHT_FUNCTION inf , WEIGHT_TYPE v)
:id(i), calculate_function(cf), initialize_function (inf),dependence_level(0)
{
	content.value=v;
}

template <class WEIGHT_TYPE,class FEATURE_LIST_TYPE>
weight<WEIGHT_TYPE,FEATURE_LIST_TYPE>::weight(unsigned int i,WEIGHT_FUNCTION cf,WEIGHT_FUNCTION inf , WEIGHT_TYPE divid, WEIGHT_TYPE divis)
:id(i), calculate_function(cf), initialize_function (inf),dependence_level(0)
{
	content.component.divident=divid;
	content.component.divisor=divis;
}

template <class WEIGHT_TYPE,class FEATURE_LIST_TYPE>
weight<WEIGHT_TYPE,FEATURE_LIST_TYPE>::weight()
:id(0), calculate_function(NULL), initialize_function(NULL),dependence_level(0)
{
	content.value=0;
}

template <class WEIGHT_TYPE,class FEATURE_LIST_TYPE>
weight<WEIGHT_TYPE,FEATURE_LIST_TYPE>::~weight()
{
}

template <class WEIGHT_TYPE,class FEATURE_LIST_TYPE>
void weight<WEIGHT_TYPE,FEATURE_LIST_TYPE>::set(WEIGHT_TYPE v)
{
	content.value=v;
}

template <class WEIGHT_TYPE,class FEATURE_LIST_TYPE>
void weight<WEIGHT_TYPE,FEATURE_LIST_TYPE>::set(WEIGHT_TYPE divid, WEIGHT_TYPE divis)
{
	content.component.divident=divid;
	content.component.divisor=divis;
}

template <class WEIGHT_TYPE,class FEATURE_LIST_TYPE>
WEIGHT_TYPE weight<WEIGHT_TYPE,FEATURE_LIST_TYPE>::get_value(void)
{
	 return(content.value);
}

template <class WEIGHT_TYPE,class FEATURE_LIST_TYPE>
WEIGHT_TYPE weight<WEIGHT_TYPE,FEATURE_LIST_TYPE>::get_divident(void)
{
	 return(content.component.divident);
}

template <class WEIGHT_TYPE,class FEATURE_LIST_TYPE>
WEIGHT_TYPE weight<WEIGHT_TYPE,FEATURE_LIST_TYPE>::get_divisor(void)
{
	 return(content.component.divisor);
}

template <class WEIGHT_TYPE,class FEATURE_LIST_TYPE>
unsigned int weight<WEIGHT_TYPE,FEATURE_LIST_TYPE>::identification(void)
{
	return(id);
}


template <class WEIGHT_TYPE,class FEATURE_LIST_TYPE>
void weight<WEIGHT_TYPE,FEATURE_LIST_TYPE>::calculate (FEATURE_LIST_TYPE fl)
{
	if (calculate_function!=NULL)
		calculate_function(content,fl);
}

template <class WEIGHT_TYPE,class FEATURE_LIST_TYPE>
void weight<WEIGHT_TYPE,FEATURE_LIST_TYPE>::define(unsigned int i,WEIGHT_FUNCTION cf,WEIGHT_FUNCTION inf )
{
	id=i;
	calculate_function=cf;
	initialize_function=inf;
}

template <class WEIGHT_TYPE,class FEATURE_LIST_TYPE>
void weight<WEIGHT_TYPE,FEATURE_LIST_TYPE>::initialize (FEATURE_LIST_TYPE fl)
{
	if (initialize_function!=NULL)
		initialize_function(content,fl);
}

template <class WEIGHT_TYPE,class FEATURE_LIST_TYPE>
unsigned int weight<WEIGHT_TYPE,FEATURE_LIST_TYPE>::get_dependence_level(void)
{
	return(dependence_level);
}

template <class WEIGHT_TYPE,class FEATURE_LIST_TYPE>
void weight<WEIGHT_TYPE,FEATURE_LIST_TYPE>::set_dependence_level(unsigned int dl)
{
	dependence_level=dl;
}
