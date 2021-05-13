#include "SHA3.cpp"
#include "BLAKE2s.cpp"
#include "BLAKE2b.cpp"
#include <chrono>

int main()
{
	auto t1 = std::chrono::high_resolution_clock::now();
	std::string ans = BLAKE2s::BLAKE2s256("", 0, "xxxx", 4); //Empty string with key generates wrong value
	ans.resize(64);
	auto t2 = std::chrono::high_resolution_clock::now();
	double duration = (double)std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() / 1000000;
	std::cout << duration << '\n' << ans;
}
