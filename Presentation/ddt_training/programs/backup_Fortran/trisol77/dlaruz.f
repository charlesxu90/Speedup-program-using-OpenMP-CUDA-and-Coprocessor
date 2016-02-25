      SUBROUTINE DLARUZ( ISEED, N, X )
      INTEGER N, ISEED(4)
      DOUBLE PRECISION X(N)
      DO  I =  1, N, 128
         NK  = MIN(N-I+1,128)
         CALL DLARUV( ISEED, NK, X(I) )
      END DO
      RETURN
*
*     End of DLARUV
*
      END

