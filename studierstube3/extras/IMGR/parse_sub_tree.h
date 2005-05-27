#ifndef PARSE_SUB_TREE_INCLUDED
#define PARSE_SUB_TREE_INCLUDED

#include "parse_tree.h"

template<class PARSE_TREE_TYPE> class parse_tree;

template<class PARSE_TREE_TYPE>
class parse_sub_tree
{
public:
	parse_sub_tree() :sub_tree(NULL)
		{}
	bool operator == (const parse_sub_tree &src) const
		{ 
		  if ((src.sub_tree !=NULL)&&(sub_tree!=NULL))
		   return src.sub_tree->get_id() == sub_tree->get_id(); 
		  else
			return false;
		}
	bool operator != (const parse_sub_tree &src) const
		{
		  if ((src.sub_tree !=NULL)&&(sub_tree!=NULL))
		   return src.sub_tree->get_id() != sub_tree->get_id(); 
		  else 
		   return false;
		}
	bool operator < (const parse_sub_tree &src) const
		{
		  if ((src.sub_tree !=NULL)&&(sub_tree!=NULL))
		   return src.sub_tree->get_id() < sub_tree->get_id(); 
	      else
	       return false;
		}
	bool operator > (const parse_sub_tree &src) const
		{ 
		  if ((src.sub_tree !=NULL)&&(sub_tree!=NULL))
		   return src.sub_tree->get_id() > sub_tree->get_id(); 
	      else
	       return false;
		}
	void set_sub_tree(parse_tree<PARSE_TREE_TYPE> *st)
		{sub_tree=st;}
    parse_tree<PARSE_TREE_TYPE> *get_sub_tree(void)
		{return(sub_tree);}
protected:
	parse_tree<PARSE_TREE_TYPE> *sub_tree;
};

#endif
