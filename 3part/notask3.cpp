#include <mpi.h>
#include <stdio.h>
#include <random>
#include <cmath>
#include <iostream>

#define MAX_PROCESSES 100

using std::cout;
using std::endl;

int main( int argc, char **argv )
{
    int rank, size, i,j;
    float table[MAX_PROCESSES];
    float table2[MAX_PROCESSES];
    int errors=0;
    int participants;
    float length;

    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );
 
    float row[MAX_PROCESSES / size];

    /* A maximum of MAX_PROCESSES processes can participate */
    if ( size < 2 ) {
	participants = 2;
	fprintf(stderr, "Mustn't use less then two processes for this example\n");
    	MPI_Abort(MPI_COMM_WORLD, 1);
    }
    else participants = size;
    if ( (rank < participants) ) {
        int send_count = MAX_PROCESSES/participants;
        int recv_count = MAX_PROCESSES/participants;
 
        /* If I'm the root (process 0), then fill out the big table */
        if (rank == 0) {
	    float min = 1;
            for ( i = 0; i<MAX_PROCESSES; i++){ 
               table[i]=((float) rand()/RAND_MAX);
	       if(min > table[i]) 
			min = table[i];		
	    }
	    printf("<><><> : = %f \n",min);
		
	}
        /* Scatter the big table to everybody's little table */

        MPI_Scatter(&table, send_count, MPI_FLOAT, 
                         &table2 , recv_count, MPI_FLOAT, 0, MPI_COMM_WORLD);
	
	MPI_Reduce(&table2,&length,1,MPI_FLOAT_INT,MPI_MINLOC,0,MPI_COMM_WORLD );	
	if(rank == 0) 
		printf(" << %f \n", length);	
	
} 

  

    MPI_Finalize();
    return errors;
}
