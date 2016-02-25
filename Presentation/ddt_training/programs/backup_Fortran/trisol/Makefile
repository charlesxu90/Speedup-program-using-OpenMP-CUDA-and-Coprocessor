####### Definitions

F90 = mpif90
FC = mpif77
objects = mod_trisol.o trisol.o gendata.o solve.o check.o matnrm.o \
          numroc.o dlaruv.o 
cflags = -g -O0
lflags = -g 
target = trisol.exe

####### Implicit rules

.SUFFIXES: .f90 .f77 .f .F
.f.o:
	$(FC) -c $(cflags) -o $@ $<
.f90.o:
	$(F90) -c $(cflags) -o $@ $<
.F.o:
	$(F90) -free -c $(cflags) -o $@ $<
.f77.o:
	$(FC) -c $(cflags) -o $@ $<

####### Target

$(target) : $(objects)
	$(F90) $(lflags) -o $(target) $(objects)

####### Clean

.PHONY : clean
clean :
	-rm -f $(target) core $(objects) *~ *.mod
