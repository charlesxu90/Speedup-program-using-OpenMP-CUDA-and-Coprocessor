#include <iostream>

#include "packetfactory.h"

SimplePacketFactory::SimplePacketFactory(Packet& p, unsigned int xnumPackets, 
                                         unsigned int ynumPackets, unsigned int procId, 
                                         unsigned int numProcs) :
  PacketFactory(procId, numProcs, p), xnumPackets(xnumPackets), 
  ynumPackets(ynumPackets), xiter(0), yiter(0)
{
}

Packet
SimplePacketFactory::operator*()
{
  while (!mine(xiter * ynumPackets + yiter))
    {
      ++yiter;
      if (yiter >= ynumPackets)
        {
          yiter = 0;
          ++xiter;
      }
    }
    

  //  cout << procId << " " << "x: " << xiter << " y:" << yiter << endl; 
  // figure the bounds
  double x1 = (packet.x2 - packet.x1) / xnumPackets * xiter + packet.x1;
  double x2 = (packet.x2 - packet.x1) / xnumPackets * (xiter + 1) + packet.x1;
  double y1 = (packet.y2 - packet.y1) / ynumPackets * yiter + packet.y1;
  double y2 = (packet.y2 - packet.y1) / ynumPackets * (yiter + 1) + packet.y1;

  // figure the number of points

  int pointsx = (packet.pointsx * (xiter + 1) /  xnumPackets) - 
    (packet.pointsx * xiter /  xnumPackets);
  int pointsy = (packet.pointsy * (yiter + 1) /  ynumPackets) - 
    (packet.pointsy * yiter / ynumPackets);

  int xstart =  packet.pointsx * xiter / xnumPackets;
  int ystart =  packet.pointsy * yiter / ynumPackets;
  
  Packet p(x1, y1, x2, y2, pointsx, pointsy, xstart, ystart);

  //  cout << procId << " " << p;
  
  return p;

}


SimplePacketFactory&
SimplePacketFactory::operator++()
{ 
  ++yiter;
  if (yiter >= ynumPackets)
    {
      yiter = 0;
      ++xiter;
    }

  return *this;
}

bool
SimplePacketFactory::done() { 
  
  while (!mine(xiter * ynumPackets + yiter))
    {
      ++yiter;
      if (yiter >= ynumPackets)
        {
          yiter = 0;
          ++xiter;
      }
    }

  return xiter >= xnumPackets;
}

void
PacketFactory::stitch(Packet& p)
{
  packet.stitch(p);
}

void
PacketFactory::plot()
{
  packet.plot();
}


bool SimplePacketFactory::mine(unsigned int x)
{
  return (x % numProcs) == procId; 
}

bool RandomPacketFactory::mine(unsigned int x)
{
  //  cout << procId << " " << numProcs <<  " " << x << endl;
  return (((x * RANDOM) % PRIME) % (unsigned) numProcs) == procId;
}

RandomPacketFactory::RandomPacketFactory (Packet &p, unsigned int xnumPackets, unsigned int ynumPackets, 
                                          unsigned int procId, unsigned int numProcs) :
  SimplePacketFactory(p, xnumPackets, ynumPackets, procId, numProcs)
{
}


