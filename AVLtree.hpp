#include <functional>

template< class Key_t, class Compare_t = std::less<Key_t> >
class AVLtree {
private:
	struct Node {
		Key_t key;
		Node *left;
		Node *right;
		Node(const Key_t &key) {
			this->key = key;
			left = right = nullptr;
		}

		~Node() {
			if (left != nullptr)
				delete left;
			if (right != nullptr)
				delete right;
		}
	};

	Node *root;
	Compare_t comp;

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
			root = new Node(key);
		Node *node = root;
		while (1) {
			if (comp(key, node->key)) {
				if (node->left == nullptr) {
					node->left = new Node(key);
					return;
				}
				node = node->left;
			}
			else if (comp(node->key, key)) {
				if (node->right == nullptr) {
					node->right = new Node(key);
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
};
