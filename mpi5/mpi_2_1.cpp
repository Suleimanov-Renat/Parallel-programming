#include "stdafx.h"
#include "header.h"
#ifdef __mpi_2_1__
using namespace std;
int main(int argc, char **argv)
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int a = 10;
	int b = 100;
	int n = 12;
	int loc_n = n / (size-1);
	vector<int> loc_x;
	vector<int> loc_y;
	vector<int> loc_z;
	
	loc_x.resize(loc_n);
	loc_y.resize(loc_n);
	loc_z.resize(loc_n);
	//cout << "The number of processes: " << size << " my number is " << rank << endl;
	if (rank == 0) {

		int tag = 0;

		random_device rd;   // non-deterministic generator
		mt19937_64 gen(rd());  // to seed mersenne twister.
		uniform_int_distribution<> dist(-10, 10);

		vector<int> x;
		vector<int> y;
		x.resize(n);
		y.resize(n);

		for (int j = 0; j<n; j++) {
			x[j] = dist(rd);
			y[j] = dist(rd);
		}


		for (int j = 0; j<n; j++) {
			cout << x[j] << " ";
		}
		cout << "\n";
		for (int j = 0; j<n; j++) {
			cout << y[j] << " ";
		}
		cout << "\n";
		cout << "\n";


		int first = 0;
		
		for (int j = 1; j<size; j++) {
			MPI_Send(&x[first], loc_n, MPI_INT, j, tag, MPI_COMM_WORLD);
			MPI_Send(&y[first], loc_n, MPI_INT, j, tag, MPI_COMM_WORLD);
			first += loc_n;
		}

		
	}
	

	if (rank == 0) {

		vector<int> x;
		vector<int> y;
		vector<int> z;
		x.resize(n);
		y.resize(n);
		z.resize(n);

		MPI_Status status;
		MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		int tag = status.MPI_TAG;

		int first = 0;
		for (int j = 1; j<size; j++) {
			MPI_Recv(&x[0 + first], loc_n, MPI_INT, j, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Recv(&y[0 + first], loc_n, MPI_INT, j, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Recv(&z[0 + first], loc_n, MPI_INT, j, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			first += loc_n;
		}
		for (int j = 0; j<n; j++) {
			cout << x[j] << " ";
		}
		cout << "\n";
		for (int j = 0; j<n; j++) {
			cout << y[j] << " ";
		}
		cout << "\n";
		for (int j = 0; j<n; j++) {
			cout << z[j] << " ";
		}
		cout << "\n";
	}
	else {
		MPI_Status status;
		MPI_Probe(0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		int tag = status.MPI_TAG;
		MPI_Recv(loc_x.data(), loc_n, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
		MPI_Recv(loc_y.data(), loc_n, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);



		if (tag == 0) {
			for (int j = 0; j < loc_n; j++) {
				loc_z[j] = a*loc_x[j] + b*loc_y[j];
			}
		}
		if (tag == 1) {
			for (int j = 0; j < loc_n; j++) {
				loc_y[j] = a*loc_x[j] + b*loc_y[j];
			}
		}
		if (tag == 2) {
			for (int j = 0; j < loc_n; j++) {
				loc_z[j] = loc_x[j] * loc_y[j];
			}
		}
		if (tag == 3) {
			for (int j = 0; j < loc_n; j++) {
				int temp = loc_x[j];
				loc_x[j] = loc_y[j];
				loc_y[j] = temp;
			}
		}

		MPI_Send(loc_x.data(), loc_n, MPI_INT, 0, tag, MPI_COMM_WORLD);
		MPI_Send(loc_y.data(), loc_n, MPI_INT, 0, tag, MPI_COMM_WORLD);
		MPI_Send(loc_z.data(), loc_n, MPI_INT, 0, tag, MPI_COMM_WORLD);
	}

	

	MPI_Finalize();
	return 0;
}
#endif