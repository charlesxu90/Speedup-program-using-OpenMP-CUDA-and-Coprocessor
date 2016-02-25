	/*
 * TO DO:
 *
 */

#define _XOPEN_SOURCE 600       /* make stdlib.h provide posix_memalign() in strict C99 mode */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
typedef char* caddr_t;
#include "utils.h"
#include "driver.h"
#include "papi.h"

#define U(i,j,k) (U1[((i)*p.jp+(j))*p.kp+(k)])
#define V(i,j,k) (U2[((i)*p.jp+(j))*p.kp+(k)])
#define Uc(i,j,k) (tempU1[((i)*p.jp+(j))*p.kp+(k)])
#define Vc(i,j,k) (tempU2[((i)*p.jp+(j))*p.kp+(k)])
#define ROC2(i,j,k) (U3[((i)*p.jp+(j))*p.kp+(k)])

// set the default values of the kernel parameters
void param_default(Parameters *p) {
  p->ip = 720;
  p->jp = 720;
  p->kp = 720;
  p->alignment = 16;
  p->domain_size = p->ip*p->jp*p->kp;
  p->n_stencils = (p->ip-8)*(p->jp-8)*(p->kp-8);
  p->n_tests = 1;
}

void csource(int nt, float fmax, float dt, float *source)
{
  int          it;
  float sigma,tau,pi,t,scale,tmp;

  sigma=0.6*fmax;
  tau=1.0;
  scale=8.0;
  pi=3.1415927;
  tmp=0.;
  for( it=0;it<nt; it++) {
    t=dt*(float)(it-1);
    source[it]=-2*scale*sigma;
    source[it]*=(sigma-2.0*sigma*scale*powf( (sigma*t-tau),2.) );
    source[it]*= expf(-scale*powf((sigma*t-tau),2.));
    tmp += source[it];
  }
  return;
}

float cfl(int dx, int dy, int dz, float cmax, float *coef) {
  int k;
  float courant, dt;

  courant = fabs(coef[0])*3;
  for (k=1;k<5;k++) courant +=fabs(coef[k])*6;

  courant=2./sqrtf(courant);

  dt=0.6*courant*dx/cmax; // ASSUMING dx=dy=dz

  return dt;
}

