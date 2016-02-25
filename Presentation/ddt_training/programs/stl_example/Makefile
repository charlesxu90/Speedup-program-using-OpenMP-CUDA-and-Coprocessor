####### Definitions

objects = stl_example.o
cflags = -g -O0
lflags = 
target = stl_example
CXX = g++

####### Implicit rules

.SUFFIXES: .cpp
.cpp.o:
	$(CXX) -c $(cflags) -o $@ $<

####### Target

$(target) : $(objects)
	$(CXX) $(cflags) $(lflags) -o $(target) $(objects)

####### Objects

stl_example.o :

####### Clean

.PHONY : clean
clean :
	-rm -f $(target) core $(objects) *~
