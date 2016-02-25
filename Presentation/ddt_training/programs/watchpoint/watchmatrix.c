/*

A simple MPI code for calculating C = A * B
 
*/
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE_M 5
#define SIZE_N 6
#define SIZE_O 5

int main(int argc, char* argv[])
{
    double A[SIZE_M][SIZE_N];
    double B[SIZE_N][SIZE_O];

    int rank;

    double C[SIZE_M][SIZE_O]; 

    int numprocs;
    int i, j, k;


    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

    printf("My rank is %d of %d\n", rank, numprocs);

    for (i = 0 ; i < SIZE_M; i++)
        for (j = 0 ; j < SIZE_N; j++)
            A[i][j] = i + j;
    for (i = 0 ; i < SIZE_N; i++)
        for (j = 0 ; j < SIZE_O; j++)
            B[i][j] = i*i + j*j;
    for (i = 0 ; i < SIZE_M; i++)
        for (j = 0 ; j < SIZE_O; j++)
            C[i][j] = 0;

    for (i = 0 ; i < SIZE_M; i++)
        for (j = 0 ; j < SIZE_N; j++)      
            for (k = 0 ; k < SIZE_O; k++)
                C[i][j] += A[i][k] * B[k][j];
    
    
    if (numprocs > 1) {
        MPI_Send(C, sizeof(C), MPI_CHAR, rank == numprocs - 1 ? 0 : rank + 1, 
                  0, MPI_COMM_WORLD);
        MPI_Recv(C, sizeof(C), MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }


    printf("My rank is %d of %d\n", rank, numprocs);

    if (argc > 1) {
        for (i = 0; i < SIZE_M; i++)
            {
                printf ("| ");
                for (j = 0 ; j < SIZE_N; j++)
                    printf(" %.2f ", A[i][j]);
                printf ("|\n");
            }

        printf (" |  *  | \n");

        for (i = 0 ; i < SIZE_N; i++)
            {
                printf ("| ");
                for (j = 0 ; j < SIZE_O; j++)
                    printf(" %.2f ", B[i][j]);
                printf ("|\n");
            }
        printf ("=\n");

        for (i = 0 ; i < SIZE_M; i++)
            {
                printf ("| ");
                for (j = 0 ; j < SIZE_O; j++)
                    printf(" %.2f ", C[i][j]);
                printf ("|\n");
            }
    }
    MPI_Finalize();
}

