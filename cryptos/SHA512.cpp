#include <string>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include "Algos.cpp"

class SHA512 {
public:
	static char* Evaluate(char* inputString, uint64_t inputLen) {
		uint64_t paddedDataLen;
		uint64_t* paddedData = extendString(inputString, inputLen, paddedDataLen);
		uint64_t* H = new uint64_t[8]{ 0x6a09e667f3bcc908, 0xbb67ae8584caa73b, 0x3c6ef372fe94f82b, 0xa54ff53a5f1d36f1, 
									   0x510e527fade682d1, 0x9b05688c2b3e6c1f, 0x1f83d9abfb41bd6b, 0x5be0cd19137e2179 };
		uint64_t* W = new uint64_t[80];
		uint64_t a, b, c, d, e, f, g, h;
		uint64_t T1, T2;
		const uint64_t* K = new uint64_t[80]{ 0x428a2f98d728ae22, 0x7137449123ef65cd, 0xb5c0fbcfec4d3b2f, 0xe9b5dba58189dbbc,
											  0x3956c25bf348b538, 0x59f111f1b605d019, 0x923f82a4af194f9b, 0xab1c5ed5da6d8118,
											  0xd807aa98a3030242, 0x12835b0145706fbe, 0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2,
											  0x72be5d74f27b896f, 0x80deb1fe3b1696b1, 0x9bdc06a725c71235, 0xc19bf174cf692694,
											  0xe49b69c19ef14ad2, 0xefbe4786384f25e3, 0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65,
											  0x2de92c6f592b0275, 0x4a7484aa6ea6e483, 0x5cb0a9dcbd41fbd4, 0x76f988da831153b5,
											  0x983e5152ee66dfab, 0xa831c66d2db43210, 0xb00327c898fb213f, 0xbf597fc7beef0ee4,
											  0xc6e00bf33da88fc2, 0xd5a79147930aa725, 0x06ca6351e003826f, 0x142929670a0e6e70,
											  0x27b70a8546d22ffc, 0x2e1b21385c26c926, 0x4d2c6dfc5ac42aed, 0x53380d139d95b3df,
											  0x650a73548baf63de, 0x766a0abb3c77b2a8, 0x81c2c92e47edaee6, 0x92722c851482353b,
											  0xa2bfe8a14cf10364, 0xa81a664bbc423001, 0xc24b8b70d0f89791, 0xc76c51a30654be30,
											  0xd192e819d6ef5218, 0xd69906245565a910, 0xf40e35855771202a, 0x106aa07032bbd1b8,
											  0x19a4c116b8d2d0c8, 0x1e376c085141ab53, 0x2748774cdf8eeb99, 0x34b0bcb5e19b48a8,
											  0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb, 0x5b9cca4f7763e373, 0x682e6ff3d6b2b8a3,
											  0x748f82ee5defb2fc, 0x78a5636f43172f60, 0x84c87814a1f0ab72, 0x8cc702081a6439ec,
											  0x90befffa23631e28, 0xa4506cebde82bde9, 0xbef9a3f7b2c67915, 0xc67178f2e372532b,
											  0xca273eceea26619c, 0xd186b8c721c0c207, 0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178,
											  0x06f067aa72176fba, 0x0a637dc5a2c898a6, 0x113f9804bef90dae, 0x1b710b35131c471b,
											  0x28db77f523047d84, 0x32caab7b40c72493, 0x3c9ebe0a15c9bebc, 0x431d67c49c100d4c,
											  0x4cc5d4becb3e42b6, 0x597f299cfc657e2a, 0x5fcb6fab3ad6faec, 0x6c44198c4a475817 };
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

private:
	static uint64_t* extendString(char* inputString, uint64_t inputStringLen, uint64_t& paddedDataLen) {
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

	static inline uint64_t Ch(uint64_t x, uint64_t y, uint64_t z) {
		return (x & y) ^ (~x & z);
	}

	static inline uint64_t Maj(uint64_t x, uint64_t y, uint64_t z) {
		return (x & y) ^ (x & z) ^ (y & z);
	}

	static inline uint64_t upperSigma0(uint64_t x) {
		return Algos::ROTR64(x, 28) ^ Algos::ROTR64(x, 34) ^ Algos::ROTR64(x, 39);
	}

	static inline uint64_t upperSigma1(uint64_t x) {
		return Algos::ROTR64(x, 14) ^ Algos::ROTR64(x, 18) ^ Algos::ROTR64(x, 41);
	}

	static inline uint64_t lowerSigma0(uint64_t x) {
		return Algos::ROTR64(x, 1) ^ Algos::ROTR64(x, 8) ^ (x >> 7);
	}

	static inline uint64_t lowerSigma1(uint64_t x) {
		return Algos::ROTR64(x, 19) ^ Algos::ROTR64(x, 61) ^ (x >> 6);
	}

	static char* dataToOutputString(uint64_t* H) {
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
};
