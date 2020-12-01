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

	Node<pair<Key_t, color_t>> *find(const Key_t &key) const {
		if (root == nullptr)
			return nullptr;
		Node<pair<Key_t, color_t>> *node = root;
		while (1) {
			if (comp(key, node->data.first)) {
				if (node->getLeft() == nullptr)
					return nullptr;
				node = node->getLeft();
			}
			else if (comp(node->data.first, key)) {
				if (node->getRight() == nullptr)
					return nullptr;
				node = node->getRight();
			}
			else
				return node;
		}
	}

	static color_t getColor(const Node<pair<Key_t, color_t>> *node) {
		if (node == nullptr)
			return black;
		return node->data.second;
	}

	static void fixInsertion(Node<pair<Key_t, color_t>> *node) {
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
				fixInsertion(grandpa);
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
				fixInsertion(grandpa);
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

	// Fixes the situation that in the left there is less by one black nodes
	static void fixLeftDeficite(Node<pair<Key_t, color_t>> *node) {
		auto child = node->getLeft();
		if (getColor(child) == red) {
			child->data.second = black;
			return;
		}

		auto sibling = node->getRight();
		if (getColor(sibling) == red) {
			std::swap(node->data.second, sibling->data.second);
			node->rotateLeft();
			fixLeftDeficite(node);
			return;
		}

		// The sibling cannot be leaf beacuse there is left deficite of black nodes
		if (getColor(sibling->getLeft()) == black && getColor(sibling->getRight()) == black) {
			if (getColor(node) == black) {
				sibling->data.second = red;
				if (node->isLeft())
					fixLeftDeficite(node->getParent());
				else if (node->isRight())
					fixRightDeficite(node->getParent());
			}
			else
				std::swap(node->data.second, sibling->data.second);
		}
		else {
			if (getColor(sibling->getRight()) == black) {
				sibling->rotateRight();
				std::swap(sibling->data.second, sibling->getParent()->data.second);
				sibling = sibling->getParent();
			}
			node->rotateLeft();
			std::swap(node->data.second, sibling->data.second);
			sibling->getRight()->data.second = black;
		}
	}

	// Fixes the situation that in the right there is less by one black nodes
	static void fixRightDeficite(Node<pair<Key_t, color_t>> *node) {
		auto child = node->getRight();
		if (getColor(child) == red) {
			child->data.second = black;
			return;
		}

		auto sibling = node->getLeft();
		if (getColor(sibling) == red) {
			std::swap(node->data.second, sibling->data.second);
			node->rotateRight();
			fixRightDeficite(node);
			return;
		}

		// The sibling cannot be leaf beacuse there is left deficite of black nodes
		if (getColor(sibling->getRight()) == black && getColor(sibling->getLeft()) == black) {
			if (getColor(node) == black) {
				sibling->data.second = red;
				if (node->isRight())
					fixRightDeficite(node->getParent());
				else if (node->isLeft())
					fixLeftDeficite(node->getParent());
			}
			else
				std::swap(node->data.second, sibling->data.second);
		}
		else {
			if (getColor(sibling->getLeft()) == black) {
				sibling->rotateLeft();
				std::swap(sibling->data.second, sibling->getParent()->data.second);
				sibling = sibling->getParent();
			}
			node->rotateRight();
			std::swap(node->data.second, sibling->data.second);
			sibling->getLeft()->data.second = black;
		}
	}

	static int check_rec(const Node<pair<Key_t, color_t>> *node) {
		if (node == nullptr)
			return 1;

		Node<pair<Key_t, color_t>> *left = node->getLeft(), *right = node->getRight();
		if (node->data.second == red)
			if (getColor(left) == red || getColor(right) == red)
				throw "Tree is incorrect!";

		int left_black_num = check_rec(left);
		int right_black_num = check_rec(right);
		if (left_black_num != right_black_num)
			throw "Tree is incorrect!";

		if (node->data.second == black)
			return left_black_num + 1;
		else
			return left_black_num;
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
			root->remove();
	}

	void insert(const Key_t &key) {
		if (root == nullptr) {
			Node<pair<Key_t, color_t>>::createRoot({key, black}, &root);
			return;
		}
		Node<pair<Key_t, color_t>> *node = root;
		while (1) {
			if (comp(key, node->data.first)) {
				if (node->getLeft() == nullptr) {
					node->createLeft({key, red});
					fixInsertion(node->getLeft());
					return;
				}
				node = node->getLeft();
			}
			else if (comp(node->data.first, key)) {
				if (node->getRight() == nullptr) {
					node->createRight({key, red});
					fixInsertion(node->getRight());
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
		Node<pair<Key_t, color_t>> *node = find(key);
		if (node == nullptr)
			return;

		Node<pair<key_t, color_t>> *n = node;
		if (n->getLeft() != nullptr && n->getRight() != nullptr) {
			n = n->getRight();
			while (n->getLeft() != nullptr)
				n = n->getLeft();
			std::swap(node->data.first, n->data.first);
			node = n;
		}

		/*
		It isn't posssible it to have
		a black child, because the number of black nodes in
		any path must be equal.
		*/

		if (node->data.second == red) {
			node->remove();
			return;
		}

		Node<pair<Key_t, color_t>> *father = node->getParent();

		Node<pair<Key_t, color_t>> *child = (node->getLeft() == nullptr ? node->getRight() : node->getLeft());
		// Replace the node by its child
		if (child != nullptr) {
			if (node->isLeft()) {
				child->bindToLeft(*father);
				fixLeftDeficite(father);
			}
			else if (node->isRight()) {
				child->bindToRight(*father);
				fixRightDeficite(father);
			}
			else {
				child->bindToRoot();
				child->data.second = black;
			}
		}
		else {
			if (node->isLeft()) {
				node->remove();
				fixLeftDeficite(father);
			}
			else if (node->isRight()) {
				node->remove();
				fixRightDeficite(father);
			}
			else
				node->remove();
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

	void check() const {
		if (root == nullptr)
			return;
		if (root->data.second == red)
			throw "Tree is incorrect!";
		check_rec(root);
	}
};

#endif /* RBTREE_HPP */
