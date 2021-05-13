#include <string>
#include <iostream>
#include "Algos.cpp"
#include <cstdio>

class BLAKE2s {
public:
	static char* BLAKE2s128(const char* inputString, uint64_t inputLen, const char* key, uint32_t keySize) {
		return Evaluate(inputString, inputLen, key, keySize, 16);
	}

	static char* BLAKE2s160(const char* inputString, uint64_t inputLen, const char* key, uint32_t keySize) {
		return Evaluate(inputString, inputLen, key, keySize, 20);
	}

	static char* BLAKE2s224(const char* inputString, uint64_t inputLen, const char* key, uint32_t keySize) {
		return Evaluate(inputString, inputLen, key, keySize, 28);
	}

	static char* BLAKE2s256(const char* inputString, uint64_t inputLen, const char* key, uint32_t keySize) {
		return Evaluate(inputString, inputLen, key, keySize, 32);
	}

private:
	static const uint32_t blockBytes = 64; //block bytes 64B = 16 * uint32_t
	static constexpr uint32_t IV[8]{ 0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19 };
	static constexpr uint32_t SIGMA[10][16]{ { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15},
							{14, 10,  4,  8,  9, 15, 13,  6,  1, 12,  0,  2, 11,  7,  5,  3},
							{11,  8, 12,  0,  5,  2, 15, 13, 10, 14,  3,  6,  7,  1,  9,  4},
							{ 7,  9,  3,  1, 13, 12, 11, 14,  2,  6,  5, 10,  4,  0, 15,  8},
							{ 9,  0,  5,  7,  2,  4, 10, 15, 14,  1, 11, 12,  6,  8,  3, 13},
							{ 2, 12,  6, 10,  0, 11,  8,  3,  4, 13,  7,  5, 15, 14,  1,  9},
							{12,  5,  1, 15, 14, 13,  4, 10,  0,  7,  6,  3,  9,  2,  8, 11},
							{13, 11,  7, 14, 12,  1,  3,  9,  5,  0, 15,  4,  8,  6,  2, 10},
							{ 6, 15, 14,  9, 11,  3,  0,  8, 12,  2, 13,  7,  1,  4, 10,  5},
							{10,  2,  8,  4,  7,  6,  1,  5, 15, 11,  9, 14,  3, 12, 13,  0} };

	static char* Evaluate(const char* inputString, uint64_t inputLen, const char* key, uint32_t keySize, uint32_t hashSize) {
		uint64_t paddedDataLen;
		uint32_t* paddedData;
		if (keySize == 0) {
			paddedDataLen = (inputLen + (blockBytes - inputLen % blockBytes) % blockBytes) / 4;
			if (paddedDataLen == 0) {
				paddedDataLen = paddedDataLen + 16;
			}
			paddedData = new uint32_t[paddedDataLen];
			memset(paddedData, 0, paddedDataLen * 4);
			memcpy(paddedData, inputString, inputLen);
		}
		else {
			paddedDataLen = (inputLen + (64 - inputLen % 64) % 64) / 4 + 16;
			paddedData = new uint32_t[paddedDataLen];
			memset(paddedData, 0, paddedDataLen * 4);
			memcpy(paddedData, key, keySize);
			memcpy(paddedData + 16, inputString, inputLen);
		}

		uint64_t dd = paddedDataLen / 16; //number of blocks
		uint32_t* h = new uint32_t[8];
		memcpy(h, IV, 32);
		h[0] = h[0] ^ 0x01010000 ^ (keySize << 8) ^ hashSize;
		for (int i = 0; i < dd - 1; ++i) {
			uint32_t* d = new uint32_t[16];
			memcpy(d, paddedData + i * 16, 64);
			h = F(h, d, (i + 1) * blockBytes, false);
		}
		uint32_t* d = new uint32_t[16];
		memcpy(d, paddedData + (dd - 1) * 16, 64);
		if (keySize == 0) {
			h = F(h, d, inputLen, true);
		}
		else {
			h = F(h, d, inputLen + blockBytes, true);
		}
		return toString(h, hashSize);
	}

	static uint32_t* G(uint32_t* v, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t y) {
		v[a] = v[a] + v[b] + x;
		v[d] = Algos::ROTR32(v[d] ^ v[a], 16);
		v[c] = v[c] + v[d];
		v[b] = Algos::ROTR32(v[b] ^ v[c], 12);
		v[a] = v[a] + v[b] + y;
		v[d] = Algos::ROTR32(v[d] ^ v[a], 8);
		v[c] = v[c] + v[d];
		v[b] = Algos::ROTR32(v[b] ^ v[c], 7);
		return v;
	}

	static uint32_t* F(uint32_t* h, uint32_t* m, uint64_t t, bool f) {
		uint32_t* v = new uint32_t[16];
		memcpy(v, h, 32);
		memcpy(v + 8, IV, 32);
		v[12] = v[12] ^ t;
		v[13] = v[13] ^ (t >> 32);
		uint32_t x = 0xffffffff;
		if (f == true) {
			v[14] = v[14] ^ x;
		}
		for (int i = 0; i < 10; ++i) {
			v = G(v, 0, 4,  8, 12, m[SIGMA[i][ 0]], m[SIGMA[i][ 1]]);
			v = G(v, 1, 5,  9, 13, m[SIGMA[i][ 2]], m[SIGMA[i][ 3]]);
			v = G(v, 2, 6, 10, 14, m[SIGMA[i][ 4]], m[SIGMA[i][ 5]]);
			v = G(v, 3, 7, 11, 15, m[SIGMA[i][ 6]], m[SIGMA[i][ 7]]);
			v = G(v, 0, 5, 10, 15, m[SIGMA[i][ 8]], m[SIGMA[i][ 9]]);
			v = G(v, 1, 6, 11, 12, m[SIGMA[i][10]], m[SIGMA[i][11]]);
			v = G(v, 2, 7,  8, 13, m[SIGMA[i][12]], m[SIGMA[i][13]]);
			v = G(v, 3, 4,  9, 14, m[SIGMA[i][14]], m[SIGMA[i][15]]);
		}

		for (int i = 0; i < 8; ++i) {
			h[i] = h[i] ^ v[i] ^ v[i + 8];
		}
		return h;
	}

	static inline char* toString(uint32_t* x, uint32_t hashSize) {
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
