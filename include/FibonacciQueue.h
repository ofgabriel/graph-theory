#pragma once

#include "FibonacciHeap.h"
#include <unordered_map>
#include <algorithm>
#include <map>

template <class TKey, class TValue, class TComp = std::less<TKey> >
class FibonacciQueue : private FibonacciHeap<TKey, TValue, TComp>
{
private:
    using Heap = FibonacciHeap<TKey, TValue, TComp>;
    using Node = typename FibonacciHeap<TKey, TValue, TComp>::Node;
    using ValueNodeIter = typename std::map<TValue, Node*>::iterator;

    std::map<TValue, Node*> m_ValueStore;

public:
    FibonacciQueue()
        : FibonacciQueue(TComp())
    {
    }

    FibonacciQueue(TComp comp)
        : Heap(comp)
    {
        m_ValueStore = std::map<TValue, Node*>();
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

    TValue &top()
    {
        return Heap::minimum()->payload;
    }


    void decrease_key(Node*node, TKey k)
    {
        m_ValueStore.insert_or_assign(node->payload, node);
        Heap::decrease_key(node, k);
    }

    void decrease_key(TValue value, TKey k)
    {
        auto node = findNode(value);
        this->decrease_key(node, k);
    }

    Node *push(TKey k, TValue pl)
    {
        Node *x = new Node(k, pl);
        Heap::insert(x);
        m_ValueStore.insert_or_assign(pl, x);
        return x;
    }

    int count(const TKey &k)
    {
        ValueNodeIter mit = m_ValueStore.find(k);
        return mit != m_ValueStore.end();
    }

    Node *findNode(TValue value)
    {
        ValueNodeIter mit = find(value);
        return mit->second;
    }

    ValueNodeIter find(const TValue &k)
    {
        auto mit = m_ValueStore.find(k);
        return mit;
    }

    void pop()
    {
        if (empty())
            return;
        Node *x = Heap::extract_min();
        if (!x)
            return; // should not happen.
            
        m_ValueStore.erase(x->payload);
        // auto range = m_ValueStore.equal_range(x->payload);
        // auto mit = std::find_if(range.first, range.second,
        //                         [x](const std::pair<TValue, Node *> &ele) {
        //                             return ele.second == x;
        //                         });
        // if (mit != range.second)
        //     m_ValueStore.erase(mit);
        // else
        //     std::cerr << "[Error]: value " << x->payload << " cannot be found in FiboQueue fast store\n";
        delete x;
    }

    void clear() override
    {
        Heap::clear();
        m_ValueStore.clear();
    }
};