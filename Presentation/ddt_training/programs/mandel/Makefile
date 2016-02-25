####### Definitions
# runtime arguments -random 5 10 15 20

CXX = mpiCC
objects = mandel.o packet.o packetfactory.o strategy.o
cflags = -O0 -g
lflags = 
target = mandel

####### Implicit rules

.SUFFIXES: .cpp
.cpp.o:
	$(CXX) -c $(cflags) -o $@ $<

####### Target

$(target) : $(objects)
	$(CXX) $(cflags) $(lflags) -o $(target) $(objects)

####### Clean

.PHONY : clean
clean :
	-rm -f $(target) core $(objects) *~
