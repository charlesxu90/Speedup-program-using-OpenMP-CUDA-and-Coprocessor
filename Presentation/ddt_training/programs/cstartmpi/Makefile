CC = mpicc

####### Definitions

objects = cstartmpi.o
cflags = -g -O0
lflags = 
target = cstartmpi.exe

####### Implicit rules

.SUFFIXES: .c
.c.o:
	$(CC) -c $(cflags) -o $@ $<

####### Target

$(target) : $(objects)
	$(CC)  $(cflags) $(lflags) -o $(target) $(objects)

####### Clean

.PHONY : clean
clean :
	-rm -f $(target) core $(objects) *~
