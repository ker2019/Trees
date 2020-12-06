#!/usr/bin/env python3

import matplotlib
matplotlib.use('GTK3Cairo')

import matplotlib.pyplot as plt
import pandas as pd
import sys

avl_file = 'out/avl.tsv'
rb_file = 'out/rb.tsv'

avl = pd.read_csv(avl_file, sep='\t')
rb = pd.read_csv(rb_file, sep='\t')

for tree, name in zip([avl, rb], ['avl', 'rb']):
	fig = plt.figure()
	ax = fig.add_subplot(1, 1, 1)

	ax.plot(tree['size_ins']/10**4, tree['insertion']*10**6, label='insertion')
	ax.plot(tree['size_acc']/10**4, tree['access']*10**6, label='access')
	ax.plot(tree['size_del']/10**4, tree['deletion']*10**6, label='deletion')
	ax.set_xlabel('$n, \ 10^4$')
	ax.set_ylabel('$time, \ ms$', y=1, rotation=0)
	ax.legend()

	fig.savefig('out/' + name + '.png', format='png')

for method, size_name in zip(['insertion', 'access', 'deletion'], ['size_ins', 'size_acc', 'size_del']):
	fig = plt.figure()
	ax = fig.add_subplot(1, 1, 1)

	for tree, name in zip([avl, rb], ['avl', 'rb']):
		ax.plot(tree[size_name]/10**4, tree[method]*10**6, label=name)
	ax.set_xlabel('$n, \ 10^4$')
	ax.set_ylabel('$time, \ ms$', y=1, rotation=0)
	ax.legend()

	fig.savefig('out/' + method + '.png', format='png')
