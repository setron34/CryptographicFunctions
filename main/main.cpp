#include "BLAKE2b.h"
#include "BLAKE2s.h"
#include "MD5.h"
#include "SHA1.h"
#include "SHA256.h"
#include "SHA3.h"
#include "SHA512.h"
#include <chrono>
#include <iostream>

int main()
{
	auto t1 = std::chrono::high_resolution_clock::now();
	std::string ans = BLAKE2b::BLAKE2b512("xx", 2, "", 0); //BLAKE2b512 is not working anymore
	ans.resize(128);
	auto t2 = std::chrono::high_resolution_clock::now();
	double duration = (double)std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() / 1000000;
	std::cout << duration << '\n' << ans;
}
