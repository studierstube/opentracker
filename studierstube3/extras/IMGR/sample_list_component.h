#ifndef SAMPLE_LIST_COMPONENT_INCLUDED
#define SAMPLE_LIST_COMPONENT_INCLUDED

template<class SAMPLE_TYPE>
class sample_list_component
{
public:
	
	bool operator == (const sample_list_component &src) const
		{ return src.id == this->id; }
	bool operator != (const sample_list_component &src) const
		{ return src.id != this->id; }
	bool operator < (const sample_list_component &src) const
		{ return src.id < this->id; }
	bool operator > (const sample_list_component &src) const
		{ return src.id > this->id; }
	void set_sample(SAMPLE_TYPE s)
		{sample=s;}
	void set_id(unsigned int i)
		{id=i;}
	SAMPLE_TYPE get_sample(void)
		{return sample;}
	unsigned int get_id(void)
		{return id;};
protected:
	SAMPLE_TYPE sample;	
	unsigned int id;
};

#endif
