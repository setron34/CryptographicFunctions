#include <cstdint>
#include <cmath>
#include <cstring>

class MD5
{
public:
	static char* Evaluate(char* inputString, uint64_t inputLen) {
		uint64_t paddedDataLen;
		uint32_t* paddedData = extendString(inputString, inputLen, paddedDataLen);
		uint32_t A = 0x67452301;
		uint32_t B = 0xefcdab89;
		uint32_t C = 0x98badcfe;
		uint32_t D = 0x10325476;

		for (size_t i = 0; i < paddedDataLen / 16; ++i) {
			uint32_t* X = new uint32_t[16];
			memcpy(X, paddedData + i * 16, 64);
			uint32_t AA = A;
			uint32_t BB = B;
			uint32_t CC = C;
			uint32_t DD = D;
			uint32_t T[65] = { 0x00000000, 0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
					   0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501, 0x698098d8,
					   0x8b44f7af, 0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193,
					   0xa679438e, 0x49b40821, 0xf61e2562, 0xc040b340, 0x265e5a51,
					   0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
					   0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905,
					   0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a, 0xfffa3942, 0x8771f681,
					   0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60,
					   0xbebfbc70, 0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
					   0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665, 0xf4292244,
					   0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92,
					   0xffeff47d, 0x85845dd1, 0x6fa87e4f, 0xfe2ce6e0, 0xa3014314,
					   0x4e0811a1, 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391, };

			/* Round 1. */
			roundFunc(A, B, C, D, &F, X[0],   7,  T[1]);
			roundFunc(D, A, B, C, &F, X[1],  12,  T[2]);
			roundFunc(C, D, A, B, &F, X[2],  17,  T[3]);
			roundFunc(B, C, D, A, &F, X[3],  22,  T[4]);
			roundFunc(A, B, C, D, &F, X[4],   7,  T[5]);
			roundFunc(D, A, B, C, &F, X[5],  12,  T[6]);
			roundFunc(C, D, A, B, &F, X[6],  17,  T[7]);
			roundFunc(B, C, D, A, &F, X[7],  22,  T[8]);
			roundFunc(A, B, C, D, &F, X[8],   7,  T[9]);
			roundFunc(D, A, B, C, &F, X[9],  12, T[10]);
			roundFunc(C, D, A, B, &F, X[10], 17, T[11]);
			roundFunc(B, C, D, A, &F, X[11], 22, T[12]);
			roundFunc(A, B, C, D, &F, X[12],  7, T[13]);
			roundFunc(D, A, B, C, &F, X[13], 12, T[14]);
			roundFunc(C, D, A, B, &F, X[14], 17, T[15]);
			roundFunc(B, C, D, A, &F, X[15], 22, T[16]);
			

			/* Round 2. */
			roundFunc(A, B, C, D, &G, X[1],   5, T[17]);
			roundFunc(D, A, B, C, &G, X[6],   9, T[18]);
			roundFunc(C, D, A, B, &G, X[11], 14, T[19]);
			roundFunc(B, C, D, A, &G, X[0],  20, T[20]);
			roundFunc(A, B, C, D, &G, X[5],   5, T[21]);
			roundFunc(D, A, B, C, &G, X[10],  9, T[22]);
			roundFunc(C, D, A, B, &G, X[15], 14, T[23]);
			roundFunc(B, C, D, A, &G, X[4],  20, T[24]);
			roundFunc(A, B, C, D, &G, X[9],   5, T[25]);
			roundFunc(D, A, B, C, &G, X[14],  9, T[26]);
			roundFunc(C, D, A, B, &G, X[3],  14, T[27]);
			roundFunc(B, C, D, A, &G, X[8],  20, T[28]);
			roundFunc(A, B, C, D, &G, X[13],  5, T[29]);
			roundFunc(D, A, B, C, &G, X[2],   9, T[30]);
			roundFunc(C, D, A, B, &G, X[7],  14, T[31]);
			roundFunc(B, C, D, A, &G, X[12], 20, T[32]);

			/* Round 3. */
			roundFunc(A, B, C, D, &H, X[5],   4, T[33]);
			roundFunc(D, A, B, C, &H, X[8],  11, T[34]);
			roundFunc(C, D, A, B, &H, X[11], 16, T[35]);
			roundFunc(B, C, D, A, &H, X[14], 23, T[36]);
			roundFunc(A, B, C, D, &H, X[1],   4, T[37]);
			roundFunc(D, A, B, C, &H, X[4],  11, T[38]);
			roundFunc(C, D, A, B, &H, X[7],  16, T[39]);
			roundFunc(B, C, D, A, &H, X[10], 23, T[40]);
			roundFunc(A, B, C, D, &H, X[13],  4, T[41]);
			roundFunc(D, A, B, C, &H, X[0],  11, T[42]);
			roundFunc(C, D, A, B, &H, X[3],  16, T[43]);
			roundFunc(B, C, D, A, &H, X[6],  23, T[44]);
			roundFunc(A, B, C, D, &H, X[9],   4, T[45]);
			roundFunc(D, A, B, C, &H, X[12], 11, T[46]);
			roundFunc(C, D, A, B, &H, X[15], 16, T[47]);
			roundFunc(B, C, D, A, &H, X[2],  23, T[48]);

			/* Round 4. */
			roundFunc(A, B, C, D, &I, X[0],   6, T[49]);
			roundFunc(D, A, B, C, &I, X[7],  10, T[50]);
			roundFunc(C, D, A, B, &I, X[14], 15, T[51]);
			roundFunc(B, C, D, A, &I, X[5],  21, T[52]);
			roundFunc(A, B, C, D, &I, X[12],  6, T[53]);
			roundFunc(D, A, B, C, &I, X[3],  10, T[54]);
			roundFunc(C, D, A, B, &I, X[10], 15, T[55]);
			roundFunc(B, C, D, A, &I, X[1],  21, T[56]);
			roundFunc(A, B, C, D, &I, X[8],   6, T[57]);
			roundFunc(D, A, B, C, &I, X[15], 10, T[58]);
			roundFunc(C, D, A, B, &I, X[6],  15, T[59]);
			roundFunc(B, C, D, A, &I, X[13], 21, T[60]);
			roundFunc(A, B, C, D, &I, X[4],   6, T[61]);
			roundFunc(D, A, B, C, &I, X[11], 10, T[62]);
			roundFunc(C, D, A, B, &I, X[2],  15, T[63]);
			roundFunc(B, C, D, A, &I, X[9],  21, T[64]);

			A = A + AA;
			B = B + BB;
			C = C + CC;
			D = D + DD;
		}

		return dataToOutputString(A, B, C, D);
	}

private:
	static uint32_t* extendString(char* inputString, uint64_t inputStringLen, uint64_t& paddedDataLen) {
		uint64_t paddedStringLen = (inputStringLen + 1) + 64 - (inputStringLen + 1) % 64; // +1 because of adding 10000000 and increased to N * 64 chars
		char* paddedString = new char[paddedStringLen];
		memset(paddedString, 0, paddedStringLen);
		memcpy(paddedString, inputString, inputStringLen);
		paddedString[inputStringLen] = 128; //Added 10000000 to the end
		paddedDataLen = paddedStringLen / 4;
		uint32_t* paddedData = new uint32_t[paddedDataLen];
		for (size_t i = 0; i < paddedDataLen; ++i) {
			memcpy(paddedData + i, paddedString + 4 * i, 4);
			//paddedData[i] = ((uint32_t)paddedString[i * 4 + 3] << 24) + ((uint32_t)paddedString[i * 4 + 2] << 16) + ((uint32_t)paddedString[i * 4 + 1] << 8) + (uint32_t)paddedString[i * 4 + 0];
		}
		paddedData[paddedDataLen - 1] = inputStringLen >> 29; //<< 3 >> 32
		paddedData[paddedDataLen - 2] = inputStringLen << 3;  //<< 3
		return paddedData;
	}

