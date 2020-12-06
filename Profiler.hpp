#ifndef PROFILER_HPP
#define PROFILER_HPP

#include <vector>
#include <utility>
#include <random>
#include <algorithm>
#include <string>
#include <fstream>

#include "getCPUTime.hpp"
#include "TreeBase.hpp"

using std::vector;
using std::pair;
using std::random_shuffle;
using std::string;
using std::fstream;

template< class Tree, class Generator = std::random_device >
class Profiler {
private:
	vector<pair<int, double>> insertionStats;
	vector<pair<int, double>> accessStats;
	vector<pair<int, double>> deletionStats;
	Generator rnd;
public:
	Profiler() : rnd() {}
	Profiler(const Generator &g) : rnd(g) {}

	void measure(int max_size, int cicles = 10000) {
		Tree tree;
		// Fill a buffer by random numbers in advance
		typename Tree::key_type *random_elems = new typename Tree::key_type[max_size + cicles];
		for (int i = 0; i < max_size + cicles; i++)
			random_elems[i] = rnd();

		int n = 0;
		while (tree.size() < max_size && n < max_size) {
			int start_size = tree.size();
			double start, stop;
			start = getCPUTime();

			for (int i = 0; i < cicles; i++)
				tree.insert(random_elems[n++]);

			stop = getCPUTime();
			int end_size = tree.size();
			if (start < 0 || stop < 0)
				throw 1;
			insertionStats.push_back(pair{(end_size + start_size)/2, (stop - start)/cicles});
		}

		random_shuffle(random_elems, random_elems + max_size + cicles);

		n = 0;
		while (tree.size() > 0 && n < max_size) {
			int start_size = tree.size();
			double start, stop;

			start = getCPUTime();

			for (int i = 0; i < cicles; i++)
				tree.contains(random_elems[n++]);

			stop = getCPUTime();
			int end_size = tree.size();
			if (start < 0 || stop < 0)
				throw 1;
			accessStats.push_back(pair{start_size, (stop - start)/cicles});

			n -= cicles;

			start = getCPUTime();

			for (int i = 0; i < cicles; i++)
				tree.erase(random_elems[n++]);

			stop = getCPUTime();
			end_size = tree.size();
			if (start < 0 || stop < 0)
				throw 1;
			deletionStats.push_back(pair{(end_size + start_size)/2, (stop - start)/cicles});
		}

		delete[] random_elems;
	}

	void saveStats(const string &filename) const {
		fstream f(filename, f.out);
		f << "size_ins\tinsertion\tsize_acc\taccess\tsize_del\tdeletion\n";
		if (!f.is_open())
			throw 1;
		auto i = insertionStats.cbegin(), j = accessStats.cbegin(), k = deletionStats.cbegin();
		bool stop = false;
		while (!stop)  {
			stop = true;
			if (i != insertionStats.cend())
				f << i->first << '\t' << i->second, stop = false, i++;
			if (j != accessStats.cend())
				f << '\t' << j->first << '\t' << j->second, stop = false, j++;
			if (k != deletionStats.cend())
				f << '\t' << k->first << '\t' << k->second, stop = false, k++;
			f << '\n';
		}
		f.close();
	}
};

#endif /* PROFILER_HPP */
