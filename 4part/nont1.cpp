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
    int i, j, k, rank, size, tag = 99, blksz, sum = 0;
    int a[N][N]={{1,2,3,4},{5,6,7,8},{9,1,2,3},{4,5,6,7,}};
    int c[N][N];
    int aa[N],cc[N];

    int res = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if(rank == 0) {
	   printf("1 2 3 4 \n");
	printf("5 6 7 8 \n");
	printf("9 1 2 3 \n");
	printf("4 5 6 7 \n");
}
    //scatter rows of first matrix to different processes     
    MPI_Scatter(a, N*N/size, MPI_INT, aa, N*N/size, MPI_INT,0,MPI_COMM_WORLD);

    //broadcast second matrix to all processes

    MPI_Barrier(MPI_COMM_WORLD);

          //perform vector multiplication by all processes
          for (i = 0; i < N; i++)
		sum += std::abs(aa[i]); 
	  //printf(" %i ", sum);
	  //printf("\n");

    MPI_Reduce(&sum, &res, 1, MPI_INT,MPI_MAX, 0, MPI_COMM_WORLD );	

    MPI_Barrier(MPI_COMM_WORLD);        
    MPI_Finalize();
    if (rank == 0)                         //I_ADDED_THIS
        printf(" norma equal to  << %i \n", res);	
}

void print_results(char *prompt, int a[N][N])
{
    int i, j;

    printf ("\n\n%s\n", prompt);
    for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                    printf(" %d", a[i][j]);
            }
            printf ("\n");
    }
    printf ("\n\n");
}