int main (int argc, char** argv)
{
  float *restrict  U1;
  float *restrict U2;
  float *restrict U3;
  float *restrict source;
  float *restrict  tempU1;
  float *restrict tempU2;
  int i,x,y,z,nt,devnum;
  float lap;
  int tests_remain;
  struct timeval tvStart, tvEnd;
  double t_start, t_end, t=0;
  double t_min = 1000000, t_max= -1, tpercycle;

  // Define PAPI counter variables
  int papi_events[2] = {PAPI_L1_DCA, PAPI_L1_DCM};
  int ret_val;
  long_long papi_values[2];

  /*int papi_events[6] = {PAPI_L1_DCA, PAPI_L1_DCM, PAPI_L1_ICA, PAPI_L1_ICM, PAPI_BR_CN, PAPI_FP_OPS};
  int ret_val;
  long_long papi_values[6];
*/


  //double starttimer, endtimer;
  float time=0.1;
  float fmax=15.0;
  float cmax = 3000.0; 
  float c = 1500.0;
  float dx,dy,dz;
  float dt;
  ////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////
  // configure the experiment's parameters
  Parameters p;
  param_default(&p);
  parse_args (argc, argv, &p);

  int ixs= p.ip/2;
  int iys= p.jp/2;
  int izs= p.kp/2;

  dx=10.0;  dy=10.0;  dz=10.0;
  
  ////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////
  // allocate and initialize the required arrays
  float coef[5] = {-205./72., 8./5., -1./5.,  8./315., -1./560.};

  posix_memalign((void **)&U1, p.alignment, sizeof(float)*p.domain_size);
  posix_memalign((void **)&U2, p.alignment, sizeof(float)*p.domain_size);
  posix_memalign((void **)&tempU1, p.alignment, sizeof(float)*p.domain_size);
  posix_memalign((void **)&tempU2, p.alignment, sizeof(float)*p.domain_size);

  posix_memalign((void **)&U3, p.alignment, sizeof(float)*p.domain_size);


  // CFL
  //dt =9.05711029E-04;
  dt = cfl(dx, dy, dz, cmax, coef);

  // Initialize wavefields and velocity model
  for (i=0; i<p.domain_size; i++) {
    U1[i] = 0.;
    U2[i] = 0.;
    tempU1[i] = 0.;
    tempU2[i] = 0.;
    U3[i] = dt*dt*c*c;
  }
 
 // Update coefficients with regard to dx dy and dz
  for(i=0;i<5;i++) {
    coef[i]=coef[i]/dx/dx;
  }
  //coef[0] *=3;

  //-- Source Positioned in the center --
  ixs= p.ip/2;
  iys= p.jp/2;
  izs= p.kp/2;

  //-- Iterations Number --
  nt=roundf(time/(2*dt))*2;
  printf("Reference code info: nt=%d    dt=%e\n",nt ,dt);

  // Generate the source term (should be nt instead of ncycles, we use this for testing )
  //nt = 1000;
  posix_memalign((void **)&source, p.alignment, sizeof(float)*nt);
  csource( nt, fmax, dt, source);

  ////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////
  // run the performance experiments of the target kernel
  printf("\n******************************************************\n");
  printf("Performance results\n");
  printf("******************************************************\n");
  if (PAPI_num_counters() <= 2){
      fprintf(stderr,"Info:: Problem with program counters, or PAPI not supported.\n");
  }

  // Start PAPI counters
  if ((ret_val = PAPI_start_counters(papi_events, 2)) != PAPI_OK){
      fprintf(stderr,"Info:: Fail to start PAPI: %s.\n", PAPI_strerror(ret_val));
  }


  gettimeofday(&tvStart, NULL);

  for (i=0; i<nt/2; i+=2) { // time loop
      //printf("Time step: %d\n", i);
      for(x=4; x< p.ip-4; x++) {
          for(y=4; y< p.jp-4; y++) {
              for(z=4; z< p.kp-4; z++) {
                  lap=coef[0]*Vc(x,y,z)
                      +coef[1]*(Vc(x+1,y  ,z  )+Vc(x-1,y  ,z  ))
                      +coef[1]*(Vc(x  ,y+1,z  )+Vc(x  ,y-1,z  ))
                      +coef[1]*(Vc(x  ,y  ,z+1)+Vc(x  ,y  ,z-1))
                      +coef[2]*(Vc(x+2,y  ,z  )+Vc(x-2,y  ,z  ))
                      +coef[2]*(Vc(x  ,y+2,z  )+Vc(x  ,y-2,z  ))
                      +coef[2]*(Vc(x  ,y  ,z+2)+Vc(x  ,y  ,z-2))
                      +coef[3]*(Vc(x+3,y  ,z  )+Vc(x-3,y  ,z  ))
                      +coef[3]*(Vc(x  ,y+3,z  )+Vc(x  ,y-3,z  ))
                      +coef[3]*(Vc(x  ,y  ,z+3)+Vc(x  ,y  ,z-3))
                      +coef[4]*(Vc(x+4,y  ,z  )+Vc(x-4,y  ,z  ))
                      +coef[4]*(Vc(x  ,y+4,z  )+Vc(x  ,y-4,z  ))
                      +coef[4]*(Vc(x  ,y  ,z+4)+Vc(x  ,y  ,z-4));

                  Uc(x,y,z) = 2.*Vc(x,y,z) - Uc(x,y,z) + ROC2(x,y,z)*lap;
                  if( (x==ixs) && (y==iys) && (z==izs) ){
                      Uc(ixs,iys,izs) =  Uc(ixs,iys,izs) + source[i];
                  }
              }
          } 
      }

      for(x=4; x< p.ip-4; x++) {
          for(y=4; y< p.jp-4; y++) {
              for(z=4; z< p.kp-4; z++) {
                  lap=coef[0]*Uc(x,y,z)
                      +coef[1]*(Uc(x+1,y  ,z  )+Uc(x-1,y  ,z  ))
                      +coef[1]*(Uc(x  ,y+1,z  )+Uc(x  ,y-1,z  ))
                      +coef[1]*(Uc(x  ,y  ,z+1)+Uc(x  ,y  ,z-1))
                      +coef[2]*(Uc(x+2,y  ,z  )+Uc(x-2,y  ,z  ))
                      +coef[2]*(Uc(x  ,y+2,z  )+Uc(x  ,y-2,z  ))
                      +coef[2]*(Uc(x  ,y  ,z+2)+Uc(x  ,y  ,z-2))
                      +coef[3]*(Uc(x+3,y  ,z  )+Uc(x-3,y  ,z  ))
                      +coef[3]*(Uc(x  ,y+3,z  )+Uc(x  ,y-3,z  ))
                      +coef[3]*(Uc(x  ,y  ,z+3)+Uc(x  ,y  ,z-3))
                      +coef[4]*(Uc(x+4,y  ,z  )+Uc(x-4,y  ,z  ))
                      +coef[4]*(Uc(x  ,y+4,z  )+Uc(x  ,y-4,z  ))
                      +coef[4]*(Uc(x  ,y  ,z+4)+Uc(x  ,y  ,z-4));

                  Vc(x,y,z) = 2.*Uc(x,y,z) - Vc(x,y,z) + ROC2(x,y,z)*lap;
                  if( (x==ixs) && (y==iys) && (z==izs) && ((i+1) <nt)){
                      Vc(ixs,iys,izs) =  Vc(ixs,iys,izs) + source[i+1];
                  }
              }
          } 
      }	    

  }// end of time LOOP

  // Read PAPI counters
  if ((ret_val = PAPI_read_counters(papi_values, 2)) != PAPI_OK){
      fprintf(stderr, "Info::PAPI failed to read counters: %s\n", PAPI_strerror(ret_val));
  }


  gettimeofday(&tvEnd, NULL);
  t = (double) (tvEnd.tv_usec - tvStart.tv_usec) / 1000000 + (double) (tvEnd.tv_sec - tvStart.tv_sec);
  printf("CPU EXECUTION TIME OF TIME LOOP = %0.4f\n",t);

  ////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////
  // print the performance results
  if (p.mpi_rank == 0) {
      printf("Total memory allocation ~ %lu MBytes\n", sizeof(float)*p.domain_size*3/1024/1024);
      printf("time/test(s)     : %e\n", t);
      printf("\nMStencil/s : %f  \n", p.n_stencils/(1e6*t));
      printf("******************************************************\n");
  }


  ////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////

  free(U1);
  free(U2);
  free(U3);

  printf("******************************************************\n"); 
  printf("PAPI Program Counter Information:\n");
  printf("L1 data cache accesses:\t %lld\n", papi_values[0]);
  printf("L1 data cache misses:\t %lld\n", papi_values[1]);
  /* printf("L1 instruction cache accesses:\t %lld\n", papi_values[2]);
     printf("L1 instruction cache misses:\t %lld\n", papi_values[3]);
     printf("No. of conditional branch instructions:\t %lld\n", papi_values[4]);
     printf("No. of floating point operations:\t %lld\n", papi_values[5]);
     printf("******************************************************\n"); 
     */
  if ((ret_val = PAPI_stop_counters(papi_values, 2)) != PAPI_OK){
      fprintf(stderr, "Info::PAPI failed to read counters: %s\n", PAPI_strerror(ret_val));
  }

  return 0;
}
