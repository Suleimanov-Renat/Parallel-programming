#include "mpi.h"
#include <stdio.h>
#include <vector>

int main(int argc, char *argv[])
{
    const int N = 24;
    int rank, size, i, j;
    MPI_Datatype type, type2;
    int buffer[N];
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
	
    if (size < 2)
    {
        printf("Please run with 2 processes.\n");
        MPI_Finalize();
        return 1;
    }
    int part_size = N / (size - 1);
    int length = part_size;
    j = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vector;
    //MPI_Type_contiguous(size - 1, MPI_INT, &type2);
    //MPI_Type_commit(&type2);
    //MPI_Type_vector(2, 3, 4, type2, &type);
    //MPI_Type_commit(&type);

    if (rank == 0)
    {
	//init data

	for (j; j < N; j++)
            		vector.push_back(j + 1);
	j = 0;
	for(i = 1; i < size; i++ ) {
		int k = 0;
        	for (j; j < length; j++) {
            		buffer[k] = vector[j];
			k++;
		}
		printf("0 sent %d numbers to %i\n", part_size, i);
		j = length;
		length += part_size;
		MPI_Send(buffer, part_size, MPI_INT, i, 123, MPI_COMM_WORLD);
		//printf("j = %i, length = %i, part_size = %i \n",j, length, part_size);
	}
//receiving
	int sum, temp;
	sum = 0;
	for(i = 1; i < size; i++ ) {
		
		printf("%i sent %d numbers to 0 \n", i, part_size);
		j = length;
		length += part_size;
		MPI_Recv(&temp, part_size, MPI_INT, i, 12, MPI_COMM_WORLD, &status);
		sum += temp;
	}
			printf("sum = %i \n", sum);
    }

    if (rank != 0)
    {
        for (i = 0; i < part_size; i++)
            buffer[i] = -1;
        MPI_Recv(buffer, part_size, MPI_INT, 0, 123, MPI_COMM_WORLD, &status);
 	printf("%d received %d numbers from 0. Message source = %d, tag = %d\n",
           rank, part_size, status.MPI_SOURCE, status.MPI_TAG);
	int res = 0;
        for (i = 0; i < part_size; i++)
            res +=  buffer[i];
	//printf("buffer[%d] = %d\n", i, buffer[i]);
        fflush(stdout);
	MPI_Send(&res, 1, MPI_INT, 0, 12, MPI_COMM_WORLD);
	printf("%i sent %d numbers to %i\n",rank,  part_size, 0);
    }

    MPI_Finalize();
    return 0;
}