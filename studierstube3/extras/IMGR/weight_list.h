#ifndef WEIGHT_LIST_INCLUDED
#define WEIGHT_LIST_INCLUDED

#include "weight.h"

template <class WEIGHT_TYPE,class FEATURE_LIST_TYPE, unsigned int DIM> 
class weight_list
{
public :
	weight_list();
	~weight_list();
	void set(unsigned int dim,weight<WEIGHT_TYPE,FEATURE_LIST_TYPE> d);
	weight<WEIGHT_TYPE,FEATURE_LIST_TYPE> get(unsigned int dim);
    weight<WEIGHT_TYPE,FEATURE_LIST_TYPE> look_up(unsigned int id);
	void calculate (FEATURE_LIST_TYPE fl);
	void initialize (FEATURE_LIST_TYPE fl);
	void set_dependence_level(unsigned int);
	unsigned int get_dependence_level(void);

protected :
	weight<WEIGHT_TYPE,FEATURE_LIST_TYPE> data[DIM];
	unsigned int dependence_level;
};


#include "weight_list.cpp"

#endif

