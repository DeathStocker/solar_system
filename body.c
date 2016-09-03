#include "calc.h"
#include "vector.h"
#include <time.h>

#define DIMENSION 3
#define DELTA_T 100
#define TIME 47336400
#define K 1
#define G 6.67E-11
#define NUMBER_OF_BODIES 2

typedef struct _state {
	vector r;
	vector v;
} state;

typedef struct _body {
	double mass;
	vector prev_r;
	vector prev_v;
} body;

int vector_input(vector* a, int dim)
{
	double arr[3];

	arr[0] = 0.0;
	arr[1] = 0.0;
	arr[2] = 0.0;

	int i = 0;

	switch (dim) {
	case 3:
		/*
		   Custom magic formula to print the respective coordinate
		   correctly without changing/adding any extra lines of code.
		 */
		printf("\t%c = ", (char)('y' + dim - 4));
		scanf("%lf", &arr[i++]);
	case 2:
		printf("\t%c = ", (char)('x' + dim - 2));
		scanf("%lf", &arr[i++]);
	case 1:
		printf("\t%c = ", (char)('w' + dim));
		scanf("%lf", &arr[i++]);
		break;
	default:
		printf("Dimension is not 1, 2, or 3. Exitting.\n");
		return 1;
	}

	a->x = arr[0];
	a->y = arr[1];
	a->z = arr[2];

	return 0;
}

int input_param(state* st, body* particle)
{
	int i;

	for (i = 0; i < NUMBER_OF_BODIES; i++) {
		printf("Particle [%d] - \n", (i + 1));
		printf("\tEnter the mass = ");
		scanf("%le", &particle[i].mass);
		printf("\tEnter the initial position (r) of the particle = \n");
		vector_input(&st[i].r, DIMENSION);
		printf("\tEnter the initial velocity (v) of the particle = \n");
		vector_input(&st[i].v, DIMENSION);
		particle[i].prev_r = st[i].r;
		particle[i].prev_v = st[i].v;
	}

	return 0;
}

vector gravitational_force(double m1, double m2, vector r1, vector r2)
{
	vector r = vector_minus(r2, r1);
	double constants = G * m1 * m2 / pow(mod_vector(r), 2);
	vector force = scalar_prod(constants, unit_vector(r));

	return force;
}

int vel_ver(state* st, body* particle, FILE* fp1, FILE* fp2, int no_of_iter)
{
	clock_t t;
	double m1 = particle[0].mass;
	double m2 = particle[1].mass;

	vector r1 = st[0].r;
	vector r2 = st[1].r;

	vector v1 = st[0].v;
	vector v2 = st[1].v;

	int i;
	double t_count = DELTA_T;

	t = clock();
	for (i = 0; i < no_of_iter; i++) {

		vector f1 = gravitational_force(m1, m2, r1, r2);                // F = -kx
		/*
		   Vector form of x = x + (delta_t * v) + (f1 * delta_t ^ 2) / 2 * m
		 */
		vector v_part = scalar_prod(DELTA_T, v1);                       // Velocity part of the equation
		vector f_part = scalar_prod(pow(DELTA_T, 2) / (m1 * 2.0), f1);
		// Force part of the equation
		r1 = vector_add(r1, v_part);
		r1 = vector_add(r1, f_part);

		vector f2 = gravitational_force(m1, m2, r1, r2);                // F = -kx
		/*
		   Vector form of v = v + (delta_t * (f1 + f2) / (2 * m))
		 */
		double scal_part = DELTA_T / (m1 * 2.0);                        // Scalar part of the equation
		v1 = vector_add(v1, scalar_prod(scal_part, vector_add(f1, f2)));


		f1 = gravitational_force(m2, m1, r2, r1);                       // F = -kx
		/*
		   Vector form of x = x + (delta_t * v) + (f1 * delta_t ^ 2) / 2 * m
		 */
		v_part = scalar_prod(DELTA_T, v2);                              // Velocity part of the equation
		f_part = scalar_prod(pow(DELTA_T, 2) / (m2 * 2.0), f1);
		// Force part of the equation
		r2 = vector_add(r2, v_part);
		r2 = vector_add(r2, f_part);

		f2 = gravitational_force(m2, m1, r2, r1);                       // F = -kx
		/*
		   Vector form of v = v + (delta_t * (f1 + f2) / (2 * m))
		 */
		scal_part = DELTA_T / (m2 * 2.0);                               // Scalar part of the equation
		v2 = vector_add(v2, scalar_prod(scal_part, vector_add(f1, f2)));

		//vector relative = vector_minus(r2, r1);
		// Printing position coordinates to file.
		fprintf(fp1, "%lf %lf %lf %lf\n", t_count, r1.x, r1.y, r1.z);
		fprintf(fp2, "%lf %lf %lf %lf\n", t_count, r2.x, r2.y, r2.z);

		t_count += DELTA_T;
	}
	t = clock() - t;
	double time_taken = ((double)t) / CLOCKS_PER_SEC * 1000;
	// Printing the final position of the particle.
	printf("The final position (xf) of 1st particle is = "
	       "%lfi + %lfj + %lfk\n", r1.x, r1.y, r1.z);
	printf("The final position (xf) of 2nd particle is = "
	       "%lfi + %lfj + %lfk\n", r2.x, r2.y, r2.z);
	printf("Time taken for vel_ver = %lfms\n", time_taken);
	return 0;
}

int main()
{
	state st[NUMBER_OF_BODIES];
	body particle[NUMBER_OF_BODIES];


	input_param(st, particle);

	FILE* fp1 = fopen("body1.dat", "w");
	FILE* fp2 = fopen("body2.dat", "w");

	int no_of_iter = TIME / DELTA_T;

	vel_ver(st, particle, fp1, fp2, no_of_iter);

	free(fp1);
	free(fp2);

	return 0;
}
