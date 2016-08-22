#include "calc.h"

typedef struct _properties {
	int dim;
	double t;
	double delta_t;
	vector r;
	vector v;
	double k;
	double m;
} properties;

int vector_input(vector* a, int dim)
{
	vector temp;

	switch (dim) {
	case 3:
		printf("x = ");
		scanf("%lf", &temp.x);
	case 2:
		temp.x = 0;
		printf("y = ");
		scanf("%lf", &temp.y);
	case 1:
		temp.x = 0;
		temp.y = 0;
		printf("z = ");
		scanf("%lf", &temp.z);
	default:
		printf("Dimension is not 1, 2, or 3. Exitting.\n");
		return 1;
	}

	a->x = temp.z;
	a->y = temp.y;
	a->z = temp.x;

	return 0;
}

properties input_param()
{
	properties props;

	printf("Enter the dimension of the problem = ");
	scanf("%d", &props.dim);
	printf("Enter the final time (t) of the trajectory = ");
	scanf("%lf", &props.t);

	/*
	 * delta_t is being user defined here because of the application of the
	 * program which may range from a couple of millimeters (where smaller
	 * values of delta will be preferred) to several hundred kilometers
	 * (where only larger deltas of maybe a centimeter or meter will matter)
	 */
	printf("Enter delta_t = ");
	scanf("%lf", &props.delta_t);

	printf("Enter the initial position (r) of the particle = ");
	vector_input(&props.r, props.dim);
	printf("Enter the initial velocity (v) of the particle = ");
	vector_input(&props.v, props.dim);

	printf("Enter the spring constant (k) = ");
	scanf("%lf", &props.k);
	printf("Enter the mass (m) of the particle = ");
	scanf("%lf", &props.m);

	return props;
}

int main(int argc, char* argv[])
{
	properties props = input_param();
	double t = props.t;
	double delta_t = props.delta_t;

	FILE* fp = fopen("results.dat", "w");

	vector r = props.r;
	vector v = props.v;

	double k = props.k;
	double m = props.m;

	int i;
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
