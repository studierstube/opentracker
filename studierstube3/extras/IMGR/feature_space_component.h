#ifndef FEATURE_SPACE_COMPONENT_INCLUDED
#define FEATURE_SPACE_COMPONENT_INCLUDED

template<class FEATURE_LIST_TYPE>
class feature_space_component
{
public:
	
	bool operator == (const feature_space_component &src) const
		{ return src.id == this->id; }
	bool operator != (const feature_space_component &src) const
		{ return src.id != this->id; }
	bool operator < (const feature_space_component &src) const
		{ return src.id < this->id; }
	bool operator > (const feature_space_component &src) const
		{ return src.id > this->id; }
	void set_feature_list(FEATURE_LIST_TYPE fl)
		{feature_list=fl;}
	void set_id(unsigned int i)
		{id=i;};
	FEATURE_LIST_TYPE get_feature_list(void)
		{return feature_list;}
	unsigned int get_id(void)
		{return id;}
protected:
	FEATURE_LIST_TYPE feature_list;
	unsigned int id;	
};

#endif
