PROGRAM TRISOL

USE MOD_TRISOL

IMPLICIT NONE

include 'mpif.h'

INTEGER                         :: i, ierr, j, n, nl, source, k, l, tag
INTEGER                         :: status(MPI_STATUS_SIZE)
INTEGER                         :: input_data(2)
REAL (KIND(1.0D0))              :: anrm, bnrm, resnrm, tol, xnrm, t(1)
REAL (KIND(1.0D0)), ALLOCATABLE :: a(:,:), b(:), x(:)
       
!
! Initiate MPI
!

CALL MPI_INIT(ierr)
CALL MPI_COMM_SIZE(MPI_COMM_WORLD, n_procs, ierr)
CALL MPI_COMM_RANK(MPI_COMM_WORLD, me, ierr)

!
! Read and distribute the input data
!

IF (me.LE.0) THEN
   input_data = (4095, 1)
END IF
CALL MPI_BCAST (input_data,2,MPI_INTEGER,0,MPI_COMM_WORLD,ierr)
n = input_data(1)
block_size = input_data(2)

!
! Allocate the required space
!

nl = NUMROC (n,block_size,me,0,n_procs)

ALLOCATE (a(n,nl),b(n))
a = ZERO

!
! Generate the distributed matrix and the RHS
!

CALL GENDATA (a,b)

tag = 13
source = n_procs - 1
l = 0

!
! Solve the trianguar system of equations
!

CALL SOLVE (a,b,x)

!
! Check the solution
!

CALL CHECK (a,b,x,resnrm)

!
! Print the solution and its tolerance
!

anrm = MATNRM(a)
IF (me.LE.0) THEN
   bnrm = MAXVAL(ABS(b))
   xnrm = MAXVAL(ABS(x))
   tol = resnrm/((anrm*xnrm+bnrm)*SQRT(DBLE(n))*EPSILON(ONE))
   IF (tol.le.ONE) THEN
      WRITE (*,'("***  Solution correct")')
   ELSE
      WRITE (*,'("***  Error in solution")')
   END IF
   WRITE (*,'("|x| / (sqrt(n)*epsilon*(|A|*|x| + |b|) = ",1PD11.4)') TOL
END IF

!
! Deallocation
!

DEALLOCATE (b)

!
! Close MPi
!

CALL MPI_Finalize(ierr)

!
! End of Program 
!

END PROGRAM TRISOL
