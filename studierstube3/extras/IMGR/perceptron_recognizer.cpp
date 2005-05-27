#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "perceptron_recognizer.h"

template<class PROPERTY_LIST_TYPE,class FEATURE_TYPE,class FEATURE_LIST_TYPE>
perceptron_recognizer<PROPERTY_LIST_TYPE, FEATURE_TYPE, FEATURE_LIST_TYPE>::perceptron_recognizer(unsigned int noc)
:number_of_clusters(noc),clusters(MAX_FEATURES,MAX_CLUSTERS)
{
  int i,j;
  double sum,norm;

  number_of_features=FEATURE_LIST_TYPE().get_dim();
  cluster_ids=new unsigned int [MAX_CLUSTERS];
  srand( (unsigned)time( NULL ) );

  //random init. and normalize weights
  for (i=0;i<number_of_clusters;i++)
  {
	  sum=0.0;
	  for(j=0;j<number_of_features;j++)
	  {
		clusters[i][j]=(double)rand()/(double)RAND_MAX;
		sum+=pow(clusters[i][j],2.0);
	  }
	  norm=sqrt(sum);
	  for(j=0;j<number_of_features;j++)
		clusters[i][j]/=norm;
  }
	
   
}

template<class PROPERTY_LIST_TYPE,class FEATURE_TYPE,class FEATURE_LIST_TYPE>
perceptron_recognizer<PROPERTY_LIST_TYPE, FEATURE_TYPE, FEATURE_LIST_TYPE>::~perceptron_recognizer()
{
//	delete cluster_ids;
}

template<class PROPERTY_LIST_TYPE,class FEATURE_TYPE,class FEATURE_LIST_TYPE>
unsigned int perceptron_recognizer<PROPERTY_LIST_TYPE, FEATURE_TYPE, FEATURE_LIST_TYPE>::recognize(FEATURE_LIST_TYPE input_features)
{
	int i,j;
	unsigned int selected;
    double max,tmp;
	double sum,norm;
	feature<FEATURE_TYPE,PROPERTY_LIST_TYPE> f;

	//normalize input features
	sum=0.0;
    for(j=0;j<number_of_features;j++)
     sum+=pow(input_features.get(j).get(),2.0);
	norm=sqrt(sum);
    for(j=0;j<number_of_features;j++)
	{
		f.set(input_features.get(j).get()/norm);
		input_features.set(j,f);
	}

	if (number_of_clusters>0)
	{
	 selected = cluster_ids[0];
	 max=0.0;
     for(j=0;j<number_of_features;j++)
      max+=clusters[0][j]*input_features.get(j).get();

	 for (i=0;i<number_of_clusters;i++)
	 {
      tmp=0.0;
      for(j=0;j<number_of_features;j++)
       tmp+=clusters[i][j]*input_features.get(j).get();
	  if(tmp>=max)
	  {
	   max=tmp;
	   selected=cluster_ids[i];
	  }
	 }
	 return selected;
	}else
	 return(NOTHING_RECOGNIZED);
}

template<class PROPERTY_LIST_TYPE,class FEATURE_TYPE,class FEATURE_LIST_TYPE>
void perceptron_recognizer<PROPERTY_LIST_TYPE, FEATURE_TYPE, FEATURE_LIST_TYPE>::learn(unsigned int id,FEATURE_LIST_TYPE input_features)
{
	int i,j,pos;
	double sum,norm;
	feature<FEATURE_TYPE,PROPERTY_LIST_TYPE> f;

	//normalize input features
	sum=0.0;
    for(j=0;j<number_of_features;j++)
     sum+=pow(input_features.get(j).get(),2.0);
	norm=sqrt(sum);
    for(j=0;j<number_of_features;j++)
	{
		f.set(input_features.get(j).get()/norm);
		input_features.set(j,f);
	}

	//find position with cluster id
	pos=-1;
	for (i=0;i<number_of_clusters;i++)
     if(cluster_ids[i]==id) pos=i;
	
	if((pos==-1)&&(number_of_clusters<MAX_CLUSTERS)) //new cluster
	{
		pos=number_of_clusters;
        for(j=0;j<number_of_features;j++)
		 clusters[pos][j]=input_features.get(j).get();
		cluster_ids[pos]=id;
		number_of_clusters++;
	}
	else //correct and normalize
	{
	 sum=0.0;
	 for(j=0;j<number_of_features;j++)
	 {
      clusters[pos][j]+=input_features.get(j).get();
	  sum+=pow(clusters[pos][j],2.0);
	 }
	 norm=sqrt(sum);
     for(j=0;j<number_of_features;j++)
	  clusters[pos][j]/=norm;
	}
}

