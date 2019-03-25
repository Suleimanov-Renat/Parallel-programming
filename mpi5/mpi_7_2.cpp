#include "stdafx.h"
#include "header.h"
#ifdef __mpi_7_2__
using namespace std;
int main(int argc, char **argv)
{
	int rank, size, new_rank = -1, new_size = -1;
	MPI_Init(&argc, &argv);


	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int colour = rank % 4;
	int dest;
	MPI_Comm new_comm, icomm;
	MPI_Comm_split(MPI_COMM_WORLD,colour, rank, &new_comm);

	MPI_Comm_rank(new_comm, &new_rank);
	MPI_Comm_size(new_comm, &new_size);




	int n = 2;
	int m = n*new_size;
	vector<double> array;
	vector<double> array_group;
	array.resize(n);

	for (int i = 0; i < n; i++) {
		array[i] = colour;
	}

	if (new_rank == 0) { array_group.resize(m); }
	MPI_Gather(array.data(), n, MPI_DOUBLE, array_group.data(), n, MPI_DOUBLE, 0, new_comm);

	if (new_rank == 0) {
		cout << "My group number: " << colour << " my array ";
		for (int i = 0; i<m; i++) {
			cout << array_group[i] << " ";
		}
		cout << "\n";

	}


	switch (colour) {
	case 0: dest = 1;
		break;
	case 1: dest = 0;
		break;
	case 2: dest = 3;
		break;
	case 3: dest = 2;
	}

	MPI_Intercomm_create(new_comm, 0, MPI_COMM_WORLD, dest, 10, &icomm);

	if (new_rank == 0) {
		MPI_Send(array_group.data(), m, MPI_DOUBLE, 0, 5, icomm);
		vector<double> array_input;
		array_input.resize(m);
		MPI_Status status;
		MPI_Recv(array_input.data(), m, MPI_DOUBLE, 0, 5, icomm, &status);


		cout << "My group number: " << colour << " my input array ";
		for (int i = 0; i<m; i++) {
			cout << array_input[i] << " ";
		}
		cout << "\n";
	}

	MPI_Comm_free(&new_comm);

	MPI_Finalize();
	return 0;
}
#endif