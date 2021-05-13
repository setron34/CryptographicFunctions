#include "SHA256.h"

char* SHA256::Evaluate(char* inputString, uint64_t inputLen) {
	uint64_t paddedDataLen;
	uint32_t* paddedData = extendString(inputString, inputLen, paddedDataLen);
	uint32_t* H = new uint32_t[8]{ 0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19 };
	uint32_t* W = new uint32_t[64];
	uint32_t a, b, c, d, e, f, g, h;
	uint32_t T1, T2;

	for (size_t i = 0; i < paddedDataLen / 16; ++i) { //M(i) = paddedData[16 * i]..paddedData[16 * i + 15]
		memcpy(W, paddedData + 16 * i, 64);
		for (size_t t = 16; t < 64; ++t) {
			W[t] = lowerSigma1(W[t - 2]) + W[t - 7] + lowerSigma0(W[t - 15]) + W[t - 16];
		}
		a = H[0];
		b = H[1];
		c = H[2];
		d = H[3];
		e = H[4];
		f = H[5];
		g = H[6];
		h = H[7];
		for (size_t t = 0; t < 64; ++t) {
			T1 = h + upperSigma1(e) + Ch(e, f, g) + K[t] + W[t];
			T2 = upperSigma0(a) + Maj(a, b, c);
			h = g;
			g = f;
			f = e;
			e = d + T1;
			d = c;
			c = b;
			b = a;
			a = T1 + T2;
		}
		H[0] = a + H[0];
		H[1] = b + H[1];
		H[2] = c + H[2];
		H[3] = d + H[3];
		H[4] = e + H[4];
		H[5] = f + H[5];
		H[6] = g + H[6];
		H[7] = h + H[7];
	}
	return dataToOutputString(H);
}

uint32_t* SHA256::extendString(char* inputString, uint64_t inputStringLen, uint64_t& paddedDataLen) {
	uint64_t paddedStringLen = (inputStringLen + 1) + 64 - (inputStringLen + 1) % 64; // +1 because of adding 10000000 and increased to N * 64 chars
	uint8_t* paddedString = new uint8_t[paddedStringLen];
	memset(paddedString, 0, paddedStringLen);
	memcpy(paddedString, inputString, inputStringLen);
	paddedString[inputStringLen] = 128; //Added 10000000 to the end
	paddedDataLen = paddedStringLen / 4;
	uint32_t* paddedData = new uint32_t[paddedDataLen];

	for (size_t i = 0; i < paddedDataLen; ++i) {
		paddedData[i] = ((uint32_t)paddedString[i * 4 + 0] << 24) + ((uint32_t)paddedString[i * 4 + 1] << 16) + ((uint32_t)paddedString[i * 4 + 2] << 8) + ((uint32_t)paddedString[i * 4 + 3] << 0);
	}
	paddedData[paddedDataLen - 2] = inputStringLen >> 29; //<< 3 >> 32
	paddedData[paddedDataLen - 1] = inputStringLen << 3;  //<< 3 == bits size -> bit size
	return paddedData;
}

char* SHA256::dataToOutputString(uint32_t* H) {
	uint8_t* outputData = new uint8_t[32];
	for (int i = 0; i < 8; ++i) {
		outputData[4 * i + 0] = H[i] >> 24;
		outputData[4 * i + 1] = H[i] >> 16;
		outputData[4 * i + 2] = H[i] >>  8;
		outputData[4 * i + 3] = H[i] >>  0;
	}
	char* outputString = new char[64];
	char* dataToHex = new char[16]{ '0', '1' , '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
	for (int i = 0; i < 32; ++i) {
		outputString[i * 2 + 0] = dataToHex[outputData[i] / 16];
		outputString[i * 2 + 1] = dataToHex[outputData[i] % 16];
	}
	return outputString;
}