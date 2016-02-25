export OMP_NUM_THREADS=1 && \
./cpu_rtm_kernel >1cpu.out && \
export OMP_NUM_THREADS=2 && \
./cpu_rtm_kernel >2cpu.out && \
#export OMP_NUM_THREADS=4 && \
#./cpu_rtm_kernel >4cpu.out && \
#export OMP_NUM_THREADS=8 && \
#./cpu_rtm_kernel >8cpu.out && \
#export OMP_NUM_THREADS=16 && \
#./cpu_rtm_kernel >16cpu.out && \
echo "CPU finished!"
