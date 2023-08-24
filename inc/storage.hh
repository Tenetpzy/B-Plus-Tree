#pragma once

#include <cstddef>
#include <string>

#include "page_size.hh"

namespace BPT
{

enum class Node_Type: uint64_t
{
    Inner, Leaf
};

struct Node_Storage_Header
{
    Node_Type node_type;
    size_t cur_element_num;
};

// 将内存中的Key类型Ram_Key转化为硬盘中存储的Key类型Storage_Key_Type
template <typename Ram_Key>
struct ram_key_to_storage_key
{
    using Storage_Key_Type = Ram_Key;
};

/*
 * 针对Key为字符串类型，需要特化。因为字符串是变长的，直接存储在节点中难以管理，
 * 所以在文件中使用一块特定区域存储所有字符串，节点中都保存字符串的文件偏移(使用uint64_t)
 */
template <>
struct ram_key_to_storage_key<std::string>
{
    using Storage_Key_Type = uint64_t;
};

// 将内存中的Value类型转化为硬盘中存储的Value类型，通常两种类型相同
template <typename Ram_Value>
struct ram_val_to_storage_val
{
    using Storage_Val_Type = Ram_Value;
};  

/*
 * 描述叶节点的存储属性
 * 对于内存中以Ram_Key类型为键，以Ram_Val类型为值的叶节点，在硬盘上存储时，
 * 键类型为Key_Storage_Type，值类型为Val_Storage_Type，键和值都按字节序列化，
 * 节点内的最大元素个数为max_element_num
 */
template <typename Ram_Key, typename Ram_Val>
struct Leaf_Node_Storage_Attr
{
    using Key_Storage_Type = typename ram_key_to_storage_key<Ram_Key>::Storage_Key_Type;
    using Val_Storage_Type = typename ram_val_to_storage_val<Ram_Val>::Storage_Val_Type;
    constexpr static size_t max_element_num = (PAGE_SIZE - sizeof(Node_Storage_Header)) / 
        (sizeof(Key_Storage_Type) + sizeof(Val_Storage_Type)); 
};

/* 叶节点在硬盘上的结构 */
template <typename Ram_Key, typename Ram_Val>
struct Leaf_Node_Storage_Struct
{
    using Storage_Attr_t = Leaf_Node_Storage_Attr<Ram_Key, Ram_Val>;

    Node_Storage_Header header;
    typename Storage_Attr_t::Key_Storage_Type keys[Storage_Attr_t::max_element_num];
    typename Storage_Attr_t::Val_Storage_Type vals[Storage_Attr_t::max_element_num];
};

/*
 * 描述内部节点的存储属性
 * 对于内存中以Ram_Key类型为键的内部节点，在硬盘上存储时，
 * 键类型存储为Key_Storage_Type，使用Storage_Pointer_Type表示子节点的文件偏移(或页号等)，
 * 节点内的最大元素个数为max_element_num，最大指针个数为max_element_num + 1
 */
template <typename Ram_Key>
struct Inner_Node_Storage_Attr
{
    using Key_Storage_Type = typename ram_key_to_storage_key<Ram_Key>::Storage_Key_Type;
    using Storage_Pointer_Type = uint64_t;  // 存储节点的位置的类型
    constexpr static size_t max_element_num = (PAGE_SIZE - sizeof(Node_Storage_Header) - sizeof(Storage_Pointer_Type)) / 
        (sizeof(Key_Storage_Type) + sizeof(Storage_Pointer_Type));
};

/* 内部节点在硬盘上的结构 */
template <typename Ram_Key>
struct Inner_Node_Storage_Struct
{
    using Storage_Attr_t = Inner_Node_Storage_Attr<Ram_Key>;

    Node_Storage_Header header;
    typename Storage_Attr_t::Key_Storage_Type keys[Storage_Attr_t::max_element_num];
    typename Storage_Attr_t::Storage_Pointer_Type sub_nodes[Storage_Attr_t::max_element_num + 1];
};


}