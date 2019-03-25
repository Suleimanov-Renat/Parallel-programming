#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[])
{
    int in, out;
    int i;
    int rank, size;
    double t1, t2;

    out = 0;
    in = 4;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    t1 = MPI_Wtime();

    MPI_Reduce(&in, &out, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD );
    MPI_Bcast(&out, 1, MPI_INT, 0, MPI_COMM_WORLD);

    t2 = MPI_Wtime();

    printf("%i => MPI_Wtime_2-1: %1.2f\n", rank, t2-t1);

    fflush(stdout);
    MPI_Finalize();
    return 0;
}
