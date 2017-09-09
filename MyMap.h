// MyMap.h

#ifndef MYMAP_INCLUDED
#define MYMAP_INCLUDED

// Skeleton for the MyMap class template.  You must implement the first six
// member functions.

template<typename KeyType, typename ValueType>
class MyMap
{
public:
	MyMap();
	~MyMap();
	void clear();
	int size() const;
	void associate(const KeyType& key, const ValueType& value);

	  // for a map that can't be modified, return a pointer to const ValueType
	const ValueType* find(const KeyType& key) const;

	  // for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const MyMap*>(this)->find(key));
	}

	  // C++11 syntax for preventing copying and assignment
	MyMap(const MyMap&) = delete;
	MyMap& operator=(const MyMap&) = delete;

private:
    struct Node {
        KeyType data_first;
        ValueType data_second;
        Node* lhs;
        Node* rhs;
    };
    Node* root;
    int m_size;
    
    // recursive function to help the clear function
    void clearHelper(Node* deleteBelow);
    
    // recursive function to help the associate function
    void associateHelper(Node*& insertBelow, const KeyType& key, const ValueType& value);
    
    // recursive function to help the find function
    ValueType* findHelper(Node* FINDMEBRO, const KeyType& key) const;
};

// constructor
template<typename KeyType, typename ValueType>
MyMap<KeyType, ValueType>::MyMap() {
    m_size = 0;
}


// destructor
template<typename KeyType, typename ValueType>
MyMap<KeyType, ValueType>::~MyMap() {
    clear();
}


// clear function
template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::clear() {
    clearHelper(root);
    root = nullptr;
    m_size = 0;
}


// size function
template<typename KeyType, typename ValueType>
int MyMap<KeyType, ValueType>::size() const {
    return m_size;
}


// associate function
template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value) {
    ValueType* switcheroo = find(key);
    if(switcheroo == nullptr) {
        associateHelper(root, key, value);
        m_size++;
    }
    else {
        *switcheroo = value;
    }
}


// find function
template<typename KeyType, typename ValueType>
const ValueType* MyMap<KeyType, ValueType>::find(const KeyType& key) const {
    return findHelper(root, key);
}


// find helper function
template<typename KeyType, typename ValueType>
ValueType* MyMap<KeyType, ValueType>::findHelper(Node* FINDMEBRO, const KeyType& key) const {
    if(FINDMEBRO == nullptr)
        return nullptr;
    if(FINDMEBRO->data_first == key)
        return &(FINDMEBRO->data_second);
    if(FINDMEBRO->data_first < key)
        return findHelper(FINDMEBRO->rhs, key);
    else
        return findHelper(FINDMEBRO->lhs, key);
}


// associate helper function
template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::associateHelper(Node*& insertBelow, const KeyType& key, const ValueType& value) {
    if(insertBelow == nullptr) {
        insertBelow = new Node;
        insertBelow->data_first = key;
        insertBelow->data_second = value;
        insertBelow->lhs = nullptr;
        insertBelow->rhs = nullptr;
        return;
    }
    else if(key < insertBelow->data_first)
        associateHelper(insertBelow->lhs, key, value);
    else
        associateHelper(insertBelow->rhs, key, value);
}


// clear helper function
template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::clearHelper(Node* deleteBelow) {
    if(deleteBelow == nullptr)
        return;
    clearHelper(deleteBelow->lhs);
    clearHelper(deleteBelow->rhs);
    delete deleteBelow;
}
                                                
                                                
#endif // MYMAP_INCLUDED

