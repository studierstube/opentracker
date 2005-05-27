#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "principle_component_generator.h"

template<class PROPERTY_LIST_TYPE,class FEATURE_TYPE,class FEATURE_LIST_TYPE>
principle_component_generator<PROPERTY_LIST_TYPE, FEATURE_TYPE, FEATURE_LIST_TYPE>::principle_component_generator()
:number_of_principle_components(0),number_of_features(FEATURE_LIST_TYPE().get_dim()),principle_components(number_of_features,MAX_PRINCIPLE_COMPONENTS)
{
}

template<class PROPERTY_LIST_TYPE,class FEATURE_TYPE,class FEATURE_LIST_TYPE>
principle_component_generator<PROPERTY_LIST_TYPE, FEATURE_TYPE, FEATURE_LIST_TYPE>::principle_component_generator(unsigned int nopc=0)
:lambda(0.0),number_of_principle_components(nopc),number_of_features(FEATURE_LIST_TYPE().get_dim()),principle_components(number_of_features,MAX_PRINCIPLE_COMPONENTS)
{
  int i,j;
  double sum,norm;

  srand( (unsigned)time( NULL ) );

  //random init.(>0) and normalize weights
  for (i=0;i<number_of_principle_components;i++)
  {
	  sum=0.0;
	  for(j=0;j<number_of_features;j++)
	  {
		principle_components[i][j]=((double)rand()+1.0)/(double)RAND_MAX;
		sum+=pow(principle_components[i][j],2.0);
	  }
	  norm=sqrt(sum);
	  for(j=0;j<number_of_features;j++)
	   principle_components[i][j]/=norm;
  } 
}

template<class PROPERTY_LIST_TYPE,class FEATURE_TYPE,class FEATURE_LIST_TYPE>
principle_component_generator<PROPERTY_LIST_TYPE, FEATURE_TYPE, FEATURE_LIST_TYPE>::~principle_component_generator()
{
}

template<class PROPERTY_LIST_TYPE,class FEATURE_TYPE,class FEATURE_LIST_TYPE>
void principle_component_generator<PROPERTY_LIST_TYPE, FEATURE_TYPE, FEATURE_LIST_TYPE>::set_feature_space(feature_space<PROPERTY_LIST_TYPE,FEATURE_TYPE,FEATURE_LIST_TYPE> d)
{
 data=d;
}

template<class PROPERTY_LIST_TYPE,class FEATURE_TYPE,class FEATURE_LIST_TYPE>
int principle_component_generator<PROPERTY_LIST_TYPE, FEATURE_TYPE, FEATURE_LIST_TYPE>::load(const char *fn)
{
	FILE *f;
	int j;
	
    if((f=fopen(fn,"r"))!=NULL)
	{
	 number_of_principle_components=0;
     while (!feof(f))
	 {
	  for(j=0;j<number_of_features;j++)
		fscanf(f,"%lf ",&(principle_components[number_of_principle_components][j]));
	  fscanf(f,"\n");
	  if (number_of_principle_components<MAX_PRINCIPLE_COMPONENTS)
	   number_of_principle_components++;
	 } 
	 fclose(f);
	 return(true);
	}else return(false);
}

template<class PROPERTY_LIST_TYPE,class FEATURE_TYPE,class FEATURE_LIST_TYPE>
int principle_component_generator<PROPERTY_LIST_TYPE, FEATURE_TYPE, FEATURE_LIST_TYPE>::save(const char *fn)
{
	FILE *f;
	int i,j;
	
    if((f=fopen(fn,"w"))!=NULL)
	{
     
     for (i=0;i<number_of_principle_components;i++)
	 {
	  for(j=0;j<number_of_features;j++)
		fprintf(f,"%lf ",principle_components[i][j]);
	  fprintf(f,"\n");
	 } 
	 fclose(f);
	 return(true);
	}else return(false);
}

