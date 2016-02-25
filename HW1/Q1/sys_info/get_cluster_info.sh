#!/bin/sh
#@ job_name         = system_information
#@ output           = $(job_name).$(jobid).$(stepid).out
#@ error            = $(job_name).$(jobid).$(stepid).err
#@ job_type         = serial
#@ wall_clock_limit  = 00:05:00
# If required uncomment the following line and add your project ID
#@ account_no = s11
#@ queue
./build/sys_info
