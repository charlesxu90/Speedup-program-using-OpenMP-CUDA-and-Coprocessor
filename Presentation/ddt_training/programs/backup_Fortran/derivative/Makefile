####### Definitions

objects = derivatives.o
cflags = -g 
lflags = 
target = derivatives

####### Implicit rules

.SUFFIXES: .f
.f.o:
	$(F90) -c $(cflags) -o $@ $<

####### Target

$(target) : $(objects)
	$(F90) $(lflags) -o $(target) $(objects)

####### Clean

.PHONY : clean
clean :
	-rm -f $(target) core $(objects) *~ *.mod
