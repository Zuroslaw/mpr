#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void pingpong(long round_trips, long data_size) {


    int my_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    int* data_send = (int*) malloc(data_size * sizeof(int));
    int* data_receive = (int*) malloc(data_size * sizeof(int));
    double* times = (double*) malloc(round_trips * sizeof(double));

    if (my_rank == 0) {
        for (int i = 0; i < round_trips; ++i) {
            double start = MPI_Wtime();
            MPI_Send(data_send, data_size, MPI_INT, 1, 0,
                     MPI_COMM_WORLD);
            MPI_Recv(data_receive, data_size, MPI_INT, 1, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            double end = MPI_Wtime();
            times[i] = end - start;
        }
    } else {
        for (int i = 0; i < round_trips; ++i) {
            MPI_Recv(data_receive, data_size, MPI_INT, 0, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(data_send, data_size, MPI_INT, 0, 0,
                     MPI_COMM_WORLD);
        }
    }

    if (my_rank == 0) {
        double sum = 0;
        for (int i = 0; i < round_trips; ++i) {
            sum += times[i];
        }
        double average = sum/(double)round_trips;
        printf("%ld: %.10e\n", data_size*sizeof(int), average);
    }

    free(data_send);
    free(data_receive);
    free(times);

}

int main(int argc, char * argv[]) {
    long round_trips = atol(argv[1]);
    long iter = atol(argv[2]);

    MPI_Init(NULL, NULL);

    pingpong(round_trips, 1);

    for (int i = 1; i <= iter; ++i) {
        long data_size = i*1024;
        pingpong(round_trips, data_size);
    }

    MPI_Finalize();

    return 0;
}

