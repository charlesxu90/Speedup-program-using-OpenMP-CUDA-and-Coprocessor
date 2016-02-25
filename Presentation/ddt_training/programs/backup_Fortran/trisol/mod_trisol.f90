MODULE MOD_TRISOL

   REAL (KIND(1.0D0)), PARAMETER :: ZERO=0.0D0, ONE=1.0D0, TWO=2.0D0
   INTEGER                       :: me, n_procs, block_size

   INTERFACE

      SUBROUTINE GENDATA (a,b)
         REAL (KIND(1.0D0)), ALLOCATABLE, INTENT(INOUT)    :: a(:,:), b(:)
      END SUBROUTINE GENDATA

      SUBROUTINE SOLVE (a,b,x)
         REAL (KIND(1.0D0)), ALLOCATABLE, INTENT(IN)    :: a(:,:), b(:)
         REAL (KIND(1.0D0)), ALLOCATABLE, INTENT(OUT)   :: x(:)
      END SUBROUTINE SOLVE

      SUBROUTINE CHECK(a,b,x,resnrm)
         REAL (KIND(1.0D0)), INTENT(OUT)                :: resnrm
         REAL (KIND(1.0D0)), ALLOCATABLE, INTENT(IN)    :: a(:,:), b(:)
         REAL (KIND(1.0D0)), ALLOCATABLE, INTENT(INOUT) :: x(:)
         REAL (KIND(1.0D0)), ALLOCATABLE                :: res(:)
      END SUBROUTINE CHECK

      SUBROUTINE DLARUV(iseed,n,x)
         INTEGER, INTENT(in)            :: n
         INTEGER,  INTENT(INOUT)        :: iseed(4)
         REAL(KIND(1.0D0)), INTENT(OUT) :: x(*)
      END SUBROUTINE DLARUV

      INTEGER FUNCTION NUMROC (n,nb,iproc,isrcproc,nprocs)
         INTEGER, INTENT(in)            :: n, nb, iproc, isrcproc, nprocs
      END FUNCTION NUMROC

      DOUBLE PRECISION FUNCTION MATNRM (a)
         REAL (KIND(1.0D0)), ALLOCATABLE, INTENT(IN)    :: a(:,:)
      END FUNCTION MATNRM

   END INTERFACE

END MODULE MOD_TRISOL
