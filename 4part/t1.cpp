#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cmath>

int *createMatrix (int nrows, int ncols) {
    int *matrix;
    int h, i, j;

    if (( matrix = (int *)malloc(nrows*ncols*sizeof(int))) == NULL) {
        printf("Malloc error");
        exit(1);
    }

    for (h=0; h<nrows*ncols; h++) {
        matrix[h] = h+1;
    }

    return matrix;
}

void printArray (int *row, int nElements) {
    int i;
    for (i=0; i<nElements; i++) {
        printf("%d ", row[i]);
    }
    printf("\n");
}

int main (int argc, char **argv) {

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS) {
        perror("Error initializing MPI");
        exit(1);
    }

    int p, id;
    MPI_Comm_size(MPI_COMM_WORLD, &p); // Get number of processes
    MPI_Comm_rank(MPI_COMM_WORLD, &id); // Get own ID

    int *matrix;

    if (id == 0) {
        matrix = createMatrix(p, p); // Master process creates matrix
        printf("Initial matrix:\n");
        printArray(matrix, p*p);
    }

    int *procRow = (int *)malloc(sizeof(int) * p); // received row will contain p integers
    if (procRow == NULL) {
        perror("Error in malloc 3");
        exit(1);
    }

    if (MPI_Scatter(matrix, p, MPI_INT, // send one row, which contains p integers
                procRow, p, MPI_INT, // receive one row, which contains p integers
                0, MPI_COMM_WORLD) != MPI_SUCCESS) {

        perror("Scatter error");
        exit(1);
    }

    printf("Process %d received elements: ", id);
    printArray(procRow, p);
				printf("\n");
		int sum = 0;
		int length = 0;
		for(int k = 0; k < p; k ++) {	
			sum += std::abs(procRow[k]);
		}
		printf(" %i ", sum);
		printf("\n");
		MPI_Reduce(&sum, &length, 1, MPI_INT,MPI_MAX, 0, MPI_COMM_WORLD );	
	
	if(id == 0) 
		printf(" << %i \n", length);	

    MPI_Finalize();

    return 0;
}
/*

int sum = 0;
		for(int k = 0; k < count; k ++) {	
			sum += a2[k];
			printf(" %i ", sum)	;
		}
		printf("\n");
		MPI_Reduce(&sum, &length, 1, MPI_INT,MPI_MAX, 0, MPI_COMM_WORLD );	
	
	if(rank == 0) 
		printf(" << %f \n", length);	

*/
