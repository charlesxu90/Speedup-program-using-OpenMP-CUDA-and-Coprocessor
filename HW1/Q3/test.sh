#!/bin/sh
#@ job_name         = test
#@ output           = $(job_name).$(jobid).$(stepid).out
#@ error            = $(job_name).$(jobid).$(stepid).err
#@ job_type         = serial
#@ wall_clock_limit  = 00:15:00
# If required uncomment the following line and add your project ID
#@ account_no = s11
#@ queue
for i in ./build/*
do 
    echo "$i"
    ./$i
done

