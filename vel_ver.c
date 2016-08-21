#include "calc.h"

typedef struct props {
	double xi;
	double xj;
	double xk;

	double vi;
	double vj;
	double vk;
} state;

int main(int argc, char* argv[])
{
	double t;
	double delta_t;

	printf("Enter the final time of the trajectory = ");
	scanf("%lf", &t);
	printf("Enter Δt = ");
	scanf("%lf", &delta_t);

	double x0, v0;
	printf("Enter the initial position of the particle = ");
	scanf("%lf", &x0);
	printf("Enter the initial velocity of the particle = ");
	scanf("%lf", &v0);

	int no_of_iter = t / delta_t;

	int i;
	double x = x0;
	double v = v0;
	double t_count = delta_t;
	for(i = 0; i < no_of_iter; i++) {
		double f1 = -x;
		x = x + (v * delta_t) + (f1 * pow(delta_t, 2)) / 2.0;
		double f2 = -x;
		v = v + ((f1 + f2) * delta_t / 2.0);

		printf("%lf %lf\n", t_count, x);
		t_count += delta_t;
	}
	return 0;
}
