#include "SHA1.h"

char* SHA1::Evaluate(char* inputString, uint64_t inputLen) {
	uint64_t paddedDataLen;
	uint32_t* paddedData = extendString(inputString, inputLen, paddedDataLen);
	uint32_t A, B, C, D, E;
	uint32_t H0 = 0x67452301;
	uint32_t H1 = 0xEFCDAB89;
	uint32_t H2 = 0x98BADCFE;
	uint32_t H3 = 0x10325476;
	uint32_t H4 = 0xC3D2E1F0;
	uint32_t* W = new uint32_t[80];
	uint32_t TEMP;
	for (size_t i = 0; i < paddedDataLen / 16; ++i) { //M(i) = paddedData[16 * i]..paddedData[16 * i + 15]
		memcpy(W, paddedData + i * 16, 64); //copy M(i) to W[0]..W[15]
		for (size_t t = 16; t < 80; ++t) {
			W[t] = Algos::ROTL32(W[t - 3] ^ W[t - 8] ^ W[t - 14] ^ W[t - 16], 1); //W(t) = S^1(W(t-3) XOR W(t-8) XOR W(t-14) XOR W(t-16))
		}
		A = H0;
		B = H1;
		C = H2;
		D = H3;
		E = H4;
		for (size_t t = 0; t < 80; ++t) {
			TEMP = Algos::ROTL32(A, 5) + f(t, B, C, D) + E + W[t] + K[t];
			E = D;  
			D = C;  
			C = Algos::ROTL32(B, 30);
			B = A; 
			A = TEMP;
		}
		H0 = H0 + A;
		H1 = H1 + B; 
		H2 = H2 + C; 
		H3 = H3 + D; 
		H4 = H4 + E;
	}
	return dataToOutputString(H0, H1, H2, H3, H4);
}

uint32_t* SHA1::extendString(char* inputString, uint64_t inputStringLen, uint64_t& paddedDataLen) {
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

uint32_t SHA1::f(uint8_t t, uint32_t B, uint32_t C, uint32_t D) { //f(t;B,C,D) = (B AND C) OR ((NOT B) AND D),          ( 0 <= t <= 19)
	if (t < 20) {														  //f(t;B,C,D) = B XOR C XOR D,                         (20 <= t <= 39)
		return (B & C) | (~B & D);										  //f(t;B,C,D) = (B AND C) OR (B AND D) OR (C AND D),   (40 <= t <= 59)
	}																      //f(t;B,C,D) = B XOR C XOR D,                         (60 <= t <= 79)
	if (t < 40) {
		return B ^ C ^ D;
	}
	if (t < 60) {
		return (B & C) | (B & D) | (C & D);
	}
	return B ^ C ^ D;
}

char* SHA1::dataToOutputString(uint32_t H0, uint32_t H1, uint32_t H2, uint32_t H3, uint32_t H4) {
	uint8_t* outputData = new uint8_t[20];
	outputData[0]  = H0 >> 24;
	outputData[1]  = H0 >> 16;
	outputData[2]  = H0 >>  8;
	outputData[3]  = H0 >>  0;
	outputData[4]  = H1 >> 24;
	outputData[5]  = H1 >> 16;
	outputData[6]  = H1 >>  8;
	outputData[7]  = H1 >>  0;
	outputData[8]  = H2 >> 24;
	outputData[9]  = H2 >> 16;
	outputData[10] = H2 >>  8;
	outputData[11] = H2 >>  0;
	outputData[12] = H3 >> 24;
	outputData[13] = H3 >> 16;
	outputData[14] = H3 >>  8;
	outputData[15] = H3 >>  0;
	outputData[16] = H4 >> 24;
	outputData[17] = H4 >> 16;
	outputData[18] = H4 >>  8;
	outputData[19] = H4 >>  0;
	char* outputString = new char[40];
	char* dataToHex = new char[16]{ '0', '1' , '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
	for (int i = 0; i < 20; ++i) {
		outputString[i * 2 + 0] = dataToHex[outputData[i] / 16];
		outputString[i * 2 + 1] = dataToHex[outputData[i] % 16];
	}
	return outputString;
}