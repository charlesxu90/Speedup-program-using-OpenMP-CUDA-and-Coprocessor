/* Some comments.... */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <unistd.h>


void gendata_ (int * me, int * n_procs, int * n, int * nl, int * block,
               double * a, double * b);
void solve_ (int * me, int * n_procs, int * n, int * nl, int * block,
	     double * a, double * b, double * x);
void check_ (int * me, int * n_procs, int * n, int * nl, int * block,
	     double * a, double * b, double * x, double * tol,
             double * temp1, double * temp2);
int numroc_ (int * n, int * block, int * me,int * i, int * n_procs);

int main(int argc, char** argv)
{

    int block, k, me, n, nl, n_procs;
    int itemp[2];
    double tol;
    double * a, * b, * x, * temp1, * temp2;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &me);
    MPI_Comm_size(MPI_COMM_WORLD, &n_procs);

//
// Read and distribute the input data
//

    if (me == 0)
	scanf ("%d %d", itemp, itemp+1);

    MPI_Bcast (itemp,2,MPI_INTEGER,0,MPI_COMM_WORLD);
    n = itemp[0];
    block = itemp[1];

//
// Allocate the required space
//

    k = 0;
    nl = numroc_ (&n,&block,&me, &k, &n_procs);

    a = malloc (n*nl*sizeof(double));
    b = malloc (n*sizeof(double));
    temp1 = malloc (n*sizeof(double));
    temp2 = malloc (n*sizeof(double));

//
// Generate the distributed matrix and the RHS
//

    gendata_ (&me, &n_procs, &n, &nl, &block, a, b);

//
// Solve the trianguar system of equations
//

    x = malloc (n*sizeof(double));
    solve_ (&me, &n_procs, &n, &nl, &block, a, b, x);

//
// Check the solution
//

    check_ (&me, &n_procs, &n, &nl, &block, a, b, x, &tol, temp1, temp2);

    free (b);
    free (x);
    free (temp1);

    if (me == 0)
    {
	if (tol < 1.0)
	    printf ("*** Solution correct\n");
	else
	    printf ("*** Error in solution\n");
	printf ("|x| / (sqrt(n)*epsilon*(|A|*|x| + |b|) = %g\n", tol);
    }
        
    return 0;

} /* main */
