#include "filter.h"

template <class COMPONENT_TYPE, unsigned int SAMPLE_DIM, unsigned int FILTER_DIM> 
filter<COMPONENT_TYPE,SAMPLE_DIM,FILTER_DIM>::filter(COMPONENT_TYPE *w) 
:number_of_samples(-1)
{
 weights=w;
}

template <class COMPONENT_TYPE, unsigned int SAMPLE_DIM, unsigned int FILTER_DIM> 
filter<COMPONENT_TYPE,SAMPLE_DIM,FILTER_DIM>::~filter()
{
}

template <class COMPONENT_TYPE, unsigned int SAMPLE_DIM, unsigned int FILTER_DIM> 
void filter<COMPONENT_TYPE,SAMPLE_DIM,FILTER_DIM>::new_sample(void)
{
 number_of_samples++;
}

template <class COMPONENT_TYPE, unsigned int SAMPLE_DIM, unsigned int FILTER_DIM> 
COMPONENT_TYPE filter<COMPONENT_TYPE,SAMPLE_DIM,FILTER_DIM>::filter_sample_component(unsigned int dim,COMPONENT_TYPE d)
{
	int i;
	COMPONENT_TYPE new_d;

	if(number_of_samples<FILTER_DIM-1)
	{
		previous_samples[dim][number_of_samples]=d;
		new_d=d;
	}else
	{
	 new_d=(COMPONENT_TYPE)0;
	 for(i=0;i<FILTER_DIM-1;i++) 
	  new_d+=(COMPONENT_TYPE)((double)previous_samples[dim][i]*weights[i]);
	 
	 new_d+=(COMPONENT_TYPE)((double)d*weights[FILTER_DIM-1]);

	 previous_samples[dim][FILTER_DIM-1]=d;

	 if(FILTER_DIM>0)
	 for(i=1;i<FILTER_DIM;i++)
	  previous_samples[dim][i-1]=previous_samples[dim][i];
	}
	return(new_d);
}

template <class COMPONENT_TYPE, unsigned int SAMPLE_DIM, unsigned int FILTER_DIM> 
void filter<COMPONENT_TYPE,SAMPLE_DIM,FILTER_DIM>::init(void)
{
	number_of_samples=-1;
}

