#include "parse_tree.h"

template <class PARSE_TREE_TYPE> 
parse_tree<PARSE_TREE_TYPE>::parse_tree():id(0)
{    
}

template <class PARSE_TREE_TYPE> 
parse_tree<PARSE_TREE_TYPE>::~parse_tree()
{
}

template <class PARSE_TREE_TYPE> 
void parse_tree<PARSE_TREE_TYPE>::init(void)
{
	//ToDo:delete sub-tree
}

template <class PARSE_TREE_TYPE> 
void parse_tree<PARSE_TREE_TYPE>::add_sub_tree(parse_tree<PARSE_TREE_TYPE> *st)
{
    PARSE_SUB_TREE_TYPE sub_tree;
    
	sub_tree.set_sub_tree(st);
	sub_trees.push_back(sub_tree);
}

template <class PARSE_TREE_TYPE> 
bool parse_tree<PARSE_TREE_TYPE>::get_first_sub_tree(parse_tree<PARSE_TREE_TYPE> **st)
{
    iterator = sub_trees.begin();
	return(get_next_sub_tree(st));
}


template <class PARSE_TREE_TYPE> 
bool parse_tree<PARSE_TREE_TYPE>::get_next_sub_tree(parse_tree<PARSE_TREE_TYPE> **st)
{
	if (iterator != sub_trees.end())
		{
		 PARSE_SUB_TREE_TYPE sub_tree = *iterator;
		 iterator++;

		 *st = sub_tree.get_sub_tree();
		 return(true);
		}else
	 return(false);
}

template <class PARSE_TREE_TYPE> 
bool parse_tree<PARSE_TREE_TYPE>::look_up_first_sub_tree(unsigned int i, parse_tree<PARSE_TREE_TYPE> **st)
{
    iterator = sub_trees.begin();
	return(look_up_next_sub_tree(i,st));
}


template <class PARSE_TREE_TYPE> 
bool parse_tree<PARSE_TREE_TYPE>::look_up_next_sub_tree(unsigned int i,parse_tree<PARSE_TREE_TYPE> **st)
{
	 bool found;

	 found=get_next_sub_tree(st);
     while ((found==true)&&((*st)->get_id()!=i))
	  found=get_next_sub_tree(st);
	 
	 return(found);
}

template <class PARSE_TREE_TYPE> 
void parse_tree<PARSE_TREE_TYPE>::set_content(PARSE_TREE_TYPE c)
{
 content=c;
}

template <class PARSE_TREE_TYPE> 
PARSE_TREE_TYPE parse_tree<PARSE_TREE_TYPE>::get_content(void)
{
 return(content);
}

template <class PARSE_TREE_TYPE> 
void parse_tree<PARSE_TREE_TYPE>::set_id(unsigned int i)
{
	id=i;
}
		
template <class PARSE_TREE_TYPE> 
unsigned int parse_tree<PARSE_TREE_TYPE>::get_id(void)
{
	return (id);
}