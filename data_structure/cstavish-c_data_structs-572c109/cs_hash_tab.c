/*
Copyright (c) 2011, Coleman Stavish
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
  * Redistributions of source code must retain the above copyright
	notice, this list of conditions and the following disclaimer.
  * Redistributions in binary form must reproduce the above copyright
	notice, this list of conditions and the following disclaimer in the
	documentation and/or other materials provided with the distribution.
  * Neither the name of Coleman Stavish nor the
	names of contributors may be used to endorse or promote products
	derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL COLEMAN STAVISH BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "cs_hash_tab.h"

static size_t hash_(const char *k) {
    size_t hash = 0;
    while (*k) {
        hash = hash * 101 + *k++;
    }
    return hash;
}
static inline void destroy_node_(cs_knode *node, uint8_t free_key) {
    if (free_key)
        free(node->key);
    free(node);
}

static inline void install_(cs_knode **buckets, cs_knode *node, size_t index) {
    if (buckets[index] == NULL) {
        buckets[index] = node;
        node->next = NULL;
    }
    else {
        cs_knode *n = buckets[index];
        node->next = n;
        buckets[index] = node;
    }
}

// allocate new bucket array; rehash keys and populate buckets; free old bucket array
static void resize_(cs_hash_tab *tab, float factor) {
    size_t size = (int)(factor * tab->size);
    if (size == 0)
        size = 1;
    cs_knode **buckets = calloc(sizeof(cs_knode *) * size, 1);
    for (int i = 0; i < tab->size; i++) {
        for (cs_knode *n = tab->buckets[i]; n != NULL;) {
            size_t index = tab->hash(n->key) % size;
            // install_() alters member 'next'; preserve it for now
            cs_knode *temp = n->next;
            install_(buckets, n, index);
            n = temp;
        }
    }
    tab->size = size;
    free(tab->buckets);
    tab->buckets = buckets;
}

static inline cs_knode *lookup_i_(cs_hash_tab *tab, const char *key, size_t index) {
    for (cs_knode *n = tab->buckets[index]; n != NULL; n = n->next) {
        if (strcmp(key, n->key) == 0)
        return n;
    }
    return NULL;
}

static void map_(cs_hash_tab *tab, const char *key, void *val) {
    size_t index = tab->hash(key) % tab->size;
    cs_knode *n = lookup_i_(tab, key, index);
    
    // a value is already defined for this key
    // cleanup old; assign new
    if (n != NULL) {
        if (tab->cleanup)
            tab->cleanup(n->key, n->val);
        n->val = val;
        return;
    }
        
    cs_knode *node = malloc(sizeof(cs_knode));
    node->key = (tab->copy_keys ? strdup(key) : (char *)key);
    node->val = val;
    
    install_(tab->buckets, node, index);
    
    if (tab->count++ / (float)tab->size >= tab->max_load)
        resize_(tab, 1.5f);
}

static void *delete_(cs_hash_tab *tab, const char *key) {
    size_t index = tab->hash(key) % tab->size;
    cs_knode *n, *p;
    uint32_t i = 0;
    for (n = p = tab->buckets[index]; n != NULL; p = n, n = n->next) {
        if (strcmp(key, n->key) == 0) {
            p->next = n->next;
            // need to specifically set the "de-facto" list head to NULL if the element to be removed is the last one in its bucket
            if (i++ == 0 && n->next == NULL)
                tab->buckets[index] = NULL;
            void *data = n->val;
            destroy_node_(n, tab->free_keys);
            
            if (tab->count-- / (float)tab->size <= tab->min_load)
                resize_(tab, 0.5f);
            
            return data;
        }   
    }
    return NULL;
}

static void print_(cs_hash_tab *tab) {
    printf("hash %p: keys: { ", tab);
    for (int i = 0; i < tab->size; i++) {
        for (cs_knode *n = tab->buckets[i]; n != NULL; n = n->next) {
            printf("%s%s ", n->key, (tab->count - i > 1) ? "," : "");
        }
    }
    printf("}\n");
}

void cs_hash_set(cs_hash_tab *tab, const char *key, void *val) {
    map_(tab, key, val);
}

void *cs_hash_get(cs_hash_tab *tab, const char *key) {
    cs_knode *n = lookup_i_(tab, key, tab->hash(key) % tab->size);
    if (n == NULL)
        return NULL;
    return n->val;
}

void *cs_hash_del(cs_hash_tab *tab, const char *key) {
    void *val = delete_(tab, key);
    if (tab->cleanup)
        tab->cleanup(key, val);
    return val;
}

cs_hash_tab *cs_hash_create_opt(size_t initial, float max_load, float min_load) {
    cs_hash_tab *tab = malloc(sizeof(cs_hash_tab));
    if (tab == NULL)
        return NULL;
    tab->count = 0;
    tab->size = initial;
    tab->max_load = max_load;
    tab->min_load = min_load;
    tab->cleanup = NULL;
    tab->hash = hash_;
    tab->free_keys = tab->copy_keys = 1;
    tab->buckets = calloc(sizeof(cs_knode *) * initial, 1);
    if (tab->buckets == NULL) {
        free(tab);
        return NULL;
    }
    return tab;
}

cs_hash_tab *cs_hash_create(void) {
    return cs_hash_create_opt(8, 0.75, 0.25);
}

cs_hash_tab *cs_hash_create_kv(const char *key, void *val, ...) {
    cs_hash_tab *t = cs_hash_create();
    if (t == NULL)
        return NULL;
    
    va_list v;
    va_start(v, val);
    
    while (key && val) {
        cs_hash_set(t, key, val);
        key = va_arg(v, const char *);
        val = va_arg(v, void *);
    }
    
    va_end(v);
    
    return t;
}

void cs_hash_iterate(cs_hash_tab *tab, void (*for_each)(cs_hash_tab *, const char *, void *, size_t)) {
    size_t count = 0;
    for (int i = 0; i < tab->size; i++) {
        for (cs_knode *n = tab->buckets[i]; n != NULL; n = n->next) {
            for_each(tab, n->key, n->val, count++);
        }
    }
}

void cs_hash_destroy(cs_hash_tab *tab) {
    for (int i = 0; i < tab->size; i++) {
        for (cs_knode *n = tab->buckets[i]; n != NULL; n = n->next) {
            if (tab->cleanup)
                tab->cleanup(n->key, n->val);
            destroy_node_(n, tab->free_keys);
        }
    }
    free(tab->buckets);
    free(tab);
}
