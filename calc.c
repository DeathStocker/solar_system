#include "calc.h"

double find_slope(point_2d a, point_2d b)
{
	return (b.y - a.y) / (b.x - a.x);
}

double diff(double (*f)(double), double x, double delta)
{
	double x1 = x - delta;
	double x2 = x + delta;
	double y1 = f(x1);
	double y2 = f(x2);

	return (y2 - y1) / (x2 - x1);
}

double integrate(double (*f)(double), double x1, double x2, double delta)
{
	double i;
	double sum = 0.0;

	for (i = x1; i <= x2; i += delta) {
		double y = f(i);
		sum += y * delta;
	}

	return sum;
}

vector cross(vector a, vector b)
{
	vector res;
	res.x = (a.y * b.z) - (b.y * a.z);
	res.y = (a.x * b.z) - (b.x * a.z);
	res.z = (a.x * b.y) - (b.x * a.y);

	return res;
}

double dot(vector a, vector b)
{
	double res = (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
	return res;
}

vector vector_add(vector a, vector b)
{
	vector res;
	res.x = a.x + b.x;
	res.y = a.y + b.y;
	res.z = a.z + b.z;

	return res;
}

vector vector_minus(vector a, vector b)
{
	vector minus_b;
	minus_b.x = -b.x;
	minus_b.y = -b.y;
	minus_b.z = -b.z;

	vector res = vector_add(a, minus_b);

	return res;
}
