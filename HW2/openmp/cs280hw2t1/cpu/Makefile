#include make.inc

CC     = icc
CFLAGS += -Wall -std=c99 -O1 -vec-report3 -openmp #-mmic -ffast-math  -lm
BUILD_DIR=build
KERNEL_cpu=cpu_rtm_kernel

.PHONY: all 

all: ${BUILD_DIR} ${BUILD_DIR}/${KERNEL_cpu}

${BUILD_DIR}:
	mkdir -p ${BUILD_DIR}

${BUILD_DIR}/base_kernel.o: src/base_kernel.c
	${CC} ${CFLAGS} -o $@ -c $<

${BUILD_DIR}/utils.o: src/utils.c
	${CC} ${CFLAGS} -o $@ -c $<

${BUILD_DIR}/cpu_driver.o: src/driver.c src/wrappers.c
	${CC} ${CFLAGS} -I${BUILD_DIR} -o $@ -c $<


${BUILD_DIR}/${KERNEL_cpu}: ${BUILD_DIR}/cpu_driver.o ${BUILD_DIR}/utils.o
	${CC} ${CFLAGS} -o $@ $^ ${LDFLAGS} ${LIBS}

clean:
	rm -f ${BUILD_DIR}/${KERNEL_cpu} ${BUILD_DIR}/*.o 
