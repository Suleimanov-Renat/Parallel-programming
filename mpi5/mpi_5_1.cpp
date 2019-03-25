#include "mpi.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int rank, size, i;
    int buffer[10];
    MPI_Status status;
 
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Datatype vec;
    double *vecin, *vecout;
    int root, n, stride, tag, errs = 0;
    root = 0;
    tag = 123;

 	    n = 12;
            stride = 1;
            vecin = (double *)malloc( n * stride * size * sizeof(double) );//common buffer 
            vecout = (double *)malloc( size * n * sizeof(double) );//count of result elements
 
            MPI_Type_vector( n, 1, stride, MPI_DOUBLE, &vec );
            MPI_Type_commit( &vec );

            for (i=0; i<n*stride; i++) vecin[i] =-2;
            for (i=0; i<n; i++) vecin[i*stride] = rank * n + i;
	 //   for (i=0; i<n; i++)
	//	        printf("!SR%d: vecin[%d] = %d \n",rank, i, (int)vecin[i]);
	if(rank != root) {
	        MPI_Send(vecin, 1, vec, root, tag, MPI_COMM_WORLD);
	} else {
		for (i=0; i<n; i++) vecout[i] = vecin[i];
	}

    

    if (rank == root)
    {
            //for (i=0; i<n*stride; i++) vecout[i] =-1;
	             //for (i=0; i < n * stride * size; i++) vecout[i] =-2;
        for (int l=1; l<size; l++) {
      
		MPI_Recv(vecin, 1, vec, l, tag, MPI_COMM_WORLD, &status);
		int count = 0;
		for (i = n*l; i<n*(l+1); i++) {vecout[i] = vecin[count];count++;}
        fflush(stdout);
        }
	for (i=0; i<n * size; i++)
		        printf("vecout[%d] = %d \n", i, (int)vecout[i]);

    }
		
 	MPI_Type_free( &vec );
            free( vecin );
            free( vecout );

    MPI_Finalize();
    return 0;
}
