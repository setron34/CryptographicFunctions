#include "BLAKE2s.h"

char* BLAKE2s::BLAKE2s128(const char* inputString, uint64_t inputLen, const char* key, uint32_t keySize) {
	return Evaluate(inputString, inputLen, key, keySize, 16);
}

char* BLAKE2s::BLAKE2s160(const char* inputString, uint64_t inputLen, const char* key, uint32_t keySize) {
	return Evaluate(inputString, inputLen, key, keySize, 20);
}

char* BLAKE2s::BLAKE2s224(const char* inputString, uint64_t inputLen, const char* key, uint32_t keySize) {
	return Evaluate(inputString, inputLen, key, keySize, 28);
}

char* BLAKE2s::BLAKE2s256(const char* inputString, uint64_t inputLen, const char* key, uint32_t keySize) {
	return Evaluate(inputString, inputLen, key, keySize, 32);
}

char* BLAKE2s::Evaluate(const char* inputString, uint64_t inputLen, const char* key, uint32_t keySize, uint32_t hashSize) {
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

uint32_t* BLAKE2s::G(uint32_t* v, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t y) {
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

uint32_t* BLAKE2s::F(uint32_t* h, uint32_t* m, uint64_t t, bool f) {
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

char* BLAKE2s::toString(uint32_t* x, uint32_t hashSize) {
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
