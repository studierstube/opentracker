#include "feature_space.h"

template <class PROPERTY_LIST_TYPE,class FEATURE_TYPE,class FEATURE_LIST_TYPE>
feature_space<PROPERTY_LIST_TYPE,FEATURE_TYPE,FEATURE_LIST_TYPE>::feature_space()
{
}

template <class PROPERTY_LIST_TYPE,class FEATURE_TYPE,class FEATURE_LIST_TYPE>
feature_space<PROPERTY_LIST_TYPE,FEATURE_TYPE,FEATURE_LIST_TYPE>::~feature_space()
{
}

template <class PROPERTY_LIST_TYPE,class FEATURE_TYPE,class FEATURE_LIST_TYPE>
void feature_space<PROPERTY_LIST_TYPE,FEATURE_TYPE,FEATURE_LIST_TYPE>::init(void)
{
	//ToDo:Delete List
}

template <class PROPERTY_LIST_TYPE,class FEATURE_TYPE,class FEATURE_LIST_TYPE>
void feature_space<PROPERTY_LIST_TYPE,FEATURE_TYPE,FEATURE_LIST_TYPE>::add(unsigned int id,FEATURE_LIST_TYPE fl)
{
	feature_space_component<FEATURE_LIST_TYPE> fsc;

	fsc.set_feature_list(fl);
	fsc.set_id(id);
	content.push_back(fsc);
}

template <class PROPERTY_LIST_TYPE,class FEATURE_TYPE,class FEATURE_LIST_TYPE>
void feature_space<PROPERTY_LIST_TYPE,FEATURE_TYPE,FEATURE_LIST_TYPE>::add(FEATURE_LIST_TYPE fl)
{
	feature_space_component<FEATURE_LIST_TYPE> fsc;

	fsc.set_feature_list(fl);
	fsc.set_id(0);
    content.push_back(fsc);
}

template <class PROPERTY_LIST_TYPE,class FEATURE_TYPE,class FEATURE_LIST_TYPE>
bool feature_space<PROPERTY_LIST_TYPE,FEATURE_TYPE,FEATURE_LIST_TYPE>::get_first(unsigned int &id,FEATURE_LIST_TYPE &fl)
{
	iterator = content.begin();
	return get_next(id,fl);
}

template <class PROPERTY_LIST_TYPE,class FEATURE_TYPE,class FEATURE_LIST_TYPE>
bool feature_space<PROPERTY_LIST_TYPE,FEATURE_TYPE,FEATURE_LIST_TYPE>::get_next(unsigned int &id,FEATURE_LIST_TYPE &fl)
{
	if (iterator != content.end())
		{
		 FEATURE_SPACE_COMPONENT_TYPE value = *iterator;
		 iterator++;

		 fl = value.get_feature_list();
		 id = value.get_id();
		 return(true);
		}else
	 return(false);
}

template <class PROPERTY_LIST_TYPE,class FEATURE_TYPE,class FEATURE_LIST_TYPE>
bool feature_space<PROPERTY_LIST_TYPE,FEATURE_TYPE,FEATURE_LIST_TYPE>::get_first(FEATURE_LIST_TYPE &fl)
{
	iterator = content.begin();
	return get_next(fl);
}

template <class PROPERTY_LIST_TYPE,class FEATURE_TYPE,class FEATURE_LIST_TYPE>
bool feature_space<PROPERTY_LIST_TYPE,FEATURE_TYPE,FEATURE_LIST_TYPE>::get_next(FEATURE_LIST_TYPE &fl)
{
	if (iterator != content.end())
		{
		 FEATURE_SPACE_COMPONENT_TYPE value = *iterator;
		 iterator++;

		 fl = value.get_feature_list();
		 return(true);
		}else
	 return(false);
}

template <class PROPERTY_LIST_TYPE,class FEATURE_TYPE,class FEATURE_LIST_TYPE>
bool feature_space<PROPERTY_LIST_TYPE,FEATURE_TYPE,FEATURE_LIST_TYPE>::load(const char *fn,bool andid)
{
	FILE *f;
    int i;
	unsigned int id,fid;
	double fv;
	FEATURE_LIST_TYPE fl;
	feature<FEATURE_TYPE,PROPERTY_LIST_TYPE> fe;

     
    if((f=fopen(fn,"r"))!=NULL)
	{
    while (!feof(f))
	{   
        if(andid==true)
		 fscanf(f,"%i\n",&id);
		for(i=0;i<fl.get_dim();i++)  
		{
		  fscanf(f,"%i:%lf ",&fid,&fv);
		  fe.set(fid,fv);
          fl.set(i,fe);
		}
		if (andid==true)
		 this->add(id,fl);
		else
		 this->add(fl);
	}
	fclose(f);
	return(true);
	}else return(false);
}

template <class PROPERTY_LIST_TYPE,class FEATURE_TYPE,class FEATURE_LIST_TYPE>
bool feature_space<PROPERTY_LIST_TYPE,FEATURE_TYPE,FEATURE_LIST_TYPE>::save(const char *fn,bool andid)
{
	FILE *f;
    
	int i;
	unsigned int id;
	FEATURE_LIST_TYPE fl;
	feature<FEATURE_TYPE,PROPERTY_LIST_TYPE> fe;
    bool more;
	 
    if((f=fopen(fn,"w"))!=NULL)
	{
    
     if (andid==true)
	  more=this->get_first(id,fl);
	 else
	  more=this->get_first(fl);

	 while(more==true)
	 {	  
       if (andid==true)
	    fprintf(f,"%i\n",id);
       for(i=0;i<fl.get_dim();i++)  
		{
		  fe=fl.get(i);
		  fprintf(f,"%i:",fe.identification());
		  fprintf(f,"%lf ",(double)fe.get());
		}
	    fprintf(f,"\n");
       if(andid==true)
		more=this->get_next(id,fl); 
	   else
		more=this->get_next(fl);
	 }
	 fclose(f);
	 return(true);
	}else return(false);
}

template <class PROPERTY_LIST_TYPE,class FEATURE_TYPE,class FEATURE_LIST_TYPE>
void feature_space<PROPERTY_LIST_TYPE,FEATURE_TYPE,FEATURE_LIST_TYPE>::sort(void)
{content.sort();}

