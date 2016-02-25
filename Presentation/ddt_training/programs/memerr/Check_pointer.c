#include <stdlib.h>
#include <stdio.h>
#include <math.h>


void Check_pointer (int length, int *data) {

/* Check_pointer checks for mishandling of pointers */

 int i, n_write;
 int * data_init;

/* Check for repeated free */

   *data = 1;

   data = (int*) malloc(sizeof(int)*length);
   data_init = data;

   for (i=0; i<=length+1; i++, data++) {
       *data = i;
   }

   data = data_init-10;

   printf ("*data is %d\n",*data);

   data = (int *) 1;

   data = data_init + 2*length;
   *data = length;
   free (data);

}

