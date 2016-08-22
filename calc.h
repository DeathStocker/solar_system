#ifndef CALC_H
#define CALC_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Struct for a point in 2-D plane in Cartesian coordinates.
typedef struct pt_2d {
	double x;
	double y;
} point_2d;

double find_slope(point_2d a, point_2d b);

double diff(double (*f)(double), double x, double delta);

double integrate(double (*f)(double), double x1, double x2, double delta);

#endif
