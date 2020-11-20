#ifndef RBTREE_HPP
#define RBTREE_HPP

#include <functional>
#include <utility>
#include <iostream>
#include <queue>

#include "Node.hpp"

using std::pair;

template< class Key_t, class Compare_t = std::less<Key_t> >
class RBtree {
private:
	enum color_t {red, black};
	Node<pair<Key_t, color_t>> *root = nullptr;
	Compare_t comp;

	void fix(Node<pair<Key_t, color_t>> *node) {
		if (node->data.second == black)
			return;

		// Root
		Node<pair<Key_t, color_t>> *father = node->getParent();
		if (father == nullptr) {
			node->data.second = black;
			return;
		}

		if (father->data.second == black)
			return;

		Node<pair<Key_t, color_t>> *grandpa = father->getParent();
		if (father->isLeft()) {
			Node<pair<Key_t, color_t>> *uncle = grandpa->getRight();
			auto uncle_color = (uncle == nullptr ? black : uncle->data.second);
			if (uncle_color == red) {
				father->data.second = black;
				uncle->data.second = black;
				grandpa->data.second = red;
				fix(grandpa);
				return;
			}

			if (node->isRight()) {
				father->rotateLeft();
				grandpa->rotateRight();
				node->data.second = black;
				grandpa->data.second = red;
				return;
			}

			if (node->isLeft()) {
				grandpa->rotateRight();
				father->data.second = black;
				grandpa->data.second = red;
			}
		}
		else if (father->isRight()) {
			Node<pair<Key_t, color_t>> *uncle = grandpa->getLeft();
			auto uncle_color = (uncle == nullptr ? black : uncle->data.second);
			if (uncle_color == red) {
				father->data.second = black;
				uncle->data.second = black;
				grandpa->data.second = red;
				fix(grandpa);
				return;
			}

			if (node->isLeft()) {
				father->rotateRight();
				grandpa->rotateLeft();
				node->data.second = black;
				grandpa->data.second = red;
				return;
			}

			if (node->isRight()) {
				grandpa->rotateLeft();
				father->data.second = black;
				grandpa->data.second = red;
			}
		}
		else
			throw 1;
	}

public:
	RBtree() {
		comp = Compare_t();
	}
	RBtree(const Compare_t &comp) {
		this->comp = comp;
	}
	~RBtree() {
		if (root != nullptr)
			delete root;
	}

	void insert(const Key_t &key) {
		if (root == nullptr) {
			Node<pair<Key_t, color_t>>::createRoot({key, black}, &root);
			fix(root);
			return;
		}
		Node<pair<Key_t, color_t>> *node = root;
		while (1) {
			if (comp(key, node->data.first)) {
				if (node->getLeft() == nullptr) {
					node->createLeft({key, red});
					fix(node->getLeft());
					return;
				}
				node = node->getLeft();
			}
			else if (comp(node->data.first, key)) {
				if (node->getRight() == nullptr) {
					node->createRight({key, red});
					fix(node->getRight());
					return;
				}
				node = node->getRight();
			}
			else
				return; // This key already exists
		}
	}

	bool contains(const Key_t &key) const {
		if (root == nullptr)
			return false;
		Node<pair<Key_t, color_t>> *node = root;
		while (1) {
			if (comp(key, node->data.first)) {
				if (node->getLeft() == nullptr)
					return false;
				node = node->getLeft();
			}
			else if (comp(node->data.first, key)) {
				if (node->getRight() == nullptr)
					return false;
				node = node->getRight();
			}
			else
				return true;
		}
	}

	void print() const {
		if (root == nullptr)
			return;
		std::queue<Node<pair<Key_t, color_t>>*> q;

		int width = (1 << root->getHeight()) - 1;
		q.push(root);
		int pos_num = 1;
		for (int j = 0; j < root->getHeight(); j++, pos_num = 2 * pos_num, width=width/2) {
			for (int k = 0; k < width/2; k++)
				std::cout << ' ';
			for (int i = 0; i < pos_num; i++) {
				if (i > 0)
					for (int k = 0; k < width; k++)
						std::cout << ' ';
				Node<pair<Key_t, color_t>> *node = q.front();
				q.pop();
				if (node == nullptr) {
					std::cout << ' ';
					q.push(nullptr);
					q.push(nullptr);
				}
				else {
					if (node->data.second == red)
						std::cout << "\x1b[31m" <<  node->data.first << "\x1b[0m";
					else
						std::cout << node->data.first;
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

#endif /* RBTREE_HPP */
