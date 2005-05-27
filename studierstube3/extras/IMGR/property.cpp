#include "property.h"


template <class PROPERTY_TYPE, class SAMPLE_TYPE>
property<PROPERTY_TYPE,SAMPLE_TYPE>::property(unsigned int i,PROP_FUNCTION uf,PROP_FUNCTION inf , PROPERTY_TYPE v)
:id(i), update_function(uf), initialize_function (inf), value(v), dependence_level(0)
{
}

template <class PROPERTY_TYPE, class SAMPLE_TYPE>
property<PROPERTY_TYPE,SAMPLE_TYPE>::property()
:id(0), update_function(NULL), initialize_function(NULL), value(0), dependence_level(0)
{
}

template <class PROPERTY_TYPE, class SAMPLE_TYPE>
property<PROPERTY_TYPE,SAMPLE_TYPE>::~property()
{
}

template <class PROPERTY_TYPE, class SAMPLE_TYPE>
void property<PROPERTY_TYPE,SAMPLE_TYPE>::set(PROPERTY_TYPE v)
{
	value=v;
}

template <class PROPERTY_TYPE, class SAMPLE_TYPE>
void property<PROPERTY_TYPE,SAMPLE_TYPE>::set(unsigned int i,PROPERTY_TYPE v)
{
	value=v;
	id=i;
}

template <class PROPERTY_TYPE, class SAMPLE_TYPE>
PROPERTY_TYPE property<PROPERTY_TYPE,SAMPLE_TYPE>::get(void)
{
	return(value);
}

template <class PROPERTY_TYPE, class SAMPLE_TYPE>
unsigned int property<PROPERTY_TYPE,SAMPLE_TYPE>::get_dependence_level(void)
{
	return(dependence_level);
}

template <class PROPERTY_TYPE, class SAMPLE_TYPE>
void property<PROPERTY_TYPE,SAMPLE_TYPE>::set_dependence_level(unsigned int dl)
{
	dependence_level=dl;
}

template <class PROPERTY_TYPE, class SAMPLE_TYPE>
unsigned int property<PROPERTY_TYPE,SAMPLE_TYPE>::identification(void)
{
	return(id);
}


template <class PROPERTY_TYPE, class SAMPLE_TYPE>
void property<PROPERTY_TYPE,SAMPLE_TYPE>::update (SAMPLE_TYPE s)
{
	if (update_function!=NULL)
		update_function(value,s);
}

template <class PROPERTY_TYPE, class SAMPLE_TYPE>
void property<PROPERTY_TYPE,SAMPLE_TYPE>::define(unsigned int i,PROP_FUNCTION uf,PROP_FUNCTION inf )
{
	id=i;
	update_function=uf;
	initialize_function=inf;
}

template <class PROPERTY_TYPE, class SAMPLE_TYPE>
void property<PROPERTY_TYPE,SAMPLE_TYPE>::initialize (SAMPLE_TYPE s)
{
	if (initialize_function!=NULL)
		initialize_function(value,s);
}

