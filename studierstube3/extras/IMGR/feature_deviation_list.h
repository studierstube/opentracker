#ifndef FEATURE_DEVIATION_LIST_INCLUDED
#define FEATURE_DEVIATION_LIST_INCLUDED

#include "feature_deviation.h"

template <class FEATURE_DEVIATION_TYPE,class FEATURE_TYPE, class PROPERTY_LIST_TYPE, unsigned int DIM> 
class feature_deviation_list
{
public :
	feature_deviation_list();
	~feature_deviation_list();
	void set(unsigned int dim,feature_deviation<FEATURE_DEVIATION_TYPE,FEATURE_TYPE,PROPERTY_LIST_TYPE> f);
	feature_deviation<FEATURE_DEVIATION_TYPE,FEATURE_TYPE,PROPERTY_LIST_TYPE> get(unsigned int dim);
    feature_deviation<FEATURE_DEVIATION_TYPE,FEATURE_TYPE,PROPERTY_LIST_TYPE> look_up(unsigned int id);
	void calculate (feature_list<FEATURE_TYPE,PROPERTY_LIST_TYPE,DIM> a,feature_list<FEATURE_TYPE,PROPERTY_LIST_TYPE,DIM> b);
	unsigned int get_dim(void);
	void set_dependence_level(unsigned int dl);
	unsigned int get_dependence_level(void);

protected :
	feature_deviation<FEATURE_DEVIATION_TYPE,FEATURE_TYPE,PROPERTY_LIST_TYPE> data[DIM];
	unsigned int dim;
	unsigned int dependence_level;
};

#include "feature_deviation_list.cpp"


#endif

