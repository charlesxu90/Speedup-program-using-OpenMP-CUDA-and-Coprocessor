/*

A simple OpenMP code for calculating C = A * B
 
*/
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE 16

int main(int argc, char* argv[])
{
double A[SIZE][SIZE];
double B[SIZE][SIZE];
double C[SIZE][SIZE]; 

  int i, j, k;
#pragma omp parallel shared(A,B,C) private (i, j, k)
  {

    /* initialize */

//    printf("I am %d\n", omp_get_thread_num());
    fflush(stdout);
#pragma omp for 
    for (i = 0 ; i < SIZE; i++)
      for (j = 0 ; j < SIZE; j++)
        {
          A[i][j] = i + j;
          B[i][j] = i*i + j*j;
          C[i][j] = 0;
        }
#pragma omp for 
    for (i = 0 ; i < SIZE; i++)
      for (j = 0 ; j < SIZE; j++)      
        for (k = 0 ; k < SIZE; k++)
          {
            C[i][j] += A[i][k] * B[k][j];
          }
    
  }

  for (i = 0; i < SIZE; i++)
    {
      printf ("| ");
      for (j = 0 ; j < SIZE; j++)
        {
          printf(" %.2f ", A[i][j]);
        }
      printf (" |  *  | ");
      for (j = 0 ; j < SIZE; j++)
        {
          printf(" %.2f ", B[i][j]);
        }
      printf (" |  =  |");
      for (j = 0 ; j < SIZE; j++)
        {
          printf(" %.2f ", C[i][j]);
        }
      printf(" |\n");
    }
}

