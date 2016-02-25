#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/time.h>
typedef char * caddr_t;
#include<papi.h>

#define MX 1024
#define NITER 20

double *ad[MX];

// ad = 1.0/3.0*ones(MX)
// ad(K+1)=ad(K)*3.0

/* Get actual CPU time in seconds */
int main () 
{
    int iter, i, j;
    // timer
    double t=0;
    struct timeval tvStart, tvEnd;
    // Flop counter
    int papi_events[1] = {PAPI_FP_OPS};
    int ret_val;
    long_long papi_values[1];

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
    
    if (PAPI_num_counters() <= 2){ 
        fprintf(stderr,"Info:: Problem with program counters, or PAPI not supported.\n");
    }
    // Start PAPI counters
    if ((ret_val = PAPI_start_counters(papi_events, 1)) != PAPI_OK){
        fprintf(stderr,"Info:: Fail to start PAPI: %s.\n", PAPI_strerror(ret_val));
    }

    gettimeofday(&tvStart, NULL);
    
    for (iter = 0; iter < NITER; iter++) {
        for (j = 0; j < MX; j++) {
            for (i = 0; i < MX; i++) {
                ad[i][j] += ad[i][j] * 3.0;
            }
        }
    }
    // Read PAPI counters
    if ((ret_val = PAPI_read_counters(papi_values, 1)) != PAPI_OK){
        fprintf(stderr, "Info::PAPI failed to read counters: %s\n", PAPI_strerror(ret_val));
    }
    gettimeofday(&tvEnd,NULL);
    t = (double) (tvEnd.tv_usec - tvStart.tv_usec) / 1000000 + (double) (tvEnd.tv_sec - tvStart.tv_sec);
    printf("%0.4f\t%lld\n",t,papi_values[0]);
}
//Floating point operations = 20X1024X1024X2=42991616;
