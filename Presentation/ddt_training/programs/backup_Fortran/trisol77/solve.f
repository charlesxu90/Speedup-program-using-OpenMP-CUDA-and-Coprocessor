      SUBROUTINE SOLVE (me,n_procs,n,nl,block,a,b,x)
      include 'mpif.h'
      INTEGER me, n_procs, n, nl, block
      DOUBLE PRECISION  a(n,nl), b(n), x(n)
      INTEGER  dest, i, ierr, j, j1, j2, k, kl, nloc, source, tag
      INTEGER  status(MPI_STATUS_SIZE)
*
*     Initialise
*
      tag = 1
*
      IF (me.LE.0) THEN
         DO  i = 1, n
            x(i) = b(i)
         END DO
      END IF
*
*     Solve A*x = b
*
      source = MOD(me + n_procs - 1, n_procs)
      dest = MOD(me + 1, n_procs)
*
      kl = 1 - block
      DO  j = me*block+1, n, n_procs*block
*
         nloc = MIN(block,n-j+1) 
         kl = kl + block
*
         IF (j.NE.1) THEN
            CALL MPI_Recv(x(j),n-j+1,MPI_DOUBLE_PRECISION,source,
     *                    MPI_ANY_TAG,MPI_COMM_WORLD,status,ierr)
         END IF
*
         DO  k = 0, nloc-1
            x(k+j) = x(k+j)/a(k+j,k+kl)
            DO i = j + k+1, n
               x(i) = x(i) - a(i,k+kl)*x(k+j)
            END DO
         END DO
*
         IF ((n-j).GE.block) THEN
            CALL MPI_SEND(x(j+nloc),n-j-nloc+1,MPI_DOUBLE_PRECISION,
     *                    dest,tag,MPI_COMM_WORLD,ierr)
         END IF
*
      END DO
*
*     Send the solution to root
*
      
      source = n_procs - 1
      IF (me.LE.0) THEN
         DO  j = 1, n, block
            nloc = MIN (block,n-j+1)
            source = MOD(source+1,n_procs) 
            IF (source.NE.0) THEN
               CALL MPI_Recv(x(j),nloc,MPI_DOUBLE_PRECISION,source,
     *                       MPI_ANY_TAG,MPI_COMM_WORLD,status,ierr)
            END IF
         END DO
      ELSE
         DO  j = me*block +1, n, n_procs*block
            nloc = MIN (block,n-j+1)
            CALL MPI_SEND(x(j),nloc,MPI_DOUBLE_PRECISION,0,tag,
     *                    MPI_COMM_WORLD,ierr)
         END DO
      END IF
*      
*     End of Subroutine SOLVE
*
      RETURN
      END
      
