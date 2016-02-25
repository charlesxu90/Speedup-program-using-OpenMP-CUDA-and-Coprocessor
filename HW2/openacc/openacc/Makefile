#include make.inc

CC     = pgcc
CCFLAGS = -I../common
ACCFLAGS = -ta=nvidia,time -Minfo=accel -acc
OPTFLAGS = -std=c99 -O3 -fast -Minfo -ffast-math  -lm
BUILD_DIR=build
KERNEL_cpu=acc_rtm_kernel

.PHONY: all 

all: ${BUILD_DIR} ${BUILD_DIR}/${KERNEL_cpu}

${BUILD_DIR}:
	mkdir -p ${BUILD_DIR}

${BUILD_DIR}/base_kernel.o: src/base_kernel.c
	${CC} ${CCFLAGS} ${ACCFLAGS} -o $@ -c $<

${BUILD_DIR}/utils.o: src/utils.c
	${CC} ${CCFLAGS} ${ACCFLAGS} -o $@ -c $<

${BUILD_DIR}/cpu_driver.o: src/driver.c src/wrappers.c
	${CC} ${CCFLAGS} ${ACCFLAGS} -I ${BUILD_DIR} -o $@ -c $<


${BUILD_DIR}/${KERNEL_cpu}: ${BUILD_DIR}/cpu_driver.o ${BUILD_DIR}/utils.o
	${CC} ${CCFLAGS} ${ACCFLAGS} -o $@ $^ ${LDFLAGS} ${LIBS}

clean:
	rm -f ${BUILD_DIR}/${KERNEL_cpu} ${BUILD_DIR}/*.o 
