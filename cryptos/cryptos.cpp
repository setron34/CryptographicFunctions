#include "MD5.cpp"
#include "SHA1.cpp"
#include "SHA256.cpp"
#include "SHA512.cpp"
#include <iostream>
#include <string>
#include <cstdint>
#include <stdint.h>
int main()
{
	std::string s;
	std::getline(std::cin, s);
	size_t n = s.length();
	char* input = &s[0];
	char* output = SHA512::Evaluate(input, n);
	for (int i = 0; i < 128; ++i) {
		std::cout << output[i];
	}
}
