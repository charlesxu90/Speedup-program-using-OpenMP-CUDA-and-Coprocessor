SUBROUTINE GENDATA (a,b)

USE MOD_TRISOL, except_this => GENDATA

IMPLICIT NONE

include 'mpif.h'

REAL (KIND(1.0D0)), ALLOCATABLE, INTENT(INOUT)  :: a(:,:), b(:)

INTEGER :: i, j, k, kl, n, nl
INTEGER :: seed(4)

!
! Initialise
!

n = SIZE(a,1)
a = ZERO

!
! Note that strictly speaking this is NOT a good parallel RNG - the
! sequences are not guaranteed to be uncorrelated.
! in this context, it does not matter
!

seed(1) = MOD (2*(537 + me * 271)+1 ,4096)
seed(2) = MOD (2*(876 + me * 35)+1 ,4096)
seed(3) = MOD (2*(1987 + me * 111)+1 ,4096)
seed(4) = MOD (2*(16 + me * 71)+1 ,4096)

!
! Generate the distributed matrix A
!

kl = - block_size
DO  j = me*block_size, n-1, n_procs*block_size
   kl = kl + block_size
   nl = MIN (block_size, n-j)
   DO  k = 1, nl
      CALL DLARUV (seed,n-j-k+1,a(k+j:n,k+kl))
      a(k+j,k+kl) = TWO + a(k+j,k+kl)
      DO  i = k+j+1, n
         a(i,k+kl) = -ONE + TWO*a(i,k+kl)
      END DO
   END DO
END DO

!
! Generate the RHS b on the root processors only
!

CALL DLARUV (seed,n,b)

!
! End of Subroutine GENDATA
!
END SUBROUTINE GENDATA
