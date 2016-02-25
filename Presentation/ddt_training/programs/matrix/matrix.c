/*

A simple code for calculating C = A * B
 
*/
#include <stdio.h>
#include <stdlib.h>

#define SIZE 42

void init_array(double array[SIZE][SIZE], int col)
{
    int i, j;
    for(i=0;i<SIZE;i++)
        for(j=0;j<SIZE;j++)
            array[i][j] = (col==1?i:j);
}

double A[SIZE][SIZE];
double B[SIZE][SIZE];
double C[SIZE][SIZE]; 

int main(int argc, char* argv[])
{
    int i, j, k;

    /* initialize */
    init_array(A, 1);
    init_array(B, 2);
    init_array(C, 0);

    /* compute */
    for (i = 0 ; i < SIZE; i++)
      for (j = 0 ; j < SIZE; j++)      
        for (k = 0 ; k < SIZE; k++)
          {
            C[i][j] += A[i][k] * B[k][j];
            if(C[i][j]<0) printf("Info: negative value at %d, %d iteration %d\n", i, j, k);
          }

    /* quick check */
    printf("Check: C[1][1] = 42 (SIZE*1)\nActual value: C[1][1] = %g\n", C[1][1]);

/*  for (i = 0; i < SIZE; i++)
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
*/
}

