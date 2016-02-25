export OMP_NUM_THREADS=60 && \
./omp_rtm_kernel >60omp.out && \
export OMP_NUM_THREADS=120 && \
./omp_rtm_kernel >120omp.out && \
#export OMP_NUM_THREADS=180 && \
#./omp_rtm_kernel >180omp.out && \
#export OMP_NUM_THREADS=240 && \
#./omp_rtm_kernel >240omp.out && \
echo "OMP finished!"
