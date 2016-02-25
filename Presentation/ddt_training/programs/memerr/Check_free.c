#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int MIN (int a, int b) {
    int minimum;
    if (a<=b) minimum = a;
    else minimum = b;

    return minimum;
}

void Check_free (int length, int *data) {

/* Check_free checks for handling of free commands */

 int i, n_write;
 int * data_init;

/* Check for repeated free */

   data = (int*) malloc(sizeof(int)*length);
   data_init = data;

   for (i=0; i<=length-1; i++, data++) {
       *data = i;
   }

   data = data_init;
   n_write = MIN((int) 10,length);

   for (i=0; i <= n_write-1; i++, data++){
       printf ("data[%d] is %d\n",i,*data);
   }
   data = data_init;
   free (data);

   n_write = MIN((int) 10,length);

   for (i=0; i <= n_write-1; i++, data++){
    //   printf ("data[%d] is %d\n",i,*data);
   }

   data = data_init;
 //  free (data);

/* Check for free in the middlee of the block */

   data   = (int*) malloc(sizeof(int)*length);
   data_init = data;

   for (i=0; i<=length-1; i++, data++) {
       *data = i;
   }

   data = data-(length/2);

  // free (data);

   data = data_init;
   free (data);

}

