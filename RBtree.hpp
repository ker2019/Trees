#include <functional>
#include <iostream>
#include <queue>

#ifndef RBTREE_HPP
#define RBTREE_HPP

template< class Key_t, class Compare_t = std::less<Key_t> >
class RBtree {
private:
	class Node {
	private:
		Node *left = nullptr;
		Node *right = nullptr;
		Node *parent = nullptr;
		int height = 1;
		Node **root;

		Node **getBindingPoint() const {
			if (parent == nullptr)
				return root;

			if (this == parent->left)
				return &parent->left;
			else if (this == parent->right)
				return &parent->right;
			else
				throw 1;
		}

		Node(const Key_t &key) {
			this->key = key;
		}

	public:
		Key_t key;
		enum color_t {red, black};
		color_t color = red;

		static void createRoot(const Key_t &key, Node **root) {
			if (*root != nullptr)
				throw 1;

			Node *new_node = new Node(key);
			new_node->root = root;
			*root = new_node;
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

		void createLeft(const Key_t &key) {
			if (left != nullptr)
				throw 1;

			Node *new_node = new Node(key);
			new_node->root = root;
			new_node->parent = this;
			left = new_node;

			for (Node *p = this; p != nullptr; p = p->parent)
				p->updateHeight();
		}

		void createRight(const Key_t &key) {
			if (right != nullptr)
				throw 1;

			Node *new_node = new Node(key);
			new_node->root = root;
			new_node->parent = this;
			right = new_node;

			for (Node *p = this; p != nullptr; p = p->parent)
				p->updateHeight();
		}

		~Node() {
			if (left != nullptr)
				delete left;
			if (right != nullptr)
				delete right;
		}

		void remove() {
			*getBindingPoint() = nullptr;
			for (Node *p = parent; p != nullptr; p = p->parent)
				p->updateHeight();
			delete this;
		}

		Node *getLeft() const {
			return left;
		}

		Node *getRight() const {
			return right;
		}

		int getHeight() const {
			return height;
		}

		Node *getParent() const {
			return parent;
		}

		bool isLeft() const {
			// Root
			if (parent == nullptr)
				return false;

			if (this == parent->left)
				return true;
			return false;
		}

		bool isRight() const {
			// Root
			if (parent == nullptr)
				return false;

			if (this == parent->right)
				return true;
			return false;
		}

		bool isRoot() const {
			if (parent == nullptr)
				return true;
			return false;
		}

		void bindToLeft(Node &node) {
			// The place is taken!
			if (node.left != nullptr)
				throw 1;

			*getBindingPoint() = nullptr;
			parent = &node;
			node.left = this;
			for (Node *p = parent; p != nullptr; p = p->parent)
				p->updateHeight();
		}

		void bindToRight(Node &node) {
			// The place is taken!
			if (node.right != nullptr)
				throw 1;

			*getBindingPoint() = nullptr;
			parent = &node;
			node.right = this;
			for (Node *p = parent; p != nullptr; p = p->parent)
				p->updateHeight();
		}

		void bindToRoot() {
			// The place is taken!
			if (*root != nullptr)
				throw 1;

			*getBindingPoint() = nullptr;
			parent = nullptr;
			*root = this;
		}

		void rotateRight() {
			Node *left = this->left;
			if (left == nullptr)
				throw 1;

			Node *parent = this->parent;
			Node **p = this->getBindingPoint();

			this->left = left->right;
			if (left->right != nullptr)
				left->right->parent = this;

			left->right = this;
			this->parent = left;

			*p = left;
			left->parent = parent;

			this->updateHeight();
			left->updateHeight();
		}

		void rotateLeft() {
			Node *right = this->right;
			if (right == nullptr)
				throw 1;

			Node *parent = this->parent;
			Node **p = this->getBindingPoint();

			this->right = right->left;
			if (right->left != nullptr)
				right->left->parent = this;

			right->left = this;
			this->parent = right;

			*p = right;
			right->parent = parent;

			this->updateHeight();
			right->updateHeight();
		}
	};

	Node *root = nullptr;
	Compare_t comp;

	void fix(Node &node) {
		if (node.color == Node::black)
			return;

		// Root
		Node *father = node.getParent();
		if (father == nullptr) {
			node.color = Node::black;
			return;
		}

		if (father->color == Node::black)
			return;

		Node *grandpa = father->getParent();
		if (father->isLeft()) {
			Node *uncle = grandpa->getRight();
			auto uncle_color = (uncle == nullptr ? Node::black : uncle->color);
			if (uncle_color == Node::red) {
				father->color = Node::black;
				uncle->color = Node::black;
				grandpa->color = Node::red;
				fix(*grandpa);
				return;
			}

			if (node.isRight()) {
				father->rotateLeft();
				grandpa->rotateRight();
				node.color = Node::black;
				grandpa->color = Node::red;
				return;
			}

			if (node.isLeft()) {
				grandpa->rotateRight();
				father->color = Node::black;
				grandpa->color = Node::red;
			}
		}
		else if (father->isRight()) {
			Node *uncle = grandpa->getLeft();
			auto uncle_color = (uncle == nullptr ? Node::black : uncle->color);
			if (uncle_color == Node::red) {
				father->color = Node::black;
				uncle->color = Node::black;
				grandpa->color = Node::red;
				fix(*grandpa);
				return;
			}

			if (node.isLeft()) {
				father->rotateRight();
				grandpa->rotateLeft();
				node.color = Node::black;
				grandpa->color = Node::red;
				return;
			}

			if (node.isRight()) {
				grandpa->rotateLeft();
				father->color = Node::black;
				grandpa->color = Node::red;
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
			Node::createRoot(key, &root);
			fix(*root);
			return;
		}
		Node *node = root;
		while (1) {
			if (comp(key, node->key)) {
				if (node->getLeft() == nullptr) {
					node->createLeft(key);
					fix(*node->getLeft());
					return;
				}
				node = node->getLeft();
			}
			else if (comp(node->key, key)) {
				if (node->getRight() == nullptr) {
					node->createRight(key);
					fix(*node->getRight());
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
		Node *node = root;
		while (1) {
			if (comp(key, node->key)) {
				if (node->getLeft() == nullptr)
					return false;
				node = node->getLeft();
			}
			else if (comp(node->key, key)) {
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
		std::queue<Node*> q;

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
				Node *node = q.front();
				q.pop();
				if (node == nullptr) {
					std::cout << ' ';
					q.push(nullptr);
					q.push(nullptr);
				}
				else {
					if (node->color == Node::red)
						std::cout << "\x1b[31m" <<  node->key << "\x1b[0m";
					else
						std::cout << node->key;
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
