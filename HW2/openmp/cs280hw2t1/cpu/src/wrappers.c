#define WRAPPER_SIG (int, int, int, const double * restrict, double * restrict, double * restrict, double * restrict )
extern void base_kernel WRAPPER_SIG;

struct Kernel {
  const char *name;
  void (*func)WRAPPER_SIG;
};

#define TARGET_KERNEL (0)
#define REFERENCE_KERNEL (0)

struct Kernel KernelList[] = {
{"base_kernel", base_kernel},
};

