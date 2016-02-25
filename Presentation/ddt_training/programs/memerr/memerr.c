#include <stdio.h>
#include "sub.h"

int main(int argc, char *argv[]) {

   int length;
   int *data1, *data2;

   length = 1000;

/*
 Check freeing pointers
*/

Check_free(length,data1);

/*
 Check pointers
*/

Check_pointer(length,data2);

return 0;

}
