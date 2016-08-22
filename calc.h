#ifndef CALC_H
#define CALC_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct pt_2d {
	double x;
	double y;
} point_2d;

// Struct of a vector in Cartesian coordinates.
typedef struct _vector {
	double x;
	double y;
	double z;
} vector;

double find_slope(point_2d a, point_2d b);

double diff(double (*f)(double), double x, double delta);

double integrate(double (*f)(double), double x1, double x2, double delta);

double my_round(double x, unsigned int digits);

#endif