template<class PROPERTY_LIST_TYPE,class FEATURE_TYPE,class FEATURE_LIST_TYPE>
void perceptron_recognizer<PROPERTY_LIST_TYPE, FEATURE_TYPE, FEATURE_LIST_TYPE>::set_id(unsigned int cluster, unsigned int id)
{
	cluster_ids[cluster]=id;
}

template<class PROPERTY_LIST_TYPE,class FEATURE_TYPE,class FEATURE_LIST_TYPE>
void perceptron_recognizer<PROPERTY_LIST_TYPE, FEATURE_TYPE, FEATURE_LIST_TYPE>::train(void)
{
  FEATURE_LIST_TYPE fl;
  unsigned int gid;
  bool more;

  more=data.get_first(gid,fl);
  while(more==true)//over all learned gestures
	{	  
	  this->learn(gid,fl);
	  more=data.get_next(gid,fl); 
	}
}

template<class PROPERTY_LIST_TYPE,class FEATURE_TYPE,class FEATURE_LIST_TYPE>
void perceptron_recognizer<PROPERTY_LIST_TYPE, FEATURE_TYPE, FEATURE_LIST_TYPE>::find_clusters(void)
{
  FEATURE_LIST_TYPE fl;
  unsigned int gid;
  bool more;

  more=data.get_first(fl);
  while(more==true)//over all learned gestures
	{	 
	  gid=this->recognize(fl);
	  this->learn(gid,fl);
	  more=data.get_next(fl); 
	}
}

template<class PROPERTY_LIST_TYPE,class FEATURE_TYPE,class FEATURE_LIST_TYPE>
int perceptron_recognizer<PROPERTY_LIST_TYPE, FEATURE_TYPE, FEATURE_LIST_TYPE>::load(const char *fn)
{
	FILE *f;
	int j;
	
    if((f=fopen(fn,"r"))!=NULL)
	{
	 number_of_clusters=0;
     while (!feof(f))
	 {
	  fscanf(f,"%i\n",&(cluster_ids[number_of_clusters]));
	  for(j=0;j<number_of_features;j++)
		fscanf(f,"%lf ",&(clusters[number_of_clusters][j]));
	  fscanf(f,"\n");
	  if (number_of_clusters<MAX_CLUSTERS)
	   number_of_clusters++;
	 } 
	 fclose(f);
	 return(true);
	}else return(false);
}

template<class PROPERTY_LIST_TYPE,class FEATURE_TYPE,class FEATURE_LIST_TYPE>
int perceptron_recognizer<PROPERTY_LIST_TYPE, FEATURE_TYPE, FEATURE_LIST_TYPE>::save(const char *fn)
{
	FILE *f;
	int i,j;
	
    if((f=fopen(fn,"w"))!=NULL)
	{
     
     for (i=0;i<number_of_clusters;i++)
	 {
	  fprintf(f,"%i\n",cluster_ids[i]);
	  for(j=0;j<number_of_features;j++)
		fprintf(f,"%lf ",clusters[i][j]);
	  fprintf(f,"\n");
	 } 
	 fclose(f);
	 return(true);
	}else return(false);
}

