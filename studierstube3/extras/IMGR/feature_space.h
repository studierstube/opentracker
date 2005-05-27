#ifndef FEATURE_SPACE_INCLUDED
#define FEATURE_SPACE_INCLUDED

#include "stl_wrapper.h"
#include "feature_space_component.h"

template <class PROPERTY_LIST_TYPE,class FEATURE_TYPE,class FEATURE_LIST_TYPE>
class feature_space
{
	typedef feature_space_component<FEATURE_LIST_TYPE> FEATURE_SPACE_COMPONENT_TYPE;

public:
	feature_space();
	~feature_space();
	void init(void);
	bool load(const char *fn,bool andid);
	bool save(const char *fn,bool andid);
	void add(unsigned int id,FEATURE_LIST_TYPE fl);
	void add(FEATURE_LIST_TYPE fl);
	bool get_first(unsigned int &id,FEATURE_LIST_TYPE &fl);
    bool get_next(unsigned int &id,FEATURE_LIST_TYPE &fl);
	bool get_first(FEATURE_LIST_TYPE &fl);
	bool get_next(FEATURE_LIST_TYPE &fl);
    void sort(void);

protected:
	STL_LIST<FEATURE_SPACE_COMPONENT_TYPE> content;
	typename STL_LIST<FEATURE_SPACE_COMPONENT_TYPE>::iterator iterator;

};



#include "feature_space.cpp"
#endif

