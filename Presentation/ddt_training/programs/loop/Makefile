####### Definitions

objects = loop.o
cflags = -g -O0
lflags = 
target = loop

CC = mpicc
####### Implicit rules

.SUFFIXES: .c
.c.o:
	$(CC) -c $(cflags) -o $@ $<

####### Target

$(target) : $(objects)
	$(CC) $(cflags) $(lflags) -o $(target) $(objects)

####### Clean

.PHONY : clean
clean :
	-rm -f $(target) core $(objects) *~
