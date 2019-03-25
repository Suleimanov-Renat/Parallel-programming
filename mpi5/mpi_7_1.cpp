#include "stdafx.h"
#include "header.h"
#ifdef __mpi_7_1__
using namespace std;
int main(int argc, char **argv)
{
	int rank, size,new_rank=-1,new_size=-1;
	MPI_Init(&argc, &argv);


	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	MPI_Group all_group, new_group;
	MPI_Comm new_comm;
	int a[5] = { 8,3,9,1,6 };
	MPI_Comm_group(MPI_COMM_WORLD, &all_group);
	MPI_Group_incl(all_group, 5, a, &new_group);
	MPI_Comm_create(MPI_COMM_WORLD, new_group, &new_comm);

	if (MPI_COMM_NULL != new_comm) {
		MPI_Comm_rank(new_comm, &new_rank);
		MPI_Comm_size(new_comm, &new_size);
	}



	int n = 8;
	vector<double> array;
	array.resize(n);

	cout << "The number of processes: " << new_size << " my number is " << new_rank << endl;
	MPI_Barrier;
	if (new_rank == 0) {
		random_device rd;   // non-deterministic generator
		mt19937_64 gen(rd());  // to seed mersenne twister.
		uniform_real_distribution<> dist(0, 10);

		for (int i = 0; i < n; i++) {
			array[i] = dist(rd);
		}

		for (int i = 0; i<n; i++) {
			cout << array[i] << " ";
		}
		cout << "\n";

	}

	if (MPI_COMM_NULL != new_comm) {
		MPI_Bcast(array.data(), n, MPI_DOUBLE, 0, new_comm);
		for (int i = 0; i<n; i++) {
			cout << array[i] << " ";
		}
		cout << "\n";
	}


	if (new_rank == new_size-1) {
		MPI_Send(array.data(), n, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
	}
	if (rank == 0) {
		MPI_Status status;
		MPI_Recv(array.data(), n, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
		for (int i = 0; i<n; i++) {
			cout << array[i] << " ";
		}
		cout << "\n";
	}

	if (MPI_COMM_NULL != new_comm) {
		MPI_Comm_free(&new_comm);
	}
	MPI_Group_free(&new_group);
	MPI_Group_free(&all_group);

	MPI_Finalize();
	return 0;
}
#endif