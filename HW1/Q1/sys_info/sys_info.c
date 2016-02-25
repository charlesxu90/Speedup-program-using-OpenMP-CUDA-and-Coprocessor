#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

typedef char* caddr_t;
typedef char* PAPI_os_info_t;
#include "papi.h"
//#include "solaris-common.h" 
//#include "papi_internal.h"

int main () 
{
    const PAPI_hw_info_t *hwinfo = NULL;
    //const PAPI_os_info_t _papi_os_info=NULL;
    if (PAPI_library_init(PAPI_VER_CURRENT) != PAPI_VER_CURRENT)
        exit(1);
    if ((hwinfo = PAPI_get_hardware_info()) == NULL)
        exit(1);
  //  if(_papi_hwi_init_os()!=PAPI_OK)
  //      exit(1);

    printf("Number of CPU's in an SMP Node:\t %d.\n",hwinfo->ncpu);
    printf("Number of Nodes in the entire system:\t %d.\n",hwinfo->nnodes);
    printf("The rate of CPU's:\t %d.\n",hwinfo->mhz);
    printf("Total number of CPU's in the entire system:\t \n",hwinfo->totalcpus);
    printf("Vendor number of CPU:\t %d.\n",hwinfo->vendor);
    printf("Vendor string of CPU:\t %s.\n",hwinfo->vendor_string);
    printf("Model number of CPU:\t %d.\n",hwinfo->model);
    printf("Model string of CPU:\t %s.\n",hwinfo->model_string);
    printf("Revision of CPU:\t %f.\n",hwinfo->revision);
    //printf("OS version:\t %d.\n",PAPI_os_info_t.version);
    
    printf("Total number of memory levels:\t %d.\n",PAPI_MAX_MEM_HIERARCHY_LEVELS); 
    for(int i = 0; i< PAPI_MAX_MEM_HIERARCHY_LEVELS; i=i+1){
        printf("Level %d cache 1 type:\t %d.\n",i+1,hwinfo->mem_hierarchy.level[i].cache[0].type);
        printf("Level %d cache 1 size:\t %d.\n",i+1,hwinfo->mem_hierarchy.level[i].cache[0].size);
        printf("Level %d cache 1 line size:\t %d.\n",i+1,hwinfo->mem_hierarchy.level[i].cache[0].line_size);
        printf("Level  %d cache 1 number of lines:\t %d.\n",i+1,hwinfo->mem_hierarchy.level[i].cache[0].num_lines);
        printf("Level  %d cache 1 associativity:\t %d.\n",i+1,hwinfo->mem_hierarchy.level[i].cache[0].associativity);
         printf("Level %d cache 2 type:\t %d.\n",i+1,hwinfo->mem_hierarchy.level[i].cache[1].type);
        printf("Level %d cache 2 size:\t %d.\n",i+1,hwinfo->mem_hierarchy.level[i].cache[1].size);
        printf("Level %d cache 2 line size:\t %d.\n",i+1,hwinfo->mem_hierarchy.level[i].cache[1].line_size);
        printf("Level  %d cache 2 number of lines:\t %d.\n",i+1,hwinfo->mem_hierarchy.level[i].cache[1].num_lines);
        printf("Level  %d cache 2 associativity:\t %d.\n",i+1,hwinfo->mem_hierarchy.level[i].cache[1].associativity);
    }
}
