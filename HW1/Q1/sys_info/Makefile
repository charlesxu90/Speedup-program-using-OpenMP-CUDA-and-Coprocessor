#include make.inc

CC     = gcc
CFLAGS += -std=c99 -O3 -ffast-math -lpapi  -lm
FFLAGS = -Wall -O3
BUILD_DIR=build
KERNEL_cpu=sys_info

.PHONY: all 

all: ${BUILD_DIR} ${BUILD_DIR}/${KERNEL_cpu}

${BUILD_DIR}:
	mkdir -p ${BUILD_DIR}

${BUILD_DIR}/sys_info.o: sys_info.c
	${CC} ${CFLAGS} -I${BUILD_DIR} -o $@ -c $<

${BUILD_DIR}/${KERNEL_cpu}: ${BUILD_DIR}/sys_info.o
	        ${CC} ${CFLAGS} -o $@ $^ ${LDFLAGS} ${LIBS}
clean:
	rm -f ${BUILD_DIR}/${KERNEL_cpu} ${BUILD_DIR}/*.o 
