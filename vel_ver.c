#include "calc.h"
#include "vector.h"

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

	temp.x = 0;
	temp.y = 0;
	temp.z = 0;

	switch (dim) {
	case 3:
		/*
		   Custom magic formula to print the respective coordinate
		   correctly without changing/adding any extra lines of code.
		 */
		printf("%c = ", (char)('y' + dim - 4));
		scanf("%lf", &temp.x);
	case 2:
		printf("%c = ", (char)('x' + dim - 2));
		scanf("%lf", &temp.y);
	case 1:
		printf("%c = ", (char)('w' + dim));
		scanf("%lf", &temp.z);
		break;
	default:
		printf("Dimension is not 1, 2, or 3. Exitting.\n");
		return 1;
	}

	a->x = temp.x;
	a->y = temp.y;
	a->z = temp.z;

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

		vector f1 = scalar_prod(-k, r);                 // F = -kx
		/*
		   Vector form of x = x + (delta_t * v) + (f1 * delta_t ^ 2) / 2 * m
		 */
		vector v_part = scalar_prod(delta_t, v);        // Velocity part of the equation
		vector f_part = scalar_prod(pow(delta_t, 2) / (m * 2.0), f1);
		// Force part of the equation
		r = vector_add(r, v_part);
		r = vector_add(r, f_part);

		vector f2 = scalar_prod(-k, r); // F = -kx
		/*
		   Vector form of v = v + (delta_t * (f1 + f2) / (2 * m))
		 */
		double scal_part = delta_t / (m * 2.0);
		v = vector_add(v, scalar_prod(scal_part, vector_add(f1, f2)));

		// Printing position coordinates to file.
		fprintf(fp, "%lf %lf %lf %lf\n", t_count, r.x, r.y, r.z);

		t_count += delta_t;
	}

	// Printing the final position of the particle.
	printf("The final position (xf) of the particle is = "
	       "%lfi + %lfj + %lfk\n", r.x, r.y, r.z);

	return 0;
}
