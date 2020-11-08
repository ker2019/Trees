#include <functional>
#include <iostream>
#include <queue>

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
		if (root == nullptr)
			root = new Node(key, nullptr);
		Node *node = root;
		while (1) {
			if (comp(key, node->key)) {
				if (node->left == nullptr) {
					node->left = new Node(key, node);
					for (Node *p = node; p != nullptr; p = p->parent)
						p->updateHeight();
					for (Node *p = node; p->parent != nullptr; p = p->parent) {
						if (p == p->parent->left)
							balance(&p->parent->left);
						else if (p == p->parent->right)
							balance(&p->parent->right);
						else
							throw 1;
					}
					return;
				}
				node = node->left;
			}
			else if (comp(node->key, key)) {
				if (node->right == nullptr) {
					node->right = new Node(key, node);
					for (Node *p = node; p != nullptr; p = p->parent)
						p->updateHeight();
					for (Node *p = node; p->parent != nullptr; p = p->parent) {
						if (p == p->parent->left)
							balance(&p->parent->left);
						else if (p == p->parent->right)
							balance(&p->parent->right);
						else
							throw 1;
					}
					balance(&root);
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

		const int width = 2*((1 << root->height) - 1);
		q.push(root);
		int pos_num = 1;
		for (int j = 0; j < root->height; j++, pos_num = 2 * pos_num) {
			for (int i = 0; i < pos_num; i++) {
				for (int k = 0; k < (width - pos_num)/(pos_num + 1); k++)
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
			std::cout << '\n';
		}
	}
};
