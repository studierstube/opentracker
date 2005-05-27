#ifndef __POWELL_H__
#define __POWELL_H__

void powell(float p[], float **xi, int n, float ftol, int *iter, float *fret,
	float (*func)(float []));

#endif
