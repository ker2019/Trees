#include <functional>
#include <iostream>
#include <queue>

#ifndef RBTREE_HPP
#define RBTREE_HPP

template< class Key_t, class Compare_t = std::less<Key_t> >
class RBtree {
private:
	struct Node {
		Key_t key;
		Node *left = nullptr;
		Node *right = nullptr;
		Node *parent = nullptr;
		Node **binding_point = nullptr;
		int height = 1;
		enum color_t {red, black};
		color_t color = red;
		Node(const Key_t &key) {
			this->key = key;
		}

		~Node() {
			if (left != nullptr)
				delete left;
			if (right != nullptr)
				delete right;
		}

		static void bindto(Node *node, Node *new_parent, Node **new_point) {
			*new_point = node;
			if (node != nullptr) {
				if (node->binding_point != nullptr)
					*(node->binding_point) = nullptr;
				node->parent = new_parent;
				node->binding_point = new_point;
			}
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
	};

	Node *root;
	Compare_t comp;

	static void rotateRight(Node &node) {
		Node *left = node.left;
		if (left == nullptr)
			throw 1;
		Node *parent = node.parent;
		Node **binding_point = node.binding_point;

		Node::bindto(left->right, &node, &node.left);
		Node::bindto(&node, left, &left->right);
		Node::bindto(left, parent, binding_point);
		node.updateHeight();
		left->updateHeight();
	}

	static void rotateLeft(Node &node) {
		Node *right = node.right;
		if (right == nullptr)
			throw 1;
		Node *parent = node.parent;
		Node **binding_point = node.binding_point;

		Node::bindto(right->left, &node, &node.right);
		Node::bindto(&node, right, &right->left);
		Node::bindto(right, parent, binding_point);
		node.updateHeight();
		right->updateHeight();
	}

	void fix(Node &node) {
		if (node.color == Node::black)
			return;

		// Root
		if (node.parent == nullptr) {
			node.color = Node::black;
			return;
		}

		Node *father = node.parent;
		if (father->color == Node::black)
			return;

		Node *grandpa = father->parent;
		if (grandpa->left == father) {
			Node *uncle = grandpa->right;
			auto uncle_color = (uncle == nullptr ? Node::black : uncle->color);
			if (uncle_color == Node::red) {
				father->color = Node::black;
				uncle->color = Node::black;
				grandpa->color = Node::red;
				fix(*grandpa);
				return;
			}

			if (father->right == &node) {
				rotateLeft(*father);
				rotateRight(*grandpa);
				node.color = Node::black;
				grandpa->color = Node::red;
				return;
			}

			if (father->left == &node) {
				rotateRight(*grandpa);
				father->color = Node::black;
				grandpa->color = Node::red;
			}
		}
		else if (grandpa->right == father) {
			Node *uncle = grandpa->left;
			auto uncle_color = (uncle == nullptr ? Node::black : uncle->color);
			if (uncle_color == Node::red) {
				father->color = Node::black;
				uncle->color = Node::black;
				grandpa->color = Node::red;
				fix(*grandpa);
				return;
			}

			if (father->left == &node) {
				rotateRight(*father);
				rotateLeft(*grandpa);
				node.color = Node::black;
				grandpa->color = Node::red;
				return;
			}

			if (father->right == &node) {
				rotateLeft(*grandpa);
				father->color = Node::black;
				grandpa->color = Node::red;
			}
		}
		else
			throw 1;
	}

public:
	RBtree() {
		root = nullptr;
		comp = Compare_t();
	}
	RBtree(const Compare_t &comp) {
		root = nullptr;
		this->comp = comp;
	}
	~RBtree() {
		if (root != nullptr)
			delete root;
	}

	void insert(const Key_t &key) {
		if (root == nullptr) {
			Node *new_node = new Node(key);
			Node::bindto(new_node, nullptr, &root);
			fix(*new_node);
			return;
		}
		Node *node = root;
		while (1) {
			if (comp(key, node->key)) {
				if (node->left == nullptr) {
					Node *new_node = new Node(key);
					Node::bindto(new_node, node, &node->left);
					for (Node *p = node; p != nullptr; p = p->parent)
						p->updateHeight();
					fix(*new_node);
					return;
				}
				node = node->left;
			}
			else if (comp(node->key, key)) {
				if (node->right == nullptr) {
					Node *new_node = new Node(key);
					Node::bindto(new_node, node, &node->right);
					for (Node *p = node; p != nullptr; p = p->parent)
						p->updateHeight();
					fix(*new_node);
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

	void print() const {
		if (root == nullptr)
			return;
		std::queue<Node*> q;

		int width = (1 << root->height) - 1;
		q.push(root);
		int pos_num = 1;
		for (int j = 0; j < root->height; j++, pos_num = 2 * pos_num, width=width/2) {
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

#endif /* RBTREE_HPP */
