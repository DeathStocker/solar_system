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

	printf("Enter the final time (t) of the trajectory = ");
	scanf("%lf", &t);
	printf("Enter Δt = ");
	scanf("%lf", &delta_t);

	double x0, v0;
	printf("Enter the initial position (x0) of the particle = ");
	scanf("%lf", &x0);
	printf("Enter the initial velocity (y0) of the particle = ");
	scanf("%lf", &v0);

	double k, m;
	printf("Enter the spring constant (k) = ");
	scanf("%lf", &k);
	printf("Enter the mass (m) of the particle = ");
	scanf("%lf", &m);

	int no_of_iter = t / delta_t;

	int i;
	double x = x0;
	double v = v0;
	double t_count = delta_t;
	for(i = 0; i < no_of_iter; i++) {
		double f1 = -(k * x);
		x = x + (v * delta_t) + (f1 * pow(delta_t, 2)) / (m * 2.0);
		double f2 = -(k * x);
		v = v + ((f1 + f2) * delta_t / (m * 2.0));

		printf("%lf %lf\n", t_count, x);

		t_count += delta_t;
	}
	return 0;
}
