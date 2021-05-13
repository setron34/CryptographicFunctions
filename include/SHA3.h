#pragma once
#include <string>
#include <vector>

class SHA3 {
private:
	class stArray {
	private:
		std::vector< std::vector< std::vector<bool> > >* A;
	public:
		stArray(size_t w) {
			A = new std::vector< std::vector< std::vector<bool> > >(5, std::vector< std::vector<bool> >(5, std::vector<bool>(w, 0)));
		}
		std::vector< std::vector<bool> >& operator[](size_t x) {
			return (*A)[x];
		}
		~stArray() {
			free(A);
		}
	};

	static std::vector<bool> trunc(const std::vector<bool>& v, int x);
	static std::vector<bool> XOR(const std::vector<bool>& v1, const std::vector<bool>& v2);
	static std::vector<bool> conc(const std::vector<bool>& v1, const std::vector<bool>& v2);
	static stArray* toStArray(const std::vector<bool>& S);
	static std::vector<bool> toBitString(stArray* A);
	static stArray* theta(stArray* A);
	static stArray* rho(SHA3::stArray* A);
	static stArray* pi(stArray* A);
	static stArray* chi(stArray* A);
	static bool rc(int t);
	static stArray* iota(stArray* A, int ir);
	static stArray* Rnd(stArray* A, int ir);
	static std::vector<bool> keccak_p(const std::vector<bool>& S, int nr);
	static std::vector<bool> keccak_f(const std::vector<bool>& S);
	static std::vector<bool> pad10etc1(int x, int m);
	static std::vector<bool> sponge(std::vector<bool>(*f)(const std::vector<bool>&), std::vector<bool>(*pad)(int, int), int r, std::vector<bool> P, int d);
	static std::vector<bool> keccak(int c, std::vector<bool> N, int d);
	static std::string toString(const std::vector<bool>& v);
	static std::vector<bool> toBitString(std::string s);

public:
	static std::string SHA3_224(std::string s);
	static std::string SHA3_256(std::string s);
	static std::string SHA3_384(std::string s);
	static std::string SHA3_512(std::string s);
};
