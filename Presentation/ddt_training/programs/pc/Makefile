####### Definitions

objects = pc.o
cflags = -g -O0
lflags =  -lpthread
target = pc
CC = cc

####### Implicit rules

.SUFFIXES: .c
.c.o:
	$(CC) -c $(cflags) -o $@ $<

####### Target

$(target) : $(objects)
	$(CC) $(cflags) -o $(target) $(objects) $(lflags)

####### Clean

.PHONY : clean
clean :
	-rm -f $(target) core $(objects) *~
