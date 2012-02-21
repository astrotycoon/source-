// #############################################################################
// # FileName:  ArrayTree.c
// # Author:    Zhang Li, CS0901, HUST
// #############################################################################
/*
 * ArrayTree.c: An implementation of the array tree data structure.
 *
 * Copyright (C) 2010 by Zhang Li.
 *
 * This file is part of free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 */
#include "ArrayTree.h"

static void DoDestroyNode(struct ArrayTreeNode* node);
static int DoSearchChild(
        struct ArrayTree* tree, struct ArrayTreeNode* node, void* key);
static void DoAppendChild(
        struct ArrayTree* tree,
        struct ArrayTreeNode* node,
        struct ArrayTreeNode* child);
static void DoKeepBalance(struct ArrayTree* tree, struct ArrayTreeNode* node);

static struct ArrayTreeNode* DoIterateHead(struct ArrayTreeNode* rootNode);
static struct ArrayTreeNode* DoIterateTail(struct ArrayTreeNode* rootNode);
static struct ArrayTreeNode* DoIterateNext(
        struct ArrayTreeNode* rootNode, struct ArrayTreeNode* node);
static struct ArrayTreeNode* DoIteratePrev(
        struct ArrayTreeNode* rootNode, struct ArrayTreeNode* node);

struct ArrayTree* ArrayTreeCreate(
        int keySize,
        int dataSize,
        int (*dataComparator)(const void* data1, const void* data2))
{
    struct ArrayTree* newTree = malloc(sizeof(struct ArrayTree));

    // initialize the new tree
    newTree->m_keySize = keySize;
    newTree->m_dataSize = dataSize;
    newTree->m_dataComparator = dataComparator;
    newTree->m_arrayTreeSize = 0;

    // initialize the root node
    newTree->m_rootNode = malloc(sizeof(struct ArrayTreeNode));
    newTree->m_rootNode->m_key = malloc(keySize);
    newTree->m_rootNode->m_data = malloc(dataSize);
    newTree->m_rootNode->m_deleted = 0;
    newTree->m_rootNode->m_deletedSize = 0;
    newTree->m_rootNode->m_size = 0;
    newTree->m_rootNode->m_capacity = 0;
    newTree->m_rootNode->m_children = malloc(0);

    memset(newTree->m_rootNode->m_key, 0, keySize);
    memset(newTree->m_rootNode->m_data, 0, dataSize);
    return newTree;
}

void ArrayTreeDestroy(struct ArrayTree* tree)
{
    DoDestroyNode(tree->m_rootNode);
    free(tree);
    return;
}

int ArrayTreeGetSize(struct ArrayTree* tree)
{
    return tree->m_arrayTreeSize;
}

void* ArrayTreeNodeData(struct ArrayTreeNode* node)
{
    return node->m_data;
}

struct ArrayTreeNode* ArrayTreeInsert(
        struct ArrayTree* tree, void* key, void* data)
{
    struct ArrayTreeNode* currentNode = tree->m_rootNode;
    struct ArrayTreeNode* newNode = malloc(sizeof(struct ArrayTreeNode));

    // initialize the new node
    newNode->m_key = memcpy(malloc(tree->m_keySize), key, tree->m_keySize);
    newNode->m_data = memcpy(malloc(tree->m_dataSize), data, tree->m_dataSize);
    newNode->m_deleted = 0;
    newNode->m_deletedSize = 0;
    newNode->m_size = 0;
    newNode->m_capacity = 0;
    newNode->m_children = malloc(0);

    // look for the position to be inserted
    while(currentNode->m_size > 0)
    {
        int index = DoSearchChild(tree, currentNode, key);

        if(index < currentNode->m_size)
            currentNode = currentNode->m_children[index];
        else break;
    }

    // insert new node
    DoAppendChild(tree, currentNode, newNode);
    DoKeepBalance(tree, currentNode);

    tree->m_arrayTreeSize += 1;
    return newNode;
}

