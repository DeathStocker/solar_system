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

// Struct of a vector in Cartesian coordinates.
typedef struct _vector {
	double x;
	double y;
	double z;
} vector;

double find_slope(point_2d a, point_2d b);

double diff(double (*f)(double), double x, double delta);

double integrate(double (*f)(double), double x1, double x2, double delta);

vector cross(vector a, vector b);

double dot(vector a, vector b);

vector vector_add(vector a, vector b);

vector vector_minus(vector a, vector b);

vector scalar_prod(double scalar, vector a);

#endif
