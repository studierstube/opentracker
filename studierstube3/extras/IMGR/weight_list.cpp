#include "weight_list.h"

template <class WEIGHT_TYPE,class FEATURE_LIST_TYPE, unsigned int DIM>  
weight_list<WEIGHT_TYPE,FEATURE_LIST_TYPE,DIM>::weight_list()
:dependence_level(0)
{
	unsigned int i;

	for(i=0;i<DIM;i++)
		data[i]=weight<WEIGHT_TYPE,FEATURE_LIST_TYPE>();
}

template <class WEIGHT_TYPE,class FEATURE_LIST_TYPE, unsigned int DIM> 
weight_list<WEIGHT_TYPE,FEATURE_LIST_TYPE,DIM>::~weight_list()
{
}

template <class WEIGHT_TYPE,class FEATURE_LIST_TYPE, unsigned int DIM>  
void weight_list<WEIGHT_TYPE,FEATURE_LIST_TYPE,DIM>::set(unsigned int dim,weight<WEIGHT_TYPE,FEATURE_LIST_TYPE> d)
{
	if ((dim<DIM)&&(dim>=0))
	 data[dim]=d;
}

template <class WEIGHT_TYPE,class FEATURE_LIST_TYPE, unsigned int DIM> 
weight<WEIGHT_TYPE,FEATURE_LIST_TYPE> weight_list<WEIGHT_TYPE,FEATURE_LIST_TYPE,DIM>::get(unsigned int dim)
{
	if ((dim<DIM)&&(dim>=0))
		return(data[dim]);
	else
	{
	        weight<WEIGHT_TYPE,FEATURE_LIST_TYPE> tmp;
		return (tmp);
	}
}

template <class WEIGHT_TYPE,class FEATURE_LIST_TYPE, unsigned int DIM> 
weight<WEIGHT_TYPE,FEATURE_LIST_TYPE> weight_list<WEIGHT_TYPE,FEATURE_LIST_TYPE,DIM>::look_up(unsigned int id)
{
	int i;
    
	for (i=0;i<DIM;i++)
		if (data[i].identification()==id)
         return(data[i]);
	
	weight<WEIGHT_TYPE,FEATURE_LIST_TYPE> tmp;
	return (tmp);
}

template <class WEIGHT_TYPE,class FEATURE_LIST_TYPE, unsigned int DIM> 
void weight_list<WEIGHT_TYPE,FEATURE_LIST_TYPE,DIM>::calculate (FEATURE_LIST_TYPE fl)
{
	unsigned int i;

	for(i=0;i<DIM;i++)
		if (data[i].get_dependence_level()==this->get_dependence_level())
		 data[i].calculate(fl);
	
}

template <class WEIGHT_TYPE,class FEATURE_LIST_TYPE, unsigned int DIM> 
void weight_list<WEIGHT_TYPE,FEATURE_LIST_TYPE,DIM>::initialize (FEATURE_LIST_TYPE fl)
{
	unsigned int i;

	for(i=0;i<DIM;i++)
		if (data[i].get_dependence_level()==this->get_dependence_level())
		 data[i].initialize(fl);
	
}

template <class WEIGHT_TYPE,class FEATURE_LIST_TYPE, unsigned int DIM> 
void weight_list<WEIGHT_TYPE,FEATURE_LIST_TYPE,DIM>::set_dependence_level(unsigned int dl)
{
	dependence_level=dl;
}

template <class WEIGHT_TYPE,class FEATURE_LIST_TYPE, unsigned int DIM> 
unsigned int weight_list<WEIGHT_TYPE,FEATURE_LIST_TYPE,DIM>::get_dependence_level(void)
{
	return(dependence_level);
}
