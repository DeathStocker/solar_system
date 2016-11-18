#include "vector.h"
#include <time.h>
#include <string.h>
#include <float.h>

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

double energy(state* st,body* particle)
{
	double Ki=0.0;
	double Gi=0.0;
	int i,j;
	// Calculating the total gravitational potential and kinetic energy
	for(i=0 ; i<NUMBER_OF_BODIES ; i++){
			Ki= Ki + 0.5 * particle[i].mass * pow(mod_vector(st[i].v),2);
		
		for(j=0 ; j<NUMBER_OF_BODIES ; j++){
			if( i != j)
				Gi = Gi + ((G * particle[i].mass * particle[j].mass)/(mod_vector(vector_minus(st[j].r,st[i].r))));
		}
	}

	Gi=Gi/2;

	printf("\nTotal energy:%lf",Ki-Gi);
	return Ki-Gi;
}

int vel_ver(state* st, body* particle, FILE** fp, int no_of_iter)
{
	clock_t t;

	int i, j;
	double t_count = DELTA_T;

	vector err;

	err.x = 10000;
	err.y = 10000;
	err.z = 10000;

	err = (vector)err;
	t = clock();
	double* max_r = calloc(NUMBER_OF_BODIES, sizeof(double));
	double* min_r = malloc(NUMBER_OF_BODIES * sizeof(double));
	double* max_v = calloc(NUMBER_OF_BODIES, sizeof(double));
	double* min_v = malloc(NUMBER_OF_BODIES * sizeof(double));
	for (i = 0; i < NUMBER_OF_BODIES; i++) {
		min_r[i] = DBL_MAX;
		min_v[i] = DBL_MAX;
	}

	for (i = 0; i < no_of_iter; i++) {
		for (j = 0; j < NUMBER_OF_BODIES; j++) {
			state next = vel_ver_diff_eq(st, particle, j, NUMBER_OF_BODIES);
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
		t_count += DELTA_T;
	}

	for (i = 0; i < NUMBER_OF_BODIES; i++)
		printf("Body %d -\n\tMAX_R = %lf\n\tMIN_R = %lf\n\tMAX_V = %lf\n\tMIN_V = %lf\n", i, max_r[i], min_r[i], max_v[i], min_v[i]);

	t = clock() - t;
	double time_taken = ((double)t) / CLOCKS_PER_SEC * 1000;

	printf("Time taken for vel_ver = %lfms\n", time_taken);

	return 0;
}

int main()
{
	state st[NUMBER_OF_BODIES];
	body particle[NUMBER_OF_BODIES];


	input_param(st, particle);
	double e1=0.0,e2=0.0,error_in_energy=0.0;

	FILE** fp = create_output_files(NUMBER_OF_BODIES);

	int no_of_iter = TIME / DELTA_T;

	e1=energy(st, particle);

	vel_ver(st, particle, fp, no_of_iter);
	
	e2=energy(st, particle);
	
	error_in_energy= abs((e2-e1)/e1)*100;
	printf("\nThe net percentage error in the total energy is %lf percent",error_in_energy);

	free(fp);

	return 0;
}
