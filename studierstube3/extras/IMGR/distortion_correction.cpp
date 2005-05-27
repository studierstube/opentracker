#include "distortion_correction.h"
#include <time.h>

template <class COMPONENT_TYPE, unsigned int RESOLUTION> 
distortion_correction<COMPONENT_TYPE,RESOLUTION>::distortion_correction()
:number_of_samples(0)
{
	radius=NEIGHBOURHOOD_RADIUS;
	learning_c=LEARNING_CONSTANT;
}

template <class COMPONENT_TYPE, unsigned int RESOLUTION> 
distortion_correction<COMPONENT_TYPE, RESOLUTION>::~distortion_correction()
{
}


template <class COMPONENT_TYPE, unsigned int RESOLUTION> 
void distortion_correction<COMPONENT_TYPE,RESOLUTION>::set_min_boundary (unsigned int dim, COMPONENT_TYPE val)
{
	if ((dim<3)&&(dim>=0))
	 min[dim]=val;
}

template <class COMPONENT_TYPE, unsigned int RESOLUTION> 
void distortion_correction<COMPONENT_TYPE,RESOLUTION>::set_max_boundary (unsigned int dim, COMPONENT_TYPE val)
{
	if ((dim<3)&&(dim>=0))
	 max[dim]=val;
}

template <class COMPONENT_TYPE, unsigned int RESOLUTION> 
COMPONENT_TYPE distortion_correction<COMPONENT_TYPE, RESOLUTION>::get_min_boundary (unsigned int dim)
{
	return(min[dim]);
}

template <class COMPONENT_TYPE, unsigned int RESOLUTION> 
COMPONENT_TYPE distortion_correction<COMPONENT_TYPE,RESOLUTION>::get_max_boundary (unsigned int dim)
{
	return (max[dim]);
}

template <class COMPONENT_TYPE, unsigned int RESOLUTION> 
void distortion_correction<COMPONENT_TYPE,RESOLUTION>::init(void)
{
	unsigned int x,y,z;
    
	if (RESOLUTION>0)
	{
     for (x=0;x<RESOLUTION;x++)
	 	 for (y=0;y<RESOLUTION;y++)
			 for (z=0;z<RESOLUTION;z++)
			 {
               original_cloud[x][y][z][0]=min[0]+x*(max[0]-min[0])/(RESOLUTION-1);
               original_cloud[x][y][z][1]=min[1]+y*(max[1]-min[1])/(RESOLUTION-1);
			   original_cloud[x][y][z][2]=min[2]+z*(max[2]-min[2])/(RESOLUTION-1);
			   distorted_cloud[x][y][z][0]=min[0]+x*(max[0]-min[0])/(RESOLUTION-1);
               distorted_cloud[x][y][z][1]=min[1]+y*(max[1]-min[1])/(RESOLUTION-1);
			   distorted_cloud[x][y][z][2]=min[2]+z*(max[2]-min[2])/(RESOLUTION-1);
			 }
	 number_of_samples=0;
	}
	
}

template <class COMPONENT_TYPE, unsigned int RESOLUTION> 
COMPONENT_TYPE distortion_correction<COMPONENT_TYPE, RESOLUTION>::get_original_point  (unsigned int x, unsigned int y, unsigned int z, unsigned int dim)
{
	return (original_cloud[x][y][z][dim]);
}

template <class COMPONENT_TYPE, unsigned int RESOLUTION> 
COMPONENT_TYPE distortion_correction<COMPONENT_TYPE, RESOLUTION>::get_distorted_point (unsigned int x, unsigned int y, unsigned int z, unsigned int dim)
{
	return (distorted_cloud[x][y][z][dim]);
}

template <class COMPONENT_TYPE, unsigned int RESOLUTION> 
unsigned int distortion_correction<COMPONENT_TYPE, RESOLUTION>::get_number_of_samples (void)
{
	return(number_of_samples);
}

template <class COMPONENT_TYPE, unsigned int RESOLUTION> 
unsigned int distortion_correction<COMPONENT_TYPE, RESOLUTION>::get_resolution (void)
{
	return(RESOLUTION);
}


template <class COMPONENT_TYPE, unsigned int RESOLUTION> 
double distortion_correction<COMPONENT_TYPE, RESOLUTION>::neighbourhood_function(unsigned int ix, unsigned int iy, unsigned int iz,
																				 unsigned int kx, unsigned int ky, unsigned int kz)
{
		unsigned int d1,d2,d3;
        unsigned int max;

		d1=abs(ix-kx);
		d2=abs(iy-ky);
		d3=abs(iz-kz);

		max=d1;
		if (d2>max) max=d2;
		if (d3>max) max=d3;

		return ((double)(radius+1-max)/(double)(radius+1));
}

