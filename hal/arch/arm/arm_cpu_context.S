    .text
    .global BolunArmCpuContextBarrier
    .type BolunArmCpuContextBarrier, %function
BolunArmCpuContextBarrier:
#if defined(__aarch64__)
    dsb sy
    isb
#elif defined(__arm__)
    dsb sy
    isb sy
#endif
    ret
