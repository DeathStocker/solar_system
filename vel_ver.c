#include "calc.h"

int main(int argc, char* argv[])
{
	double t;
	double delta_t;

	// User input.
	printf("Enter the final time (t) of the trajectory = ");
	scanf("%lf", &t);

	/*
	 * delta_t is being user defined here because of the application of the
	 * program which may range from a couple of millimeters (where smaller
	 * values of delta will be preferred) to several hundred kilometers
	 * (where only larger deltas of maybe a centimeter or meter will matter)
	 */
	printf("Enter delta_t = ");
	scanf("%lf", &delta_t);

	double x0, v0;
	printf("Enter the initial position (x0) of the particle = ");
	scanf("%lf", &x0);
	printf("Enter the initial velocity (v0) of the particle = ");
	scanf("%lf", &v0);

	double k, m;
	printf("Enter the spring constant (k) = ");
	scanf("%lf", &k);
	printf("Enter the mass (m) of the particle = ");
	scanf("%lf", &m);

	FILE* fp = fopen("results.dat", "w");

	int i;
	double x = x0;
	double v = v0;
	double t_count = delta_t;
	int no_of_iter = t / delta_t;

	// Calculating the position and velocity of the particle at ith iterations.
	for (i = 0; i < no_of_iter; i++) {
		double f1 = -(k * x);
		x = x + (v * delta_t) + (f1 * pow(delta_t, 2)) / (m * 2.0);
		double f2 = -(k * x);
		v = v + ((f1 + f2) * delta_t / (m * 2.0));

		fprintf(fp, "%lf %lf\n", t_count, x);

		t_count += delta_t;
	}

	// Printing the final position of the particle.
	printf("The final position (xf) of the particle is = %lf\n", x);

	return 0;
}
