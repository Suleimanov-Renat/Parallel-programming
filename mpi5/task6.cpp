#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>	 
#include <time.h>
#include <iostream>
#include <malloc.h>
#include <vector>

int main(int argc, char** argv) {
  MPI_Init(NULL, NULL);
  int x;   
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  if (world_size < 2) {
    fprintf(stderr, "Must use more than two processes for this example\n");
    MPI_Abort(MPI_COMM_WORLD, 1);
  }
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int number_amount;
	/**/        
 static const int arr[] = {16,2,77,29,3 ,5 ,1, 3};
	//std::vector<int> x (arr, arr + sizeof(arr) / sizeof(arr[0]) );
	std::vector<int> y (arr, arr + sizeof(arr) / sizeof(arr[0]) );	
  if (world_rank == 0) {
	x = 12;
    	number_amount = 8 / world_size;
	int j = number_amount;
  MPI_Send(x, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
/*
  for(int i = 1; i < world_size; i++) {
	std::vector<int>::const_iterator first = x.begin();
	std::vector<int>::const_iterator last = x.begin() + j;
	std::vector<int> new_x(first, last);
	//static_cast<void*>(x.data());
	//vector<int> send_y(j, number_amount * (i + 1));
	//std::cout << "!@ k = " << k << ", j = " << j << ", i = " << i << "\n";
	MPI_Send(&new_x, number_amount, MPI_INT, 1, 0, MPI_COMM_WORLD);
	//MPI_Send(y, number_amount, MPI_INT, i, 1, MPI_COMM_WORLD);
	printf("1 sent %d numbers to %i\n", number_amount,i);
	j = number_amount * (i + 1);
}*/

  } else if (world_rank != 0) {
    MPI_Status status;
    //x.resize(8);
    //MPI_Probe(0, 0, MPI_COMM_WORLD, &status);
    //MPI_Get_count(&status, MPI_INT, &number_amount);


    MPI_Recv(arr, number_amount, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

  	
   //for (auto& i: x)
  	//std::cout << i << ' ';
    printf("%d received %d numbers from 0. Message source = %d, tag = %d\n",
           world_rank, number_amount, status.MPI_SOURCE, status.MPI_TAG);

  
  }
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();
}

