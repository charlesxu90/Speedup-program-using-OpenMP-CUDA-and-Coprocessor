####### Definitions

objects = trisol.o gendata.o solve.o check.o matnrm.o \
          numroc.o dlaruz.o dlaruv.o 
cflags = -g -O0
lflags = -g
target = trisol.exe
FC = mpif77
CC = mpicc

####### Implicit rules

.SUFFIXES: .c  .f

.f.o:
	$(FC) -c $(cflags) -o $@ $<
.c.o:
	$(CC) -c $(cflags) -o $@ $<

####### Target

$(target) : $(objects)
	$(FC) $(cflags) $(lflags) -o $(target) $(objects)

####### Clean

.PHONY : clean
clean :
	-rm -f $(target) core $(objects) *~
