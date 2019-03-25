#include "mpi.h"
#include <stdio.h>
#include <cmath>

#define MAX_PROCESSES 2

int main( int argc, char **argv )
{
    int rank, size, i,j;
    int table[MAX_PROCESSES][1];
    int row[1];
    int errors=0;
    int participants;
    int length;

    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );
 
    /* A maximum of MAX_PROCESSES processes can participate */
    if ( size != 2 ) {
	participants = 2;
	fprintf(stderr, "Must use two processes for this example\n");
    	MPI_Abort(MPI_COMM_WORLD, 1);
    }
    else participants = size;
    if ( (rank < participants) ) {
        int send_count = 1;
        int recv_count = 1;
 
        /* If I'm the root (process 0), then fill out the big table */
        if (rank == 0) {/*
            for ( i = 0; i<participants; i++) 
                for ( j = 0; j<MAX_PROCESSES; j++ ) {
                    table[i][j] = i+j;
		}*/
table[0][0] = 5;
	table[1][0] = 12;
	}
        /* Scatter the big table to everybody's little table */

        MPI_Scatter(&table, send_count, MPI_INT, 
                         &row , recv_count, MPI_INT, 0, MPI_COMM_WORLD);
	//printf(" << rank: = %i << data: = %i \n", rank , send_count);	 
        /* Now see if our row looks right */
	printf(" >>>>> !!!!!!! <<<<<<< \n");
        for (i=0; i<1; i++) {
	    row[i] = row[i] * row[i];
	    printf(" >> : = %i \n", row[i]);
            if ( row[i] != i+rank ) 
		errors++;
	}  
    	} 
	MPI_Reduce(&row,&length,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD );
	if(rank == 0) {
		length = std::sqrt(length);
		printf(" << %i \n", length);	
	}

    //printf(" >> error := %i \n", errors); 
 		//int length = std::sqrt(row[0] * row[0] + row[1] * row[1]);   
	//printf("%i \n", length);

    MPI_Finalize();
    return errors;
}