void ArrayTreeDeleteNode(struct ArrayTree* tree, struct ArrayTreeNode* node)
{
    // mark the node as deleted
    node->m_deleted = 1;
    node->m_parent->m_deletedSize += 1;

    // keep balance
    DoKeepBalance(tree, node);

    tree->m_arrayTreeSize -= 1;
    return;
}

struct ArrayTreeNode* ArrayTreeSearch(struct ArrayTree* tree, void* key)
{
    struct ArrayTreeNode* currentNode;
    struct ArrayTreeNode* newNode;
    int i;

    // insert a new node and mark as deleted
    newNode = ArrayTreeInsert(tree, key, tree->m_rootNode->m_data);
    newNode->m_deleted = 1;
    tree->m_arrayTreeSize -= 1;

    // search the key along the path from new node to root
    currentNode = newNode;
    while(currentNode != tree->m_rootNode)
    {
        for(i = currentNode->m_index - 1; i >= 0; i--)
        {
            if(tree->m_dataComparator(
                        currentNode->m_parent->m_children[i]->m_key, key) == 0)
            {
                if(!currentNode->m_parent->m_children[i]->m_deleted)
                {
                    return currentNode->m_parent->m_children[i];
                }
            }
            else break;
        }
        currentNode = currentNode->m_parent;
    }
    return NULL;
}

struct ArrayTreeNode* ArrayTreeIterateHead(struct ArrayTree* tree)
{
    struct ArrayTreeNode* node = DoIterateHead(tree->m_rootNode);

    while(node != tree->m_rootNode)
    {
        if(node->m_deleted)
            node = DoIterateNext(tree->m_rootNode, node);
        else return node;
    }
    return NULL;
}

struct ArrayTreeNode* ArrayTreeIterateTail(struct ArrayTree* tree)
{
    struct ArrayTreeNode* node = DoIterateTail(tree->m_rootNode);

    if(node != tree->m_rootNode)
    {
        if(node->m_deleted)
            node = DoIteratePrev(tree->m_rootNode, node);
        else return node;
    }
    return NULL;
}

struct ArrayTreeNode* ArrayTreeIterateNext(
        struct ArrayTree* tree, struct ArrayTreeNode* node)
{
    node = DoIterateNext(tree->m_rootNode, node);
    while(node != tree->m_rootNode)
    {
        if(node->m_deleted)
            node = DoIterateNext(tree->m_rootNode, node);
        else return node;
    }
    return NULL;
}

struct ArrayTreeNode* ArrayTreeIteratePrev(
        struct ArrayTree* tree, struct ArrayTreeNode* node)
{
    node = DoIteratePrev(tree->m_rootNode, node);
    while(node != tree->m_rootNode)
    {
        if(node->m_deleted)
            node = DoIteratePrev(tree->m_rootNode, node);
        else return node;
    }
    return NULL;
}

static void DoDestroyNode(struct ArrayTreeNode* node)
{
    int i;

    // destroy node and all its children
    if(node != NULL)
    {
        for(i = 0; i < node->m_size; i++)
            DoDestroyNode(node->m_children[i]);
        free(node->m_key);
        free(node->m_data);
        free(node->m_children);
        free(node);
    }
    return;
}

static int DoSearchChild(
        struct ArrayTree* tree, struct ArrayTreeNode* node, void* key)
{
    int m;
    int l = 0;
    int r = node->m_size - 1;

    // binary search for the 1st node greater than the given key
    while(l <= r)
    {
        m = (l + r) / 2;
        if(tree->m_dataComparator(key, node->m_children[m]->m_key) < 0)
            r = m - 1;
        else
            l = m + 1;
    }
    return l;
}

static void DoAppendChild(
        struct ArrayTree* tree,
        struct ArrayTreeNode* node,
        struct ArrayTreeNode* child)
{
    // resize children array if necessary
    node->m_size += 1;
    if(node->m_size > node->m_capacity)
    {
        node->m_capacity = node->m_size * 2;
        node->m_children = realloc(
                node->m_children,
                node->m_capacity * sizeof(struct ArrayTreeNode*));
    }

