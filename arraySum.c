#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define n 10000 // size of array
#define numSlaves 2
#define rank1 1
#define rank2 2

// array to be summed
int a[n];
// temporary array for slave process
int a2[n/2];

int main(int argc, char* argv[])
{
    // np -> no. of processes
	// pid -> process id
	int pid, np,
	elements_per_process,
	n_elements_received;

	MPI_Status status;

	// creation of parallel processes
	MPI_Init(&argc, &argv);

	// find out process ID (rank) and how many processes were started
	MPI_Comm_rank(MPI_COMM_WORLD, &pid);
	MPI_Comm_size(MPI_COMM_WORLD, &np);
	
	// checks if 3 processes are used
	if (np != 3) {
        printf("Please run 3 processes (1 master, 2 slaves).\n");
        MPI_Finalize();
        return 1;
    }
	// checks if batch size is provided as an argument
	if (argc != 2) {
		printf("Please provide an argument for the batch size.\n");
		MPI_Finalize();
        return 1;
	}
	// checks if batch size is greater than 0
	int batches = atoi(argv[1]);
	if (batches < 1) {
		printf("Please provide batch size greater than 0.\n");
		MPI_Finalize();
        return 1;
	}

	MPI_Barrier(MPI_COMM_WORLD);
	double start_time = MPI_Wtime();

	// master process
	if (pid == 0) {

		// initialize array with values 1 to n
		for (int i = 0; i < n; i++) {
			a[i] = i + 1;
		}

		// calculates the number of elements per process
		elements_per_process = n / (numSlaves * batches);
		int index = 0;
		int sum = 0;

		// distributes the portion of array to each slave
		for (int i = 1; i < batches + 1; i++)
		{
			// distributes the first portion of array to slave A
			MPI_Send(&elements_per_process,
				1, MPI_INT, rank1, 0,
				MPI_COMM_WORLD);
			MPI_Send(&a[index],
					elements_per_process,
					MPI_INT, rank1, 0,
					MPI_COMM_WORLD);

			index += elements_per_process;
			// distributes the second portion of array to slave B
			if (i < batches) {
				MPI_Send(&elements_per_process,
					1, MPI_INT, rank2, 0,
					MPI_COMM_WORLD);
				MPI_Send(&a[index],
					elements_per_process,
					MPI_INT, rank2, 0,
					MPI_COMM_WORLD);
				index += elements_per_process;
			}
			// special case for last portion of array
			else {
				int elements_left = n - index;

				MPI_Send(&elements_left,
					1, MPI_INT,
					rank2, 0,
					MPI_COMM_WORLD);
				MPI_Send(&a[index],
					elements_left,
					MPI_INT, rank2, 0,
					MPI_COMM_WORLD);
			}

			// collect partial sums
			int tmp;
			for (int j = 1; j < np; j++) {
				MPI_Recv(&tmp, 1, MPI_INT,
						MPI_ANY_SOURCE, 0,
						MPI_COMM_WORLD,
						&status);
				int sender = status.MPI_SOURCE;

				sum += tmp;
			}
		}			 
		// prints the final sum of array
		printf("Total sum : %d\n", sum);
	}

	// slave processes
	else {
		for (int i = 0; i < batches; i++) {
			MPI_Recv(&n_elements_received,
					1, MPI_INT, 0, 0,
					MPI_COMM_WORLD,
					&status);

			// stores the received array segment in local array a2
			MPI_Recv(&a2, n_elements_received,
					MPI_INT, 0, 0,
					MPI_COMM_WORLD,
					&status);

			// calculates its partial sum
			int partial_sum = 0;
			for (int j = 0; j < n_elements_received; j++)
					partial_sum += a2[j];

			// sends the partial sum to the root process
			MPI_Send(&partial_sum, 1, MPI_INT,
					0, 0, MPI_COMM_WORLD);
    	}
	}

	MPI_Barrier(MPI_COMM_WORLD); 
	double end_time = MPI_Wtime();
	MPI_Finalize();

	double time = end_time - start_time;

	if (pid == 0) {
		// declare a character array
		char output[100];

		// format the string and store it in the array
		sprintf(output, "Execution time: %f seconds, with batch number: %d\n", time, batches);

		printf("%s", output);

		FILE *file = fopen("results.txt", "a");
		if (file != NULL) {
			fprintf(file, "%s", output);
			fclose(file);
		}
	}

	return 0;
}
