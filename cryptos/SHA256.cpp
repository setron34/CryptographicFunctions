#include <string>
#include <iostream>
#include <iomanip>
#include <bitset>

class SHA256 {
public:
	static char* Evaluate(char* inputString, uint64_t inputLen) {
		uint64_t paddedDataLen;
		uint32_t* paddedData = extendString(inputString, inputLen, paddedDataLen);
		uint32_t* H = new uint32_t[8]{ 0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19 };
		uint32_t* W = new uint32_t[64];
		uint32_t a, b, c, d, e, f, g, h;
		uint32_t T1, T2;
		const uint32_t* K = new uint32_t[64]{ 0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
										0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
										0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
										0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
										0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
										0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
										0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
										0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2 };
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

private:
	static uint32_t* extendString(char* inputString, uint64_t inputStringLen, uint64_t& paddedDataLen) {
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

	static inline uint32_t rotateLeft(uint32_t x, uint32_t s) {
		return x << s | x >> (32 - s);
	}

	static inline uint32_t rotateRight(uint32_t x, uint32_t s) {
		return x >> s | x << (32 - s);
	}

	static inline uint32_t Ch(uint32_t x, uint32_t y, uint32_t z) {
		return (x & y) ^ (~x & z);
	}

	static inline uint32_t Maj(uint32_t x, uint32_t y, uint32_t z) {
		return (x & y) ^ (x & z) ^ (y & z);
	}

	static inline uint32_t upperSigma0(uint32_t x) {
		return rotateRight(x, 2) ^ rotateRight(x, 13) ^ rotateRight(x, 22);
	}

	static inline uint32_t upperSigma1(uint32_t x) {
		return rotateRight(x, 6) ^ rotateRight(x, 11) ^ rotateRight(x, 25);
	}

	static inline uint32_t lowerSigma0(uint32_t x) {
		return rotateRight(x, 7) ^ rotateRight(x, 18) ^ (x >> 3);
	}

	static inline uint32_t lowerSigma1(uint32_t x) {
		return rotateRight(x, 17) ^ rotateRight(x, 19) ^ (x >> 10);
	}

	static char* dataToOutputString(uint32_t* H) {
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
};