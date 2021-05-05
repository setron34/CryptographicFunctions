#pragma once
#include <vector>
#include <iostream>
using namespace std;

class BerlekampMassey {
private:
	vector<char> bits;
	vector<char> mask;
	
public:
	BerlekampMassey(vector<char> bits) {
		this->bits = bits;
		int i = 1;
		mask.push_back(1);
		int last_l = 0;
		vector<char> curbits;
		curbits.push_back(0);
		while (bits[i] == 0) {
			i++;
			mask.push_back(0);
			curbits.push_back(0);
		}
		int a = i - last_l;
		last_l = i + 1;
		mask.push_back(0);
		curbits.push_back(1);

		for (; i < bits.size(); ++i) {
			if (evaluateRegister(curbits, mask) != bits[i]) {


			}
		}

	}

	static char evaluateRegister(const vector<char>& curbits, const vector<char>& mask) {
		char ans = 0;
		for (int i = 0; i < curbits.size(); ++i) {
			ans = ans ^ (curbits[i] & mask[i]);
		}
		return ans;
	}

};