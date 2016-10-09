#include "calc.h"
#include "vector.h"
#include <time.h>
#include <string.h>

#define DIMENSION 3
#define DELTA_T 10
#define TIME 47336400
#define K 1
#define G 6.67E-11
#define NUMBER_OF_BODIES 3

typedef struct _state {
	vector r;
	vector v;
} state;

typedef struct _body {
	double mass;
	vector prev_r;
	vector prev_v;
} body;

FILE** create_output_files(int N)
{
	FILE** fp = malloc(N * sizeof(FILE*));
	char file_name[128] = "body";
	char file_extension[8] = ".dat";
	int i;

	for (i = 0; i < N; i++) {
		char name[256];
		strcpy(name, file_name);
		char file_num[128];
		sprintf(file_num, "%d", i);
		strcat(name, file_num);
		strcat(name, file_extension);
		fp[i] = fopen(name, "w");
	}

	return fp;
}

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

vector net_gravitational_force(body* bd, int i, state* st, int N)
{
	int j;
	vector grav;
	grav.x = 0;
	grav.y = 0;
	grav.z = 0;

	for (j = 0; j < N; j++) {
		if (j == i)
			continue;
		grav = vector_add(gravitational_force(bd[i].mass, bd[j].mass, st[i].r, st[j].r), grav);
	}

	return grav;
}

state vel_ver_diff_eq(state* st, body* bd, int i, int N)
{
	vector r = st[i].r;
	vector v = st[i].v;
	vector f1 = net_gravitational_force(bd, i, st, N);              // F = -kx
	/*
	   Vector form of x = x + (delta_t * v) + (f1 * delta_t ^ 2) / 2 * m
	 */
	vector v_part = scalar_prod(DELTA_T, v);                        // Velocity part of the equation
	vector f_part = scalar_prod(pow(DELTA_T, 2) / (bd[i].mass * 2.0), f1);

	// Force part of the equation
	r = vector_add(r, v_part);
	r = vector_add(r, f_part);

	st[i].r = r;

	vector f2 = net_gravitational_force(bd, i, st, N);                      // F = -kx
	/*
	   Vector form of v = v + (delta_t * (f1 + f2) / (2 * m))
	 */
	double scal_part = DELTA_T / (bd[i].mass * 2.0);                        // Scalar part of the equation
	v = vector_add(v, scalar_prod(scal_part, vector_add(f1, f2)));

	st[i].v = v;

	state res;
	res.r = r;
	res.v = v;

	return res;
}

int vel_ver(state* st, body* particle, FILE** fp, int no_of_iter)
{
	clock_t t;
	// double m1 = particle[0].mass;
	// double m2 = particle[1].mass;

	// vector r1 = st[0].r;
	// vector r2 = st[1].r;

	// vector v1 = st[0].v;
	// vector v2 = st[1].v;

	// vector init_pos = r2;

	// vector init_sun = r1;

	int i, j;
	double t_count = DELTA_T;

	// int flag = 0;

	vector err;

	err.x = 10000;
	err.y = 10000;
	err.z = 10000;

	err = (vector)err;

	// double tp = 0.0;

	/////////////////////////////////////

	// double Ei = 0.5 * m1 * pow(mod_vector(v1), 2) + 0.5 * m2 * pow(mod_vector(v2), 2) - G * m1 * m2 / mod_vector(vector_minus(r2, r1));

	/////////////////////////////////////

	// printf("The initial position of 2nd particle is = "
	//        "%lfi + %lfj + %lfk\n", init_pos.x, init_pos.y, init_pos.z);

	t = clock();

	for (i = 0; i < no_of_iter; i++) {
		for (j = 0; j < NUMBER_OF_BODIES; j++) {
			state next = vel_ver_diff_eq(st, particle, j, NUMBER_OF_BODIES);
			particle[j].prev_r = next.r;
			particle[j].prev_v = next.v;
			st[j].r = next.r;
			st[j].v = next.v;
			vector r1 = next.r;

			if (i % 1000 == 0)
				fprintf(fp[j], "%lf %lf %lf %lf\n", t_count, r1.x, r1.y, r1.z);
			 //
			// vector f1 = gravitational_force(m2, m1, r2, r1);                        // F = -kx
			// /*
			//    Vector form of x = x + (delta_t * v) + (f1 * delta_t ^ 2) / 2 * m
			//  */
			// vector v_part = scalar_prod(DELTA_T, v2);                               // Velocity part of the equation
			// vector f_part = scalar_prod(pow(DELTA_T, 2) / (m2 * 2.0), f1);
			// // Force part of the equation
			// r2 = vector_add(r2, v_part);
			// r2 = vector_add(r2, f_part);
			//
			// vector f2 = gravitational_force(m2, m1, r2, r1);                        // F = -kx
			// /*
			//    Vector form of v = v + (delta_t * (f1 + f2) / (2 * m))
			//  */
			// double scal_part = DELTA_T / (m2 * 2.0);                                // Scalar part of the equation
			// v2 = vector_add(v2, scalar_prod(scal_part, vector_add(f1, f2)));

			// vector relative = vector_minus(r2, r1);
			// if (i % 1000 == 0) {
			//      // Printing position coordinates to file.
			//      fprintf(fp1, "%lf %lf %lf %lf\n", t_count, r1.x, r1.y, r1.z);
			//      fprintf(fp2, "%lf %lf %lf %lf\n", t_count, relative.x, relative.y, relative.z);
			// }

		}

		// vector diff = vector_minus(init_pos, relative);
		//
		// if (fabs(diff.x) <= err.x && fabs(diff.y) <= err.y && fabs(diff.z) <= err.z && flag == 0) {
		//      flag = 1;
		//      tp = t_count;
		//      printf("The position at time period is = "
		//             "%lfi + %lfj + %lfk\n", diff.x, diff.y, diff.z);
		//      tp = tp / (3600.0 * 24);
		//
		//      printf("Time period = %f days.\n", tp);
		t_count += DELTA_T;
	}

	t = clock() - t;
	double time_taken = ((double)t) / CLOCKS_PER_SEC * 1000;
	// // Printing the final position of the particle.
// printf("The final position (xf) of 1st particle is = "
//        "%lfi + %lfj + %lfk\n", r1.x, r1.y, r1.z);
// printf("The final position (xf) of 2nd particle is = "
	// "%lfi + %lfj + %lfk\n", r2.x, r2.y, r2.z);
	printf("Time taken for vel_ver = %lfms\n", time_taken);

	// vector pos_sun = vector_minus(init_sun, r1);
	// printf("Delta r1 = "
	//        "%lfi + %lfj + %lfk\n", pos_sun.x, pos_sun.y, pos_sun.z);

	/////////////////////////////////////

	// double Ef = 0.5 * m1 * pow(mod_vector(v1), 2) + 0.5 * m2 * pow(mod_vector(v2), 2) - G * m1 * m2 / mod_vector(vector_minus(r2, r1));

	/////////////////////////////////////

	// printf("\n\nEi = %.12lf\nEf = %.12lf\n %cE = %.12lf\n", Ei, Ef, '%', fabs((Ef - Ei) / Ei) * 100.0);
	return 0;
}

int main()
{
	state st[NUMBER_OF_BODIES];
	body particle[NUMBER_OF_BODIES];


	input_param(st, particle);

	FILE** fp = create_output_files(NUMBER_OF_BODIES);

	int no_of_iter = TIME / DELTA_T;

	vel_ver(st, particle, fp, no_of_iter);

	free(fp);

	return 0;
}
