#include <iostream>

#include "AVLtree.hpp"

using std::cin;
using std::cout;

int main() {
	AVLtree<int> K;
	while (1) {
		K.print();
		char ch;
		int n;
		cin >> ch;
		switch (ch) {
		case 'a':
			cin >> n;
			K.insert(n);
			break;
		case 'e':
			cin >> n;
			cout << (K.contains(n) ? "YES\n" : "NO\n");
			break;
		case 'q':
			return 0;
		}
	}
	return 0;
}
