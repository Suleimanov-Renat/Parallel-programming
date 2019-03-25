#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>	 
#include <time.h>
#include <iostream>
#include <malloc.h>


int main(int argc, char** argv) {
  MPI_Init(NULL, NULL);
  int b[10];      
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  if (world_size < 2) {
    fprintf(stderr, "Must use more than two processes for this example\n");
    MPI_Abort(MPI_COMM_WORLD, 1);
  }
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  const int MAX_NUMBERS = 100;
  int number_amount;
  if (world_rank == 1) {
	for(int i = 0; i < 10; i++) 
	b[i] = i;
	
    // Pick a random amount of integers to send to process one
    //srand(time(NULL));
    number_amount = 10;
    // Send the amount of integers to process one

	MPI_Send(b, number_amount, MPI_INT, 0, 0, MPI_COMM_WORLD);
    	printf("1 sent %d numbers to %i\n", number_amount,0);

    for(int i = 2; i < world_size; i++) {
	MPI_Send(b, number_amount, MPI_INT, i, 0, MPI_COMM_WORLD);
    	printf("1 sent %d numbers to %i\n", number_amount,i);
  }

  } else if (world_rank != 1) {
    MPI_Status status;
    MPI_Probe(1, 0, MPI_COMM_WORLD, &status);
    MPI_Get_count(&status, MPI_INT, &number_amount);
    int* number_buf = (int*)malloc(sizeof(int) * number_amount);

    MPI_Recv(number_buf, number_amount, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);

  
    for(int i = 0; i < 10; i++) 
	std::cout << i << " = " << number_buf[i] << "\n";
    printf("%d received %d numbers from 0. Message source = %d, tag = %d\n",
           world_rank, number_amount, status.MPI_SOURCE, status.MPI_TAG);

  
  }
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();
}

