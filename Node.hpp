#ifndef NODE_HPP
#define NODE_HPP

template< typename Data_t >
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

	Node(const Data_t &data) {
		this->data = data;
	}

public:
	Data_t data;

	static void createRoot(const Data_t &data, Node **root) {
		if (*root != nullptr)
			throw 1;

		Node *new_node = new Node(data);
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

	void createLeft(const Data_t &data) {
		if (left != nullptr)
			throw 1;

		Node *new_node = new Node(data);
		new_node->root = root;
		new_node->parent = this;
		left = new_node;

		for (Node *p = this; p != nullptr; p = p->parent)
			p->updateHeight();
	}

	void createRight(const Data_t &data) {
		if (right != nullptr)
			throw 1;

		Node *new_node = new Node(data);
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

#endif /* NODE_HPP */
