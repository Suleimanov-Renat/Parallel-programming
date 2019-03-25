#include "stdafx.h"
#include "header.h"
#ifdef __mpi_2_2__
using namespace std;
int main(int argc, char **argv)
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);


	int n = 4;
	int loc_n = n / (size - 1);
	vector<vector<int>> loc_x;
	vector<vector<int>> loc_y;
	vector<vector<int>> loc_z;

	loc_x.resize(loc_n);
	loc_y.resize(loc_n);
	loc_z.resize(loc_n);
	for (int j = 0; j<loc_n; j++) {
		loc_x[j].resize(n);
		loc_y[j].resize(n);
		loc_z[j].resize(n);
	}
	//cout << "The number of processes: " << size << " my number is " << rank << endl;
	if (rank == 0) {

		int tag = 1;

		random_device rd;   // non-deterministic generator
		mt19937_64 gen(rd());  // to seed mersenne twister.
		uniform_int_distribution<> dist(0, 10);

		vector<vector<int>> x;
		vector<vector<int>> y;

		x.resize(n);
		y.resize(n);
		for (int j = 0; j<n; j++) {
			x[j].resize(n);
			y[j].resize(n);
		}

		for (int i = 0; i < n; i++) {
			for (int j = 0; j<n; j++) {
				x[i][j] = dist(rd);
				y[i][j] = dist(rd);
			}
		}
		


		int first = 0;
		for (int j = 1; j<size; j++) {
			for (int i = first; i < first + loc_n; i++) {
				MPI_Send(&x[i][0], n, MPI_INT, j, tag, MPI_COMM_WORLD);
				if (tag != 2) {
					MPI_Send(&y[i][0], n, MPI_INT, j, tag, MPI_COMM_WORLD);
				}
			}

			first += loc_n;
		}

		if (tag == 1) {
			first = 0;
			for (int j = 1; j < size; j++) {
				for (int i = first + loc_n; i < n; i++) {
					MPI_Send(&y[i][0], n, MPI_INT, j, tag, MPI_COMM_WORLD);
				}
			}
			first += loc_n;
		}


		for (int i = 0; i<n; i++) {
			for (int j = 0; j<n; j++) {
				cout << x[i][j] << " ";
			}
			cout << "\n";
		}
		cout << "\n";

		for (int i = 0; i<n; i++) {
			for (int j = 0; j<n; j++) {
				cout << y[i][j] << " ";
			}
			cout << "\n";
		}
		cout << "\n";


	}


	if (rank == 0) {

		vector<vector<int>> z;
		z.resize(n);
		for (int j = 0; j<n; j++) {
			z[j].resize(n);
		}


		MPI_Status status;
		MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		int tag = status.MPI_TAG;

		if (tag != 2) {
			int first = 0;
			for (int j = 1; j<size; j++) {
				for (int i = first; i < first + loc_n; i++) {
					MPI_Recv(&z[i][0], n, MPI_INT, j, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				}
				first += loc_n;
			}
		}
		else {		
			int first = 0;
			for (int i = 0; i < n; i++) {
				int first = 0;
				for (int j = 1; j<size; j++) {
					MPI_Recv(&z[i][first], loc_n, MPI_INT, j, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
					first += loc_n;
				}
			}
		}

		for (int i = 0; i<n; i++) {
			for (int j = 0; j<n; j++) {
				cout << z[i][j] << " ";
			}
			cout << "\n";
		}
		cout << "\n";

	}
	else {
		MPI_Status status;
		MPI_Probe(0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		int tag = status.MPI_TAG;

		for (int i = 0; i < loc_n; i++) {
			MPI_Recv(&loc_x[i][0], n, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
			if (tag != 2) {
				MPI_Recv(&loc_y[i][0], n, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
			}
		}
		



		if (tag == 0) {
			for (int i = 0; i < loc_n; i++) {
				for (int j = 0; j < n; j++) {
					loc_z[i][j] = loc_x[i][j] * loc_y[i][j];
				}
			}
		}
		if (tag == 1) {
			loc_y.resize(n);
			for (int i = loc_n; i < n; i++) {
				loc_y[i].resize(n);
			}
			for (int i = loc_n; i < n; i++) {
				MPI_Recv(&loc_y[i][0], n, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
			}

			for (int i = 0; i < loc_n; i++) {
				for (int j = 0; j < n; j++) {
					for (int k = 0; k < n; k++) {
						loc_z[i][j] += loc_x[i][k] * loc_y[k][j];
					}
				}
			}
		}


		if (tag == 2) {
			loc_z.resize(n);
			for (int i = 0; i < n; i++) {
				loc_z[i].resize(loc_n);
			}
			for (int i = 0; i < n; i++) {
				for (int j = 0; j < loc_n; j++) {
					loc_z[i][j] = loc_x[j][i];
				}
			}

		}


		if (tag != 2) {
			for (int i = 0; i < loc_n; i++) {
				MPI_Send(&loc_z[i][0], n, MPI_INT, 0, tag, MPI_COMM_WORLD);
			}
		}
		else {
			for (int i = 0; i < n; i++) {
				MPI_Send(&loc_z[i][0], loc_n, MPI_INT, 0, tag, MPI_COMM_WORLD);
			}
		}
		
		
	}



	MPI_Finalize();
	return 0;
}
#endif