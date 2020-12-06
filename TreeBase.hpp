#ifndef TREEBASE_HPP
#define TREEBASE_HPP

template< class Key_t, class Compare_t = std::less<Key_t> >
class TreeBase {
public:
	virtual void insert(const Key_t &key) = 0;
	virtual bool contains(const Key_t &key) const = 0;
	virtual void erase(const Key_t &key) = 0;
	virtual int size() const = 0;
	virtual void print() const = 0;
	typedef Key_t key_type;
	typedef Compare_t key_compare;
};

#endif /* TREEBASE_HPP */
