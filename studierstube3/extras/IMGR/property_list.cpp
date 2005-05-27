#include "property_list.h"

template <class PROPERTY_TYPE, class SAMPLE_TYPE, unsigned int DIM> 
property_list<PROPERTY_TYPE,SAMPLE_TYPE,DIM>::property_list()
:dependence_level(0)
{
    unsigned int i;

	dim=DIM;

	for(i=0;i<DIM;i++)
		data[i]=property<PROPERTY_TYPE,SAMPLE_TYPE>();
}

template <class PROPERTY_TYPE, class SAMPLE_TYPE, unsigned int DIM> 
property_list<PROPERTY_TYPE,SAMPLE_TYPE,DIM>::~property_list()
{
}

template <class PROPERTY_TYPE, class SAMPLE_TYPE, unsigned int DIM> 
void property_list<PROPERTY_TYPE,SAMPLE_TYPE,DIM>::set(unsigned int dim,property<PROPERTY_TYPE,SAMPLE_TYPE> d)
{
	if ((dim<DIM)&&(dim>=0))
	 data[dim]=d;
}

template <class PROPERTY_TYPE, class SAMPLE_TYPE, unsigned int DIM>
property<PROPERTY_TYPE,SAMPLE_TYPE> property_list<PROPERTY_TYPE,SAMPLE_TYPE,DIM>::get(unsigned int dim)
{
	if ((dim<DIM)&&(dim>=0))
		return(data[dim]);
	else
	{
		property<PROPERTY_TYPE,SAMPLE_TYPE> tmp;
		return tmp;
	}
}

template <class PROPERTY_TYPE, class SAMPLE_TYPE, unsigned int DIM>
property<PROPERTY_TYPE,SAMPLE_TYPE> property_list<PROPERTY_TYPE,SAMPLE_TYPE,DIM>::look_up(unsigned int id)
{
	int i;
    
	for (i=0;i<DIM;i++)
		if (data[i].identification()==id)
         return(data[i]);
	
	property<PROPERTY_TYPE,SAMPLE_TYPE> tmp;
	return (tmp);
}

template <class PROPERTY_TYPE, class SAMPLE_TYPE, unsigned int DIM>
void property_list<PROPERTY_TYPE,SAMPLE_TYPE,DIM>::store_value_to(unsigned int id,PROPERTY_TYPE v)
{
    int i;
    
	for (i=0;i<DIM;i++)
		if (data[i].identification()==id)
         data[i].set(v);
}


template <class PROPERTY_TYPE, class SAMPLE_TYPE, unsigned int DIM>
void property_list<PROPERTY_TYPE,SAMPLE_TYPE,DIM>::update (SAMPLE_TYPE s)
{
	unsigned int i;

	for(i=0;i<DIM;i++)
		if(data[i].get_dependence_level()==this->get_dependence_level())
		 data[i].update(s);
}

template <class PROPERTY_TYPE, class SAMPLE_TYPE, unsigned int DIM>
void property_list<PROPERTY_TYPE,SAMPLE_TYPE,DIM>::initialize (SAMPLE_TYPE s)
{
	unsigned int i;

	for(i=0;i<DIM;i++)
		if(data[i].get_dependence_level()==this->get_dependence_level())
		 data[i].initialize(s);
	
}

template <class PROPERTY_TYPE, class SAMPLE_TYPE, unsigned int DIM>
unsigned int property_list<PROPERTY_TYPE,SAMPLE_TYPE,DIM>::get_dim(void)
{
	return(dim);
}

template <class PROPERTY_TYPE, class SAMPLE_TYPE, unsigned int DIM>
unsigned int property_list<PROPERTY_TYPE,SAMPLE_TYPE,DIM>::get_dependence_level(void)
{
	return(dependence_level);
}

template <class PROPERTY_TYPE, class SAMPLE_TYPE, unsigned int DIM>
void property_list<PROPERTY_TYPE,SAMPLE_TYPE,DIM>::set_dependence_level(unsigned int dl)
{
	dependence_level=dl;
}
