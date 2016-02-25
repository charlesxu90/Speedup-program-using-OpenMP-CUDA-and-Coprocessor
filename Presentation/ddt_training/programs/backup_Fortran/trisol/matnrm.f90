DOUBLE PRECISION FUNCTION MATNRM (a)

USE MOD_TRISOL, except => MATNRM

include 'mpif.h'

REAL (KIND(1.0D0)), ALLOCATABLE, INTENT(IN)    :: a(:,:)

INTEGER                                        :: i, j, j1, j2, k, n
REAL (KIND(1.0D0)), ALLOCATABLE                :: temp1(:), temp2(:)

!
! Initialise
!

n = SIZE(a,1)
ALLOCATE (temp2(n), temp1(n))

!
! Compute  the local contribution to the matrix norm
!

temp1 = ZERO

DO  j = me*block_size+1, n, n_procs*block_size
   j1 = block_size*((j-1)/(n_procs*block_size)) + 1
   j2 = j1 + MIN(block_size,n-j+1) + 1
   DO  k=j1,j2
      DO i=j,n
         temp1(i) = temp1(i) + ABS(a(i,k))
      END DO
   END DO
END DO

!
! Add up all the contributions
!

CALL MPI_REDUCE (temp1,temp2,n,MPI_DOUBLE_PRECISION,MPI_SUM,0, &
                 MPI_COMM_WORLD,ierr)

IF (me.LE.0) THEN
   MATNRM = MAXVAL(temp2)
END IF

!
! Deallocation
!

DEALLOCATE (temp1)
DEALLOCATE (temp2)

!
! End of Subroutine MATNRM
!

END FUNCTION MATNRM
