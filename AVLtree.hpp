#ifndef AVLTREE_HPP
#define AVLTREE_HPP

#include <functional>
#include <utility>
#include <iostream>
#include <queue>

#include "TreeBase.hpp"
#include "Node.hpp"

template< class Key_t, class Compare_t = std::less<Key_t> >
class AVLtree : public TreeBase<Key_t, Compare_t> {
private:
	Node<Key_t> *root;
	Compare_t comp;
	int elems_num = 0;

	Node<Key_t> *find(const Key_t &key) const {
		if (root == nullptr)
			return nullptr;
		Node<Key_t> *node = root;
		while (1) {
			if (comp(key, node->data)) {
				if (node->getLeft() == nullptr)
					return nullptr;
				node = node->getLeft();
			}
			else if (comp(node->data, key)) {
				if (node->getRight() == nullptr)
					return nullptr;
				node = node->getRight();
			}
			else
				return node;
		}
	}

	static void balance(Node<Key_t> *node) {
		int lh = (node->getLeft() != nullptr ? node->getLeft()->getHeight() : 0);
		int rh = (node->getRight() != nullptr ? node->getRight()->getHeight() : 0);
		if (lh - rh == 2) {
			int llh = (node->getLeft()->getLeft() != nullptr ? node->getLeft()->getLeft()->getHeight() : 0);
			int lrh = (node->getLeft()->getRight() != nullptr ? node->getLeft()->getRight()->getHeight() : 0);
			if (lrh > llh) {
				node->getLeft()->rotateLeft();
				node->rotateRight();
			}
			else
				node->rotateRight();
			return;
		}
		if (rh - lh == 2) {
			int rrh = (node->getRight()->getRight() != nullptr ? node->getRight()->getRight()->getHeight() : 0);
			int rlh = (node->getRight()->getLeft() != nullptr ? node->getRight()->getLeft()->getHeight() : 0);
			if (rlh > rrh) {
				node->getRight()->rotateRight();
				node->rotateLeft();
			}
			else
				node->rotateLeft();
			return;
		}
	}

	void fix(Node<Key_t> *node) {
		for (Node<Key_t> *p = node; p->getParent() != nullptr; p = p->getParent())
			balance(p);
		balance(root);
	}

public:
	AVLtree() {
		root = nullptr;
		comp = Compare_t();
	}

	AVLtree(const Compare_t &comp) {
		root = nullptr;
		this->comp = comp;
	}

	~AVLtree() {
		if (root != nullptr)
			root->remove();
	}

	void insert(const Key_t &key) {
		if (root == nullptr) {
			Node<Key_t>::createRoot(key, &root);
			elems_num++;
			return;
		}
		Node<Key_t> *node = root;
		while (1) {
			if (comp(key, node->data)) {
				if (node->getLeft() == nullptr) {
					node->createLeft(key);
					elems_num++;
					fix(node);
					return;
				}
				node = node->getLeft();
			}
			else if (comp(node->data, key)) {
				if (node->getRight() == nullptr) {
					node->createRight(key);
					elems_num++;
					fix(node);
					return;
				}
				node = node->getRight();
			}
			else
				return; // This key already exists
		}
	}

	bool contains(const Key_t &key) const {
		return !(find(key) == nullptr);
	}

	void erase(const Key_t &key) {
		Node<Key_t> *node = find(key);
		if (node == nullptr)
			return;

		Node<Key_t> *n = node;
		while (1) {
				if (n->getRight() != nullptr) {
					n = n->getRight();
					while (n->getLeft() != nullptr)
						n = n->getLeft();
					std::swap(n->data, node->data);
					node = n;
				}
				else if (n->getLeft() != nullptr) {
					n = n->getLeft();
					while (n->getRight() != nullptr)
						n = n->getRight();
					std::swap(n->data, node->data);
					node = n;
				}
				else
					break;
		}
		Node<Key_t> *p = n->getParent();
		n->remove();
		elems_num--;
		if (p != nullptr)
			fix(p);
	}

	int size() const {
		return elems_num;
	}

	void print() const {
		if (root == nullptr)
			return;
		std::queue<Node<Key_t>*> q;

		int width = (1 << root->getHeight()) - 1;
		q.push(root);
		int pos_num = 1;
		for (int j = 0; j < root->getHeight(); j++, pos_num = 2 * pos_num, width = width/2) {
			for (int k = 0; k < width/2; k++)
				std::cout << ' ';
			for (int i = 0; i < pos_num; i++) {
				if (i > 0)
					for (int k = 0; k < width; k++)
						std::cout << ' ';
				Node<Key_t> *node = q.front();
				q.pop();
				if (node == nullptr) {
					std::cout << ' ';
					q.push(nullptr);
					q.push(nullptr);
				}
				else {
					std::cout << node->data;
					q.push(node->getLeft());
					q.push(node->getRight());
				}
			}
			for (int k = 0; k < width/2; k++)
				std::cout << ' ';
			std::cout << '\n';
		}
	}
};

#endif /* AVLTREE_HPP */
