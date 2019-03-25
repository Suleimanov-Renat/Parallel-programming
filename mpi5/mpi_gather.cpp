#include "mpi.h"
#include <stdlib.h>
#include <stdio.h>

/* Gather data from a vector to contiguous */

int main( int argc, char **argv )
{
    MPI_Datatype vec;
    MPI_Comm comm;
    double *vecin, *vecout;
    int root, i, n, stride;
    int rank, size;
 
    MPI_Init( &argc, &argv );
    comm = MPI_COMM_WORLD;
    /* Determine the sender and receiver */
    MPI_Comm_rank( comm, &rank );
    MPI_Comm_size( comm, &size );
 root = 0;
            n = 12;
            stride = 10;
            vecin = (double *)malloc( n * stride * size * sizeof(double) );//common buffer 
            vecout = (double *)malloc( size * n * sizeof(double) );//count of result elements
 
            MPI_Type_vector( n, 1, stride, MPI_DOUBLE, &vec );
            MPI_Type_commit( &vec );

            for (i=0; i<n*stride; i++) vecin[i] =-2;
            for (i=0; i<n; i++) vecin[i*stride] = rank * n + i;
	    
            MPI_Gather( vecin, 1, vec, vecout, n, MPI_DOUBLE, root, comm );

            if (rank == root) 
                for (i=0; i<n*size; i++) 
                    if (vecout[i] == i) 
                            fprintf( stderr, "vecout[%d]=%d\n", i, (int)vecout[i] );fflush(stderr);
                       
         
            MPI_Type_free( &vec );
            free( vecin );
            free( vecout );
        
 
    /* do a zero length gather */
    MPI_Gather( NULL, 0, MPI_BYTE, NULL, 0, MPI_BYTE, 0, MPI_COMM_WORLD );
 
    MPI_Finalize();
    return 0;
}
