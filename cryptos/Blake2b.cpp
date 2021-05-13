#include <string>
#include <iostream>
#include "Algos.cpp"
#include <cstdio>

class BLAKE2b {
public:
	static char* BLAKE2b256(const char* inputString, uint64_t inputLen, const char* key, uint32_t keySize) {
		return Evaluate(inputString, inputLen, key, keySize, 32);
	}

	static char* BLAKE2b320(const char* inputString, uint64_t inputLen, const char* key, uint32_t keySize) {
		return Evaluate(inputString, inputLen, key, keySize, 40);
	}

	static char* BLAKE2b448(const char* inputString, uint64_t inputLen, const char* key, uint32_t keySize) {
		return Evaluate(inputString, inputLen, key, keySize, 56);
	}

	static char* BLAKE2b512(const char* inputString, uint64_t inputLen, const char* key, uint32_t keySize) {
		return Evaluate(inputString, inputLen, key, keySize, 64);
	}

private:
	static const uint64_t blockBytes = 128; //block bytes 128B = 16 * uint64_t
	static constexpr uint64_t IV[8]{ 0x6a09e667f3bcc908, 0xbb67ae8584caa73b, 0x3c6ef372fe94f82b, 0xa54ff53a5f1d36f1, 0x510e527fade682d1, 0x9b05688c2b3e6c1f, 0x1f83d9abfb41bd6b, 0x5be0cd19137e2179 };
	static constexpr uint64_t SIGMA[12][16]{ { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15},
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

	static char* Evaluate(const char* inputString, uint64_t inputLen, const char* key, uint64_t keySize, uint64_t hashSize) {
		uint64_t paddedDataLen;
		uint64_t* paddedData;
		if (keySize == 0) {
			paddedDataLen = (inputLen + (blockBytes - inputLen % blockBytes) % blockBytes) / 8;
			if (paddedDataLen == 0) {
				paddedDataLen = paddedDataLen + 16;
			}
			paddedData = new uint64_t[paddedDataLen];
			memset(paddedData, 0, paddedDataLen * 8);
			memcpy(paddedData, inputString, inputLen);
		}
		else {
			paddedDataLen = (inputLen + (blockBytes - inputLen % blockBytes) % blockBytes) / 8 + 16;
			paddedData = new uint64_t[paddedDataLen];
			memset(paddedData, 0, paddedDataLen * 8);
			memcpy(paddedData, key, keySize);
			memcpy(paddedData + 16, inputString, inputLen);
		}

		uint64_t dd = paddedDataLen / 16; //number of blocks
		uint64_t* h = new uint64_t[8];
		memcpy(h, IV, 64);
		h[0] = h[0] ^ 0x01010000 ^ (keySize << 8) ^ hashSize;
		for (int i = 0; i < dd - 1; ++i) {
			uint64_t* d = new uint64_t[16];
			memcpy(d, paddedData + i * 16, 128);
			h = F(h, d, (i + 1) * blockBytes, false);
		}
		uint64_t* d = new uint64_t[16];
		memcpy(d, paddedData + (dd - 1) * 16, 128);
		if (keySize == 0) {
			h = F(h, d, inputLen, true);
		}
		else {
			h = F(h, d, inputLen + blockBytes, true);
		}
		return toString(h, hashSize);
	}

	static uint64_t* G(uint64_t* v, uint64_t a, uint64_t b, uint64_t c, uint64_t d, uint64_t x, uint64_t y) {
		v[a] = v[a] + v[b] + x;
		v[d] = Algos::ROTR64(v[d] ^ v[a], 32);
		v[c] = v[c] + v[d];
		v[b] = Algos::ROTR64(v[b] ^ v[c], 24);
		v[a] = v[a] + v[b] + y;
		v[d] = Algos::ROTR64(v[d] ^ v[a], 16);
		v[c] = v[c] + v[d];
		v[b] = Algos::ROTR64(v[b] ^ v[c], 63);
		return v;
	}

	static uint64_t* F(uint64_t* h, uint64_t* m, uint64_t t, bool f) {
		uint64_t* v = new uint64_t[16];
		memcpy(v, h, 64);
		memcpy(v + 8, IV, 64);
		v[12] = v[12] ^ t;
		v[13] = v[13] ^ (t >> 64);
		uint64_t x = 0xffffffffffffffff;
		if (f == true) {
			v[14] = v[14] ^ x;
		}
		for (int i = 0; i < 12; ++i) {
			v = G(v, 0, 4, 8, 12, m[SIGMA[i][0]], m[SIGMA[i][1]]);
			v = G(v, 1, 5, 9, 13, m[SIGMA[i][2]], m[SIGMA[i][3]]);
			v = G(v, 2, 6, 10, 14, m[SIGMA[i][4]], m[SIGMA[i][5]]);
			v = G(v, 3, 7, 11, 15, m[SIGMA[i][6]], m[SIGMA[i][7]]);
			v = G(v, 0, 5, 10, 15, m[SIGMA[i][8]], m[SIGMA[i][9]]);
			v = G(v, 1, 6, 11, 12, m[SIGMA[i][10]], m[SIGMA[i][11]]);
			v = G(v, 2, 7, 8, 13, m[SIGMA[i][12]], m[SIGMA[i][13]]);
			v = G(v, 3, 4, 9, 14, m[SIGMA[i][14]], m[SIGMA[i][15]]);
		}

		for (int i = 0; i < 8; ++i) {
			h[i] = h[i] ^ v[i] ^ v[i + 8];
		}
		return h;
	}

	static inline char* toString(uint64_t* x, uint64_t hashSize) {
		uint8_t* outputData = new uint8_t[hashSize];
		memcpy(outputData, x, hashSize);
		char* outputString = new char[hashSize * 2];
		char* dataToHex = new char[16]{ '0', '1' , '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
		for (int i = 0; i < hashSize; ++i) {
			outputString[i * 2 + 0] = dataToHex[outputData[i] / 16];
			outputString[i * 2 + 1] = dataToHex[outputData[i] % 16];
		}
		return outputString;
	}
};
