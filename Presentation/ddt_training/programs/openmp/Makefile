####### Definitions
CC ?= gcc
ompflags ?= -fopenmp
objects = matrix.o
cflags = -g -O0 ${ompflags}
lflags = 
target = matrix

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
