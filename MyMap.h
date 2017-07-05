//
//  mymap.hpp
//  project 4
//
//  Created by Katie Cai on 3/9/17.
//  Copyright © 2017 Katie Cai. All rights reserved.
//

#ifndef MyMap_h
#define MyMap_h

#include <stdio.h>

template<typename KeyType, typename ValueType>

class MyMap
{
public:
    MyMap() {
        m_size = 0;
        m_root = nullptr;
    }
    
    ~MyMap() {
        clear();
    }
    
    void clear() {
        clearHelper(m_root);
        m_size = 0;
    }
    
    int size() const {
        return m_size;
    }
    
    void associate(const KeyType& key, const ValueType& value) {
        if (m_root == nullptr) {
            m_root = new Node(key, value);
            m_size++;
            return;
        }
        
        Node* cur = m_root;
        
        for (;;) {
            if (key == cur->m_key) {
                cur->m_value = value;
                return;
            }
            if (key < cur->m_key) {
                if (cur->m_left != nullptr)
                    cur = cur->m_left;
                else {
                    cur->m_left = new Node(key, value);
                    m_size++;
                    return;
                }
            }
            else if (key > cur->m_key) {
                if (cur->m_right != nullptr)
                    cur = cur->m_right;
                else {
                    cur->m_right = new Node(key, value);
                    m_size++;
                    return;
                }
            }
        }
    }
    
    // for a map that can't be modified, return a pointer to const ValueType
    const ValueType* find(const KeyType& key) const {
        Node* cur = m_root;
        while (cur != nullptr) {
            if (key == cur->m_key)
                return &(cur->m_value);
            else if (key < cur->m_key)
                cur = cur->m_left;
            else
                cur = cur->m_right;
        }
        return nullptr;
    }
    
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
        Node(const KeyType& key, const ValueType& value) {
            m_key = key;
            m_value = value;
            m_right = nullptr;
            m_left = nullptr;
        }
        KeyType m_key;
        ValueType m_value;
        Node* m_right;
        Node* m_left;
    };
    int m_size;
    Node* m_root;
    
    void clearHelper(Node* cur) {
        if (cur == nullptr)
            return;
        clearHelper(cur->m_left);
        clearHelper(cur->m_right);
        delete cur;
    }
};

#endif /* MyMap_h */
