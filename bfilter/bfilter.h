
#ifndef _ULIB_BLOOM_FILTER_H
#define _ULIB_BLOOM_FILTER_H

#include <stdint.h>
#include <string.h>

#define BITS_PER_BYTE       8
#define BITS_PER_LONG       __WORDSIZE

#define DIV_ROUND_UP(n,d)   (((n) + (d) - 1) / (d))
#define BITS_TO_LONGS(nr)   DIV_ROUND_UP(nr, BITS_PER_BYTE * sizeof(long))
#define BIT_WORD(nr)        ((nr) / BITS_PER_LONG)
#define BIT_MASK(nr)        (1UL << ((nr) % BITS_PER_LONG))

struct bloom_filter {
	unsigned long *bitmap;
	unsigned long  nbits;
	unsigned long  nelem;  /* estimated number of elements */
	int	       nfunc;  /* number of hash functions */
	uint64_t      *seeds;  /* seeds for hash functions */
};

static inline int test_bit(int nr, const volatile unsigned long *addr)
{
    return 1UL & (addr[BIT_WORD(nr)] >> (nr & (BITS_PER_LONG-1)));
}

static inline void set_bit(int nr, volatile unsigned long *addr)
{
    *(addr + BIT_WORD(nr)) |= BIT_MASK(nr);
}

#define small_const_nbits(nbits)                    \
    (__builtin_constant_p(nbits) && (nbits) <= BITS_PER_LONG)

static inline void bitmap_zero(unsigned long *dst, int nbits)
{
    if (small_const_nbits(nbits))
        *dst = 0UL;
    else {
        int len = BITS_TO_LONGS(nbits) * sizeof(unsigned long);
        memset(dst, 0, len);
    }
}

#ifdef __cplusplus
extern "C" {
#endif

int  bfilter_create(struct bloom_filter *bf, unsigned long nbits, unsigned long nelem);
void bfilter_destroy(struct bloom_filter *bf);
void bfilter_zero(struct bloom_filter *bf);
void bfilter_set(struct bloom_filter *bf, const void *buf, unsigned long buflen);
int  bfilter_get(struct bloom_filter *bf, const void *buf, unsigned long buflen);
void bfilter_set_hash(struct bloom_filter *bf, unsigned long hash);
int  bfilter_get_hash(struct bloom_filter *bf, unsigned long hash);

#ifdef __cplusplus
}
#endif

#endif
