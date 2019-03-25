#define N 4
#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stddef.h>
#include "mpi.h"


void print_results(char *prompt, int a[N][N]);

int main(int argc, char *argv[])
{
    int i, j, k, rank, size, tag = 99, blksz;
    int a[N][N]={{1,2,3,4},{5,6,7,8},{9,1,2,3},{4,5,6,7,}};
    int b[N]={1,2,3,4};
    int c[N];
    int aa[N];
    int cc = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
 
 if(rank == 0) {
	   printf("1 2 3 4 \n");
	printf("5 6 7 8 \n");
	printf("9 1 2 3 \n");
	printf("4 5 6 7 \n");

	printf("  X \n");

	printf("1 2 3 4 \n");
	}
    //scatter rows of first matrix to different processes     
    MPI_Scatter(a, N*N/size, MPI_INT, aa, N*N/size, MPI_INT,0,MPI_COMM_WORLD);

    //broadcast second matrix to all processes
    MPI_Bcast(b, N, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);

          //perform vector multiplication by all processes
        		int sum = 0;
                    for (j = 0; j < N; j++)
                    {
                            sum += aa[j] * b[j];  //MISTAKE_WAS_HERE               
                    }
                    cc = sum;
                    sum = 0;

    MPI_Gather(&cc, N/size, MPI_INT, c, N/size, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);        
    MPI_Finalize();
    if (rank == 0)                         //I_ADDED_THIS
        {
            printf (" = \n");
  for (j = 0; j < N; j++) {
                    printf(" %d", c[j]);
            }
            printf ("\n");
}
}
