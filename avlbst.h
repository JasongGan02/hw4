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
    void insert_fix(AVLNode<Key,Value>* p, AVLNode<Key,Value>* n);
    bool isZigZig(AVLNode<Key,Value>* g, AVLNode<Key,Value>* p, AVLNode<Key,Value>* n);
    void rotateRight(AVLNode<Key,Value>* n);
    void rotateLeft(AVLNode<Key,Value>* n);
    void remove_fix(AVLNode<Key,Value>* n, int8_t diff);
    void rotateHelper(AVLNode<Key,Value>* n, int8_t diff);
};


template<class Key, class Value>
bool AVLTree<Key, Value>::isZigZig(AVLNode<Key,Value>* g, AVLNode<Key,Value>* p, AVLNode<Key,Value>* n)
{
    return (g->getLeft() == p && p->getLeft() == n) || (g->getRight() == p && p->getRight() == n);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key,Value>* n)
{   
    AVLNode<Key,Value>* p = n->getParent();
    AVLNode<Key,Value>* child = n->getLeft();

    if (child == nullptr)
        return;
    if (p == nullptr)
        this->root_ = child;
    else
    {
        if (p->getLeft() == n)
            p->setLeft(child);
        else
            p->setRight(child);
    }
    child->setParent(p);

    AVLNode<Key,Value>* child_right = child->getRight();
    n->setLeft(child_right);
    if (child_right != nullptr)
        child_right->setParent(n);

    child->setRight(n);
    n->setParent(child);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key,Value>* n)
{   
    AVLNode<Key,Value>* p = n->getParent();
    AVLNode<Key,Value>* child = n->getRight();

    if (child == nullptr)
        return;
    if (p == nullptr)
        this->root_ = child;
    else
    {
        if (p->getLeft() == n)
            p->setLeft(child);
        else
            p->setRight(child);
    }
    child->setParent(p);

    AVLNode<Key,Value>* child_left = child->getLeft();
    n->setRight(child_left);
    if (child_left != nullptr)
        child_left->setParent(n);

    child->setLeft(n);
    n->setParent(child);
}

