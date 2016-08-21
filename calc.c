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

	return my_round(sum, 0);
}

double my_round(double x, unsigned int digits)
{
	double fac = pow(10, digits);

	return round(x * fac) / fac;
}
