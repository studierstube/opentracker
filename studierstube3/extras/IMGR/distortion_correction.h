#ifndef DISTORTION_CORRECTION_INCLUDED
#define DISTORTION_CORRECTION_INCLUDED

#include <stdio.h>

#define NEIGHBOURHOOD_RADIUS 1
#define LEARNING_CONSTANT	 1.0


template <class COMPONENT_TYPE, unsigned int RESOLUTION>
class distortion_correction
{
public:
	distortion_correction();
	~distortion_correction();
	void set_min_boundary (unsigned int dim, COMPONENT_TYPE val);
    void set_max_boundary (unsigned int dim, COMPONENT_TYPE val);
	COMPONENT_TYPE get_min_boundary (unsigned int dim);
    COMPONENT_TYPE get_max_boundary (unsigned int dim);
	COMPONENT_TYPE get_original_point  (unsigned int x, unsigned int y, unsigned int z, unsigned int dim);
	COMPONENT_TYPE get_distorted_point (unsigned int x, unsigned int y, unsigned int z, unsigned int dim);
	unsigned int get_number_of_samples (void);
	unsigned int get_resolution (void);
	void init(void);
	void update(COMPONENT_TYPE x, COMPONENT_TYPE y, COMPONENT_TYPE z);
    void initialize_point(unsigned int px, unsigned int py, unsigned int pz, 
		                  COMPONENT_TYPE dx, COMPONENT_TYPE dy, COMPONENT_TYPE dz);
	void load_distorted_points(const char *fn);
	void save_distorted_points(const char *fn);
	void correct(COMPONENT_TYPE *x, COMPONENT_TYPE *y, COMPONENT_TYPE *z);
	void init_measurement (const char *fn);
	void measure (COMPONENT_TYPE x, COMPONENT_TYPE y, COMPONENT_TYPE z);
	void close_measurement (void);
	void init_training (const char *fn);
    void train (void);
	void close_training (void);

protected:
	COMPONENT_TYPE	original_cloud	[RESOLUTION][RESOLUTION][RESOLUTION][3];
	COMPONENT_TYPE  distorted_cloud [RESOLUTION][RESOLUTION][RESOLUTION][3];
	COMPONENT_TYPE	min[3];
	COMPONENT_TYPE	max[3];
	unsigned long number_of_samples;
	int radius;
	double learning_c;
	FILE *measurement;
	COMPONENT_TYPE *measurements_x;
    COMPONENT_TYPE *measurements_y;
	COMPONENT_TYPE *measurements_z;
	unsigned long number_of_measurements;

    double neighbourhood_function (unsigned int ix, unsigned int iy, unsigned int iz,
		                           unsigned int kx, unsigned int ky, unsigned int kz);
};

#include "distortion_correction.cpp"
#endif