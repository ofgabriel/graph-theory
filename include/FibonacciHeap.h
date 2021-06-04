//#pragma once

#include <iostream>
#include <math.h>

template <class TKey, class TValue, class TComp = std::less<TKey>>
class FibonacciHeap
{
public:
    // node
    class Node
    {
    public:
        Node(TKey k, TValue value) : key(k),
                                  mark(false),
                                  p(nullptr),
                                  left(nullptr),
                                  right(nullptr),
                                  child(nullptr),
                                  degree(-1),
                                  payload(value)
        {
        }

        ~Node()
        {
        }

        TKey key;
        bool mark;
        Node *p;
        Node *left;
        Node *right;
        Node *child;
        int degree;
        TValue payload;
    };

    FibonacciHeap() 
        : FibonacciHeap(TComp())
    {
    }

    FibonacciHeap(TComp comp)
        : m_Size(0), m_Min(nullptr), m_Comparator(comp)
    {
    }

    ~FibonacciHeap()
    {
        clear();
    }

    virtual void clear()
    {
        delete_Nodes(m_Min);
        m_Min = nullptr;
        m_Size = 0;
    }

    unsigned int size()
    {
        return (unsigned int)this->m_Size;
    }

protected:
    int m_Size;
    Node *m_Min;
    TComp m_Comparator;

    void delete_Nodes(Node *x)
    {
        if (!x)
            return;

        Node *cur = x;
        while (true)
        {
            if (cur->left && cur->left != x)
            {
                Node *tmp = cur;
                cur = cur->left;
                if (tmp->child)
                {
                    delete_Nodes(tmp->child);
                }
                delete tmp;
            }
            else
            {
                if (cur->child)
                    delete_Nodes(cur->child);
                delete cur;
                break;
            }
        }
    }

    void insert(Node *x)
    {
        x->degree = 0;
        x->p = nullptr;
        x->child = nullptr;
        x->mark = false;
        if (m_Min == nullptr)
        {
            m_Min = x->left = x->right = x;
        }
        else
        {
            m_Min->left->right = x;
            x->left = m_Min->left;
            m_Min->left = x;
            x->right = m_Min;
            if (m_Comparator(x->key, m_Min->key))
            {
                m_Min = x;
            }
        }
        ++m_Size;
    }

    Node *minimum()
    {
        return m_Min;
    }

    Node *extract_min()
    {
        auto z = m_Min;
        if (z != nullptr)
        {
            auto x = z->child;
            if (x != nullptr)
            {
                auto childList = new Node *[z->degree];
                auto next = x;
                for (int i = 0; i < (int)z->degree; i++)
                {
                    childList[i] = next;
                    next = next->right;
                }
                for (int i = 0; i < (int)z->degree; i++)
                {
                    x = childList[i];
                    m_Min->left->right = x;
                    x->left = m_Min->left;
                    m_Min->left = x;
                    x->right = m_Min;
                    x->p = nullptr;
                }
                delete[] childList;
            }
            z->left->right = z->right;
            z->right->left = z->left;
            if (z == z->right)
            {
                m_Min = nullptr;
            }
            else
            {
                m_Min = z->right;
                consolidate();
            }
            m_Size--;
        }
        return z;
    }

    void consolidate()
    {
        // Max degree <= log base golden ratio of n
        int max_degree = static_cast<int>(floor(log(static_cast<double>(m_Size)) / log(static_cast<double>(1 + sqrt(static_cast<double>(5))) / 2)));

        auto A = new Node *[max_degree + 2]; // plus two both for indexing to max degree and so A[max_degree+1] == NIL
        std::fill_n(A, max_degree + 2, nullptr);

        auto w = m_Min;
        auto next = w;
        
        int rootSize = 0;
        do
        {
            rootSize++;
            next = next->right;
        } while (next != w);

        auto rootList = new Node *[rootSize];
        for (int i = 0; i < rootSize; i++)
        {
            rootList[i] = next;
            next = next->right;
        }
        for (int i = 0; i < rootSize; i++)
        {
            w = rootList[i];
            auto x = w;
            int d = x->degree;
            while (A[d] != nullptr)
            {
                auto y = A[d];
                if (m_Comparator(y->key, x->key))
                {
                    auto temp = x;
                    x = y;
                    y = temp;
                }
                fib_heap_link(y, x);
                A[d] = nullptr;
                d++;
            }
            A[d] = x;
        }
        delete[] rootList;
        m_Min = nullptr;
        for (int i = 0; i < max_degree + 2; i++)
        {
            if (A[i] != nullptr)
            {
                if (m_Min == nullptr)
                {
                    m_Min = A[i]->left = A[i]->right = A[i];
                }
                else
                {
                    m_Min->left->right = A[i];
                    A[i]->left = m_Min->left;
                    m_Min->left = A[i];
                    A[i]->right = m_Min;
                    if (m_Comparator(A[i]->key, m_Min->key))
                    {
                        m_Min = A[i];
                    }
                }
            }
        }
        delete[] A;
    }

    void fib_heap_link(Node *y, Node *x)
    {
        y->left->right = y->right;
        y->right->left = y->left;
        if (x->child != nullptr)
        {
            x->child->left->right = y;
            y->left = x->child->left;
            x->child->left = y;
            y->right = x->child;
        }
        else
        {
            x->child = y;
            y->right = y;
            y->left = y;
        }
        y->p = x;
        x->degree++;
        y->mark = false;
    }

    void decrease_key(Node *x, TKey k)
    {
        Node *y;
        if (m_Comparator(x->key, k))
        {
            //error( "new key is greater than current key" );
            return;
        }
        x->key = k;
        y = x->p;
        if (y != nullptr && m_Comparator(x->key, y->key))
        {
            cut(x, y);
            cascading_cut(y);
        }
        if (m_Comparator(x->key, m_Min->key))
        {
            m_Min = x;
        }
    }

    void cut(Node *x, Node *y)
    {
        if (x->right == x)
        {
            y->child = nullptr;
        }
        else
        {
            x->right->left = x->left;
            x->left->right = x->right;
            if (y->child == x)
            {
                y->child = x->right;
            }
        }
        y->degree--;
        m_Min->right->left = x;
        x->right = m_Min->right;
        m_Min->right = x;
        x->left = m_Min;
        x->p = nullptr;
        x->mark = false;
    }

    void cascading_cut(Node *y)
    {
        auto z = y->p;
        if (z != nullptr)
        {
            if (y->mark == false)
            {
                y->mark = true;
            }
            else
            {
                cut(y, z);
                cascading_cut(z);
            }
        }
    }

    void remove_Node(Node *x)
    {
        decrease_key(x, std::numeric_limits<TKey>::min());
        Node *fn = extract_min();
        delete fn;
    }
};