#ifndef FEATURE_DEVIATION_INCLUDED
#define FEATURE_DEVIATION_INCLUDED

#include "feature.h"

template <class FEATURE_DEVIATION_TYPE,class FEATURE_TYPE, class PROPERTY_LIST_TYPE>
class feature_deviation
{
	typedef FEATURE_DEVIATION_TYPE (*FEATURE_DEVIATION_FUNCTION)(feature<FEATURE_TYPE,PROPERTY_LIST_TYPE> a,feature<FEATURE_TYPE,PROPERTY_LIST_TYPE> b); 

public :
 
	feature_deviation(unsigned int i,FEATURE_DEVIATION_FUNCTION df);
	feature_deviation();
	~feature_deviation();
	FEATURE_DEVIATION_TYPE get(void);
	unsigned int identification(void);
	void calculate (feature<FEATURE_TYPE,PROPERTY_LIST_TYPE> a,feature<FEATURE_TYPE,PROPERTY_LIST_TYPE> b);
	void define(unsigned int i,FEATURE_DEVIATION_FUNCTION df);
    void set_dependence_level(unsigned int dl);
	unsigned int get_dependence_level(void);

protected :
    FEATURE_DEVIATION_TYPE			value;
	unsigned int					id;
	unsigned int					dependence_level;
	FEATURE_DEVIATION_FUNCTION		calculate_function;
};

#include "feature_deviation.cpp"
#endif

