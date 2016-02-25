#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/time.h>

//#define char* caddr_t;

#include "papi.h"

#define MX 1024
#define NITER 20
#define MEGA 1000000
#define TOT_FLOPS (2*MX*MX*NITER)

double *ad[MX];

/* Get actual CPU time in seconds */
float gettime() 
{
    return((float)PAPI_get_virt_usec()*1000000.0);
}
int main () 
{
    float t0, t1;
    int iter, i, j;
    int events[2] = {PAPI_L1_DCM, PAPI_FP_OPS }, ret;
    long_long values[2];

    if (PAPI_num_counters() < 2) {
        fprintf(stderr, "No hardware counters here, or PAPI not supported.\n");
        exit(1);
    }
    for (i = 0; i < MX; i++) {
        if ((ad[i] = malloc(sizeof(double)*MX)) == NULL) {
            fprintf(stderr,"malloc failed\n");
            exit(1);
        }
    }
    for (j = 0; j < MX; j++) { 
        for (i = 0; i < MX; i++) {
            ad[i][j] = 1.0/3.0; /* Initialize the data */
        }
    }
    t0 = gettime();
    if ((ret = PAPI_start_counters(events, 2)) != PAPI_OK) {
        fprintf(stderr, "PAPI failed to start counters: %s\n", PAPI_strerror(ret));
        exit(1);
    }
    for (iter = 0; iter < NITER; iter++) {
        for (j = 0; j < MX; j++) {
            for (i = 0; i < MX; i++) {
                ad[i][j] += ad[i][j] * 3.0;
            }
        }
    }
    if ((ret = PAPI_read_counters(values, 2)) != PAPI_OK) {
        fprintf(stderr, "PAPI failed to read counters: %s\n", PAPI_strerror(ret));
        exit(1);
    }
    t1 = gettime();

    printf("Total software flops = %f\n",(float)TOT_FLOPS);
    printf("Total hardware flops = %lld\n",(float)values[1]);
    printf("MFlop/s = %f\n", (float)(TOT_FLOPS/MEGA)/(t1-t0));
    printf("L1 data cache misses is %lld\n", values[0]);
}
