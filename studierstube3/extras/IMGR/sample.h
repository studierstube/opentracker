#ifndef SAMPLE_INCLUDED
#define SAMPLE_INCLUDED

template <class COMPONENT_TYPE, unsigned int DIM> 
class sample
{
public :
	sample();
	~sample();
	void set(unsigned int dim,COMPONENT_TYPE d);
	COMPONENT_TYPE get(unsigned int dim);
	COMPONENT_TYPE get_previous(unsigned int dim);
	void new_sample(void);
	unsigned long number_of_samples(void);
	void initialize (unsigned int dim,COMPONENT_TYPE d);
	unsigned int get_dim(void);

protected :
	COMPONENT_TYPE data[DIM];
	COMPONENT_TYPE previous_data[DIM];
	unsigned long n_o_s; 
	unsigned int dim;
};

#include "sample.cpp"


#endif

