####### Definitions

F90 = mpif90
objects = array.o 
cflags = -g -O0
lflags = -g 
target = array

####### Implicit rules

.SUFFIXES: .f90
.f90.o:
	$(F90) -c $(cflags) -o $@ $<

####### Target

$(target) : $(objects)
	$(F90) $(lflags) -o $(target) $(objects)

####### Clean

.PHONY : clean
clean :
	-rm -f $(target) core $(objects) *~ *.mod
