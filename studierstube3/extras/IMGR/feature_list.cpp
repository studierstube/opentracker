#include "feature_list.h"

template <class FEATURE_TYPE, class PROPERTY_LIST_TYPE, unsigned int DIM> 
feature_list<FEATURE_TYPE,PROPERTY_LIST_TYPE,DIM>::feature_list()
:dependence_level(0)
{
	unsigned int i;
 
	dim=DIM;
	for(i=0;i<DIM;i++)
		data[i]=feature<FEATURE_TYPE,PROPERTY_LIST_TYPE>();
}

template <class FEATURE_TYPE, class PROPERTY_LIST_TYPE, unsigned int DIM>  
feature_list<FEATURE_TYPE,PROPERTY_LIST_TYPE,DIM>::~feature_list()
{
}

template <class FEATURE_TYPE, class PROPERTY_LIST_TYPE, unsigned int DIM> 
void feature_list<FEATURE_TYPE,PROPERTY_LIST_TYPE,DIM>::set(unsigned int dim,feature<FEATURE_TYPE,PROPERTY_LIST_TYPE> f)
{
	if ((dim<DIM)&&(dim>=0))
	 data[dim]=f;
}

template <class FEATURE_TYPE, class PROPERTY_LIST_TYPE, unsigned int DIM> 
feature<FEATURE_TYPE,PROPERTY_LIST_TYPE> feature_list<FEATURE_TYPE,PROPERTY_LIST_TYPE,DIM>::get(unsigned int dim)
{
	if ((dim<DIM)&&(dim>=0))
		return(data[dim]);
	else
	{
		feature<FEATURE_TYPE,PROPERTY_LIST_TYPE> tmp;
	        return tmp;
	}
}

template <class FEATURE_TYPE, class PROPERTY_LIST_TYPE, unsigned int DIM> 
feature<FEATURE_TYPE,PROPERTY_LIST_TYPE> feature_list<FEATURE_TYPE,PROPERTY_LIST_TYPE,DIM>::look_up(unsigned int id)
{
	int i;
    
	for (i=0;i<DIM;i++)
		if (data[i].identification()==id)
         return(data[i]);
	
	feature<FEATURE_TYPE,PROPERTY_LIST_TYPE> tmp;
	return (tmp);
}

template <class FEATURE_TYPE, class PROPERTY_LIST_TYPE, unsigned int DIM>
void feature_list<FEATURE_TYPE,PROPERTY_LIST_TYPE,DIM>::store_value_to(unsigned int id,FEATURE_TYPE v)
{
    int i;
    
	for (i=0;i<DIM;i++)
		if (data[i].identification()==id)
			data[i].set(v);
}

template <class FEATURE_TYPE, class PROPERTY_LIST_TYPE, unsigned int DIM> 
void feature_list<FEATURE_TYPE,PROPERTY_LIST_TYPE,DIM>::calculate (PROPERTY_LIST_TYPE pl)
{
	unsigned int i;

	for(i=0;i<DIM;i++)
		if (data[i].get_dependence_level()==this->get_dependence_level())
		 data[i].calculate(pl);
	
}

template <class FEATURE_TYPE, class PROPERTY_LIST_TYPE, unsigned int DIM> 
void feature_list<FEATURE_TYPE,PROPERTY_LIST_TYPE,DIM>::initialize (PROPERTY_LIST_TYPE pl)
{
	unsigned int i;

	for(i=0;i<DIM;i++)
		if (data[i].get_dependence_level()==this->get_dependence_level())
		 data[i].initialize(pl);
	
}

template <class FEATURE_TYPE, class PROPERTY_LIST_TYPE, unsigned int DIM> 
unsigned int feature_list<FEATURE_TYPE,PROPERTY_LIST_TYPE,DIM>::get_dim (void)
{
	return(dim);
}

template <class FEATURE_TYPE, class PROPERTY_LIST_TYPE, unsigned int DIM> 
unsigned int feature_list<FEATURE_TYPE,PROPERTY_LIST_TYPE,DIM>::get_dependence_level(void)
{
	return(dependence_level);
}

template <class FEATURE_TYPE, class PROPERTY_LIST_TYPE, unsigned int DIM> 
void feature_list<FEATURE_TYPE,PROPERTY_LIST_TYPE,DIM>::set_dependence_level(unsigned int dl)
{
	dependence_level=dl;
}