template <class COMPONENT_TYPE, unsigned int RESOLUTION> 
void distortion_correction<COMPONENT_TYPE, RESOLUTION>::update(COMPONENT_TYPE x, COMPONENT_TYPE y, COMPONENT_TYPE z)
{
	int a,b,c;
	int min_x,min_y,min_z;
	double min_dist,current_dist;
    double factor;

	//find neuron with highest excitation
    min_x=0;min_y=0;min_z=0;
	min_dist=(distorted_cloud[0][0][0][0]-x)*(distorted_cloud[0][0][0][0]-x)+
             (distorted_cloud[0][0][0][1]-y)*(distorted_cloud[0][0][0][1]-y)+
			 (distorted_cloud[0][0][0][2]-z)*(distorted_cloud[0][0][0][2]-z);
              
	for (a=0;a<RESOLUTION;a++)
		for (b=0;b<RESOLUTION;b++)
			for (c=0;c<RESOLUTION;c++)
			{
			 current_dist=(distorted_cloud[a][b][c][0]-x)*(distorted_cloud[a][b][c][0]-x)+
					      (distorted_cloud[a][b][c][1]-y)*(distorted_cloud[a][b][c][1]-y)+
					      (distorted_cloud[a][b][c][2]-z)*(distorted_cloud[a][b][c][2]-z);	
			 if (current_dist<min_dist)
			 {
				 min_x=a;
				 min_y=b;
				 min_z=c;
				 min_dist=current_dist;
			 }
			}
 

	//update neighbourhood
    for (a=min_x-radius;a<min_x+radius+1;a++)
     for (b=min_y-radius;b<min_y+radius+1;b++)
	  for (c=min_z-radius;c<min_z+radius+1;c++)
       {
       //clip boundaries
	   if ((a>=0)&&(a<RESOLUTION)&&(b>=0)&&(b<RESOLUTION)&&(c>=0)&&(c<RESOLUTION))
	   {
	     factor=learning_c*neighbourhood_function(min_x,min_y,min_z,a,b,c);
         distorted_cloud[a][b][c][0]+=factor*(x-distorted_cloud[a][b][c][0]);
         distorted_cloud[a][b][c][1]+=factor*(y-distorted_cloud[a][b][c][1]);
		 distorted_cloud[a][b][c][2]+=factor*(z-distorted_cloud[a][b][c][2]); 	 
	   }
	  }

	  number_of_samples++;
}

template <class COMPONENT_TYPE, unsigned int RESOLUTION> 
void distortion_correction<COMPONENT_TYPE, RESOLUTION>::initialize_point(unsigned int px, unsigned int py, unsigned int pz,
															             COMPONENT_TYPE dx, COMPONENT_TYPE dy, COMPONENT_TYPE dz)
{
	int a,b,c;
    double factor;

	//update neighbourhood
    for (a=px-radius;a<px+radius+1;a++)
     for (b=py-radius;b<py+radius+1;b++)
	  for (c=pz-radius;c<pz+radius+1;c++)
       {
       //clip boundaries
	   if ((a>=0)&&(a<RESOLUTION)&&(b>=0)&&(b<RESOLUTION)&&(c>=0)&&(c<RESOLUTION))
	   {
	     factor=learning_c*neighbourhood_function(px,py,pz,a,b,c);
         distorted_cloud[a][b][c][0]+=factor*(dx-distorted_cloud[a][b][c][0]);
         distorted_cloud[a][b][c][1]+=factor*(dy-distorted_cloud[a][b][c][1]);
		 distorted_cloud[a][b][c][2]+=factor*(dz-distorted_cloud[a][b][c][2]); 	 
	   }
	  }

}

template <class COMPONENT_TYPE, unsigned int RESOLUTION> 
void distortion_correction<COMPONENT_TYPE, RESOLUTION>::load_distorted_points(const char *fn)
{
	FILE *f;
    int x,y,z;
	double a,b,c;

    if((f=fopen(fn,"r"))!=NULL)
	{
     for (x=0;x<RESOLUTION;x++)
	  for (y=0;y<RESOLUTION;y++)
	   for (z=0;z<RESOLUTION;z++)
	   {
        fscanf(f,"%lf %lf %lf \n",&a,&b,&c);
		distorted_cloud[x][y][z][0]=(COMPONENT_TYPE)a;
		distorted_cloud[x][y][z][1]=(COMPONENT_TYPE)b;
	    distorted_cloud[x][y][z][2]=(COMPONENT_TYPE)c;
	   }

	 fclose(f);
    }
}

template <class COMPONENT_TYPE, unsigned int RESOLUTION> 
void distortion_correction<COMPONENT_TYPE, RESOLUTION>::save_distorted_points(const char *fn)
{
	FILE *f;
    int x,y,z;

    if((f=fopen(fn,"w"))!=NULL)
	{
     for (x=0;x<RESOLUTION;x++)
	  for (y=0;y<RESOLUTION;y++)
	   for (z=0;z<RESOLUTION;z++)
        fprintf(f,"%lf %lf %lf \n",(double)distorted_cloud[x][y][z][0],(double)distorted_cloud[x][y][z][1],(double)distorted_cloud[x][y][z][2]);
	 fclose(f);
    }
}

