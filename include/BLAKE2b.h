#pragma once
#include <cstring>
#include "Algos.h"

class BLAKE2b {
public:
	static char* BLAKE2b160(const char* inputString, uint64_t inputLen, const char* key, uint32_t keySize);
	static char* BLAKE2b256(const char* inputString, uint64_t inputLen, const char* key, uint32_t keySize);
	static char* BLAKE2b384(const char* inputString, uint64_t inputLen, const char* key, uint32_t keySize);
	static char* BLAKE2b512(const char* inputString, uint64_t inputLen, const char* key, uint32_t keySize);

private:
	static uint64_t const blockBytes = 128;
	static uint64_t constexpr IV[8]{ 0x6a09e667f3bcc908, 0xbb67ae8584caa73b, 0x3c6ef372fe94f82b, 0xa54ff53a5f1d36f1,
									 0x510e527fade682d1, 0x9b05688c2b3e6c1f, 0x1f83d9abfb41bd6b, 0x5be0cd19137e2179 };
	static uint64_t constexpr SIGMA[12][16]{ { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15},
											 {14, 10,  4,  8,  9, 15, 13,  6,  1, 12,  0,  2, 11,  7,  5,  3},
											 {11,  8, 12,  0,  5,  2, 15, 13, 10, 14,  3,  6,  7,  1,  9,  4},
											 { 7,  9,  3,  1, 13, 12, 11, 14,  2,  6,  5, 10,  4,  0, 15,  8},
											 { 9,  0,  5,  7,  2,  4, 10, 15, 14,  1, 11, 12,  6,  8,  3, 13},
											 { 2, 12,  6, 10,  0, 11,  8,  3,  4, 13,  7,  5, 15, 14,  1,  9},
											 {12,  5,  1, 15, 14, 13,  4, 10,  0,  7,  6,  3,  9,  2,  8, 11},
											 {13, 11,  7, 14, 12,  1,  3,  9,  5,  0, 15,  4,  8,  6,  2, 10},
											 { 6, 15, 14,  9, 11,  3,  0,  8, 12,  2, 13,  7,  1,  4, 10,  5},
											 {10,  2,  8,  4,  7,  6,  1,  5, 15, 11,  9, 14,  3, 12, 13,  0},
											 { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15},
											 {14, 10,  4,  8,  9, 15, 13,  6,  1, 12,  0,  2, 11,  7,  5,  3}, };

	static char* Evaluate(const char* inputString, uint64_t inputLen, const char* key, uint64_t keySize, uint64_t hashSize);
	static uint64_t* G(uint64_t* v, uint64_t a, uint64_t b, uint64_t c, uint64_t d, uint64_t x, uint64_t y);
	static uint64_t* F(uint64_t* h, uint64_t* m, uint64_t t, bool f);
	static char* toString(uint64_t* x, uint64_t hashSize);
};


