/*
 * utils.h
 *
 *  Created on: Dec 9, 2012
 *      Author: malastm
 */

#ifndef UTILS_H_
#define UTILS_H_

typedef struct{
  int ip,jp,kp, alignment, domain_size, n_stencils;
  int reference_kernel, target_kernel;
  int mpi_rank, mpi_size;
  int n_tests, n_cycles;
}Parameters;

#endif /* UTILS_H_ */