template <class COMPONENT_TYPE, unsigned int RESOLUTION> 
void distortion_correction<COMPONENT_TYPE, RESOLUTION>::correct(COMPONENT_TYPE *x, COMPONENT_TYPE *y, COMPONENT_TYPE *z)
{
	int a,b,c;
	int min_x,min_y,min_z;
	
	double min_dist,current_dist;
    double factor;

	//find neuron with highest excitation
    min_x=0;min_y=0;min_z=0;
	min_dist=(distorted_cloud[0][0][0][0]-*x)*(distorted_cloud[0][0][0][0]-*x)+
             (distorted_cloud[0][0][0][1]-*y)*(distorted_cloud[0][0][0][1]-*y)+
			 (distorted_cloud[0][0][0][2]-*z)*(distorted_cloud[0][0][0][2]-*z);
              
	for (a=0;a<RESOLUTION;a++)
		for (b=0;b<RESOLUTION;b++)
			for (c=0;c<RESOLUTION;c++)
			{
			 current_dist=(distorted_cloud[a][b][c][0]-*x)*(distorted_cloud[a][b][c][0]-*x)+
					      (distorted_cloud[a][b][c][1]-*y)*(distorted_cloud[a][b][c][1]-*y)+
					      (distorted_cloud[a][b][c][2]-*z)*(distorted_cloud[a][b][c][2]-*z);	
			 if (current_dist<min_dist)
			 {
				 min_x=a;
				 min_y=b;
				 min_z=c;
				 min_dist=current_dist;
			 }
			}
 

	//correct sample
    (*x)+=(original_cloud[min_x][min_y][min_z][0]-distorted_cloud[min_x][min_y][min_z][0]);
	(*y)+=(original_cloud[min_x][min_y][min_z][1]-distorted_cloud[min_x][min_y][min_z][1]);
	(*z)+=(original_cloud[min_x][min_y][min_z][2]-distorted_cloud[min_x][min_y][min_z][2]);
}

template <class COMPONENT_TYPE, unsigned int RESOLUTION> 
void distortion_correction<COMPONENT_TYPE, RESOLUTION>::init_measurement (const char *fn)
{
 measurement=fopen(fn,"w");	
 number_of_measurements=0;
}

template <class COMPONENT_TYPE, unsigned int RESOLUTION> 
void distortion_correction<COMPONENT_TYPE, RESOLUTION>::measure (COMPONENT_TYPE x, COMPONENT_TYPE y, COMPONENT_TYPE z)
{
 fprintf(measurement,"%lf\n%lf\n%lf\n",(double)x,(double)y,(double)z);
 number_of_measurements++;
}

template <class COMPONENT_TYPE, unsigned int RESOLUTION> 
void distortion_correction<COMPONENT_TYPE, RESOLUTION>::close_measurement (void)
{
 fprintf(measurement,"%lf\n",(double)number_of_measurements);
 fclose(measurement);
 number_of_measurements=0;
}

template <class COMPONENT_TYPE, unsigned int RESOLUTION> 
void distortion_correction<COMPONENT_TYPE, RESOLUTION>::init_training (const char *fn)
{
 int i;
 double nos;
 double a,b,c;

 measurement=fopen(fn,"r");
 number_of_measurements=0;
 while(!feof(measurement))
	 fscanf(measurement,"%lf\n",&nos);
 fclose(measurement);

 number_of_measurements=(unsigned long)nos;

 measurements_x=new COMPONENT_TYPE [number_of_measurements];
 measurements_y=new COMPONENT_TYPE [number_of_measurements];
 measurements_z=new COMPONENT_TYPE [number_of_measurements];

 measurement=fopen(fn,"r");
 for (i=0;i<number_of_measurements;i++)
 {
  fscanf(measurement,"%lf\n%lf\n%lf\n",&a,&b,&c);
  measurements_x[i]=(COMPONENT_TYPE)a;
  measurements_y[i]=(COMPONENT_TYPE)b;
  measurements_z[i]=(COMPONENT_TYPE)c;
 }

 fclose(measurement);
 srand( (unsigned)time( NULL ) );
}

template <class COMPONENT_TYPE, unsigned int RESOLUTION> 
void distortion_correction<COMPONENT_TYPE, RESOLUTION>::train (void)
{
  unsigned long index;
  
   index=(unsigned long)(((double)rand()+1.0)/(double)RAND_MAX*number_of_measurements);
   update(measurements_x[index],measurements_y[index],measurements_z[index]);

}

template <class COMPONENT_TYPE, unsigned int RESOLUTION> 
void distortion_correction<COMPONENT_TYPE, RESOLUTION>::close_training (void)
{
	delete (measurements_x);
	delete (measurements_y);
	delete (measurements_z);
}