#include <cstdint>

class Algos
{
public:
	static inline uint32_t ROTL32(uint32_t x, uint32_t s) {
		return x << s | x >> (32 - s);
	}

	static inline uint32_t ROTR32(uint32_t x, uint32_t s) {
		return x >> s | x << (32 - s);
	}

	static inline uint64_t ROTL64(uint64_t x, uint64_t s) {
		return x << s | x >> (64 - s);
	}

	static inline uint64_t ROTR64(uint64_t x, uint64_t s) {
		return x >> s | x << (64 - s);
	}
};

