#ifndef PROPERTY_LIST_INCLUDED
#define PROPERTY_LIST_INCLUDED

#include "property.h"

template <class PROPERTY_TYPE, class SAMPLE_TYPE, unsigned int DIM> 
class property_list
{
public :
	property_list();
	~property_list();
	void set(unsigned int dim,property<PROPERTY_TYPE,SAMPLE_TYPE> d);
	property<PROPERTY_TYPE,SAMPLE_TYPE> get(unsigned int dim);
    property<PROPERTY_TYPE,SAMPLE_TYPE> look_up(unsigned int id);
	void store_value_to(unsigned int id,PROPERTY_TYPE v);
	void update (SAMPLE_TYPE s);
	void initialize (SAMPLE_TYPE s);
	unsigned int get_dim(void);
	unsigned int get_dependence_level(void);
	void set_dependence_level(unsigned int dl);

protected :
	property<PROPERTY_TYPE,SAMPLE_TYPE> data[DIM];
	unsigned int dependence_level;
	unsigned int dim;
};

#include "property_list.cpp"

#endif

