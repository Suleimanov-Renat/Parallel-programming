#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>	 
#include <time.h>
#include <iostream>
#include <cmath>

using namespace std;
class Point
{
public:
   int x, y;
    public:
	void set_values(int, int);
};

void Point::set_values(int x0, int y0) {
x = x0;
y = y0;
}
float distance(const Point& point1, const Point& point2)
{
  return sqrt((point1.x * point2.x) + (point1.y * point2.y));
}

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
  if (world_rank == 0) {
	Point point;	
	point.set_values (3, 4);
	cout << " :: " << point.x << " :: " << point.y << "\n";

  MPI_Send(&number_amount, number_amount, MPI_INT, 1, 0, MPI_COMM_WORLD);

  }
 if (world_rank != 0) {
    MPI_Status status;
    //MPI_Probe(0, 0, MPI_COMM_WORLD, &status);
    //MPI_Get_count(&status, MPI_INT, &number_amount);


    MPI_Recv(&number_amount, number_amount, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

  
  	std::cout << number_amount << " \n";
    printf("%d received %d numbers from 0. Message source = %d, tag = %d\n",
           world_rank, number_amount, status.MPI_SOURCE, status.MPI_TAG);

  
  }
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();
}

