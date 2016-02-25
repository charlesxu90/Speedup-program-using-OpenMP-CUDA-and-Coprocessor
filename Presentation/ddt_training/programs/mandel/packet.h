#include <iostream>

using namespace std;
struct Packet
{
  double x1, y1, x2, y2;
  int pointsx, pointsy;         /* number of points to compute */
  int xstart, ystart;           /* point number of starting point (relative) */
  int *iterations;

 public:
 Packet(double x1, double y1, double x2, double y2, int pointsx, int pointsy, int xstart, int ystart);
  Packet(const Packet& p);
  Packet() : iterations(0) {}

  void compute();
  ~Packet();

  friend ostream& operator<<(ostream &s, Packet& p); 

  void stitch(Packet& p);
  void allocate();

  void plot();
};




ostream& operator<<(ostream &s, Packet &p);


