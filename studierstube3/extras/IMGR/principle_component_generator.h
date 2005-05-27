#ifndef PRINCIPLE_COMPONENT_GENERATOR_INCLUDED
#define PRINCIPLE_COMPONENT_GENERATOR_INCLUDED

#include "feature_list.h"
#include "feature_space.h"
#include "matrix.h"

#define MAX_PRINCIPLE_COMPONENTS 100
#define LAMBDA	1.0 //0.0<LAMBDA<=1.0
#define DELTA_LAMBDA	0.999 //(99.9%)
#define EPSILON	0.01 //(|1.0-|pc(i)||)<EPSILON

template <class PROPERTY_LIST_TYPE,class FEATURE_TYPE,class FEATURE_LIST_TYPE>
class principle_component_generator
{
	
 public:
	
	principle_component_generator();
	principle_component_generator(unsigned int nopc);
	~principle_component_generator();
	void generate(void);
	void set_feature_space(feature_space<PROPERTY_LIST_TYPE,FEATURE_TYPE,FEATURE_LIST_TYPE> d);
    double deviation (unsigned int pc); 
	int load(const char *fn);
	int save(const char *fn);

 protected:
 
 protected:
	unsigned int			number_of_principle_components;
	unsigned int			number_of_features;
	matrix_t<double>		principle_components;
	feature_space<PROPERTY_LIST_TYPE,FEATURE_TYPE,FEATURE_LIST_TYPE> data;
	double lambda;
};

#include "principle_component_generator.cpp"

#endif