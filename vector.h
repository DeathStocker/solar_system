#ifndef VECTOR_H
#define VECTOR_H

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

// Struct of a vector in Cartesian coordinates.
typedef struct _vector {
	double x;
	double y;
	double z;
} vector;

vector cross(vector a, vector b);

double dot(vector a, vector b);

vector vector_add(vector a, vector b);

vector vector_minus(vector a, vector b);

vector scalar_prod(double scalar, vector a);

double mod_vector(vector a);

vector unit_vector(vector a);

#endif
