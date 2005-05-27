	#ifndef PARSE_TREE_INCLUDED
	#define PARSE_TREE_INCLUDED

	#include "stl_wrapper.h"
	#include "parse_sub_tree.h"

    template<class PARSE_TREE_TYPE>
	class parse_tree
	{
	typedef parse_sub_tree<PARSE_TREE_TYPE> PARSE_SUB_TREE_TYPE;

	public :
		parse_tree();
		~parse_tree();
		void init();
		void add_sub_tree(parse_tree<PARSE_TREE_TYPE> *st);
		bool get_first_sub_tree(parse_tree<PARSE_TREE_TYPE> **st);
		bool get_next_sub_tree(parse_tree<PARSE_TREE_TYPE> **st);
		bool look_up_first_sub_tree(unsigned int i,parse_tree<PARSE_TREE_TYPE> **st);
		bool look_up_next_sub_tree(unsigned int i,parse_tree<PARSE_TREE_TYPE> **st);
        void set_content(PARSE_TREE_TYPE c);
		void set_id(unsigned int i);
		unsigned int get_id(void);
		PARSE_TREE_TYPE get_content(void);
	protected :
		PARSE_TREE_TYPE content;
		unsigned int id;
		STL_LIST<PARSE_SUB_TREE_TYPE> sub_trees;
		STL_LIST<PARSE_SUB_TREE_TYPE>::iterator iterator;
	};

	#include "parse_tree.cpp"

	#endif