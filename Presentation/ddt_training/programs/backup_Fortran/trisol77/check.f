      SUBROUTINE CHECK (me,n_procs,n,nl,block,a,b,x,tol,temp1,temp2)
      include 'mpif.h'
      INTEGER me, n_procs, n, nl, block
      DOUBLE PRECISION resnrm, tol, anrm, bnrm, xnrm, matnrm
      DOUBLE PRECISION a(n,nl), b(n), x(n), temp1(n), temp2(n)
*
      INTEGER i, j, j1, j2, k, kl, nloc
*
*     Initialise
*
      IF (me.LE.0) THEN
         DO  i = 1, n
            temp2(i) = b(i)
         END DO
      ELSE
         DO  i = 1, n
            temp2(i) = 0.0D0
         END DO
      END IF
*
      CALL MPI_BCAST (x,n,MPI_DOUBLE_PRECISION,0,MPI_COMM_WORLD,ierr)
*
*     Compute the local contribution to  res = b - A*x
*
      kl = 1 - block
      DO  j = me*block+1, n, n_procs*block
         nloc = MIN(block,n-j+1) 
         kl = kl + block
         DO  k = 0, nloc-1
            DO i = j + k, n
               temp2(i) = temp2(i) - a(i,k+kl)*x(k+j)
            END DO
         END DO
      END DO
*
*     Add up all the contributions
*
      CALL MPI_REDUCE (temp2,temp1,n,MPI_DOUBLE_PRECISION,MPI_SUM,0,
     *                 MPI_COMM_WORLD,ierr)
*
      IF (me.LE.0) THEN
         resnrm = 0.0D0
         DO  i = 1, n
            resnrm = MAX(resnrm,ABS(temp1(i)))
         END DO
      END IF
*
*     Compute the tolerance
*
      anrm = MATNRM (me,n_procs,n,nl,block,a,temp1,temp2)
      IF (me.LE.0) THEN
         bnrm = 0.0D0
         xnrm = 0.0D0
         DO  i = 1, n
            bnrm = MAX(bnrm,ABS(b(i)))
            xnrm = MAX(xnrm,ABS(x(i)))
         END DO
      END IF
      tol = resnrm/((anrm*xnrm+bnrm)*n*1.0D-16)
*
*     End of Subroutine CHECK
*
      RETURN
      END
