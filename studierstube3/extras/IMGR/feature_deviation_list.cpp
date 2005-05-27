#include "feature_deviation_list.h"

template <class FEATURE_DEVIATION_TYPE,class FEATURE_TYPE, class PROPERTY_LIST_TYPE, unsigned int DIM> 
feature_deviation_list<FEATURE_DEVIATION_TYPE,FEATURE_TYPE,PROPERTY_LIST_TYPE,DIM>::feature_deviation_list()
:dependence_level(0)
{
	unsigned int i;
 
	dim=DIM;
	for(i=0;i<DIM;i++)
		data[i]=feature_deviation<FEATURE_DEVIATION_TYPE,FEATURE_TYPE,PROPERTY_LIST_TYPE>();
}

template <class FEATURE_DEVIATION_TYPE,class FEATURE_TYPE, class PROPERTY_LIST_TYPE, unsigned int DIM>  
feature_deviation_list<FEATURE_DEVIATION_TYPE,FEATURE_TYPE,PROPERTY_LIST_TYPE,DIM>::~feature_deviation_list()
{
}

template <class FEATURE_DEVIATION_TYPE,class FEATURE_TYPE, class PROPERTY_LIST_TYPE, unsigned int DIM> 
void feature_deviation_list<FEATURE_DEVIATION_TYPE,FEATURE_TYPE,PROPERTY_LIST_TYPE,DIM>::set(unsigned int dim,feature_deviation<FEATURE_DEVIATION_TYPE,FEATURE_TYPE,PROPERTY_LIST_TYPE> f)
{
	if ((dim<DIM)&&(dim>=0))
	 data[dim]=f;
}

template <class FEATURE_DEVIATION_TYPE,class FEATURE_TYPE, class PROPERTY_LIST_TYPE, unsigned int DIM> 
feature_deviation<FEATURE_DEVIATION_TYPE,FEATURE_TYPE,PROPERTY_LIST_TYPE> feature_deviation_list<FEATURE_DEVIATION_TYPE,FEATURE_TYPE,PROPERTY_LIST_TYPE,DIM>::get(unsigned int dim)
{
	if ((dim<DIM)&&(dim>=0))
		return(data[dim]);
	else
	{
	    feature_deviation<FEATURE_DEVIATION_TYPE,FEATURE_TYPE,PROPERTY_LIST_TYPE> tmp;
	    return (tmp);
	}
}

template <class FEATURE_DEVIATION_TYPE,class FEATURE_TYPE, class PROPERTY_LIST_TYPE, unsigned int DIM> 
feature_deviation<FEATURE_DEVIATION_TYPE,FEATURE_TYPE,PROPERTY_LIST_TYPE> feature_deviation_list<FEATURE_DEVIATION_TYPE,FEATURE_TYPE,PROPERTY_LIST_TYPE,DIM>::look_up(unsigned int id)
{
  int i;
  
  for (i=0;i<DIM;i++)
    if (data[i].identification()==id)
      return(data[i]);
	
  feature_deviation<FEATURE_DEVIATION_TYPE,FEATURE_TYPE,PROPERTY_LIST_TYPE> tmp;
  return (tmp);
}

template <class FEATURE_DEVIATION_TYPE,class FEATURE_TYPE, class PROPERTY_LIST_TYPE, unsigned int DIM> 
void feature_deviation_list<FEATURE_DEVIATION_TYPE,FEATURE_TYPE,PROPERTY_LIST_TYPE,DIM>::calculate (feature_list<FEATURE_TYPE,PROPERTY_LIST_TYPE,DIM> a,feature_list<FEATURE_TYPE,PROPERTY_LIST_TYPE,DIM> b)
{
	unsigned int i;

	for(i=0;i<DIM;i++)
		if (data[i].get_dependence_level()==this->get_dependence_level())
		 data[i].calculate(a.look_up(i),b.look_up(i));
	
}


template <class FEATURE_DEVIATION_TYPE,class FEATURE_TYPE, class PROPERTY_LIST_TYPE, unsigned int DIM> 
unsigned int feature_deviation_list<FEATURE_DEVIATION_TYPE,FEATURE_TYPE,PROPERTY_LIST_TYPE,DIM>::get_dim (void)
{
	return(dim);
}

template <class FEATURE_DEVIATION_TYPE,class FEATURE_TYPE, class PROPERTY_LIST_TYPE, unsigned int DIM> 
unsigned int feature_deviation_list<FEATURE_DEVIATION_TYPE,FEATURE_TYPE,PROPERTY_LIST_TYPE,DIM>::get_dependence_level(void)
{
	return(dependence_level);
}

template <class FEATURE_DEVIATION_TYPE,class FEATURE_TYPE, class PROPERTY_LIST_TYPE, unsigned int DIM> 
void feature_deviation_list<FEATURE_DEVIATION_TYPE,FEATURE_TYPE,PROPERTY_LIST_TYPE,DIM>::set_dependence_level(unsigned int dl)
{
	dependence_level=dl;
}
