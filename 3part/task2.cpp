#include <mpi.h>
#include <stdio.h>
#include <cmath>

#define MAX_PROCESSES 2

int main( int argc, char **argv )
{
    int rank, size, i,j;
    int table[MAX_PROCESSES][MAX_PROCESSES];
    int row[MAX_PROCESSES];
    int errors=0;
    int participants;
    int length;
    int res;
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
        int send_count = 2;
        int recv_count = 2;
 
        /* If I'm the root (process 0), then fill out the big table */
        if (rank == 0) {/*
            for ( i = 0; i<participants; i++) 
                for ( j = 0; j<MAX_PROCESSES; j++ ) {
                    table[i][j] = i+j;
		}*/
		table[0][0] = 5; //x1
		table[1][0] = 12; //y1
		table[0][1] = 4; //x2
		table[1][1] = 3; //y2
	}
        /* Scatter the big table to everybody's little table */

        MPI_Scatter(&table, send_count, MPI_INT, 
                         &row , recv_count, MPI_INT, 0, MPI_COMM_WORLD);
	//printf(" << rank: = %i << data: = %i \n", rank , send_count);	 
        /* Now see if our row looks right */
	printf(" >>>>> !!!!!!! <<<<<<< \n");	
	res = row[0] * row[1];
	printf(" >> : = %i \n", res);
           
    	} 
	MPI_Reduce(&res,&length,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD );
	if(rank == 0) {
		printf(" << %i \n", length);	
	}

    //printf(" >> error := %i \n", errors); 
 		//int length = std::sqrt(row[0] * row[0] + row[1] * row[1]);   
	//printf("%i \n", length);

    MPI_Finalize();
    return errors;
}