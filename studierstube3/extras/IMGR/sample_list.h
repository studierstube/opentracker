	#ifndef SAMPLE_LIST_INCLUDED
	#define SAMPLE_LIST_INCLUDED

	#include "stl_wrapper.h"
	#include "sample_list_component.h"

	template <class SAMPLE_TYPE> 
	class sample_list
	{
	typedef sample_list_component<SAMPLE_TYPE> SAMPLE_LIST_COMPONENT_TYPE;

	public :
		sample_list();
		~sample_list();
		void init();
		void add(SAMPLE_TYPE s);
		bool get_first(unsigned int &id,SAMPLE_TYPE &s);
        bool get_next(unsigned int &id,SAMPLE_TYPE &s);
		bool get_first(SAMPLE_TYPE &s);
		bool get_next(SAMPLE_TYPE &s);

	protected :
		STL_LIST<SAMPLE_LIST_COMPONENT_TYPE> content;
		typename STL_LIST<SAMPLE_LIST_COMPONENT_TYPE>::iterator iterator;
		unsigned int current_id;
	};

	#include "sample_list.cpp"

	#endif

