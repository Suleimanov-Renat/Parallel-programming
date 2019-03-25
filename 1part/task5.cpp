#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>	 
#include <time.h>
#include <iostream>
#include <malloc.h>


int main(int argc, char** argv) {
  MPI_Init(NULL, NULL);
  int b[12];   
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  if (world_size < 2) {
    fprintf(stderr, "Must use more than two processes for this example\n");
    MPI_Abort(MPI_COMM_WORLD, 1);
  }
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int number_amount;
  if (world_rank == 0) {
	for(int i = 0; i < 12 ; i++) 
		b[i] = i * i;
			//std::cout << i << " = " << b[i] << "\n";
    	number_amount = 12 / world_size;
	int j = 0;

    for(int i = 0; i < world_size; i++) {
	int a[number_amount];   
	int k = 0;
	if(i == 0) {
		for(j; (j < 12 / world_size); j++) {
			a[k] = b[j];
						std::cout << k << " = " << a[k] << "\n";
			k++;
		}
	   	printf("0 display %d numbers \n", number_amount);
		//std::cout << "k = " << k << ", j = " << j << "\n";
	}
	else {
			//std::cout << "!@ k = " << k << ", j = " << j << ", i = " << i << "\n";
		for(j; j < (12 / world_size) * (i + 1); j++) {
			a[k] = b[j];
			k++;
		}
			MPI_Send(a, number_amount, MPI_INT, i, 0, MPI_COMM_WORLD);
    			printf("1 sent %d numbers to %i\n", number_amount,i);
	}
  }

  } else if (world_rank != 0) {
    MPI_Status status;
    MPI_Probe(0, 0, MPI_COMM_WORLD, &status);
    MPI_Get_count(&status, MPI_INT, &number_amount);
    int a[number_amount];   
    MPI_Recv(a, number_amount, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

  
    for(int i = 0; i < number_amount; i++) 
	std::cout << i << " = " << a[i] << "\n";
    printf("%d received %d numbers from 0. Message source = %d, tag = %d\n",
           world_rank, number_amount, status.MPI_SOURCE, status.MPI_TAG);

  
  }
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();
}

