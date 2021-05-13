#include "BLAKE2b.h"

char* BLAKE2b::BLAKE2b160(const char* inputString, uint64_t inputLen, const char* key, uint32_t keySize) {
	return Evaluate(inputString, inputLen, key, keySize, 20);
}

char* BLAKE2b::BLAKE2b256(const char* inputString, uint64_t inputLen, const char* key, uint32_t keySize) {
	return Evaluate(inputString, inputLen, key, keySize, 32);
}

char* BLAKE2b::BLAKE2b384(const char* inputString, uint64_t inputLen, const char* key, uint32_t keySize) {
	return Evaluate(inputString, inputLen, key, keySize, 48);
}

char* BLAKE2b::BLAKE2b512(const char* inputString, uint64_t inputLen, const char* key, uint32_t keySize) {
	return Evaluate(inputString, inputLen, key, keySize, 64);
}

char* BLAKE2b::Evaluate(const char* inputString, uint64_t inputLen, const char* key, uint64_t keySize, uint64_t hashSize) {
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

uint64_t* BLAKE2b::G(uint64_t* v, uint64_t a, uint64_t b, uint64_t c, uint64_t d, uint64_t x, uint64_t y) {
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

uint64_t* BLAKE2b::F(uint64_t* h, uint64_t* m, uint64_t t, bool f) {
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

char* BLAKE2b::toString(uint64_t* x, uint64_t hashSize) {
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
