#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <mpi.h>


/**
 * Simple Hello World style OpenMP + MPI program that outputs the OpenMP thread
 * id and MPI process id in the form:
 * "Hello from from proc <proc_id>, thread <thread_id>".
 */
int main(int argc, char* argv[])
{
	int thread_id;
	int proc_id;
	int num_procs;
	int* array;
	
	MPI_Init(&argc, &argv);
	
	MPI_Comm_rank(MPI_COMM_WORLD, &proc_id);
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
	
	array = (int*) calloc( num_procs, sizeof(int) );

	array[0] = (proc_id >= num_procs/2) ? 0 : 1;
	array[1] = (proc_id >= num_procs/2) ? 1 : 0;
	
	#pragma omp parallel private(thread_id)
	{
		thread_id = omp_get_thread_num();

		printf("Hello from proc %d, thread %d\n", proc_id, thread_id);

		/* Dummy line of code to use for a breakpoint and to prevent intel optimising out the array */
		array[3] = 1;
	}

	MPI_Finalize();
}

