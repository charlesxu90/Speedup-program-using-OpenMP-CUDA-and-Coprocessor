####### Definitions

# for gcc add "-fopenmp"
CC=mpicc
objects = c_openmp_mpi.o
cflags = -g
lflags = $(LFLAGS)
ompflags ?= -openmp
target = c_openmp_mpi

####### Implicit rules

.SUFFIXES: .c
.c.o:
ifeq ($(ompflags), )
	@echo 'This is an OpenMP program. You must specify the OMPFLAGS environment variable before making'
	@exit 1
else
	$(CC) -c $(cflags) $(ompflags) -o $@ $<
endif

####### Target

$(target) : $(objects)
	$(CC) $(cflags) $(ompflags) $(lflags) -o $(target) $(objects)

####### Objects

c_openmp_mpi.o :

####### Clean

.PHONY : clean
clean :
	-rm -f $(target) core $(objects) *~
