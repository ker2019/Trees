#ifndef PROFILER_HPP
#define PROFILER_HPP

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
	vector<pair<int, double>> deletionStats;
	vector<pair<int, double>> accessStats;
	random_device rnd;

	void measureTime(vector<Tree> &trees, int size, int min, int max, int *buffer) {
		// Fill the buffer by random numbers in advance
		for (int i = 0; i < trees.size(); i++)
			buffer[i] = min + rnd() % (max - min);

		double start, stop;
		start = getCPUTime();
		int i = 0;
		for (auto it = trees.begin(); it != trees.end(); it++, i++)
			it->insert(buffer[i]);
		stop = getCPUTime();
		if (start < 0 || stop < 0)
			throw 1;
		insertionStats.push_back({size, (stop - start)/trees.size()});

		start = getCPUTime();
		for (auto it = trees.begin(); it != trees.end(); it++)
			it->contains(buffer[i]);
		stop = getCPUTime();
		if (start < 0 || stop < 0)
			throw 1;
		accessStats.push_back({size, (stop - start)/trees.size()});

		start = getCPUTime();
		for (auto it = trees.begin(); it != trees.end(); it++)
			it->erase(buffer[i]);
		stop = getCPUTime();
		if (start < 0 || stop < 0)
			throw 1;
		deletionStats.push_back({size, (stop - start)/trees.size()});
	}

	void toSize(vector<Tree> &trees, int size, int min, int max) {
		for (auto tree: trees) {
			while (tree.size() < size)
				tree.insert(min + rnd() % (max - min));
			while (tree.size() > size)
				tree.erase(min + rnd() % (max - min));
		}
	}

public:
	void measure(int max_size, int cicles = 1000) {
		vector<Tree> trees(cicles);
		int *buffer = new int[cicles];
		int size = 1;
		while (size < max_size) {
			toSize(trees, size, 0, 2*size);
			measureTime(trees, size, 0, 2*size, buffer);
			size++;
		}
		delete[] buffer;
	}

	void saveStats(const string &filename) const {
		fstream f(filename, f.out);
		f << "n\tinsertion\taccess\tdeletion\n";
		if (!f.is_open())
			throw 1;
		for (auto it: insertionStats)
			f << it.first << '\t' << it.second << '\n';
		for (auto it: accessStats)
			f << it.first << '\t' << '\t' << it.second << '\n';
		for (auto it: deletionStats)
			f << it.first << '\t' << '\t' << '\t' << it.second << '\n';
		f.close();
	}
};

#endif /* PROFILER_HPP */
