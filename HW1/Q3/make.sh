# All flags:
# -O or -O1
# -O2
# -O3 : -ffast-math
# -Os
# -Ofast
# -Og
# -finline-functions
# -fdelayed-branch
# -ftree-loop-distribution
# -floop-interchange
# -funroll-loops/-funroll-all-loops
# -floop-block
# -fprefecth-loop-arrays
# -funsafe-math-optimizations
# -fgcse
# -fgcse-lm
# -fgcse-sm
# -fgcse-las
# -pthreads
# -fopenmp
# -mmmx -msse -msse2 -msse3 -msse4.1 -msse4.2 -msse4 -mavx -mavx2
# -march=name -mcpu=name -mtune=name 
# -S assembly code output, -c do not link source 
BUILD_DIR=build

mkdir -p ${BUILD_DIR} && \
gcc -lpapi -o ${BUILD_DIR}/MX_0 test.c
gcc -lpapi -O1 -o ${BUILD_DIR}/MX_1 test.c
gcc -lpapi -O2 -o ${BUILD_DIR}/MX_2 test.c
gcc -lpapi -O3 -o ${BUILD_DIR}/MX_3 test.c
gcc -lpapi -Os -o ${BUILD_DIR}/MX_size test.c
#gcc -Ofast -o  ${BUILD_DIR}/MX_Ofast test.c
#gcc -Og -o ${BUILD_DIR}/MX_Og test.c
gcc -lpapi -finline-functions -o build/MX_finlien-functions test.c
gcc -lpapi -fdelayed-branch -o build/MX_fdelayed-branch test.c
gcc -lpapi -ftree-loop-distribution -o build/MX_ftree-loop-distribution test.c
gcc -lpapi -floop-interchange -o build/MX_floop-interchange test.c
gcc -lpapi -funroll-loops -o build/MX_funroll-loops test.c
gcc -lpapi -floop-block -o build/MX_floop-block test.c
#gcc -fprefecth-loop-arrays -o build/MX_fprefecth-loop-arrays test.c
gcc -lpapi -funsafe-math-optimizations -o build/MX_funsafe-math-optimizations test.c
gcc -lpapi -fgcse -o build/MX_fgcse test.c
gcc -lpapi -fgcse-lm -o build/MX_fgcse-lm test.c
gcc -lpapi -fgcse-sm -o build/MX_fgcse-sm test.c
gcc -lpapi -fgcse-las -o build/MX_fgcse-las test.c
gcc -lpapi -pthreads -o build/MX_pthreads test.c
gcc -lpapi -fopenmp -o build/MX_fopenmp test.c
gcc -lpapi -ffast-math -o build/MX_ffast-math test.c

