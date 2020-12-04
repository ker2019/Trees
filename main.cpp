#include <iostream>
#include <getopt.h>
#include <filesystem>

#include "TreeBase.hpp"
#include "AVLtree.hpp"
#include "RBtree.hpp"
#include "Profiler.hpp"

using std::cin;
using std::cout;
using std::cerr;
using std::filesystem::create_directory;
using std::filesystem::exists;

void game(TreeBase<int> &K) {
	while (1) {
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
		case 'd':
			cin >> n;
			K.erase(n);
			break;
		case 'q':
			return;
		}
	}
}

int main(int argc, char *argv[]) {
	int opt_index = 0;
	struct option options[] = {{"avl", no_argument, 0, 0}, {"rb", no_argument, 0, 0}};
	if (getopt_long_only(argc, argv, "", options, &opt_index) == 0) {
		if (opt_index == 0) {
			AVLtree<int> K;
			game(K);
		}
		else if (opt_index == 1) {
			RBtree<int> K;
			K.size();
			game(K);
		}
		else
			throw 1;
	}
	else {
		if (!exists("out"))
			if (!create_directory("out")) {
				cerr << "Cannot make directory: out; Aborting\n";
				return 2;
			}
		Profiler<AVLtree<int>> ap;
		ap.measure(1000);
		ap.saveStats("out/avl.tsv");
		Profiler<RBtree<int>> rp;
		rp.measure(1000);
		rp.saveStats("out/rb.tsv");
	}
	return 0;
}
