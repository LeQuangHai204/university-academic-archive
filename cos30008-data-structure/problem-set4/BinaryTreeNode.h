
// COS30008, Problem Set 4, Problem 1, 2022

#pragma once

#include <algorithm>
#include <stdexcept>

template <typename T>
struct BinaryTreeNode
{
    using BNode = BinaryTreeNode<T>;
    using BTreeNode = BNode*;

    T key;
    BTreeNode left;
    BTreeNode right;

    static BNode NIL;

    const T& findMax() const
    {
        if (empty())
        {
            throw std::domain_error("Empty tree encountered.");
        }

        return right->empty() ? key : right->findMax();
    }

    const T& findMin() const
    {
        if (empty())
        {
            throw std::domain_error("Empty tree encountered.");
        }

        return left->empty() ? key : left->findMin();
    }

    bool remove(const T& aKey, BTreeNode aParent)
    {
        BTreeNode x = this;
        BTreeNode y = aParent;
        while (!x->empty())
        {
            if (aKey == x->key) break;
            y = x;  // new parent
            x = aKey < x->key ? x->left : x->right;
        }

        if (x->empty())
        {
            return false;  // delete failed
        }

        if (!x->left->empty())
        {
            const T& lKey = x->left->findMax();  // find max to left
            x->key = lKey;
            x->left->remove(lKey, x);
        }
        else if (!x->right->empty())
        {
            const T& lKey = x->right->findMin();  // find min to right
            x->key = lKey;
            x->right->remove(lKey, x);
        }
        else
        {
            if (y != &NIL)  // y can be NIL
            {
                if (y->left == x)
                {
                    y->left = &NIL;
                }
                else
                {
                    y->right = &NIL;
                }
            }

            delete x;  // free deleted node
        }

        return true;
    }

    // PS4 starts here

    BinaryTreeNode() : key(T()), left(&NIL), right(&NIL) {}

    BinaryTreeNode(const T& aKey) : key(aKey), left(&NIL), right(&NIL) {}

    BinaryTreeNode(T&& aKey) : key(std::move(aKey)), left(&NIL), right(&NIL) {}

    ~BinaryTreeNode()
    {
        if (!left->empty()) delete left;
        if (!right->empty()) delete right;
    }

    bool empty() const
    {
        return this == &NIL;
    }

    bool leaf() const
    {
        if (empty()) throw std::domain_error("Empty tree encountered");
        return left->empty() && right->empty();
    }

    size_t height() const
    {
        if (leaf()) return 0;  // base case
        if (left->empty()) return right->height() + 1;
        if (right->empty()) return left->height() + 1;
        return std::max(left->height(), right->height()) + 1;
    }

    bool insert(const T& aKey)
    {
        if (empty()) return false;
        BTreeNode lParent;
        BTreeNode lCurrent = this;

        do
        {
            if (lCurrent->key == aKey) return false;
            lParent = lCurrent;
            lCurrent
                = (lCurrent->key > aKey) ? lCurrent->left : lCurrent->right;
        } while (lCurrent != &NIL);

        BNode* newNode = new BNode(aKey);
        if (lParent->key > aKey) lParent->left = newNode;
        else lParent->right = newNode;
        return true;
    }

    void print()
    {
        if (empty())
        {
            std::cout << " Nil ";
        }
        else
        {
            std::cout << " " << key << " ";
        }

        if (!left->empty()) left->print();
        if (!right->empty()) right->print();
    }
};

template <typename T>
BinaryTreeNode<T> BinaryTreeNode<T>::NIL;
