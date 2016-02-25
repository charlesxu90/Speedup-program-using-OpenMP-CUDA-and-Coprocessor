
#include "packet.h"
#include "mandel.h"
#include <string.h>
#include <stdlib.h>
using namespace std;

Packet::Packet(double x1, double y1, double x2, double y2, int pointsx, 
               int pointsy, int xstart, int ystart) :
  x1(x1), y1(y1), x2(x2), y2(y2), pointsx(pointsx), pointsy(pointsy),
  xstart(xstart), ystart(ystart),
  iterations(0)
{
}

Packet::~Packet()
{
}


Packet::Packet(const Packet& p)
{
  x1 = p.x1;
  x2 = p.x2;
  y1 = p.y1;
  y2 = p.y2;
  pointsx = p.pointsx;
  pointsy = p.pointsy;
  xstart = p.xstart;
  ystart = p.ystart;
  iterations = 0;
  if (p.iterations)
    {
      allocate();
      memcpy(iterations, p.iterations, sizeof(int) * pointsx * pointsy);
    }
  else iterations = 0;
}


void Packet::compute()
{
  allocate();

  for (int i = 0;  i < pointsx; i++) {
    double x = (x2 - x1) / (double) pointsx * i + x1;
    for (int j = 0; j < pointsy; j++)
      {
        double y = (y2 - y1) / (double) pointsy * j + y1;
        int n = mandel(x, y);

        iterations[i * pointsy + j] = n;
      }
  }
}


ostream& operator<<(ostream& s, Packet &p)
{
  return s << "(" << p.x1 << "," <<p.y1 << ")...(" <<
    p.x2 << "," << p.y2 << ") -- [(" << p.xstart << ", " << p.ystart <<")] [ ( " << p.pointsx << " x " << p.pointsy<<" )" << endl;
}

/*
  Copy the segments from p into the major packet
*/
void
Packet::stitch(Packet &p)
{
  allocate();
  for (int x = p.xstart; x < p.xstart + p.pointsx; x++)
    {
      for (int y = p.ystart; y < p.ystart + p.pointsy; y++)
        {
          iterations[x * pointsy + y] = p.iterations[(x - p.xstart) * p.pointsy + y - p.ystart];
        }
    }

}


void Packet::allocate()
{

  if (!iterations)
    iterations = (int*)malloc(sizeof(int) * pointsx * pointsy);
}

void Packet::plot()
{
  if (!iterations) 
    {
      cout << "No data!";
      return;
    }
  
  for (int j = 0 ; j < pointsy; j++)
    {
      for (int i = 0 ; i < pointsx; i++)
        {
          int val = iterations[i * pointsy + j];
          cout << (char) (val + 65) ;

        }
      cout << endl;
    }
}
