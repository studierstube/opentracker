#include "sample.h"

template <class COMPONENT_TYPE,unsigned int DIM>
sample<COMPONENT_TYPE,DIM>::sample()
:n_o_s(0)
{
	unsigned int i;

	dim=DIM;

	for(i=0;i<DIM;i++)
	{
		data[i]=0;
		previous_data[i]=0;
	}
}

template <class COMPONENT_TYPE,unsigned int DIM>
sample<COMPONENT_TYPE,DIM>::~sample()
{
}

template <class COMPONENT_TYPE,unsigned int DIM>
void sample<COMPONENT_TYPE,DIM>::set(unsigned int dim,COMPONENT_TYPE d)
{
	if ((dim<DIM)&&(dim>=0))
	{
	 previous_data[dim]=data[dim];
	 data[dim]=d;
	}
}

template <class COMPONENT_TYPE,unsigned int DIM>
COMPONENT_TYPE sample<COMPONENT_TYPE,DIM>::get(unsigned int dim)
{
	if ((dim<DIM)&&(dim>=0))
		return(data[dim]);
	else
		return(0);
}

template <class COMPONENT_TYPE,unsigned int DIM>
COMPONENT_TYPE sample<COMPONENT_TYPE,DIM>::get_previous(unsigned int dim)
{
	if ((dim<DIM)&&(dim>=0))
		return(previous_data[dim]);
	else
		return(0);
}

template <class COMPONENT_TYPE,unsigned int DIM>
void sample<COMPONENT_TYPE,DIM>::new_sample(void)
{
	n_o_s++;
}

template <class COMPONENT_TYPE,unsigned int DIM>
unsigned long sample<COMPONENT_TYPE,DIM>::number_of_samples(void)
{
	return(n_o_s);
}

template <class COMPONENT_TYPE,unsigned int DIM>
void sample<COMPONENT_TYPE,DIM>::initialize(unsigned int dim,COMPONENT_TYPE d)
{
	if ((dim<DIM)&&(dim>=0))
	{
	 previous_data[dim]=d;
	 data[dim]=d;
	 n_o_s=1;
	}
}

template <class COMPONENT_TYPE,unsigned int DIM>
unsigned int sample<COMPONENT_TYPE,DIM>::get_dim(void)
{return dim;}