	static char* dataToOutputString(uint32_t A, uint32_t B, uint32_t C, uint32_t D) {
		uint8_t* outputData = new uint8_t[16];
		memcpy(outputData, new uint32_t[4]{ A, B, C, D }, 16);
		char* outputString = new char[32];
		char* dataToHex = new char[16]{ '0', '1' , '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
		for (int i = 0; i < 16; ++i) {
			outputString[i * 2 + 0] = dataToHex[outputData[i] / 16];
			outputString[i * 2 + 1] = dataToHex[outputData[i] % 16];
		}
		return outputString;
	}

	static inline uint32_t F(uint32_t X, uint32_t Y, uint32_t Z) { //F(X, Y, Z) = XY v not(X)Z
		return (X & Y) | (~X & Z); 
	} 
	static inline uint32_t G(uint32_t X, uint32_t Y, uint32_t Z) { //G(X, Y, Z) = XZ v Y not(Z)
		return (X & Z) | (Y & ~Z);
	}
	static inline uint32_t H(uint32_t X, uint32_t Y, uint32_t Z) { //H(X, Y, Z) = X xor Y xor Z
		return X ^ Y ^ Z;
	}
	static inline uint32_t I(uint32_t X, uint32_t Y, uint32_t Z) { //I(X, Y, Z) = Y xor (X v not(Z))
		return Y ^ (X | ~Z);
	}
	static inline void roundFunc(uint32_t& A, uint32_t& B, uint32_t& C, uint32_t& D, uint32_t (*func)(uint32_t, uint32_t, uint32_t), int Xk, int s, int Ti) {
		A = B + rotateLeft(A + func(B, C, D) + Xk + Ti, s);
	}
	static inline uint32_t rotateLeft(uint32_t x, uint32_t s) {
		return x << s | x >> (32 - s);
	}
};
