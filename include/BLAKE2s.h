#pragma once
#include <cstring>
#include "Algos.h"

class BLAKE2s {
public:
	static char* BLAKE2s128(const char* inputString, uint64_t inputLen, const char* key, uint32_t keySize);
	static char* BLAKE2s160(const char* inputString, uint64_t inputLen, const char* key, uint32_t keySize);
	static char* BLAKE2s224(const char* inputString, uint64_t inputLen, const char* key, uint32_t keySize);
	static char* BLAKE2s256(const char* inputString, uint64_t inputLen, const char* key, uint32_t keySize);

private:
	static uint32_t const blockBytes = 64; //block bytes 64B = 16 * uint32_t
	static uint32_t constexpr IV[8]{ 0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19 };
	static uint32_t constexpr SIGMA[10][16]{ { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15},
											 {14, 10,  4,  8,  9, 15, 13,  6,  1, 12,  0,  2, 11,  7,  5,  3},
											 {11,  8, 12,  0,  5,  2, 15, 13, 10, 14,  3,  6,  7,  1,  9,  4},
											 { 7,  9,  3,  1, 13, 12, 11, 14,  2,  6,  5, 10,  4,  0, 15,  8},
											 { 9,  0,  5,  7,  2,  4, 10, 15, 14,  1, 11, 12,  6,  8,  3, 13},
											 { 2, 12,  6, 10,  0, 11,  8,  3,  4, 13,  7,  5, 15, 14,  1,  9},
											 {12,  5,  1, 15, 14, 13,  4, 10,  0,  7,  6,  3,  9,  2,  8, 11},
											 {13, 11,  7, 14, 12,  1,  3,  9,  5,  0, 15,  4,  8,  6,  2, 10},
											 { 6, 15, 14,  9, 11,  3,  0,  8, 12,  2, 13,  7,  1,  4, 10,  5},
											 {10,  2,  8,  4,  7,  6,  1,  5, 15, 11,  9, 14,  3, 12, 13,  0} };

	static char* Evaluate(const char* inputString, uint64_t inputLen, const char* key, uint32_t keySize, uint32_t hashSize);
	static uint32_t* G(uint32_t* v, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t y);
	static uint32_t* F(uint32_t* h, uint32_t* m, uint64_t t, bool f);
	static char* toString(uint32_t* x, uint32_t hashSize);
};

