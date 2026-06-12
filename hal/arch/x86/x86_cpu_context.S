    .text
    .global BolunX86CpuContextBarrier
    .type BolunX86CpuContextBarrier, @function
BolunX86CpuContextBarrier:
#if defined(__x86_64__) || defined(__i386__)
    mfence
#endif
    ret
