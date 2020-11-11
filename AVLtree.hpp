#include <functional>
#include <iostream>
#include <queue>

#ifndef AVLTREE_HPP
#define AVLTREE_HPP

template< class Key_t, class Compare_t = std::less<Key_t> >
class AVLtree {
private:
	struct Node {
		Key_t key;
		Node *left;
		Node *right;
		Node *parent;
		int height = 1;
		Node(const Key_t &key, Node *parent) {
			this->key = key;
			left = right = nullptr;
			this->parent = parent;
		}

		Node(const Key_t &key) {
			this->key = key;
			left = right = nullptr;
			this->parent = nullptr;
		}

		~Node() {
			if (left != nullptr)
				delete left;
			if (right != nullptr)
				delete right;
		}

		void updateHeight() {
			if (left == nullptr) {
				if (right == nullptr)
					height = 1;
				else
					height = right->height + 1;
			}
			else {
				if (right == nullptr)
					height = left->height + 1;
				else
					height = left->height > right->height ? left->height + 1 : right->height + 1;
			}
		}

		Node **getBindingPoint() {
			if (parent == nullptr)
				return nullptr; // Binding point is root
			if (this == parent->left)
				return &parent->left;
			if (this == parent->right)
				return &parent->right;
			throw 1;
		}
	};

	Node *root;
	Compare_t comp;

	static void rotateRight(Node **node) {
		Node *left = (*node)->left;
		if (left == nullptr)
			throw 1;
		(*node)->left = left->right;
		if (left->right != nullptr)
			left->right->parent = *node;

		left->right = *node;
		left->parent = (*node)->parent;
		(*node)->parent = left;
		*node = left;

		(*node)->right->updateHeight();
		(*node)->updateHeight();
	}

	static void rotateLeft(Node **node) {
		Node *right = (*node)->right;
		if (right == nullptr)
			throw 1;
		(*node)->right = right->left;
		if (right->left != nullptr)
			right->left->parent = *node;

		right->left = *node;
		right->parent = (*node)->parent;
		(*node)->parent = right;
		(*node) = right;

		(*node)->left->updateHeight();
		(*node)->updateHeight();
	}

	static void balance(Node **node) {
		int lh = ((*node)->left != nullptr ? (*node)->left->height : 0);
		int rh = ((*node)->right != nullptr ? (*node)->right->height : 0);
		if (lh - rh == 2) {
			int llh = ((*node)->left->left != nullptr ? (*node)->left->left->height : 0);
			int lrh = ((*node)->left->right != nullptr ? (*node)->left->right->height : 0);
			if (lrh > llh) {
				rotateLeft(&(*node)->left);
				rotateRight(node);
			}
			else
				rotateRight(node);
			return;
		}
		if (rh - lh == 2) {
			int rrh = ((*node)->right->right != nullptr ? (*node)->right->right->height : 0);
			int rlh = ((*node)->right->left != nullptr ? (*node)->right->left->height : 0);
			if (rlh > rrh) {
				rotateRight(&(*node)->right);
				rotateLeft(node);
			}
			else
				rotateLeft(node);
			return;
		}
	}

	void fix(Node *node) {
		for (Node *p = node; p != nullptr; p = p->parent)
			p->updateHeight();
		for (Node *p = node; p->parent != nullptr; p = p->parent) {
			Node **point = p->getBindingPoint();
			if (point == nullptr)
				point = &root;
			balance(point);
		}
	}

	static void swap(Node *n1, Node *n2) {
		Key_t tmp = n1->key;
		n1->key = n2->key;
		n2->key = tmp;
	}

	void removeLeaf(Node *n) {
			Node *parent = n->parent;
			Node **point = n->getBindingPoint();
			delete n;
			if (point != nullptr)
				*point = nullptr;
			else
				root = nullptr;
			if (parent != nullptr)
				fix(parent);
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
			delete root;
	}

	void insert(const Key_t &key) {
		if (root == nullptr) {
			root = new Node(key, nullptr);
			return;
		}
		Node *node = root;
		while (1) {
			if (comp(key, node->key)) {
				if (node->left == nullptr) {
					node->left = new Node(key, node);
					fix(node);
					return;
				}
				node = node->left;
			}
			else if (comp(node->key, key)) {
				if (node->right == nullptr) {
					node->right = new Node(key, node);
					fix(node);
					return;
				}
				node = node->right;
			}
			else
				return; // This key already exists
		}
	}

	bool contains(const Key_t &key) const {
		if (root == nullptr)
			return false;
		Node *node = root;
		while (1) {
			if (comp(key, node->key)) {
				if (node->left == nullptr)
					return false;
				node = node->left;
			}
			else if (comp(node->key, key)) {
				if (node->right == nullptr)
					return false;
				node = node->right;
			}
			else
				return true;
		}
	}

	void erase(const Key_t &key) {
		if (root == nullptr)
			return;
		Node *node = root;
		while (1) {
			if (comp(key, node->key)) {
				if (node->left == nullptr)
					return;
				node = node->left;
			}
			else if (comp(node->key, key)) {
				if (node->right == nullptr)
					return;
				node = node->right;
			}
			else {
				Node *n = node;
				while (1) {
					if (n->right != nullptr) {
						n = n->right;
						while (n->left != nullptr)
							n = n->left;
						swap(node, n);
						node = n;
					}
					else if (n->left != nullptr) {
						n = n->left;
						while (n->right != nullptr)
							n = n->right;
						swap(node, n);
						node = n;
					}
					else
						break;
				}
				removeLeaf(n);
				return;
			}
		}
	}

	void print() const {
		if (root == nullptr)
			return;
		std::queue<Node*> q;

		int width = (1 << root->height) - 1;
		q.push(root);
		int pos_num = 1;
		for (int j = 0; j < root->height; j++, pos_num = 2 * pos_num, width = width/2) {
			for (int k = 0; k < width/2; k++)
				std::cout << ' ';
			for (int i = 0; i < pos_num; i++) {
				if (i > 0)
					for (int k = 0; k < width; k++)
						std::cout << ' ';
				Node *node = q.front();
				q.pop();
				if (node == nullptr) {
					std::cout << ' ';
					q.push(nullptr);
					q.push(nullptr);
				}
				else {
					std::cout << node->key;
					q.push(node->left);
					q.push(node->right);
				}
			}
			for (int k = 0; k < width/2; k++)
				std::cout << ' ';
			std::cout << '\n';
		}
	}
};

#endif /* AVLTREE_HPP */
