SUBROUTINE SOLVE (a,b,x)

USE MOD_TRISOL, except_this => SOLVE

IMPLICIT NONE

include 'mpif.h'

REAL (KIND(1.0D0)), ALLOCATABLE, INTENT(IN)    :: a(:,:), b(:)
REAL (KIND(1.0D0)), ALLOCATABLE, INTENT(OUT)   :: x(:)

INTEGER  :: dest, i, ierr, j, j1, j2, k, kl, n, nl, source, tag
INTEGER  :: status(MPI_STATUS_SIZE)

!
! Initialise
!

n = SIZE(a,1)
tag = 1

!
! Allocation
!

ALLOCATE (x(n))
IF (me.LE.0) THEN
   x = b
END IF

!
! Solve A*x = b
!

source = MOD(me + n_procs - 1, n_procs)
dest = MOD(me + 1, n_procs)

kl = 1 - block_size
DO  j = me*block_size+1, n, n_procs*block_size

   nl = MIN(block_size,n-j+1) 
   kl = kl + block_size

   IF (j.NE.1) THEN
      CALL MPI_Recv(x(j),n-j+1,MPI_DOUBLE_PRECISION,source,MPI_ANY_TAG, &
                    MPI_COMM_WORLD,status,ierr)
   END IF

   DO  k = 0, nl-1
      x(k+j) = x(k+j)/a(k+j,k+kl)
      DO i = j + k+1, n
         x(i) = x(i) - a(i,k+kl)*x(k+j)
      END DO
   END DO

   IF ((n-j).GE.block_size) THEN
      CALL MPI_SEND(x(j+nl),n-j-nl+1,MPI_DOUBLE_PRECISION,dest,tag, &
                    MPI_COMM_WORLD,ierr)
   END IF

END DO
 
!
! Send the solution to root
!

source = n_procs - 1
IF (me.LE.0) THEN
   DO  j = 1, n, block_size
      nl = MIN (block_size,n-j+1)
      source = MOD(source+1,n_procs) 
      IF (source.NE.0) THEN
         CALL MPI_Recv(x(j),nl,MPI_DOUBLE_PRECISION,source,MPI_ANY_TAG, &
                    MPI_COMM_WORLD,status,ierr)
      END IF
   END DO
ELSE
   DO  j = me*block_size +1, n, n_procs*block_size
      nl = MIN (block_size,n-j+1)
      CALL MPI_SEND(x(j),nl,MPI_DOUBLE_PRECISION,0,tag, &
                    MPI_COMM_WORLD,ierr)
   END DO
END IF

!
! End of Subroutine SOLVE
!

END SUBROUTINE SOLVE
