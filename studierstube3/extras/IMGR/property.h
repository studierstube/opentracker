#ifndef PROPERTY_INCLUDED
#define PROPERTY_INCLUDED

template <class PROPERTY_TYPE, class SAMPLE_TYPE> 
class property
{
	typedef void (*PROP_FUNCTION)(PROPERTY_TYPE &v, SAMPLE_TYPE s); 

public :

	property(unsigned int i,PROP_FUNCTION uf,PROP_FUNCTION inf, PROPERTY_TYPE v=0);
	property();
	~property();
	void set(PROPERTY_TYPE v);
	void set(unsigned int i,PROPERTY_TYPE v);
	PROPERTY_TYPE get(void);
	unsigned int get_dependence_level(void);
	void set_dependence_level (unsigned int dl);
	unsigned int identification(void);
	void update (SAMPLE_TYPE s);
	void define(unsigned int i,PROP_FUNCTION uf,PROP_FUNCTION inf);
	void initialize (SAMPLE_TYPE s);

protected :
	PROPERTY_TYPE	value;
	unsigned int	id;
	unsigned int	dependence_level;
	PROP_FUNCTION	update_function;
	PROP_FUNCTION	initialize_function;
};

#include "property.cpp"

#endif

