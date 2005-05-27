#include "sample_list.h"

template <class SAMPLE_TYPE> 
sample_list<SAMPLE_TYPE>::sample_list()
:current_id(0)
{    
}

template <class SAMPLE_TYPE> 
sample_list<SAMPLE_TYPE>::~sample_list()
{
}

template <class SAMPLE_TYPE>
void sample_list<SAMPLE_TYPE>::init(void)
{
	current_id=0;
        content.erase(content.begin(),content.end());
}

template <class SAMPLE_TYPE>
void sample_list<SAMPLE_TYPE>::add(SAMPLE_TYPE s)
{
    sample_list_component<SAMPLE_TYPE> slc;

	slc.set_sample(s);
	slc.set_id(current_id);
	current_id++;
	content.push_back(slc);
}

template <class SAMPLE_TYPE>
bool sample_list<SAMPLE_TYPE>::get_first(unsigned int &id,SAMPLE_TYPE &s)
{
	iterator = content.begin();
	return get_next(id,s);
}

template <class SAMPLE_TYPE>
bool sample_list<SAMPLE_TYPE>::get_next(unsigned int &id,SAMPLE_TYPE &s)
{
	if (iterator != content.end())
		{
		 SAMPLE_LIST_COMPONENT_TYPE value = *iterator;
		 iterator++;

		 s = value.get_sample();
		 id = value.get_id();
		 return(true);
		}else
	 return(false);
}

template <class SAMPLE_TYPE>
bool sample_list<SAMPLE_TYPE>::get_first(SAMPLE_TYPE &s)
{
	iterator = content.begin();
	return get_next(s);
}

template <class SAMPLE_TYPE>
bool sample_list<SAMPLE_TYPE>::get_next(SAMPLE_TYPE &s)
{
	if (iterator != content.end())
		{
		 SAMPLE_LIST_COMPONENT_TYPE value = *iterator;
		 iterator++;

		 s = value.get_sample();
		 return(true);
		}else
	 return(false);
}

