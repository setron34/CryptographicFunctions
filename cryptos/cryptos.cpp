#include "MD5.cpp"
#include "SHA1.cpp"
#include <iostream>
#include <string>
int main()
{
	std::string s;
	std::getline(std::cin, s);
	size_t n = s.size();
	char* input = new char[n]{*s.c_str()};
	char* output = SHA1::EvaluateMethod1(input, n);
	for (int i = 0; i < 40; ++i) {
		std::cout << output[i];
	}
}
