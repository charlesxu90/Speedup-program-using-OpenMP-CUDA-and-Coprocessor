#include <mpi.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 1024*1024
int max = 5;
int myid;
int numprocs;
int received;

void passItOn()
{
  int from;
  int to;
  int *buffer;
  int *send_buffer;
  MPI_Status status;
  from = (myid +  numprocs - 1) % numprocs;
  to = (myid + 1) % numprocs;
  buffer = malloc(sizeof(int)*BUFSIZE);

  MPI_Recv(buffer, BUFSIZE, MPI_INT, from, 0, MPI_COMM_WORLD, &status);
  received = received + 1;

  send_buffer = malloc(sizeof(int)*BUFSIZE);
  memcpy(send_buffer, buffer, sizeof(int)*BUFSIZE); 
  MPI_Send(send_buffer, BUFSIZE, MPI_INT, to, 0, MPI_COMM_WORLD);
  free(send_buffer);
}


int main(int argc, char** argv)
{
  
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD,&myid); 

  received = 0;

  if (myid == 0)                /* inject the message into the loop */
    {
      MPI_Send(malloc(sizeof(int)*BUFSIZE), BUFSIZE, MPI_INT, 1, 0, MPI_COMM_WORLD);
    }

  while (received < max)
    {
      passItOn();
    }

  MPI_Barrier(MPI_COMM_WORLD);

  MPI_Finalize();
  
}
