#ifndef GOOGLE_BREAKPAD_COMMON_MINIDUMP_CPU_E2K_H__
#define GOOGLE_BREAKPAD_COMMON_MINIDUMP_CPU_E2K_H__

typedef struct {
    uint32_t    context_flags;
    uint64_t    usbr;
    uint64_t    usd_lo;
    uint64_t    usd_hi;
    uint64_t    psp_lo;
    uint64_t    psp_hi;
    uint64_t    cr0_lo;
    uint64_t    cr0_hi;
    uint64_t    cr1_lo;
    uint64_t    cr1_hi;
    uint64_t    pcsp_lo;
    uint64_t    pcsp_hi;
} MDRawContextE2K; /* CONTEXT */

/* This value was chosen to avoid likely conflicts with MD_CONTEXT_*
 * for other CPUs. */
#define MD_CONTEXT_E2K                   0x00000800
#define MD_CONTEXT_E2K_FULL              MD_CONTEXT_E2K
#define MD_CONTEXT_E2K_ALL               MD_CONTEXT_E2K

#endif /* GOOGLE_BREAKPAD_COMMON_MINIDUMP_CPU_E2K_H__ */
