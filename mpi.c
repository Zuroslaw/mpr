#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void pingpong(long round_trips, long data_size) {

    MPI_Init(NULL, NULL);

    int my_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    int* data = (int*) malloc(1024*1024*1024 * sizeof(int));
    double* times = (double*) malloc(round_trips * sizeof(double));

    if (my_rank == 0) {
        for (int i = 0; i < round_trips; ++i) {
            double start = MPI_Wtime();
            MPI_Send(&data, data_size, MPI_INT, 1, 0,
                     MPI_COMM_WORLD);
            MPI_Recv(&data, data_size, MPI_INT, 1, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            double end = MPI_Wtime();
            times[i] = end - start;
        }
    } else {
        for (int i = 0; i < round_trips; ++i) {
            MPI_Recv(&data, data_size, MPI_INT, 0, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(&data, data_size, MPI_INT, 0, 0,
                     MPI_COMM_WORLD);
        }
    }

    if (my_rank == 0) {
        double sum = 0;
        for (int i = 0; i < round_trips; ++i) {
            sum += times[i];
        }
        double average = sum/(double)round_trips;
        printf("Round trips: %ld\n", round_trips);
        printf("Element size: %d bytes\n", sizeof(int));
        printf("Data size: %ld elements\n", data_size);
        printf("Average round trip time: %.10e\n", average);
    }

    MPI_Finalize();

}

int main(int argc, char * argv[]) {
    long round_trips = atol(argv[1]);
    long data_size = atol(argv[2]);
    pingpong(round_trips, data_size);
}