template<class PROPERTY_LIST_TYPE,class FEATURE_TYPE,class FEATURE_LIST_TYPE>
void principle_component_generator<PROPERTY_LIST_TYPE, FEATURE_TYPE, FEATURE_LIST_TYPE>::generate()
{
 double omega=0.0;
 double tmp,sum,norm,length;
 int i,j;
 bool more;
 FEATURE_LIST_TYPE fl;
 feature<FEATURE_TYPE,PROPERTY_LIST_TYPE> f;
 int pc;

 for (pc=0;pc<number_of_principle_components;pc++)
 {
  lambda=LAMBDA;
 
  length=0.0;
  while((fabs(1.0-length))>EPSILON)
  {
   more=data.get_first(fl);
   while(more==true)
   {	 
   
    //normalize input features
    sum=0.0;
    for(i=0;i<number_of_features;i++)
     sum+=pow(fl.get(i).get(),2.0);
    norm=sqrt(sum);
    for(i=0;i<number_of_features;i++)
    { 
 	 f.set(fl.get(i).get()/norm);
	 fl.set(i,f);
    }

	
	//new input for next pc (X0->PC0)	
    for(i=0;i<pc;i++)
	{
	 omega=0.0;
	 for (j=0;j<number_of_features;j++) //Oi=Xi*PCi
      omega+=fl.get(j).get()*principle_components[i][j];
	 for(j=0;j<number_of_features;j++) //X(i+1)=Xi-Oi*PCi -> PC(i+1)
     { 
 	  f.set(fl.get(j).get()-omega*principle_components[i][j]);
	  fl.set(j,f);
     }
	}

    cout << "pc : " << pc  << " length : " << length << " lambda : " << lambda << "\n"; 

    omega=0.0;
    for(i=0;i<number_of_features;i++)
     omega+=fl.get(i).get()*principle_components[pc][i];  

    for(i=0;i<number_of_features;i++)
    {
     tmp=lambda*omega*(fl.get(i).get()-omega*principle_components[pc][i]);
     principle_components[pc][i]+=tmp;
    }
    more=data.get_next(fl);
    lambda*=DELTA_LAMBDA;
   }
   tmp=0.0;
   for (i=0;i<number_of_features;i++)
   {
    tmp+=pow(principle_components[pc][i],2.0);
   }
   length=sqrt(tmp);
  }
 }
}


template<class PROPERTY_LIST_TYPE,class FEATURE_TYPE,class FEATURE_LIST_TYPE>
double principle_component_generator<PROPERTY_LIST_TYPE, FEATURE_TYPE, FEATURE_LIST_TYPE>::deviation (unsigned int pc)
{
 double tmp,result,sum,norm;
 int i,j,n;
 bool more;
 double omega;
 FEATURE_LIST_TYPE fl;
 feature<FEATURE_TYPE,PROPERTY_LIST_TYPE> f;
 
 if(pc<number_of_principle_components)
 {
  n=0;
  result=0.0;
  more=data.get_first(fl);
  while(more==true)
  {
   n++;
   //normalize input features
   sum=0.0;
   for(i=0;i<number_of_features;i++)
    sum+=pow(fl.get(i).get(),2.0);
   norm=sqrt(sum);
   for(i=0;i<number_of_features;i++)
   { 
	f.set(fl.get(i).get()/norm);
	fl.set(i,f);
   }

   //new input for next pc (X0->PC0)	
   for(i=0;i<pc;i++)
   {
    omega=0.0;
	for (j=0;j<number_of_features;j++) //Oi=Xi*PCi
     omega+=fl.get(j).get()*principle_components[i][j];
	for(j=0;j<number_of_features;j++) //X(i+1)=Xi-Oi*PCi -> PC(i+1)
    { 
 	 f.set(fl.get(j).get()-omega*principle_components[i][j]);
	 fl.set(j,f);
    }
   }

   tmp=0.0;
   for(i=0;i<number_of_features;i++)
   {
    tmp+=fl.get(i).get()*principle_components[pc][i];
   }
   result+=pow(tmp,2.0);
   more=data.get_next(fl);
  }
  return(1.0-result/n);
 }else return(1.0);
}
