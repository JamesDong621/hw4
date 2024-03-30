#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);
    
    // Add helper functions here
    void updatePathBalance(AVLNode<Key, Value>* node);
    int calculateBalanceFactor(AVLNode<Key, Value>* node);
    AVLNode<Key, Value>* leftRotate(AVLNode<Key, Value>* x);
    AVLNode<Key, Value>* rightRotate(AVLNode<Key, Value>* x);
};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value>& new_item) {
    this->BinarySearchTree<Key, Value>::insert(new_item);

    AVLNode<Key, Value>* insertedNode = static_cast<AVLNode<Key, Value>*>(this->BinarySearchTree<Key, Value>::internalFind(new_item.first));
    if (!insertedNode) return;  // If node wasn't actually inserted, don't continue.

    // Update balance factors from this node upwards.
    this->updatePathBalance(insertedNode);
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key) {
    // Standard BST removal but keep track of the parent for rebalancing
    AVLNode<Key, Value>* nodeToRemove = static_cast<AVLNode<Key, Value>*>(this->BinarySearchTree<Key, Value>::internalFind(key));
    if (!nodeToRemove) return;

    AVLNode<Key, Value>* parent = static_cast<AVLNode<Key, Value>*>(nodeToRemove->getParent());
    AVLNode<Key, Value>* nodeToRebalance = (nodeToRemove->getLeft() && nodeToRemove->getRight()) ? static_cast<AVLNode<Key, Value>*>(this->predecessor(nodeToRemove)) : parent;

    // Perform standard BST removal
    BinarySearchTree<Key, Value>::remove(key);

    // Update path balance starting from the parent of the removed node or the predecessor if two children
    updatePathBalance(nodeToRebalance);
}

template<class Key, class Value>
void AVLTree<Key, Value>::updatePathBalance(AVLNode<Key, Value>* node) {
    while (node != nullptr) {
        // Update balance factor
        int balance = calculateBalanceFactor(node);

       if (balance > 1) {
            if (calculateBalanceFactor(node->getLeft()) < 0) {
                node->setLeft(leftRotate(node->getLeft()));  // Left-Right Case
            }
            node = rightRotate(node);  // Left-Left Case
        } else if (balance < -1) {
            if (calculateBalanceFactor(node->getRight()) > 0) {
                node->setRight(rightRotate(node->getRight()));  // Right-Left Case
            }
            node = leftRotate(node);  // Right-Right Case
        }

        // If the node has no parent, it's the root of the tree
        if (node->getParent() == nullptr) {
            this->root_ = node;
        }

        // Move to the parent of the node
        node = static_cast<AVLNode<Key, Value>*>(node->getParent());
    }
}



template<class Key, class Value>
int AVLTree<Key, Value>::calculateBalanceFactor(AVLNode<Key, Value>* node) {
    if (!node) return -1;
    return this->getHeight(dynamic_cast<AVLNode<Key, Value>*>(node->getLeft())) - 
           this->getHeight(dynamic_cast<AVLNode<Key, Value>*>(node->getRight()));
}



template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::leftRotate(AVLNode<Key, Value>* x) {
    AVLNode<Key, Value>* y = x->getRight();
    x->setRight(y->getLeft());
    if (y->getLeft() != nullptr) {
        y->getLeft()->setParent(x);
    }
    y->setParent(x->getParent());
    if (x->getParent() == nullptr) {
        this->root_ = y;
    } else if (x == x->getParent()->getLeft()) {
        x->getParent()->setLeft(y);
    } else {
        x->getParent()->setRight(y);
    }
    y->setLeft(x);
    x->setParent(y);

    return y;
}

template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::rightRotate(AVLNode<Key, Value>* y) {
    AVLNode<Key, Value>* x = y->getLeft();
    y->setLeft(x->getRight());
    if (x->getRight() != nullptr) {
        x->getRight()->setParent(y);
    }
    x->setParent(y->getParent());
    if (y->getParent() == nullptr) {
        this->root_ = x;
    } else if (y == y->getParent()->getLeft()) {
        y->getParent()->setLeft(x);
    } else {
        y->getParent()->setRight(x);
    }
    x->setRight(y);
    y->setParent(x);

    return x;
}



template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif