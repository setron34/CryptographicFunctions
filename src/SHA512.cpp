#include "SHA512.h"

char* SHA512::Evaluate(char* inputString, uint64_t inputLen) {
	uint64_t paddedDataLen;
	uint64_t* paddedData = extendString(inputString, inputLen, paddedDataLen);
	uint64_t* H = new uint64_t[8]{ 0x6a09e667f3bcc908, 0xbb67ae8584caa73b, 0x3c6ef372fe94f82b, 0xa54ff53a5f1d36f1, 
								   0x510e527fade682d1, 0x9b05688c2b3e6c1f, 0x1f83d9abfb41bd6b, 0x5be0cd19137e2179 };
	uint64_t* W = new uint64_t[80];
	uint64_t a, b, c, d, e, f, g, h;
	uint64_t T1, T2;
	for (size_t i = 0; i < paddedDataLen / 16; ++i) { //M(i) = paddedData[16 * i]..paddedData[16 * i + 15]
		memcpy(W, paddedData + 16 * i, 128);
		for (size_t t = 16; t < 80; ++t) {
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
		for (size_t t = 0; t < 80; ++t) {
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

uint64_t* SHA512::extendString(char* inputString, uint64_t inputStringLen, uint64_t& paddedDataLen) {
	uint64_t paddedStringLen = (inputStringLen + 1) + 128 - (inputStringLen + 1) % 128; // +1 because of adding 10000000 and increased to N * 128 chars
	uint8_t* paddedString = new uint8_t[paddedStringLen];
	memset(paddedString, 0, paddedStringLen);
	memcpy(paddedString, inputString, inputStringLen);
	paddedString[inputStringLen] = 128; //Added 10000000 to the end
	paddedDataLen = paddedStringLen / 8; //?
	uint64_t* paddedData = new uint64_t[paddedDataLen];

	for (size_t i = 0; i < paddedDataLen; ++i) {
		paddedData[i] = ((uint64_t)paddedString[i * 8 + 0] << 56) + ((uint64_t)paddedString[i * 8 + 1] << 48) + ((uint64_t)paddedString[i * 8 + 2] << 40) + ((uint64_t)paddedString[i * 8 + 3] << 32) +
						((uint64_t)paddedString[i * 8 + 4] << 24) + ((uint64_t)paddedString[i * 8 + 5] << 16) + ((uint64_t)paddedString[i * 8 + 6] << 8) + ((uint64_t)paddedString[i * 8 + 7] << 0);
	}
	paddedData[paddedDataLen - 1] = inputStringLen << 3;  //<< 3 == bits size -> bit size
	return paddedData;
}

char* SHA512::dataToOutputString(uint64_t* H) {
	uint8_t* outputData = new uint8_t[64];
	for (int i = 0; i < 8; ++i) {
		outputData[8 * i + 0] = H[i] >> 56;
		outputData[8 * i + 1] = H[i] >> 48;
		outputData[8 * i + 2] = H[i] >> 40;
		outputData[8 * i + 3] = H[i] >> 32;
		outputData[8 * i + 4] = H[i] >> 24;
		outputData[8 * i + 5] = H[i] >> 16;
		outputData[8 * i + 6] = H[i] >> 8;
		outputData[8 * i + 7] = H[i] >> 0;
	}
	char* outputString = new char[128];
	char* dataToHex = new char[16]{ '0', '1' , '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
	for (int i = 0; i < 64; ++i) {
		outputString[i * 2 + 0] = dataToHex[outputData[i] / 16];
		outputString[i * 2 + 1] = dataToHex[outputData[i] % 16];
	}
	return outputString;
}
