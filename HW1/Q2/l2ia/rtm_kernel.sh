#!/bin/sh 
#@ job_name         = rtm_kernel_l2ia
#@ output           = $(job_name).$(jobid).$(stepid).out
#@ error            = $(job_name).$(jobid).$(stepid).err
#@ job_type         = serial
#@ wall_clock_limit  = 00:20:00
# If required uncomment the following line and add your project ID
#@ account_no = s11
#@ queue
./build/cpu_rtm_kernel
