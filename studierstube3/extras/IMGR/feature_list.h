#ifndef FEATURE_LIST_INCLUDED
#define FEATURE_LIST_INCLUDED

#include "feature.h"

template <class FEATURE_TYPE, class PROPERTY_LIST_TYPE, unsigned int DIM> 
class feature_list
{
public :
	feature_list();
	~feature_list();
	void set(unsigned int dim,feature<FEATURE_TYPE,PROPERTY_LIST_TYPE> f);
	feature<FEATURE_TYPE,PROPERTY_LIST_TYPE> get(unsigned int dim);
    feature<FEATURE_TYPE,PROPERTY_LIST_TYPE> look_up(unsigned int id);
	void store_value_to(unsigned int id,FEATURE_TYPE v);
	void calculate (PROPERTY_LIST_TYPE pl);
	void initialize (PROPERTY_LIST_TYPE pl);
	unsigned int get_dim(void);
	void set_dependence_level(unsigned int dl);
	unsigned int get_dependence_level(void);

protected :
	feature<FEATURE_TYPE,PROPERTY_LIST_TYPE> data[DIM];
	unsigned int dim;
	unsigned int dependence_level;
};

#include "feature_list.cpp"


#endif

