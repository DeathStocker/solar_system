#include "calc.h"
#include "vector.h"

#define DIMENSION 3
#define DELTA_T 0.001
#define TIME 10
#define K 1
#define G 1
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
		scanf("%lf", &particle[i].mass);
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
	vector r = vector_minus(r1, r2);
	double constants = G * m1 * m2 / pow(mod_vector(r), 2);
	vector force = scalar_prod(constants, unit_vector(r));

	return force;
}

state vel_ver_part_1(vector f1, double m, vector* r, vector* v)
{
	/*
	   Vector form of x = x + (delta_t * v) + (f1 * delta_t ^ 2) / 2 * m
	 */
	vector v_part = scalar_prod(DELTA_T, *v);        // Velocity part of the equation
	vector f_part = scalar_prod(pow(DELTA_T, 2) / (m * 2.0), f1);
	// Force part of the equation
	*r = vector_add(*r, v_part);
	*r = vector_add(*r, f_part);

	return ;
}

int vel_ver_part_2(vector f1, vector f2, double m, vector* r, vector* v)
{
	/*
	   Vector form of v = v + (delta_t * (f1 + f2) / (2 * m))
	 */
	double scal_part = DELTA_T / (m * 2.0); // Scalar part of the equation
	*v = vector_add(*v, scalar_prod(scal_part, vector_add(f1, f2)));

	return
}

int simulate(state* st, body* particle, FILE* fp)
{
	double m1 = particle[0].mass;
	double m2 = particle[1].mass;

	vector r1 = st[0].r;
	vector r2 = st[1].r;

	vector v1 = st[0].v;
	vector v2 = st[0].v;

	int i;
	double t_count = DELTA_T;
	int no_of_iter = TIME / DELTA_T;

	// Calculating the position and velocity of the particle at ith iterations.
	for (i = 0; i < no_of_iter; i++) {

		vector f1 = gravitational_force(m1, m2, r1, r2);                 // F = -kx

		vector r = vel_ver_part_1(f1, m1, &r1, &v1);
		r = vel_ver_part_2()

		vector f2 = scalar_prod(-k, r);         // F = -kx
		/*
		   Vector form of v = v + (delta_t * (f1 + f2) / (2 * m))
		 */
		double scal_part = delta_t / (m * 2.0); // Scalar part of the equation
		v = vector_add(v, scalar_prod(scal_part, vector_add(f1, f2)));

		// Printing position coordinates to file.
		fprintf(fp, "%lf %lf %lf %lf\n", t_count, r.x, r.y, r.z);

		t_count += DELTA_T;
	}

	// Printing the final position of the particle.
	printf("The final position (xf) of the particle is = "
	       "%lfi + %lfj + %lfk\n", r.x, r.y, r.z);
}

int main()
{
	state* st = malloc(NUMBER_OF_BODIES * sizeof(state*));
	body* particle = malloc(NUMBER_OF_BODIES * sizeof(body*));

	input_param(st, particle);
	FILE* fp = fopen("results.dat", "w");

	simulate(st, particle, fp);

	return 0;
}
