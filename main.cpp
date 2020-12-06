#include <iostream>
#include <getopt.h>
#include <filesystem>
#include <string>
#include <random>

#include "TreeBase.hpp"
#include "AVLtree.hpp"
#include "RBtree.hpp"
#include "Profiler.hpp"

using std::cin;
using std::cout;
using std::cerr;
using std::filesystem::create_directory;
using std::filesystem::exists;
using std::string;
using std::random_device;
using std::stoi;

template< class Tree >
void game() {
	Tree K;
	while (1) {
		char ch;
		typename Tree::key_type k;
		cin >> ch;
		switch (ch) {
		case 'a':
			cin >> k;
			K.insert(k);
			K.print();
			break;
		case 'e':
			cin >> k;
			cout << (K.contains(k) ? "YES\n" : "NO\n");
			break;
		case 'd':
			cin >> k;
			K.erase(k);
			K.print();
			break;
		case 'q':
			return;
		}
	}
}

static const int max_str_len = 10;

struct getRandomString {
	string operator()() {
		random_device rnd;
		int len = rnd() % max_str_len;
		char str[max_str_len + 1];
		for (int i = 0; i < len; i++)
			str[i] = (char)rnd();
		str[len] = 0;
		return str;
	}
};

static const int general_failure_err = 2;
static const int incorrect_usage_err = 1;

void usage() {
	cerr << "Usage: tree [--string] [--game avl|rb]\n";
	exit(incorrect_usage_err);
}

int main(int argc, char *argv[]) {
	int opt_index = -1;
	int is_game = 0, use_str = 0;
	string tree_type;
	int max_size = 1000000;

	option options[] = {{"game", required_argument, &is_game, 1}, {"string", no_argument, &use_str, 1}};
	char ch;
	while ((ch = getopt_long(argc, argv, "n:", options, &opt_index)) != -1) {
		if (opt_index == 0)
			tree_type = optarg;
		if (ch == 'n')
			max_size = stoi(optarg);
	}

	if (is_game) {
		if (tree_type == "avl") {
			if (use_str)
				game<AVLtree<string>>();
			else
				game<AVLtree<int>>();
		}
		else if (tree_type == "rb") {
			if (use_str)
				game<RBtree<string>>();
			else
				game<RBtree<int>>();
		}
		else
			usage();
		return 0;
	}

	if (!exists("out"))
		if (!create_directory("out")) {
			cerr << "Cannot make directory: out; Aborting\n";
			return general_failure_err;
		}

	if (use_str) {
		Profiler<AVLtree<string>, getRandomString> ap;
		ap.measure(max_size);
		ap.saveStats("out/avl.tsv");
		Profiler<RBtree<string>, getRandomString> rp;
		rp.measure(max_size);
		rp.saveStats("out/rb.tsv");
	}
	else {
		Profiler<AVLtree<int>> ap;
		ap.measure(max_size);
		ap.saveStats("out/avl.tsv");
		Profiler<RBtree<int>> rp;
		rp.measure(max_size);
		rp.saveStats("out/rb.tsv");
	}
	return 0;
}
