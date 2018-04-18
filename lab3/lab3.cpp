#define _USE_MATH_DEFINES
#include <cmath>
#include <stdio.h>
#include "mpi.h"
#pragma comment(lib, "mpich.lib")

int main(int argc, char* argv[]) {
	int rank, size, resultlen, n, i, h;
	double x, sum, startTime, resultSum;
	char name[MPI_MAX_PROCESSOR_NAME];
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Get_processor_name(name, &resultlen);
	do {
		if (rank == 0) {
			printf("Enter n (0 or less for exit): ");
			fflush(stdout);
			scanf("%d", &n);
			startTime = MPI_Wtime();
		}
		MPI_Barrier(MPI_COMM_WORLD);
		MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
		if (n <= 0) {
			MPI_Finalize();
			return 0;
		}
		sum = 0;
		h = 2 * n;
		i = rank + 1;
		while (i <= n) {
			x = (2.0 * i - 1.0) / h;
			sum += 4.0 / (1.0 + x * x);
			i += size;
		}

		sum *= 1.0 / n;
		resultSum = 0;
		MPI_Reduce(&sum, &resultSum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
		if (rank == 0) {
			printf("Result is %2.15f, error of value %2.15f\nRun time in seconds: %2.6f\n", resultSum, resultSum - M_PI, MPI_Wtime() - startTime);
			fflush(stdout);
		}
	} while (true);
	MPI_Finalize();
	return 0;
}