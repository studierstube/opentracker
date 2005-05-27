#ifndef FEATURE_INCLUDED
#define FEATURE_INCLUDED

template <class FEATURE_TYPE, class PROPERTY_LIST_TYPE>
class feature
{
	typedef void (*FEATURE_FUNCTION)(FEATURE_TYPE &v, PROPERTY_LIST_TYPE pl); 

public :
 
	feature(unsigned int i,FEATURE_FUNCTION cf,FEATURE_FUNCTION inf, FEATURE_TYPE v=0);
	feature();
	~feature();
	void set(FEATURE_TYPE v);
	void set(unsigned int i,FEATURE_TYPE v);
	FEATURE_TYPE get(void);
	unsigned int identification(void);
	void calculate (PROPERTY_LIST_TYPE pl);
	void define(unsigned int i,FEATURE_FUNCTION cf,FEATURE_FUNCTION inf);
	void initialize (PROPERTY_LIST_TYPE pl);
	void set_dependence_level(unsigned int dl);
	unsigned int get_dependence_level(void);

protected :
	FEATURE_TYPE		value;
	unsigned int		id;
	unsigned int		dependence_level;
	FEATURE_FUNCTION	calculate_function;
	FEATURE_FUNCTION	initialize_function;
};

#include "feature.cpp"
#endif

