      DOUBLE PRECISION FUNCTION MATNRM (me,n_procs,n,nl,block,a,temp1,
     *                                  temp2)
*
      include 'mpif.h'
*
      INTEGER  me, n, nl, n_procs, block
      DOUBLE PRECISION a(n,nl), temp1(n), temp2(n)
      INTEGER  i, j, j1, j2, k, kl
*
      DO  i=1,n
         temp1(i) = 0.0D0
      END DO
*
      kl = 1 - block
      DO   j = me*block+1, n, n_procs*block
         j1 = block*((j-1)/(n_procs*block)) + 1
         j2 = j1 + MIN(block,n-j+1) - 1
         nloc = MIN(block,n-j+1) 
         kl = kl + block
         DO  k=0, nloc-1
            DO  i=j,n
               temp1(i) = temp1(i) + ABS(a(i,k+kl))
            END DO
         END DO
      END DO
*
*     Add up all the contributions
*
      CALL MPI_REDUCE (temp1,temp2,n,MPI_DOUBLE_PRECISION,MPI_SUM,0,
     *                 MPI_COMM_WORLD,ierr)

      IF (me.LE.0) THEN
         MATNRM = 0.0D0
         DO  i = 1, n
            MATNRM = MAX(MATNRM,ABS(temp2(i)))
         END DO
      END IF
*
*      End of Subroutine MATNRM
*
      RETURN
      END
