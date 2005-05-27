#ifndef FILTER_POOL_INCLUDED
#define FILTER_POOL_INCLUDED

#include "filter.h"

#define DEFINED_FILTER_COMPONENT_TYPE	float
#define DEFINED_FILTER_SAMPLE_DIMENSION		2
#define DEFINED_FILTER_DIMENSION		2	
//-------------------------------------------------------------------------
double AVERAGE[10]=						 {1.0/DEFINED_FILTER_DIMENSION,
                                          1.0/DEFINED_FILTER_DIMENSION,
										  1.0/DEFINED_FILTER_DIMENSION,
                                          1.0/DEFINED_FILTER_DIMENSION,
										  1.0/DEFINED_FILTER_DIMENSION,
                                          1.0/DEFINED_FILTER_DIMENSION,
										  1.0/DEFINED_FILTER_DIMENSION,
                                          1.0/DEFINED_FILTER_DIMENSION,
										  1.0/DEFINED_FILTER_DIMENSION,
                                          1.0/DEFINED_FILTER_DIMENSION
};
//-------------------------------------------------------------------------

typedef filter<DEFINED_COMPONENT_TYPE,DEFINED_FILTER_SAMPLE_DIMENSION,DEFINED_FILTER_DIMENSION> DEFINED_FILTER_TYPE;

#endif

