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

	double no_of_iter = t / delta_t;

	int i;
	return 0;
}
