#include "math.h"

#include <string>
#include <list>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "mandel.h"
#include "packetfactory.h"

#include <mpi.h>


#define ITER_MAX 120
// 40 would be a good enough value - there's some theorem about it being good enough.. 
// but we want to exaggerate for MPI demo purpose

using namespace std;


static int rank;
static int procs;
#define BASETAG 1
#define RESULTS 3
#define DONE 4

int done = 0;

void receive(SimplePacketFactory &factory, int rank, int procs)
{
  MPI_Status status;
  for (int i = 0 ; i < procs - 1 && done < procs - 1 ; i++)
    {
      MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      
      if (status.MPI_TAG == DONE)
        { 
          done++;
          MPI_Recv(0, 0, MPI_DOUBLE, status.MPI_SOURCE, status.MPI_TAG, MPI_COMM_WORLD, &status);
        }
      else 
        {
          int src = status.MPI_SOURCE;
          Packet p;
          MPI_Recv(&p, sizeof(p), MPI_CHAR, src, BASETAG, MPI_COMM_WORLD, &status);
          p.iterations = 0;
          p.allocate();
          MPI_Recv(p.iterations, p.pointsx * p.pointsy, MPI_INT, src, RESULTS, MPI_COMM_WORLD, &status);

          factory.stitch(p);
          
          
          // cout << "\033[H\033[J";
          // factory.plot();
        }
    }
}
void strategy1(SimplePacketFactory &factory, int rank, int procs)
{
  done = 0;
    while (!factory.done())
    {

      Packet p = *(factory);
      ++factory;

      p.compute();

      if (rank != 0)
        {
          
          MPI_Ssend(&p, sizeof(p), MPI_CHAR, 0, BASETAG, MPI_COMM_WORLD);

          MPI_Ssend(p.iterations, p.pointsx * p.pointsy, MPI_INT, 0, RESULTS, 
                   MPI_COMM_WORLD);
        }
      else 
        {
          factory.stitch(p);
          receive(factory, rank, procs);
        }
    }
    
    if (rank != 0) 
      {
        MPI_Ssend(0, 0, MPI_DOUBLE, 0, DONE, MPI_COMM_WORLD);
      }
    
    if (rank == 0)
      while (done < procs - 1)
        receive(factory, rank, procs);
}

void strategy2(SimplePacketFactory &factory, int rank, int procs)
{
  done = 0; 
  list<Packet> packets;
  while (!factory.done())
    {
      Packet p = *(factory);
      ++factory;

      p.compute();

      packets.push_back(p);
    }
  
  for (list<Packet>::iterator it = packets.begin(); it != packets.end(); ++it)
    {
      Packet p = *(it);
      if (rank == 0)
        factory.stitch(p);
      else 
        {
          
          MPI_Ssend(&p, sizeof(p), MPI_CHAR, 0, BASETAG, MPI_COMM_WORLD);
          MPI_Ssend(p.iterations, p.pointsx * p.pointsy, MPI_INT, 0, RESULTS, 
                   MPI_COMM_WORLD);
        }
    }
 
  
  if (rank == 0)
    {
      while (done < procs - 1)
        receive(factory, rank, procs);
    }
  else {
    MPI_Ssend(0, 0, MPI_DOUBLE, 0, DONE, MPI_COMM_WORLD);
  }
}

#ifdef RANDOM
#undef RANDOM
#endif
#define RANDOM 0 
#define CACHE 1
#define IMMEDIATE 2

int main(int argc, char* argv[])
{
  double x1, y1, x2, y2;
  int pointsx, pointsy;
  int xpackets, ypackets;
  int method = 0;
  int p = 0;

  if (argc != 1 && argc < 8)
    {
      cout << "Usage: " << argv[0] << " -{random|cache|immediate} x1 y1 x2 y2 pointsx pointsy xpackets ypackets" << endl;
      exit(1);
    }
  
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &procs);

  if (rank == 0)
    {
      if (argc == 8) 
        {
          string s (argv[++p]);
          if (s == "-random")
        	method = RANDOM;
          else if (s == "-cache")
        	method = CACHE;
          else if (s == "-immediate")
        	method = IMMEDIATE;
          else {
        	cout << "not recognized strategy! try random, cache or immediate: " << argv[p];
        	exit(1);
          }

          x1 = atof(argv[++p]);
          y1 = atof(argv[++p]);
          x2 = atof(argv[++p]);
          y2 = atof(argv[++p]);
          pointsx = atoi(argv[++p]);
          pointsy = atoi(argv[++p]);
          xpackets = atoi(argv[++p]);
          ypackets = atoi(argv[++p]);
        }
      else
        {
          method = IMMEDIATE;
          x1 = 0;
          y1 = 0;
          x2 = 100;
          y2 = 100;
          pointsx = 2000;
          pointsy = 2000;
          xpackets = 10;
          ypackets = 10;
        }
    }

  Packet all(x1, y1, x2, y2, pointsx, pointsy, 0, 0);

  MPI_Bcast(&all, sizeof(all), MPI_CHAR, 0, MPI_COMM_WORLD);
  MPI_Bcast(&xpackets, sizeof(int), MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&ypackets, sizeof(int), MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&method, sizeof(int), MPI_INT, 0, MPI_COMM_WORLD);
  
  double t0 = MPI_Wtime();

  SimplePacketFactory *factory;

  switch (method)
    {
    case RANDOM:
      factory = new RandomPacketFactory(all, xpackets, ypackets , rank, procs);
      strategy2(*factory, rank, procs);
      break;
    case CACHE:
      factory = new SimplePacketFactory(all, xpackets, ypackets, rank, procs);
      strategy2(*factory, rank, procs);
      break;
    case IMMEDIATE:
      factory = new SimplePacketFactory(all, xpackets, ypackets, rank, procs);
      strategy1(*factory, rank, procs);
      break;
    }

  MPI_Barrier(MPI_COMM_WORLD);
 
  double t1 = MPI_Wtime();

  if (rank == 0) 
    {
      // factory->plot(); 
      cout << "Elapsed: " << t1 - t0 << "s" <<endl;
    }
  MPI_Finalize();
}

int mandel(double x, double y)
{
  // iterate f (z)  = z^2 + (x + iy)
  // if |f(z)|> 2 for any n-th iteration, it's not in the set, return n

  int n = 0;
  double zx = 0;
  double zy = 0;
  double zi = 0;  double zj = 0;  double zk = 0; // timewasters to exaggerate program
  while (n < ITER_MAX && (zx * zx + zy * zy) <= 2 && (zi + zj + zk < 100000))
    {
      double zx1;
      double zy1;

      // compute next value
      zx1 = (zx*zx) - (zy*zy) + x;
      zy1 = 2 * zx * zy + y;
        
      zx = zx1;
      zy = zy1;

      zj = zx * zy;		// timewasting
      zi = sqrt(zx / zy1 + zj);
      zk = zx1 * zy + zi;


      n++;
    }
  return n;
}
