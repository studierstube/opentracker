#ifndef WEIGHT_INCLUDED
#define WEIGHT_INCLUDED

#define COMPONENTS 1
#define VALUE	   2


template <class WEIGHT_TYPE,class FEATURE_LIST_TYPE> 
class weight
{
public :
	typedef union
	{
		struct
		{
		 WEIGHT_TYPE divident, 
			         divisor;
		}component;
		WEIGHT_TYPE value;
	}WEIGHT_CONTENT_TYPE;

	typedef void (*WEIGHT_FUNCTION)(WEIGHT_CONTENT_TYPE &v, FEATURE_LIST_TYPE fl); 

	weight(unsigned int i,WEIGHT_FUNCTION cf,WEIGHT_FUNCTION inf, WEIGHT_TYPE v);
    weight(unsigned int i,WEIGHT_FUNCTION cf,WEIGHT_FUNCTION inf, WEIGHT_TYPE divid, WEIGHT_TYPE divis);
	weight();
	~weight();
	void set(WEIGHT_TYPE v);
    void set(WEIGHT_TYPE divid, WEIGHT_TYPE divis);
	WEIGHT_TYPE get_value(void);
	WEIGHT_TYPE get_divisor(void);
	WEIGHT_TYPE get_divident(void);
	unsigned int identification(void);
	void calculate (FEATURE_LIST_TYPE fl);
	void define(unsigned int i,WEIGHT_FUNCTION cf,WEIGHT_FUNCTION inf);
	void initialize (FEATURE_LIST_TYPE fl);
	void set_dependence_level(unsigned int);
	unsigned int get_dependence_level(void);

protected :
	
	WEIGHT_CONTENT_TYPE content;

    unsigned int	id;
	WEIGHT_FUNCTION	calculate_function;
	WEIGHT_FUNCTION	initialize_function;
	unsigned int dependence_level;
};

#include "weight.cpp"

#endif

