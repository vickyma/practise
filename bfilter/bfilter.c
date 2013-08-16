#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <math_rand_prot.h>
#include "bfilter.h"

#define OPTIMAL_NFUNC(nbits, nelem) ((int)ceilf((9 * nbits)/(13.0 * nelem)))
#define HASH_FUNCTION(buf,len,seed) hash_fast64(buf, len, seed)

uint64_t hash_fast64(const void *buf, size_t len, uint64_t seed)
{
	const uint64_t    m = 0x880355f21e6d1965ULL;
	const uint64_t *pos = (const uint64_t *)buf;
	const uint64_t *end = pos + (len >> 3);
	const unsigned char *pc;
	uint64_t h = len * m ^ seed;
	uint64_t v;

	while (pos != end) {
		v  = *pos++;
		v ^= v >> 23;
		v *= 0x2127599bf4325c37ULL;
		h ^= v ^ (v >> 47);
		h *= m;
	}

	pc = (const unsigned char*)pos;
	v = 0;

	switch (len & 7) {
		case 7: v ^= (uint64_t)pc[6] << 48;
		case 6: v ^= (uint64_t)pc[5] << 40;
		case 5: v ^= (uint64_t)pc[4] << 32;
		case 4: v ^= (uint64_t)pc[3] << 24;
		case 3: v ^= (uint64_t)pc[2] << 16;
		case 2: v ^= (uint64_t)pc[1] << 8;
		case 1: v ^= (uint64_t)pc[0];
				v ^= v >> 23;
				v *= 0x2127599bf4325c37ULL;
				h ^= v ^ (v >> 47);
				h *= m;
	}

	h ^= h >> 23;
	h *= 0x2127599bf4325c37ULL;
	h ^= h >> 47;

	return h;
}

	static inline void
__init_seeds(uint64_t *seeds, int nseed)
{
	uint64_t x, y, z;
	uint64_t seed = (uint64_t)time(NULL);
	int i;

	RAND_NR_INIT(x, y, z, seed);
	for (i = 0; i < nseed; i++)
		seeds[i] = RAND_NR_NEXT(x, y, z);
}

int bfilter_create(struct bloom_filter *bf, unsigned long nbits, unsigned long nelem)
{
	bf->nbits = nbits;
	bf->nelem = nelem;
	bf->nfunc = OPTIMAL_NFUNC(nbits, nelem);
	bf->seeds = (uint64_t *) malloc(8 * bf->nfunc);
	if (bf->seeds == NULL)
		return -1;
	bf->bitmap = (unsigned long *) malloc(BITS_TO_LONGS(nbits)*sizeof(long));
	if (bf->bitmap == NULL) {
		free(bf->seeds);
		return -1;
	}
	__init_seeds(bf->seeds, bf->nfunc);
	bfilter_zero(bf);
	return 0;
}

void bfilter_destroy(struct bloom_filter *bf)
{
	free(bf->seeds);
	free(bf->bitmap);
}

void bfilter_zero(struct bloom_filter *bf)
{
	bitmap_zero(bf->bitmap, bf->nbits);
}

void bfilter_set(struct bloom_filter *bf, const void *buf, unsigned long buflen)
{
	uint64_t hash;
	int i;

	for (i = 0; i < bf->nfunc; i++) {
		hash = HASH_FUNCTION(buf, buflen, bf->seeds[i]);
		bfilter_set_hash(bf, hash);
	}
}

int bfilter_get(struct bloom_filter *bf, const void *buf, unsigned long buflen)
{
	uint64_t hash;
	int i;

	for (i = 0; i < bf->nfunc; i++) {
		hash = HASH_FUNCTION(buf, buflen, bf->seeds[i]);
		if (bfilter_get_hash(bf, hash) == 0)
			return 0;
	}

	return 1;
}

void bfilter_set_hash(struct bloom_filter *bf, unsigned long hash)
{
	unsigned long *addr;
	hash = hash % bf->nbits;
	addr = bf->bitmap + BIT_WORD(hash);
	set_bit(hash & (BITS_PER_LONG - 1), addr);
}

int bfilter_get_hash(struct bloom_filter *bf, unsigned long hash)
{
	unsigned long *addr;
	hash = hash % bf->nbits;
	addr = bf->bitmap + BIT_WORD(hash);
	return test_bit(hash & (BITS_PER_LONG - 1), addr);
}
