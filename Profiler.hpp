#ifndef PROFILER_HPP
#define PROFILER_HPP

#include <iostream>
#include <vector>
#include <utility>
#include <random>
#include <string>
#include <fstream>

#include "getCPUTime.hpp"
#include "TreeBase.hpp"

using std::vector;
using std::pair;
using std::random_device;
using std::string;
using std::fstream;

template< class Tree >
class Profiler {
private:
	vector<pair<int, double>> insertionStats;
	vector<pair<int, double>> accessStats;
	vector<pair<int, double>> deletionStats;
	random_device rnd;

public:
	void measure(int max_size, int cicles = 10000) {
		Tree tree;
		// Fill a buffer by random numbers in advance
		int *random_elems = new int[2*max_size + cicles];
		for (int i = 0; i < 2*max_size + cicles; i++)
			random_elems[i] = rnd();

		int n = 0;
		while (tree.size() < max_size && n < 2*max_size) {
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

		n = 0;
		while (tree.size() > 0 && n < 2*max_size) {
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
				f << i->first << '\t' << i->second << '\t', stop = false, i++;
			if (j != accessStats.cend())
				f << j->first << '\t' << j->second << '\t', stop = false, j++;
			if (k != deletionStats.cend())
				f << k->first << '\t' << k->second << '\n', stop = false, k++;
		}
		f.close();
	}
};

#endif /* PROFILER_HPP */