template<class Key, class Value>
void AVLTree<Key, Value>::insert_fix(AVLNode<Key,Value>* p, AVLNode<Key,Value>* n)
{
    if (p == nullptr || p->getParent() == nullptr)
        return;

    AVLNode<Key, Value>* g = p->getParent();
    int signMultiplier = 1;
    if (g->getLeft() == p)
        signMultiplier = -1;
    g->updateBalance(signMultiplier);

    if (g->getBalance() == 0)
        return;
    else if (g->getBalance() == signMultiplier)
        insert_fix(g, p);
    else
    {
        //p is left child
        if (signMultiplier == -1)
        {
            if (isZigZig(g, p, n))
            {
                rotateRight(g);
                p->setBalance(0);
                g->setBalance(0);
            }
            else
            {
                rotateLeft(p);
                rotateRight(g);
                if (n->getBalance() == -1)
                {
                    p->setBalance(0);
                    g->setBalance(1);
                    n->setBalance(0);
                }
                else if (n->getBalance() == 0)
                {
                    p->setBalance(0);
                    g->setBalance(0);
                    n->setBalance(0);
                }
                else
                {
                    p->setBalance(-1);
                    g->setBalance(0);
                    n->setBalance(0);
                }
            }
        }
        else
        {
            if (isZigZig(g, p, n))
            {
                rotateLeft(g);
                p->setBalance(0);
                g->setBalance(0);
            }
            else
            {
                rotateRight(p);
                rotateLeft(g);
                if (n->getBalance() == 1)
                {
                    p->setBalance(0);
                    g->setBalance(-1);
                    n->setBalance(0);
                }
                else if (n->getBalance() == 0)
                {
                    p->setBalance(0);
                    g->setBalance(0);
                    n->setBalance(0);
                }
                else
                {
                    p->setBalance(1);
                    g->setBalance(0);
                    n->setBalance(0);
                }
            }
        }
    }
}

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    AVLNode<Key, Value>* nodeToInsert = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
    if (this->root_ == nullptr)
    {
        this->root_ = nodeToInsert;
        dynamic_cast<AVLNode<Key, Value>*>(this->root_)->setBalance(0);
        return;
    }

    AVLNode<Key, Value>* cur = dynamic_cast<AVLNode<Key, Value>*>(this->root_);  
    AVLNode<Key, Value>* p = nullptr;

    while (cur != nullptr)
    {
        p = cur; 
        if (nodeToInsert->getKey() > cur->getKey())
            cur = cur->getRight();
        else if (nodeToInsert->getKey() < cur->getKey())
            cur = cur->getLeft();
        else
        {
            cur->setValue(nodeToInsert->getValue());
            delete nodeToInsert;
            return;
        }
    }

    int diff = 0;
    if (nodeToInsert->getKey() > p->getKey()) 
    {
        p->setRight(nodeToInsert);
        diff = 1;
    } 
    else 
    {
        p->setLeft(nodeToInsert);
        diff = -1;
    }
    nodeToInsert->setParent(p);
    nodeToInsert->setBalance(0);

    if (abs(p->getBalance()) == 1)
    {
        p->setBalance(0);
        return;
    }

    p->updateBalance(diff);
    insert_fix(p, nodeToInsert);
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key) {
    AVLNode<Key, Value>* current = dynamic_cast<AVLNode<Key, Value>*>(this->internalFind(key));
    if (current == nullptr)
        return;
     
    // If the node has two children, swap with its predecessor.
    if (current->getLeft() != nullptr && current->getRight() != nullptr) 
    {
        nodeSwap(current, dynamic_cast<AVLNode<Key, Value>*>(this->predecessor(current)));
    }
     
    // Now, the node to be removed has at most one child.
    AVLNode<Key, Value>* child = (current->getLeft() != nullptr) ? current->getLeft() : current->getRight();
    AVLNode<Key, Value>* parent = current->getParent();

    int8_t diff = -1;

    if (child != nullptr) 
    {
        child->setParent(parent);
    }
     
    if (parent == nullptr) 
    {
        this->root_ = child;
    } 
    else 
    {
        if (parent->getLeft() == current) 
        {
            parent->setLeft(child);
            diff = 1;
        } 
        else 
        {
            parent->setRight(child);
        }
    }

    delete current;

    if (parent != nullptr) 
    {
        remove_fix(parent, diff);
    }
}
template<class Key, class Value> 
void AVLTree<Key, Value>::rotateHelper(AVLNode<Key,Value>* n, int8_t diff)
{
    
    if (diff == -1)
    {
        //std::cout << "rotate right" << std::endl;
        rotateRight(n);

    }
    else if (diff == 1)
    {
        //std::cout << "rotate left" << std::endl;
        rotateLeft(n);

    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::remove_fix(AVLNode<Key,Value>* n, int8_t diff)
{
    if (n == nullptr)
        return;

    AVLNode<Key, Value>* p = n->getParent();
    int8_t ndiff = (p != nullptr && p->getLeft() == n) ? 1 : -1;
    int8_t bdiff = n->getBalance();
    //std::cout << "bdiff + diff = " << bdiff + diff << std::endl;
    if (bdiff + diff == diff * 2)
    {
        AVLNode<Key, Value>* c = diff == -1 ? n->getLeft() : n->getRight();
        int8_t cdiff = c->getBalance();
        if (cdiff == diff * 1)
        {
            rotateHelper(n, diff);
            n->setBalance(0);
            c->setBalance(0);
            remove_fix(p, ndiff);
        }
        else if (cdiff == 0)
        {
            rotateHelper(n, diff);
            n->setBalance(diff * 1);
            c->setBalance(diff * -1);
        }
        else if (cdiff == diff * -1)//zig zag
        {
            AVLNode<Key, Value>* g = diff == -1 ? c->getRight() : c->getLeft() ;
            int8_t gdiff = g->getBalance();
            rotateHelper(c, diff * -1);
            rotateHelper(n, diff);  
            if (gdiff == diff * -1)
            {
                n->setBalance(0);
                c->setBalance(diff * 1);
                g->setBalance(0);
            }
            else if (gdiff == 0)
            {
                n->setBalance(0);
                c->setBalance(0);
                g->setBalance(0);
            }
            else 
            {
                n->setBalance(diff * -1);
                c->setBalance(0);
                g->setBalance(0);
            }
            remove_fix(p, ndiff);
        }
    }
    else if (bdiff + diff == diff)
    {
        n->setBalance(diff);
    }
    else
    {
        n->setBalance(0);
        remove_fix(p, ndiff);
    }
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
