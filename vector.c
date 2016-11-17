#include "vector.h"

/*
   Gives the cross product between two vectors.
 */
vector cross(vector a, vector b)
{
	vector res;

	res.x = (a.y * b.z) - (b.y * a.z);
	res.y = (a.x * b.z) - (b.x * a.z);
	res.z = (a.x * b.y) - (b.x * a.y);

	return res;
}

/*
   Gives the dot product between two vectors.
 */
double dot(vector a, vector b)
{
	double res = (a.x * b.x) + (a.y * b.y) + (a.z * b.z);

	return res;
}

/*
   Gives the vector sum of two vectors.
 */
vector vector_add(vector a, vector b)
{
	vector res;

	res.x = a.x + b.x;
	res.y = a.y + b.y;
	res.z = a.z + b.z;

	return res;
}

/*
   Gives the vector subtraction of two vectors.
 */
vector vector_minus(vector a, vector b)
{
	vector minus_b;

	minus_b.x = -b.x;
	minus_b.y = -b.y;
	minus_b.z = -b.z;

	vector res = vector_add(a, minus_b);

	return res;
}

/*
   Gives the resultant of the product between a scalar and a vector.
 */
vector scalar_prod(double scalar, vector a)
{
	vector res;

	res.x = scalar * a.x;
	res.y = scalar * a.y;
	res.z = scalar * a.z;

	return res;
}

/*
   Calculates the mod of the vector.
 */
double mod_vector(vector a)
{
	double res = a.x * a.x + a.y * a.y + a.z * a.z;

	res = sqrt(res);

	return res;
}

/*
   Give the unit vector r^ in the direction of the vector r.
 */
vector unit_vector(vector a)
{
	vector res;
	double mod = mod_vector(a);
	mod = 1.0 / mod;

	res = scalar_prod(mod, a);

	return res;
}
