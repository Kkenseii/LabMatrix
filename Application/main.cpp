#include "tvector.h"
#include <iostream>
#include <vector>

int main() {
	std::vector<int> vec(5);
	TVector<int> my_vec(5);

	int val = 1;

	std::vector<int>::iterator it;
	for (it = vec.begin(); it != vec.end(); it++) {
		*it = val++;
	}

	for (auto it = vec.begin(); it != vec.end(); it++) {
		std::cout << *it << " ";
	}

	std::cout << std::endl;

	val = 1;

	for (int i = 0; i < 5; i++) {
		my_vec[i] = val++;
	}

	for (int i = 0; i < 5; i++) {
		std::cout << my_vec[i] << " ";
	}

	return 0;
}

