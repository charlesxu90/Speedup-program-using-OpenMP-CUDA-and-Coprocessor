#include <stdio.h>
#include <stdlib.h>

bool verify(int data[], int length)
{
  for (int i = 1 ; i < length; ++i)
    {
      if (data[i] - data [i - 1] != i )
	{ printf("error at element %d\n", i); return false; }
    }
  return true;
}

#define DUMP(x) printf("%s %d\n", #x, props.x)

void dumpCUDAProps(cudaDeviceProp & props)
{
	DUMP(canMapHostMemory);
	DUMP(clockRate);
	DUMP(computeMode);
	DUMP(deviceOverlap);
	DUMP(integrated);
	DUMP(kernelExecTimeoutEnabled);
	DUMP(major);
	DUMP(maxGridSize[0]);
	DUMP(maxGridSize[1]);
	DUMP(maxGridSize[2]);
	DUMP(maxThreadsDim[0]);
	DUMP(maxThreadsDim[1]);
	DUMP(maxThreadsDim[2]);
	DUMP(maxThreadsPerBlock);
	DUMP(memPitch);
	DUMP(minor);
	DUMP(multiProcessorCount);
	printf("name %s\n", props.name);
	DUMP(regsPerBlock);
	DUMP(sharedMemPerBlock);
	DUMP(textureAlignment);
	DUMP(totalConstMem);
	DUMP(totalGlobalMem);
	DUMP(warpSize);

}

#define BLOCK_SIZE 64 

__global__ void prefixsumblock(int *in, int *out, int length)
{
	int x = threadIdx.x + blockIdx.x * BLOCK_SIZE;

	if (x < length)
          out[x] += in[x];

	__syncthreads();

	for ( int i = 1; i < BLOCK_SIZE; i <<= 1)
	{
	  if (threadIdx.x + i < BLOCK_SIZE && x + i < length) 
	    {
	      out[x + i] = in[x] + in[x + i];
	    }
 	  __syncthreads();

	  if (x < length)
  	    in[x] = out[x];

	  __syncthreads();
	}		
}

__global__ void correctsumends(int *ends, int *in, int *out)
{
	int x = threadIdx.x + blockIdx.x * BLOCK_SIZE;
	int end = ends[blockIdx.x];
	out[x] = in[x] + end;
}


__global__ void gathersumends(int *in, int *out)
{
	int x = threadIdx.x + blockIdx.x * BLOCK_SIZE;
	if (x > 0)
	   out[x] = in[x * BLOCK_SIZE - 1];
	else 
	   out[x] = 0;
}

__global__ void zarro(int *data, int length)
{
	int x = threadIdx.x + blockIdx.x * BLOCK_SIZE;

	if (x < length) 
	   data[threadIdx.x] = 0;
}


void prefixsum(int* in, int *out, int length)
{
	int blocks = (length + BLOCK_SIZE - 1) / BLOCK_SIZE;


	dim3 dimGrid(blocks, 1, 1);
	dim3 dimBlock(BLOCK_SIZE, 1, 1);

	zarro<<<dimGrid, dimBlock>>>(out, length);
	prefixsumblock<<<dimGrid, dimBlock>>>(in, out, length);

	if (blocks > 1)	{
	  int *devEnds;	
	  int *devTmpEnds;

  	  cudaMalloc((void**) &devEnds, blocks * sizeof(int));
 	  cudaMalloc((void**) &devTmpEnds, blocks * sizeof(int));
	  
	  int subblocks = (blocks + BLOCK_SIZE - 1) / BLOCK_SIZE;
	
	  dim3 subgrid(subblocks, 1, 1);
	  dim3 subblock(BLOCK_SIZE, 1, 1);
	  gathersumends<<<subgrid, subblock>>>(out, devEnds);
	  prefixsum(devEnds, devTmpEnds, blocks);

	  cudaFree(devEnds);

  	  correctsumends<<<dimGrid, dimBlock>>>(devTmpEnds, in, out);

	  cudaFree(devTmpEnds);
        }
}


void cudasummer(int data[], int length)
{
	
	int *devIn, *devOut;
	cudaEvent_t start, stop;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);
	cudaEventRecord(start, 0);
        cudaMalloc((void**) &devIn, length * sizeof(int));
        cudaMalloc((void**) &devOut, length * sizeof(int));
	cudaMemcpy(devIn, data, length * sizeof(int), cudaMemcpyHostToDevice);

	prefixsum(devIn, devOut, length);

	cudaMemcpy(data, devOut, length * sizeof(int), cudaMemcpyDeviceToHost);
        
	cudaFree(devIn);
	cudaFree(devOut);
        
	cudaEventRecord(stop, 0);
	cudaEventSynchronize(stop);
        
	float t;

	cudaEventElapsedTime(&t, start, stop);
	
	printf("Elapsed time %3fms\n", t);
        cudaEventDestroy(start);
        cudaEventDestroy(stop);

}

void devicesDump()
{

	int deviceCount;
	cudaGetDeviceCount(&deviceCount);
	int device;
	for (device = 0; device < deviceCount; ++device) {
	    cudaDeviceProp deviceProp;
	    cudaGetDeviceProperties(&deviceProp, device);
	    dumpCUDAProps(deviceProp);
        }
}


int main(int argc, char *argv[]) 
{

	int length;
	if (argc < 2) {
		length = 500;
	}	
	else length = atoi(argv[1]);

	int *data = (int*) malloc(length * sizeof(int));
	for (int i = 0; i < length; ++i) {
	    data[i] = i; //rand();
	}   		

	devicesDump();

	cudasummer(data, length);





	if (length < 1000) 
	for (int i = 0 ; i < length; ++i)
	    {
		printf("%d\n", data[i]);
	    }
	verify(data, length);
}

