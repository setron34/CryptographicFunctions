#include <string>
#include <iostream>
#include <bitset>
#include <vector>

class SHA3 {
public:
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

	static std::vector<bool> trunc(const std::vector<bool>& v, int x) {
		std::vector<bool> ans(x);
		for (int i = 0; i < x; ++i) {
			ans[i] = v[i];
		}
		return ans;
	}

	static std::vector<bool> XOR(const std::vector<bool>& v1, const std::vector<bool>& v2) {
		std::vector<bool> ans(v1.size());
		for (int i = 0; i < ans.size(); i++) {
			ans[i] = v1[i] ^ v2[i];
		}
		return ans;
	}

	static std::vector<bool> conc(const std::vector<bool>& v1, const std::vector<bool>& v2) {
		std::vector<bool> ans(v1.size() + v2.size());
		for (int i = 0; i < v1.size(); i++) {
			ans[i] = v1[i];
		}
		for (int i = 0; i < v2.size(); i++) {
			ans[v1.size() + i] = v2[i];
		}
		return ans;
	}

	static void out(const std::vector<bool>& v) {
		char* dataToHex = new char[16]{ '0', '1' , '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
		int cnt = 0;
		for (int i = 0; i < v.size()/ 8; ++i) {
			std::cout << dataToHex[v[i * 8 + 7] * 8 + v[i * 8 + 6] * 4 + v[i * 8 + 5] * 2 + v[i * 8 + 4]] << dataToHex[v[i * 8 + 3] * 8 + v[i * 8 + 2] * 4 + v[i * 8 + 1] * 2 + v[i * 8 + 0]] << " ";
			cnt++;
			if (cnt == 16) {
				std::cout << '\n';
				cnt = 0;
			}
		}
	}

	static stArray* toStArray(const std::vector<bool>& S) {
		size_t w = S.size() / 25;
		stArray *A = new stArray(w);
		for (size_t x = 0; x < 5; ++x) {
			for (size_t y = 0; y < 5; ++y) {
				for (size_t z = 0; z < w; ++z) {
					(*A)[x][y][z] = S[w * (5 * y + x) + z];
				}
			}
		}
		return A;
	}

	static std::vector<bool> toBitString(stArray *A) {
		size_t b = (*A)[0][0].size() * 25;
		size_t w = (*A)[0][0].size();
		std::vector<bool> S(b);
		for (size_t x = 0; x < 5; ++x) {
			for (size_t y = 0; y < 5; ++y) {
				for (size_t z = 0; z < w; ++z) {
					S[w * (5 * y + x) + z] = (*A)[x][y][z];
				}
			}
		}
		return S;
	}

	static stArray* theta(stArray* A) {
		size_t w = (*A)[0][0].size();
		std::vector<std::vector<bool> > C(5, std::vector<bool>(w));
		for (size_t x = 0; x < 5; ++x) {
			for (size_t z = 0; z < w; ++z) {
				C[x][z] = (*A)[x][0][z] ^ (*A)[x][1][z] ^ (*A)[x][2][z] ^ (*A)[x][3][z] ^ (*A)[x][4][z];
			}
		}

		std::vector<std::vector<bool> > D(5, std::vector<bool>(w));
		for (size_t x = 0; x < 5; ++x) {
			for (size_t z = 0; z < w; ++z) {
				D[x][z] = C[(5 + x - 1) % 5][z] ^ C[(5 + x + 1) % 5][(w + z - 1) % w];
			}
		}

		stArray* newA = new stArray(w);
		for (size_t x = 0; x < 5; ++x) {
			for (size_t y = 0; y < 5; ++y) {
				for (size_t z = 0; z < w; ++z) {
					(*newA)[x][y][z] = (*A)[x][y][z] ^ D[x][z];
				}
			}
		}
		return newA;
	}

	static stArray* rho(stArray* A) {
		size_t w = (*A)[0][0].size();
		stArray* newA = new stArray(w);
		for (size_t z = 0; z < w; ++z) {
			(*newA)[0][0][z] = (*A)[0][0][z];
		}
		int x = 1;
		int y = 0;
		for (int t = 0; t < 24; ++t) {
			for (int z = 0; z < w; ++z) {
				(*newA)[x][y][z] = (*A)[x][y][(z - (t + 1) * (t + 2) / 2) % w];
			}
			int newx = y;
			int newy = (2 * x + 3 * y) % 5;
			x = newx;
			y = newy;
		}
		return newA;
	}

	static stArray* pi(stArray* A) {
		size_t w = (*A)[0][0].size();
		stArray* newA = new stArray(w);
		for (size_t x = 0; x < 5; ++x) {
			for (size_t y = 0; y < 5; ++y) {
				for (size_t z = 0; z < w; ++z) {
					(*newA)[x][y][z] = (*A)[(x + 3 * y) % 5][x][z];
				}
			}
		}
		return newA;
	}

	static stArray* chi(stArray* A) {
		size_t w = (*A)[0][0].size();
		stArray* newA = new stArray(w);
		for (size_t x = 0; x < 5; ++x) {
			for (size_t y = 0; y < 5; ++y) {
				for (size_t z = 0; z < w; ++z) {
					(*newA)[x][y][z] = (*A)[x][y][z] ^ (((*A)[(x + 1) % 5][y][z] ^ 1) & (*A)[(x + 2) % 5][y][z]);
				}
			}
		}
		return newA;
	}

	static bool rc(int t) {
		t = t % 255;
		if (t == 0) {
			return true;
		}
		std::vector<bool> R(8, false);
		R[0] = true;
		for (int i = 1; i <= t; ++i) {
			R = conc(std::vector<bool>{ 0 }, R);
			R[0] = R[0] ^ R[8];
			R[4] = R[4] ^ R[8];
			R[5] = R[5] ^ R[8];
			R[6] = R[6] ^ R[8];
			R = trunc(R, 8);
		}
		return R[0];
	}

	static stArray* iota(stArray* A, int ir) {
		size_t w = (*A)[0][0].size();
		stArray* newA = new stArray(w);
		for (size_t x = 0; x < 5; ++x) {
			for (size_t y = 0; y < 5; ++y) {
				for (size_t z = 0; z < w; ++z) {
					(*newA)[x][y][z] = (*A)[x][y][z];

				}
			}
		}
		std::vector<bool> RC(w, 0);
		int el = log2(w);
		for (int j = 0; j <= el; ++j) {
			RC[pow(2, j) - 1] = rc(j + 7 * ir);
		}
		for (size_t z = 0; z < w; ++z) {
			(*newA)[0][0][z] = (*A)[0][0][z] ^ RC[z];

		}
		return newA;
	}

	static stArray* Rnd(stArray* A, int ir) { 
		return iota(chi(pi(rho(theta(A)))), ir);
	}

	static std::vector<bool> keccak_p(const std::vector<bool>& S, int nr) {
		stArray* A = toStArray(S);
		int w = S.size() / 25;
		int el = log2(w);
		for (int ir = 12 + 2 * el - nr; ir < 12 + 2 * el; ++ir) {
			A = Rnd(A, ir);
		}
		std::vector<bool> newS = toBitString(A);
		return newS;
	}

	static std::vector<bool> keccak_f(const std::vector<bool>& S) {
		int w = S.size() / 25;
		int el = log2(w);
		int nr = 12 + 2 * el;
		return keccak_p(S, nr);
	}

	static std::vector<bool> pad10etc1(int x, int m) {
		int j = x - (m + 2) % x;
		std::vector<bool> pad(j + 2, 0);
		pad[0] = 1;
		pad[j + 1] = 1;
		return pad;
	}

	static std::vector<bool> sponge(std::vector<bool>(*f)(const std::vector<bool>&), std::vector<bool>(*pad)(int, int), int r, std::vector<bool> P, int d) {
		std::vector<bool> padVec = (*pad)(r, P.size());
		P = conc(P, padVec);
		int n = P.size() / r;
		int b = 1600;
		int c = b - r;
		std::vector<std::vector<bool> > PP(n, std::vector<bool>(b, 0));
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < r; ++j) {
				PP[i][j] = P[i * r + j];
			}
		}
		std::vector<bool> S(b, 0);
		for (int i = 0; i < n; ++i) {
			S = (*f)(XOR(S, PP[i]));
		}
		std::vector<bool> Z;
		while (true) {
			Z = conc(Z,trunc(S, r));
			if (Z.size() >= d) {
				break;
			}
			S = (*f)(S);
		}
		return trunc(Z, d);
	}

	static std::vector<bool> keccak(int c, std::vector<bool> N, int d) {
		return sponge(&keccak_f, &pad10etc1, 1600 - c, N, d);
	}

	static std::string toString(const std::vector<bool>& v) {
		std::string s(v.size() / 4, ' ');
		std::string dataToHex("0123456789abcdef");
		for (int i = 0; i < v.size() / 8; ++i) {
			s[i * 2 + 0] = dataToHex[v[i * 8 + 7] * 8 + v[i * 8 + 6] * 4 + v[i * 8 + 5] * 2 + v[i * 8 + 4]];
			s[i * 2 + 1] = dataToHex[v[i * 8 + 3] * 8 + v[i * 8 + 2] * 4 + v[i * 8 + 1] * 2 + v[i * 8 + 0]];
		}
		return s;
	}

	static std::vector<bool> toBitString(std::string s) {
		std::vector<bool> v(s.size() * 8);
		for (int i = 0; i < s.size(); ++i) {
			v[i * 8 + 7] = s[i] / 128 % 2;
			v[i * 8 + 6] = s[i] / 64  % 2;
			v[i * 8 + 5] = s[i] / 32  % 2;
			v[i * 8 + 4] = s[i] / 16  % 2;
			v[i * 8 + 3] = s[i] / 8   % 2;
			v[i * 8 + 2] = s[i] / 4   % 2;
			v[i * 8 + 1] = s[i] / 2   % 2;
			v[i * 8 + 0] = s[i] / 1   % 2;
		}
		return v;
	}
	static std::string SHA3_224(std::string s) {
		return toString(keccak(448, conc(toBitString(s), std::vector<bool>{0, 1}), 224));
	}

	static std::string SHA3_256(std::string s) {
		return toString(keccak(512, conc(toBitString(s), std::vector<bool>{0, 1}), 256));
	}

	static std::string SHA3_384(std::string s) {
		return toString(keccak(768, conc(toBitString(s), std::vector<bool>{0, 1}), 384));
	}

	static std::string SHA3_512(std::string s) {
		return toString(keccak(1024, conc(toBitString(s), std::vector<bool>{0, 1}), 512));
	}
};