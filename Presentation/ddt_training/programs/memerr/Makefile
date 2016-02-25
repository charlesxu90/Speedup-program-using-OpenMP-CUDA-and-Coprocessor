####### Definitions

objects = memerr.o Check_free.o Check_pointer.o
cflags = -g -O0
lflags = 
target = memerr.exe
CC = cc

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
