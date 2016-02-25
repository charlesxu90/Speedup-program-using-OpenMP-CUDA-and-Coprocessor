      SUBROUTINE GENDATA (me,n_procs,n,nl,block,a,b)
*
      include 'mpif.h'
*
      INTEGER me, n_procs, n, nl, block
      DOUBLE PRECISION a(n,nl), b(n)
*
      INTEGER i, j, k, kl, nloc
      INTEGER seed(4)
*
*     Initialise
*
*     Note that strictly speaking this is NOT a good parallel RNG - the
*     sequences are not guaranteed to be uncorrelated.
*     in this context, it does not matter
*
      seed(1) = MOD (2*(537 + me * 271)+1 ,4096)
      seed(2) = MOD (2*(876 + me * 35)+1 ,4096)
      seed(3) = MOD (2*(1987 + me * 111)+1 ,4096)
      seed(4) = MOD (2*(16 + me * 71)+1 ,4096)
*
*     Generate the distributed matrix A
*
      DO  j = 1, nl
         DO  i = 1, n
            a(i,j) = 0.0D0
         END DO
      END DO
*
      kl = - block
      DO  j = me*block, n-1, n_procs*block
         kl = kl + block
         nloc = MIN (block, n-j)
         DO  k = 1, nloc
            CALL DLARUZ (seed,n-j-k+1,a(k+j:n,k+kl))
            a(k+j,k+kl) = 2.0D0 + a(k+j,k+kl)
            DO  i = k+j+1, n
               a(i,k+kl) = -1.0D0 + 2.0D0*a(i,k+kl)
            END DO
         END DO
      END DO
*
*     Generate the RHS b on the root processors only
*
      CALL DLARUZ (seed,n,b)
*
*     End of Subroutine GENDATA
*
      RETURN
      END
