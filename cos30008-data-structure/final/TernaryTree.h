#pragma once
#include <algorithm>
#include <iostream>
#include <stdexcept>

template <typename T>
class TernaryTreePrefixIterator;

template <typename T>
class TernaryTree
{
public:
    using TTree = TernaryTree<T>;
    using TSubTree = TTree*;

private:
    T fKey;
    TSubTree fSubTrees[3];

    TernaryTree() : fKey(T())
    {
        for (size_t i = 0; i < 3; i++)
        {
            fSubTrees[i] = &NIL;
        }
    }

public:
    using Iterator = TernaryTreePrefixIterator<T>;
    static TTree NIL;  // sentinel

    // getters for subtrees
    const TTree& getLeft() const
    {
        return *fSubTrees[0];
    }
    const TTree& getMiddle() const
    {
        return *fSubTrees[1];
    }
    const TTree& getRight() const
    {
        return *fSubTrees[2];
    }

    // add a subtree
    void addLeft(const TTree& aTTree)
    {
        addSubTree(0, aTTree);
    }
    void addMiddle(const TTree& aTTree)
    {
        addSubTree(1, aTTree);
    }
    void addRight(const TTree& aTTree)
    {
        addSubTree(2, aTTree);
    }

    // remove a subtree, may through a domain error
    const TTree& removeLeft()
    {
        return removeSubTree(0);
    }
    const TTree& removeMiddle()
    {
        return removeSubTree(1);
    }
    const TTree& removeRight()
    {
        return removeSubTree(2);
    }

    // ****************************************************************** //
    // Problem 1: TernaryTree Basic Infrastructure
private:
    // remove a subtree, may throw a domain error [22]
    const TTree& removeSubTree(size_t aSubtreeIndex)
    {
        if (empty()) throw std::domain_error("Empty ternary tree");
        if (aSubtreeIndex >= 3) throw std::out_of_range("Illegal index");
        if (fSubTrees[aSubtreeIndex]->empty())
            throw std::domain_error("Subtree is NIL");

        const TTree& lTree = *fSubTrees[aSubtreeIndex];
        fSubTrees[aSubtreeIndex] = &NIL;
        return lTree;
    }

    // add a subtree; must avoid memory leaks; may throw domain error [18]
    void addSubTree(size_t aSubtreeIndex, const TTree& aTTree)
    {
        if (empty()) throw std::domain_error("Empty ternary tree");
        if (aSubtreeIndex >= 3) throw std::out_of_range("Illegal index");
        if (!fSubTrees[aSubtreeIndex]->empty())
            throw std::domain_error("Subtree is not NIL");

        fSubTrees[aSubtreeIndex] = const_cast<TTree*>(&aTTree);
    }

public:
    // TernaryTree l-value constructor [10]
    TernaryTree(const T& aKey) : fKey(aKey)
    {
        for (int i = 0; i < 3; i++)
        {
            fSubTrees[i] = &NIL;
        }
    }

    // destructor (free sub-trees, must not free empty trees) [14]
    ~TernaryTree()
    {
        // if (empty()) std::cout << "Delete NIL" << std::endl;

        for (int i = 0; i < 3; i++)
        {
            if (!fSubTrees[i]->empty())
            {
                delete fSubTrees[i];
            }
        }
    }

    // return key value, may throw domain_error if empty [2]
    const T& operator*() const
    {
        if (empty()) throw std::domain_error("Empty ternary tree");
        return fKey;
    }

    // returns true if this ternary tree is empty [4]
    bool empty() const
    {
        return this == &NIL;
    }

    // returns true if this ternary tree is a leaf [10]
    bool leaf() const
    {
        // throw std::domain_error("Operation not supported");

        for (int i = 0; i < 3; i++)
        {
            if (!fSubTrees[i]->empty())
            {
                return false;
            }
        }

        return true;
    }

    // return height of ternary tree, may throw domain_error if empty [48]
    size_t height() const
    {
        if (empty())
        {
            throw std::domain_error("Operation not supported");
        }

        if (leaf())
        {
            return 0;  // base case for recursion
        }

        size_t lHeight0 = fSubTrees[0]->empty() ? 0 : fSubTrees[0]->height();
        size_t lHeight1 = fSubTrees[1]->empty() ? 0 : fSubTrees[1]->height();
        size_t lHeight2 = fSubTrees[2]->empty() ? 0 : fSubTrees[2]->height();

        // return the maximum height + 1
        return std::max({lHeight0, lHeight1, lHeight2}) + 1;
    }

    // ****************************************************************** //
    // Problem 2: TernaryTree Copy Semantics

    // copy constructor, must not copy empty ternary tree [10]
    TernaryTree(const TTree& aOtherTTree)
    {
        if (empty()) throw std::domain_error("Must not copy empty tree");

        // create a copy of an object when its instantialise
        for (size_t i = 0; i < 3; i++) fSubTrees[i] = &NIL;
        *this = aOtherTTree;
    }

    // copy assignment operator, must not copy empty ternary tree
    // may throw a domain error on attempts to copy NIL [36]
    TTree& operator=(const TTree& aOtherTTree)
    {
        if (this == &aOtherTTree) return *this;  // self assignment
        if (aOtherTTree.empty())
            throw std::domain_error("NIL as source not permitted");

        this->~TernaryTree();
        fKey = aOtherTTree.fKey;

        for (size_t i = 0; i < 3; i++)
        {
            if (!aOtherTTree.fSubTrees[i]->empty())
            {
                fSubTrees[i] = aOtherTTree.fSubTrees[i]->clone();
            }
            else
            {
                fSubTrees[i] = &NIL;
            }
        }

        return *this;
    }

    // clone ternary tree, must not copy empty trees
    TSubTree clone() const
    {
        if (!empty()) return new TTree(*this);
        throw std::domain_error("Clonning of NIL detected");
    }

    // ****************************************************************** //
    // Problem 3: TernaryTree Move Semantics

    // TTree r-value constructor
    TernaryTree(T&& aKey) : fKey(std::move(aKey))
    {
        for (int i = 0; i < 3; i++)
        {
            fSubTrees[i] = &NIL;
        }
    }

    // move constructor, must not copy empty ternary tree
    TernaryTree(TTree&& aOtherTTree) : fKey(std::move(aOtherTTree.fKey))
    {
        for (size_t i = 0; i < 3; i++)
        {
            fSubTrees[i] = aOtherTTree.fSubTrees[i];
            aOtherTTree.fSubTrees[i] = &NIL;
        }
    }

    // move assignment operator, must not copy empty ternary tree
    TTree& operator=(TTree&& aOtherTTree)
    {
        if (this == &aOtherTTree) return *this;
        if (aOtherTTree.empty())
            throw std::domain_error("NIL as source not permitted");

        this->~TernaryTree();
        fKey = std::move(aOtherTTree.fKey);

        for (int i = 0; i < 3; i++)
        {
            if (!aOtherTTree.fSubTrees[i]->empty())
            {
                fSubTrees[i]
                    = const_cast<TSubTree>(&aOtherTTree.removeSubTree(i));
            }
            else
            {
                fSubTrees[i] = &NIL;
            }
        }

        return *this;
    }

    // return ternary tree prefix iterator positioned at start
    Iterator begin() const
    {
        return Iterator(this).begin();
    }
    // return ternary prefix iterator positioned at end
    Iterator end() const
    {
        return Iterator(this).end();
    }
};

template <typename T>
TernaryTree<T> TernaryTree<T>::NIL;