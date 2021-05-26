#pragma once

#include "FibonacciHeap.h"
#include <unordered_map>
#include <algorithm>

template <class TKey, class TValue, class TComp = std::less<TKey>>
class FibonacciQueue : private FibonacciHeap<TKey, TValue, TComp>
{
private:
    using Heap = FibonacciHeap<TKey, TValue, TComp>;
    using Node = typename FibonacciHeap<TKey, TValue, TComp>::Node;
    using KeyNodeIter = typename std::unordered_map<TKey, Node *>::iterator;

    std::unordered_multimap<TKey, Node *> m_KeyStore;

public:
    FibonacciQueue()
        : Heap()
    {
    }

    FibonacciQueue(TComp comp)
        : Heap(comp)
    {
    }

    ~FibonacciQueue()
    {
    }

    bool empty() const
    {
        return this->m_Size == 0;
    }

    Node *topNode()
    {
        return Heap::minimum();
    }

    TKey &top()
    {
        return Heap::minimum()->key;
    }


    void decrease_key(Node*node, TKey k)
    {
        KeyNodeIter mit = m_KeyStore.find(node->key);
        m_KeyStore.erase(mit);
        m_KeyStore.insert({ k, node });
        Heap::decrease_key(node, std::move(k));
    }

    void decrease_key(TKey originalKey, TKey k)
    {
        auto node = findNode(originalKey);
        this->decrease_key(node, std::move(k));
    }

    Node *push(TKey k, TValue pl)
    {
        Node *x = new Node(std::move(k), pl);
        Heap::insert(x);
        m_KeyStore.insert({k, x});
        return x;
    }

    KeyNodeIter find(const TKey &k)
    {
        KeyNodeIter mit = m_KeyStore.find(k);
        return mit;
    }

    int count(const TKey &k)
    {
        KeyNodeIter mit = m_KeyStore.find(k);
        return mit != m_KeyStore.end();
    }

    Node *findNode(const TKey &k)
    {
        KeyNodeIter mit = find(k);
        return mit->second;
    }

    void pop()
    {
        if (empty())
            return;
        Node *x = Heap::extract_min();
        if (!x)
            return; // should not happen.
        auto range = m_KeyStore.equal_range(x->key);
        auto mit = std::find_if(range.first, range.second,
                                [x](const std::pair<TKey, Node *> &ele) {
                                    return ele.second == x;
                                });
        if (mit != range.second)
            m_KeyStore.erase(mit);
        else
            std::cerr << "[Error]: key " << x->key << " cannot be found in FiboQueue fast store\n";
        delete x;
    }

    void clear() override
    {
        Heap::clear();
        m_KeyStore.clear();
    }
};