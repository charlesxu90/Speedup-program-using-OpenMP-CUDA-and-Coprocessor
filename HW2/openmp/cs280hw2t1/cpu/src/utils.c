#include <string.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include "utils.h"

void print_param(Parameters p) {
  printf("******************************************************\n");
  printf("Parameters settings\n");
  printf("******************************************************\n");
  printf("Domian size=%d    i=%d    j=%d    k=%d\n", p.domain_size, p.ip,p.jp,p.kp);
  printf("Number of stencils=%d\n",p.n_stencils);
  printf("Alignment size = %d Bytes\n", p.alignment);
  printf("MPI size = %d\n", p.mpi_size);
  printf("Number of tests    %d\n", p.n_tests);
  printf("Cycles per tests   %d\n", p.n_cycles);
  printf("******************************************************\n");
}

void parse_args (int argc, char** argv, Parameters * p)
{ // for more details see http://libslack.org/manpages/getopt.3.html
  int c;
  int digit_optind = 0;

  while (1)
  {
    int this_option_optind = optind ? optind : 1;
    int option_index = 0;
    static struct option long_options[] =
    {
        {"ip", 1, 0, 0},
        {"jp", 1, 0, 0},
        {"kp", 1, 0, 0},
        {"alignment", 1, 0, 0},
        {"reference_kernel", 1, 0, 0},
        {"target_kernel", 1, 0, 0},
        {"n_tests", 1, 0, 0},
        {"n_cycles", 1, 0, 0},
        {0, 0, 0, 0}
    };

    c = getopt_long (argc, argv, "",
        long_options, &option_index);
    if (c == -1)
      break;

    switch (c)
    {
    case 0:
//      printf ("option %s", long_options[option_index].name);
//      if (optarg)
//        printf (" with arg %s", optarg);
//      printf ("\n");

           if(strcmp(long_options[option_index].name, "ip") == 0) p->ip = atoi(optarg);
      else if(strcmp(long_options[option_index].name, "jp") == 0) p->jp = atoi(optarg);
      else if(strcmp(long_options[option_index].name, "kp") == 0) p->kp = atoi(optarg);
      else if(strcmp(long_options[option_index].name, "alignment") == 0) p->alignment = atoi(optarg);
      else if(strcmp(long_options[option_index].name, "reference_kernel") == 0) p->reference_kernel = atoi(optarg);
      else if(strcmp(long_options[option_index].name, "target_kernel") == 0) p->target_kernel = atoi(optarg);
      else if(strcmp(long_options[option_index].name, "n_tests") == 0) p->n_tests = atoi(optarg);
      else if(strcmp(long_options[option_index].name, "n_cycles") == 0) p->n_cycles = atoi(optarg);
      // re-calculate the domain information in case if size updated
      p->domain_size = p->ip*p->jp*p->kp;
      p->n_stencils = (p->ip-8)*(p->jp-8)*(p->kp-8);

      break;

    case '0':
    case '1':
    case '2':
      if (digit_optind != 0 && digit_optind != this_option_optind)
        printf ("digits occur in two different argv-elements.\n");
      digit_optind = this_option_optind;
      printf ("option %c\n", c);

      break;

    case 'a':
      printf ("option a\n");
      break;

    case 'b':
      printf ("option b\n");
      break;

    case 'c':
      printf ("option c with value `%s'\n", optarg);
      break;

    case 'd':
      printf ("option d with value `%s'\n", optarg);
      break;

    case '?':
      break;

    default:
      printf ("?? getopt returned character code 0%o ??\n", c);
      break;
  }
  }

  if (optind < argc)
  {
    printf ("non-option ARGV-elements: ");
    while (optind < argc)
      printf ("%s ", argv[optind++]);
    printf ("\n");
  }

}
