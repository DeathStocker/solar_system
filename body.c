#include "vector.h"
#include <time.h>
#include <string.h>
#include <float.h>

#define DIMENSION 3
#define G 6.67E-11

static int number_of_bodies;
static double sim_time;
static double delta_t;

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

int vector_input(FILE** fp, vector* a, int dim)
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
		fscanf(*fp, "%le", &arr[i++]);
	case 2:
		fscanf(*fp, "%le", &arr[i++]);
	case 1:
		fscanf(*fp, "%le", &arr[i++]);
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
	vector v_part = scalar_prod(delta_t, v);                        // Velocity part of the equation
	vector f_part = scalar_prod(pow(delta_t, 2) / (bd[i].mass * 2.0), f1);

	// Force part of the equation
	r = vector_add(r, v_part);
	r = vector_add(r, f_part);

	st[i].r = r;

	vector f2 = net_gravitational_force(bd, i, st, N);                      // F = -kx
	/*
	   Vector form of v = v + (delta_t * (f1 + f2) / (2 * m))
	 */
	double scal_part = delta_t / (bd[i].mass * 2.0);                        // Scalar part of the equation
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

	int i, j;
	double t_count = delta_t;

	vector err;

	err.x = 10000;
	err.y = 10000;
	err.z = 10000;

	err = (vector)err;

	/////////////////////////////////////

	// double Ei = 0.5 * m1 * pow(mod_vector(v1), 2) + 0.5 * m2 * pow(mod_vector(v2), 2) - G * m1 * m2 / mod_vector(vector_minus(r2, r1));

	/////////////////////////////////////

	t = clock();
	double* max_r = calloc(number_of_bodies, sizeof(double));
	double* min_r = malloc(number_of_bodies * sizeof(double));
	double* max_v = calloc(number_of_bodies, sizeof(double));
	double* min_v = malloc(number_of_bodies * sizeof(double));
	for (i = 0; i < number_of_bodies; i++) {
		min_r[i] = DBL_MAX;
		min_v[i] = DBL_MAX;
	}

	for (i = 0; i < no_of_iter; i++) {
		for (j = 0; j < number_of_bodies; j++) {
			state next = vel_ver_diff_eq(st, particle, j, number_of_bodies);
			particle[j].prev_r = next.r;
			particle[j].prev_v = next.v;
			st[j].r = next.r;
			st[j].v = next.v;
			vector r1 = next.r;

			double mod = mod_vector(r1);
			if (mod > max_r[j]) {
				max_r[j] = mod;
				min_v[j] = mod_vector(next.v);
			}

			if (min_r[j] > mod) {
				min_r[j] = mod;
				max_v[j] = mod_vector(next.v);
			}

			if (i % 1000 == 0)
				fprintf(fp[j], "%lf %lf %lf %lf\n", t_count, r1.x, r1.y, r1.z);
		}
		t_count += delta_t;
	}

	for (i = 0; i < number_of_bodies; i++)
		printf("Body %d -\n\tMAX_R = %lf\n\tMIN_R = %lf\n\tMAX_V = %lf\n\tMIN_V = %lf\n", i, max_r[i], min_r[i], max_v[i], min_v[i]);

	t = clock() - t;
	double time_taken = ((double)t) / CLOCKS_PER_SEC * 1000;

	printf("Time taken for vel_ver = %lfms\n", time_taken);

	return 0;
}

int input_param(char* filename)
{
	int i;

	FILE* fp = fopen(filename, "r");

	if (fp == NULL) {
		printf("Cannot open file %s. Exitting.\n", filename);
		exit(1);
	}

	fscanf(fp, "%d", &number_of_bodies);

	state *st = malloc(number_of_bodies * sizeof(state));
	body* particle = malloc(number_of_bodies * sizeof(body));

	for (i = 0; i < number_of_bodies; i++) {
		fscanf(fp, "%le", &particle[i].mass);
		vector_input(&fp, &st[i].r, DIMENSION);
		vector_input(&fp, &st[i].v, DIMENSION);
		particle[i].prev_r = st[i].r;
		particle[i].prev_v = st[i].v;
	}

	fscanf(fp, "%lf", &sim_time);
	fscanf(fp, "%lf", &delta_t);

	FILE** list_fp = create_output_files(number_of_bodies);

	int no_of_iter = sim_time / delta_t;

	vel_ver(st, particle, list_fp, no_of_iter);

	fclose(fp);

	for (i = 0; i < number_of_bodies; i++)
		fclose(list_fp[i]);

	return 0;
}

int main(int argc, char* argv[])
{
	if (argc != 2) {
		printf("Enter 1 file name.\n");
		return 1;
	}

	char* filename = argv[1];

	input_param(filename);

	return 0;
}
