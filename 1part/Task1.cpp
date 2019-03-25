#include <stdio.h>
#include <mpi.h>


 int main(int argc, char *argv[]) {
	 int rank, size;

	  // Initialize the MPI environment
	 MPI_Init(&argc, &argv);
	  // Find out rank, size
	 int world_rank;
	 MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	 int world_size;
	 MPI_Comm_size(MPI_COMM_WORLD, &size);

	printf("Hallo world\n");

	MPI_Finalize();

	return 0;
}
