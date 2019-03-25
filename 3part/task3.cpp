#include <mpi.h>
#include <stdio.h>
#include <cmath>
#include <random>
#include <iostream>

#define MAX_PROCESSES 100

using std::cout;
using std::endl;

int main( int argc, char **argv )
{
    int rank, size, i,j;
    float table[MAX_PROCESSES];
    float row[MAX_PROCESSES];
    int errors=0;
    int participants;
    float length;
    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );
 
    /* A maximum of MAX_PROCESSES processes can participate */
    if ( size < 2 ) {
	participants = 2;
	fprintf(stderr, "Must use two processes for this example\n");
    	MPI_Abort(MPI_COMM_WORLD, 1);
    }
    else participants = size;
    if ( (rank < participants) ) {
        int send_count = MAX_PROCESSES / size;
        int recv_count = MAX_PROCESSES / size;
 
        /* If I'm the root (process 0), then fill out the big table */
        if (rank == 0) {
            for ( i = 0; i<MAX_PROCESSES; i++)  {
                table[i]=((float) rand()/RAND_MAX);
	}		
		
	}

        /* Scatter the big table to everybody's little table */

        MPI_Scatter(&table, send_count, MPI_FLOAT 
                         &row , recv_count, MPI_FLOAT, 0, MPI_COMM_WORLD);
	//printf(" << rank: = %i << data: = %i \n", rank , send_count);	 

	printf(" >>>>> !!!!!!! <<<<<<< \n");

	MPI_Reduce(&row,&length,1,MPI_FLOAT,MPI_MINLOC,0,MPI_COMM_WORLD );
	if(rank == 0) {
		printf(" << %f \n", length);	
	}

    //printf(" >> error := %i \n", errors); 
 		//int length = std::sqrt(row[0] * row[0] + row[1] * row[1]);   
	//printf("%i \n", length);
	
    MPI_Finalize();
    return errors;
}