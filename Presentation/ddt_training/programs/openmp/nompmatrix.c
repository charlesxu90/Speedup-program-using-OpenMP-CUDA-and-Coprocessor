#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

void mmult(double *A, double *B, double *C, int I, int J, int K)
{
  //  A(K, I) * B(J, K) = C(J, I)
  int i, j, k;
#pragma omp parallel private(j,k)
  for (i = 0 ; i < I; i++)
    for (j = 0 ; j < J; j++) 
      for (k = 0 ; k < K; k++)
        C[i * J + j] += A[i * K + k] * B[k * J + j];
}



int main(int argc, char* argv[]) 
{
  if (argc < 4) {
    fprintf(stderr, "fail");
    exit(1);
  }

  int i, j, k;
  int I = atoi(argv[1]);
  int J = atoi(argv[2]);
  int K = atoi(argv[3]);
  double *A = calloc(sizeof(double), J * I);
  double *B = calloc(sizeof(double), K * J);
  double *C = calloc(sizeof(double), J * I);
  for (i = 0; i <  I; i++)
    for (k = 0; k < K; k++)
      A[i * K + k] = i + k;
  for (k = 0; k <  K; k++)
    for (j = 0; j < J; j++)
      B[j * K + k] = j + k;
  
  mmult(A, B, C, I, J, K);

  for (i = 0; i <  I; i++) {
    for (k = 0; k < K; k++)
      printf("%2g " , A[i * K + k]);
    printf("\n");
  }
  for (k = 0; k <  K; k++) {
    for (j = 0; j < J; j++)
      printf("%2g " , B[J * k + j]);
    printf("\n");
  }
  for (i = 0; i <  I; i++) {
    for (j = 0; j < J; j++)
      printf("%2g " , C[i * J + j]);
    printf("\n");
  }
    

}


