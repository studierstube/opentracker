#ifndef FILTER_INCLUDED
#define FILTER_INCLUDED

template <class COMPONENT_TYPE, unsigned int SAMPLE_DIM, unsigned int FILTER_DIM> 
class filter
{
public:
	filter(COMPONENT_TYPE *w);
	~filter();
	COMPONENT_TYPE filter_sample_component(unsigned int dim,COMPONENT_TYPE d);
	void new_sample(void);
	void init(void);

protected:
	COMPONENT_TYPE *weights;		
	COMPONENT_TYPE previous_samples[SAMPLE_DIM][FILTER_DIM];
	int number_of_samples;

};

#include "filter.cpp"

#endif
