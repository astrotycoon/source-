/*
 *      bstADT.c
 *
 *      Copyright 2010:
 *          Pablo Alejandro Costesich <pcostesi@alu.itba.edu.ar>
 *
 *      Redistribution and use in source and binary forms, with or without
 *      modification, are permitted provided that the following conditions are
 *      met:
 *
 *      * Redistributions of source code must retain the above copyright
 *        notice, this list of conditions and the following disclaimer.
 *      * Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following disclaimer
 *        in the documentation and/or other materials provided with the
 *        distribution.
 *      * Neither the name of the Owner nor the names of its
 *        contributors may be used to endorse or promote products derived from
 *        this software without specific prior written permission.
 *
 *      THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *      "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *      LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *      A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *      OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *      SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *      LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *      DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *      THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *      (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *      OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stddef.h>
#include "bst.h"


#define BST_LEFT 1
#define BST_RIGHT 0
#define SET 0
#define ADD 1

struct bstnode{
    char * str;
    void * val;
    size_t size;
    struct bstnode * left;
    struct bstnode * parent;
    struct bstnode * right;
};

static bstnode * new_bstnode(char * key, void * val, size_t size);
static bstnode * insert(bstnode * root, bstnode * n);
static bstnode * minormax(bstnode * root, int i);
static bstnode * _delete(bstnode *root, bstnode * n);
static bstnode * _update(bstnode *n, char * key, void * val, size_t size);
static bstnode * _set(bstnode *root, char * key, void * val, size_t size, int act);
static void swapData(bstnode * target, bstnode * source);


void bst_dispose(bstnode * n){
    if (n != NULL){
        free(n->val);
        free(n->str);
    }
    free(n);
}

void bst_free(bstnode * n){
    if (n != NULL){
        bst_free(n->left);
        bst_free(n->right);
        bst_dispose(n);
    }
}

static bstnode * new_bstnode(char * key, void * val, size_t size){
    bstnode *n = NULL;
    char * str = NULL;
    void * mem = NULL;

    size = size == 0 ? strlen(val) + 1 : size;
    mem = malloc(size);

    if (mem == NULL) return NULL;

    str = malloc(strlen(key) + 1);
    if (str == NULL){
        free(mem);
        return NULL;
     }

    n = malloc(sizeof(struct bstnode));
    if (n == NULL){
        free(mem);
        free(str);
    } else {
        n->str = str;
        strcpy(str, key);
        n->val = memcpy(mem, val, size);
        n->size = size;
        n->left = n->right = NULL;
    }
    
    return n;
}

static bstnode * insert(bstnode *parent, bstnode *n){
    int cmp = 0;

    if (parent != NULL && n != NULL){
        cmp = strcmp(parent->str, n->str);
        if (cmp >= 0)
            parent->left = insert(parent->left, n);
        else if (cmp < 0)
            parent->right = insert(parent->right, n);
        n->parent = parent;
        return parent;
    }
    return n;

}

static bstnode * _update(bstnode *n, char * key, void * val, size_t size){
    void *aux = NULL;
    
    size = size == 0 ? strlen(val) + 1 : size;
    
    aux = realloc(n->val, size);
    if (val == NULL)
		return NULL;
    
    memcpy(aux, val, size);
    n->val = aux;
    n->size = size;
    
    return n;
}

bstnode * bst_update(bstnode *root, char * key, void * val, size_t size){
    bstnode * n = NULL;
    
    n = bst_search(root, key);
    if (n != NULL)
        n = _update(n, key, val, size);
    
    return n;
}

bstnode * bst_add(bstnode *root, char * key, void * val, size_t size){
    return _set(root, key, val, size, ADD);
}

bstnode * bst_set(bstnode *root, char * key, void * val, size_t size){
    return _set(root, key, val, size, SET);
}

/* Although add and set could be `#define'd, you wouldn't be able to use
 * pointers to functions. */
static bstnode * _set(bstnode *root, char * key, void * val, size_t size, int act){
    bstnode * n = NULL;
    
    n = bst_search(root, key);
    
    if (n == NULL){
        n = new_bstnode(key, val, size);
        if (n != NULL){
            insert(root, n);
        }
    } else {
		n = (act == SET) ? _update(n, key, val, size) : NULL;
    }
    
    return n;
}

bstnode * bst_search(bstnode *root, char * key)
{
    int cmp = 0;

    if (root == NULL)
		return NULL;
		
	cmp = strcmp(root->str, key);
	
	if (cmp > 0)
		return bst_search(root->left, key);

	else if (cmp < 0)
		return bst_search(root->right, key);

	else
		return root;
}

bstnode * bst_minimum(bstnode * root){
    return minormax(root, BST_LEFT);
}

bstnode * bst_maximum(bstnode * root){
    return minormax(root, BST_RIGHT);
}

static bstnode * minormax(bstnode * root, int i){
    bstnode *aux = root;
    
    while (aux != NULL){
        aux = (i == BST_RIGHT) ? aux->right : aux->left;
        root = (aux == NULL) ? root : aux;
    }
    return root;
}

size_t bst_nearest(bstnode * root, char * key, void * d, size_t s){
    size_t bytes = 0;
    bstnode * nearest = NULL;
    bstnode * prev = NULL;
    bstnode * next = NULL;
    int cmp = 0;

    if (root == NULL)
        return 0;
        
    cmp = strcmp(root->str, key);
    if (cmp > 0)
        bytes = bst_nearest(root->left, key, d, s);
    else if (cmp < 0)
        bytes = bst_nearest(root->right, key, d, s);
    else
        nearest = root;

    if (nearest == NULL && bytes == 0){
		prev = bst_maximum(root->right);
		next = bst_minimum(root->left);
		
        if (prev && next)
            nearest = strcmp(prev->str, next->str) <= 0 ? prev : next;        
        else if (root->left)
            nearest = bst_minimum(root->left);
        else
			nearest = bst_maximum(root->right);
    }

    return nearest != NULL ? bst_node_content(nearest, d, s) : 0;
}

static void swapData(bstnode * target, bstnode * source)
{
    bstnode tmp;
    /* copy the struct until we hit `left' */
    memcpy(&tmp, target, offsetof(bstnode, left));
    memcpy(target, source, offsetof(bstnode, left));
    memcpy(target, &tmp, offsetof(bstnode, left));
}

bstnode * bst_delete(bstnode * root, char * key){
    bstnode * n = bst_search(root, key);
    return _delete(root, n);
}

static bstnode * _delete(bstnode * root, bstnode * n){
    bstnode * c = NULL;
    
    if(n->left == NULL && n->right == NULL){
        if (n == root)
            root = NULL;
        bst_dispose(n);
    } else if (n->left != NULL && n->right != NULL){
        c = bst_minimum(n->right);
        swapData(n, c);
        _delete(root, c);
    } else {
        c = n->left == NULL? n->right : n->left;
        if (root == n)
            root = c;
        c->parent = n->parent;
        bst_dispose(n);
    }
    return root;
}


size_t bst_node_size(bstnode *n)
{
    return n->size;
}

size_t bst_node_content(bstnode * n, void * d, size_t s)
{
    if (n != NULL && d != NULL){
        s = s == 0 || n->size < s ? n->size : s;
        memcpy(d, n->val, s);
        return s;
    }
    return 0;
}


/**
 * get searches for an entry with a given key and returns the bstnode. */
size_t bst_get(bstnode * r, char * key, void * d, size_t s){
    if(r == NULL)
        return 0;
    
    return bst_node_content(bst_search(r, key), d, s);
}

#undef BST_LEFT
#undef BST_RIGHT
#undef SET
#undef ADD
