####### Definitions

cflags = -g -G
lflags = 
target = prefix
NVCC = nvcc

$(target) : prefix.cu
	$(NVCC) $(cflags) $(lflags) -o $(target) prefix.cu

####### Clean

.PHONY : clean
clean :
	-rm -f $(target) core *~
