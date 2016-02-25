SUBROUTINE CHECK (a,b,x,resnrm)

USE MOD_TRISOL, except_this => CHECK

include 'mpif.h'

REAL (KIND(1.0D0)), INTENT(OUT)                :: resnrm
REAL (KIND(1.0D0)), ALLOCATABLE, INTENT(INOUT) :: a(:,:), b(:), x(:)

INTEGER                                        :: i, j, j1, j2, k, kl, n, nl
REAL (KIND(1.0D0)), ALLOCATABLE                :: res(:), temp(:)

!
! Initialise
!

n = SIZE(a,1)
ALLOCATE (res(n),temp(n))

IF (me.LE.0) THEN
   temp = b
ELSE
   temp = ZERO
END IF

DO k = 0, block_size
  res(k+1) = k+1
END DO

CALL MPI_BCAST (x,n,MPI_DOUBLE_PRECISION,0,MPI_COMM_WORLD,ierr)

!
! Compute the local contribution to  res = b - A*x
!


kl = 1 - block_size
DO  j = me*block_size+1, n, n_procs*block_size

   nl = MIN(block_size,n-j+1) 
   kl = kl + block_size

   DO  k = 0, nl-1
      DO i = j + k, n
         temp(i) = temp(i) - a(i,k+kl)*x(k+j)
      END DO
   END DO

END DO

!
! Add up all the contributions
!

CALL MPI_REDUCE (temp,res,n,MPI_DOUBLE_PRECISION,MPI_SUM,0, &
                 MPI_COMM_WORLD,ierr)

IF (me.LE.0) THEN
   resnrm = MAXVAL(ABS(res))
END IF

!
! Deallocation
!

DEALLOCATE (res)
DEALLOCATE (temp)

!
! End of Subroutine CHECK
!

END SUBROUTINE CHECK
