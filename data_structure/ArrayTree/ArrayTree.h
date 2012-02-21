// #############################################################################
// # FileName:  ArrayTree.h
// # Author:    Zhang Li, CS0901, HUST
// #############################################################################
/*
 * ArrayTree.h: An implementation of the array tree data structure.
 *
 * Copyright (C) 2010 by Zhang Li.
 *
 * This file is part of free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 */
#ifndef ARRAYTREE_H_INCLUDED
#define ARRAYTREE_H_INCLUDED

#include <stdlib.h>
#include <string.h>
#include <time.h>

struct ArrayTreeNode
{
    void* m_key;
    void* m_data;
    int m_deleted;
    int m_deletedSize;
    int m_size;
    int m_capacity;
    int m_index;
    struct ArrayTreeNode* m_parent;
    struct ArrayTreeNode** m_children;
};
struct ArrayTree
{
    int (*m_dataComparator)(const void* data1, const void* data2);
    int m_keySize;
    int m_dataSize;
    int m_arrayTreeSize;
    struct ArrayTreeNode* m_rootNode;
};

struct ArrayTree* ArrayTreeCreate(
        int keySize,
        int dataSize,
        int (*dataComparator)(const void* data1, const void* data2));
void ArrayTreeDestroy(struct ArrayTree* tree);

int ArrayTreeGetSize(struct ArrayTree* tree);
void* ArrayTreeNodeData(struct ArrayTreeNode* node);

struct ArrayTreeNode* ArrayTreeInsert(
        struct ArrayTree* tree, void* key, void* data);
void ArrayTreeDeleteNode(struct ArrayTree* tree, struct ArrayTreeNode* node);
struct ArrayTreeNode* ArrayTreeSearch(struct ArrayTree* tree, void* key);

struct ArrayTreeNode* ArrayTreeIterateHead(struct ArrayTree* tree);
struct ArrayTreeNode* ArrayTreeIterateTail(struct ArrayTree* tree);
struct ArrayTreeNode* ArrayTreeIterateNext(
        struct ArrayTree* tree, struct ArrayTreeNode* node);
struct ArrayTreeNode* ArrayTreeIteratePrev(
        struct ArrayTree* tree, struct ArrayTreeNode* node);

#endif
// #############################################################################
// # EOF