    // append child to the tail of children array
    node->m_children[node->m_size - 1] = child;
    child->m_parent = node;
    child->m_index = node->m_size - 1;
    return;
}

static void DoKeepBalance(struct ArrayTree* tree, struct ArrayTreeNode* node)
{
    struct ArrayTreeNode** undeletedArray;
    struct ArrayTreeNode** deletedArray;
    struct ArrayTreeNode* currentNode;
    struct ArrayTreeNode* parentNode;
    int undeletedSize = 0;
    int deletedSize = 0;
    int capacity1 = 0;
    int capacity2 = 0;
    int i;

    if(node != tree->m_rootNode)
    {
        parentNode = node->m_parent;

        if(node->m_size * 2 > node->m_parent->m_size
                || node->m_parent->m_deletedSize * 2 > node->m_parent->m_size)
        {

            // initialize two arrays, used to store all children
            undeletedArray = malloc(0);
            deletedArray = malloc(0);

            // store all children into two temp nodes
            currentNode = DoIterateHead(parentNode);
            while(currentNode != parentNode)
            {
                if(currentNode->m_deleted)
                {
                    if(deletedSize >= capacity1)
                    {
                        capacity1 = (deletedSize + 1) * 2;
                        deletedArray = realloc(deletedArray,
                                capacity1 * sizeof(struct ArrayTreeNode*));
                    }
                    deletedArray[deletedSize++] = currentNode;
                }
                else
                {
                    if(undeletedSize >= capacity2)
                    {
                        capacity2 = (undeletedSize + 1) * 2;
                        undeletedArray = realloc(undeletedArray,
                                capacity2 * sizeof(struct ArrayTreeNode*));
                    }
                    undeletedArray[undeletedSize++] = currentNode;
                }
                currentNode = DoIterateNext(parentNode, currentNode);
            }

            // destroy all deleted nodes
            for(i = 0; i < deletedSize; i++)
            {
                deletedArray[i]->m_size = 0;
                DoDestroyNode(deletedArray[i]);
            }

            // restore all undeleted nodes
            parentNode->m_deletedSize = 0;
            parentNode->m_size = 0;
            for(i = 0; i < undeletedSize; i++)
            {
                undeletedArray[i]->m_size = 0;
                DoAppendChild(tree, parentNode, undeletedArray[i]);
            }

            // free two temp nodes
            free(undeletedArray);
            free(deletedArray);

            // rebalance parent
            DoKeepBalance(tree, parentNode);
        }
    }
    return;
}

static struct ArrayTreeNode* DoIterateHead(struct ArrayTreeNode* rootNode)
{
    struct ArrayTreeNode* node = rootNode;

    // get the minimal node under the root node
    while(node->m_size > 0)
        node = node->m_children[0];
    return node;
}

static struct ArrayTreeNode* DoIterateTail(struct ArrayTreeNode* rootNode)
{
    struct ArrayTreeNode* node = rootNode;

    // get the maximal node under the root node
    while(node->m_size > 0)
        node = node->m_children[node->m_size - 1];
    return node;
}

static struct ArrayTreeNode* DoIterateNext(
        struct ArrayTreeNode* rootNode, struct ArrayTreeNode* node)
{
    // iterate next node
    if(node->m_index + 1 < node->m_parent->m_size)
        node = DoIterateHead(node->m_parent->m_children[node->m_index + 1]);
    else
        node = node->m_parent;
    return node;
}

static struct ArrayTreeNode* DoIteratePrev(
        struct ArrayTreeNode* rootNode, struct ArrayTreeNode* node)
{
    // iterate previous node
    if(node->m_index - 1 >= 0)
        node = DoIterateTail(node->m_parent->m_children[node->m_index - 1]);
    else
    {
        while(node->m_parent != rootNode)
        {
            node = node->m_parent;
            if(node->m_index - 1 >= 0)
            {
                node = node->m_parent->m_children[node->m_index - 1];
                break;
            }
        }
    }
    return node;
}
// #############################################################################
// EOF
