void base_kernel( int ip,  int jp,  int kp,
    const double * restrict coef, double * restrict u,
    const double * restrict v, const double * restrict roc2) {

  int i,j,k;
  double lap;

#define U(i,j,k) (u[((i)*jp+(j))*kp+(k)])
#define V(i,j,k) (v[((i)*jp+(j))*kp+(k)])
#define ROC2(i,j,k) (roc2[((i)*jp+(j))*kp+(k)])
  for(i=4; i<ip-4; i++) 
    for(j=4; j<jp-4; j++)
      for(k=4; k<kp-4; k++)
	{
        lap=coef[0]*V(i,j,k)
           +coef[1]*(V(i+1,j  ,k  )+V(i-1,j  ,k  ))
           +coef[1]*(V(i  ,j+1,k  )+V(i  ,j-1,k  ))
           +coef[1]*(V(i  ,j  ,k+1)+V(i  ,j  ,k-1))
           +coef[2]*(V(i+2,j  ,k  )+V(i-2,j  ,k  ))
           +coef[2]*(V(i  ,j+2,k  )+V(i  ,j-2,k  ))
           +coef[2]*(V(i  ,j  ,k+2)+V(i  ,j  ,k-2))
           +coef[3]*(V(i+3,j  ,k  )+V(i-3,j  ,k  ))
           +coef[3]*(V(i  ,j+3,k  )+V(i  ,j-3,k  ))
           +coef[3]*(V(i  ,j  ,k+3)+V(i  ,j  ,k-3))
           +coef[4]*(V(i+4,j  ,k  )+V(i-4,j  ,k  ))
           +coef[4]*(V(i  ,j+4,k  )+V(i  ,j-4,k  ))
           +coef[4]*(V(i  ,j  ,k+4)+V(i  ,j  ,k-4));

        U(i,j,k) = 2.*V(i,j,k) - U(i,j,k) + ROC2(i,j,k)*lap;
      }

}
