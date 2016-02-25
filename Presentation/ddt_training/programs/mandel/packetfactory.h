#include "packet.h"

/* The Queen's Birthday, a prime number in ISO format */
#define PRIME 19260421U
#define RANDOM 324981U

class PacketFactory
{
 protected:
  unsigned int procId;
  unsigned int numProcs;
  Packet packet;
 public:

  PacketFactory(unsigned int procId, unsigned int numProcs, Packet& p)
    : procId(procId), numProcs(numProcs), packet(p) {}

  virtual Packet operator*() = 0;

  void stitch(Packet& p);
  void plot();
};



class SimplePacketFactory : public PacketFactory
{
  unsigned   int xnumPackets;  
  unsigned   int ynumPackets;
  unsigned   int xiter;                     /* the number of packets made so far */
  unsigned   int yiter;                     /* the number of packets made so far */

  
 public:
  /* The packet parameter is the whole space to cover
   */
  SimplePacketFactory(Packet &p,unsigned  int xnumPackets, unsigned  int ynumPackets, 
                      unsigned int procId, unsigned  int numProcs);
 
  virtual bool mine(unsigned int x);
  virtual Packet operator*();
  virtual SimplePacketFactory& operator++();
  bool done();
};


class RandomPacketFactory : public SimplePacketFactory
{
  
 public:
  RandomPacketFactory (Packet &p, unsigned int xnumPackets, unsigned int ynumPackets, 
                       unsigned int procId, unsigned int numProcs);

  
  virtual bool mine(unsigned int);
};
