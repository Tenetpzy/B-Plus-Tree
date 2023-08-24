#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "page_size.hh"
#include "storage.hh"

namespace BPT
{

template <typename K, typename V> class Leaf_Node;

template <typename Key, typename Val>
struct Leaf_Link
{
    using leaf_node_t = Leaf_Node<Key, Val>;

    leaf_node_t *prev;
    leaf_node_t *next;

    Leaf_Link(leaf_node_t *prev = nullptr, leaf_node_t *next = nullptr)
    {
        this->prev = prev;
        this->next = next;
    }
};

template <typename Key, typename Val>
class Leaf_Node
{
public:
    

protected:
    std::vector<Key> keys;
    std::vector<Val> vals;
    Leaf_Link<Key, Val> link;

protected:
    constexpr size_t get_max_element_num()
    {
        return Leaf_Node_Storage_Attr<Key, Val>::max_element_num;
    }
};

template <typename Key>
class Inner_Node
{
public:

protected:
    
};

template <typename Key, typename Val>
class B_Plus_Tree
{
public:

protected:

    template <typename K, typename V> class Leaf_Node;
    using leaf_node_t = Leaf_Node<Key, Val>;

    struct Leaf_Link
    {
        leaf_node_t *prev;
        leaf_node_t *next;

        Leaf_Link(leaf_node_t *prev = nullptr, leaf_node_t *next = nullptr)
        {
            this->prev = prev;
            this->next = next;
        }
    };

    template <typename K, typename V>
    class Leaf_Node
    {
    public:

    private:


    };

    constexpr static const size_t max_element_num = (PAGE_SIZE - sizeof(Leaf_Link)) / (sizeof(Key) + sizeof(Val)); 
};

}