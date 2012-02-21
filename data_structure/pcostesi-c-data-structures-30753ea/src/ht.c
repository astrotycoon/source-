/*
 *      ht.c
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


#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "ht.h"

#define LOW 0.4
#define HIGH 0.75
#define MAX ULONG_MAX
#define HT_MINSIZE (1 << 8)
/* I should *really* use an enum with primes for each table size */
#define HASH(T, K, S)	((T)->hash_f((K), (S)) % (T)->buckets_size)
#define MIN(A, B)		((A) < (B) ? (A) : (B))

typedef struct KVPair kv;

static unsigned int PRIMES[] = {  0, 0, 0, 0, 0, 53, 97, 193, 389, 769, \
                                1543, 3079, 6151, 12289, 24593, 49157, \
                                98317, 196613, 393241, 786433, 1572869, \
                                3145739, 6291469, 12582917, 25165843, \
                                50331653, 100663319, 201326611, 402653189, \
                                805306457, 1610612741};

struct KVPair{
    void * key;
    size_t key_len;
    void * val;
    size_t size;
    struct KVPair * next;
};

struct Hashtable{
    ht_hashf hash_f;
    kv ** buckets;
    size_t buckets_size;
    size_t used;
    size_t min;
    size_t max;
    float low;
    float high;
    unsigned int write_lock:1;
};

/* (See Chapter 6 @ K&R2) */
static unsigned hash(void * k, size_t s)
{
    unsigned r = 0;
    for (; s > 0 && *((char *) k) != 0; k++, s--)
        r = *((char *) k) + 31 * r;
    return r % UINT_MAX;
}

static int key_is_node(void * key, size_t size, kv * node){
	assert(node != NULL);
	
	if (size != node->key_len)
		return 0;
	return !memcmp(key, node->key, MIN(size, node->key_len));
}

static void kv_free_node(kv * elem){
	
    free(elem->val);
    free(elem->key);
    free(elem);
}

static void kv_free_list(kv * list){
    kv * aux = NULL;
    
    for (; list != NULL; list = aux){
        aux = list->next;
        kv_free_node(list);
    }
}

static kv * _walk(kv * list, void * key, size_t n){
    
    while(list != NULL && !key_is_node(key, n, list))
        list = list->next;
    
    return list;
}

static kv * _new_kv(void * key, size_t ks, void * val, size_t size){
    kv * n = NULL;

    n = malloc(sizeof(kv));
    if (n == NULL)
        goto nod_cleanup;

	n->key_len = (ks == 0) ? strlen((char *) ks) : ks;
    n->key = malloc(n->key_len);
    if(n->key == NULL)
        goto str_cleanup;

    n->size = size == 0 ? strlen(val) + 1 : size;
    n->val = malloc(n->size);
    if(n->val == NULL)
        goto val_cleanup;

    memcpy(n->key, key, n->key_len);
    memcpy(n->val, val, n->size);
    n->next = NULL;
    return n;

    /* This is a pattern called RAII
     * (and cross platform. Using __attribute__((cleanup(n)))
     * is gcc-dependent and not exactly what we want) */
    val_cleanup:
        free(n->key);
    str_cleanup:
        free(n);
    nod_cleanup:
        return NULL;
}

static kv * _append(kv * list, void * key, size_t s, void * val, size_t size){
    kv * n = _new_kv(key, s, val, size);
    if (n != NULL)
        n->next = list;
    return n;
}

static int _update(kv * elem, void * val, size_t size){
    void * aux = NULL;
    if (elem == NULL)
        return 0;

    size = size == 0 ? strlen(val) + 1 : size;
    aux = realloc(elem->val, size);
    if(aux == NULL)
        return 0;
    elem->val = aux;
    elem->size = size;
    memcpy(elem->val, val, size);
    return 1;
}

static void _reinsert(ht * t, kv ** list, kv * pair, size_t newsize){
	
    unsigned hash = t->hash_f(pair->key, pair->key_len) % newsize;
    pair->next = list[hash];
    list[hash] = pair;
}

static void _rehash_all(ht * t, kv ** newlist, size_t newsize){
    kv * item = NULL;
    kv * next = NULL;
    int i = 0;
    
    t->write_lock = 1;
    for (i = 0; i < t->buckets_size; i++){
        for (item = t->buckets[i]; item != NULL; item = next){
            next = item->next;
            _reinsert(t, newlist, item, newsize);
        }
    }
    t->write_lock = 0;
}

static ht * _resize(ht * t){
    kv ** newlist = NULL;
    float ratio = t->used / t->buckets_size;
    size_t newsize = 0;

    if ((t->low < ratio && ratio < t->high) || /* between low and high */ \
        (t->low > ratio && t->min > t->used) || /* less than min elements */ \
        (t->high < ratio && t->max < t->used)) /* more than max elements */
        return t;

    newsize = 2 * t->used / (t->low + t->high) + 1;
    newlist = calloc(newsize, sizeof(kv **));
    if (newlist == NULL)
        return t;
    _rehash_all(t, newlist, newsize);
    free(t->buckets);
    t->buckets = newlist;
    t->buckets_size = newsize;
    return t;
}

static kv * _del(kv * list, void * key, size_t s){
    kv * orig = list;
    kv * prev = NULL;
    kv * ret = NULL;
    
    for (; list != NULL && !key_is_node(key, s, list); list = list->next)
        prev = list;

    if (list != NULL){
        if (orig == list){
            ret = list->next;
            kv_free_node(list);
            return ret;
        } else {
            ret = orig;
            prev->next = list->next;
        }
    }
    return NULL;
}

ht * ht_new(ht_hashf f){
    ht * t = malloc(sizeof(ht));
    if (t == NULL)
        return NULL;

    t->buckets = calloc(HT_MINSIZE, sizeof(kv *));
    if(t->buckets == NULL){
        free(t);
        return NULL;
    }

    t->buckets_size = HT_MINSIZE;
    t->used = 0;
    t->write_lock = 0;
    t->min = HT_MINSIZE;
    t->low = LOW;
    t->high = HIGH;
    t->max = MAX;
    t->hash_f = f ? f : hash;
    return t;
}

void * ht_aget(ht * t, void * key, size_t s, size_t * size){
    kv * list = NULL;
    unsigned hash = HASH(t, key, s);
    void * data = NULL;
    size_t bytes = 0;

	s = s == 0 ? strlen((char *) key) : s;
    list = _walk(t->buckets[hash], key, s);
    if (list == NULL)
        return NULL;

    if (size){
        *size = (*size > list->size || *size == 0) ? list->size : *size;
        bytes = *size;
    } else
        bytes = list->size;

    data = malloc(bytes);
    if (!data)
        return NULL;

    return memcpy(data, list->val, bytes);
}

size_t ht_get(ht * t, void * key, size_t s, void * buffer, size_t size){
    kv * list = NULL;
    unsigned hash = 0;
    
    s = s == 0 ? strlen((char *) key) : s;
    hash = HASH(t, key, s);
    list = _walk(t->buckets[hash], key, s);
    if (list == NULL)
        return 0;
        
    size = size > list->size ? list->size : size;
    if (size && buffer != NULL){
		memcpy(buffer, list->val, size);
		return size;
	} else {
		return list->size;
	}
}

ht * ht_set(ht * t, void * key, size_t s, void * val, size_t size){
    kv * list = NULL;
    kv * n = NULL;
    unsigned hash = 0;
    
    s = s == 0 ? strlen((char *) key) : s;
    hash = HASH(t, key, s);
    if (t->write_lock)
        return NULL;

    list = t->buckets[hash];
    if(_walk(list, key, s) != NULL)
        return NULL;
        
    n = _append(list, key, s, val, size);
    if (n == NULL)
        return NULL;
        
    t->buckets[hash] = n;
    t->used++;

    /* check for boundaries and resize if necessary */
    return _resize(t);
}

ht * ht_del(ht * t, void * key, size_t s){
    kv * list = NULL;
    unsigned hash = HASH(t, key, s);

    if (t->write_lock)
        return NULL;
	
	s = s == 0 ? strlen((char *) key) : s;
    list = _del(t->buckets[hash], key, s);
    if(list == NULL)
        return NULL;
    
    t->buckets[hash] = list;
    t->used--;

    return _resize(t);
}

ht * ht_update(ht * t, void * key, size_t s, void * buffer, size_t size){
    unsigned hash = 0;
    kv * list = NULL;
    
    s = s == 0 ? strlen((char *) key) : s;
    hash = HASH(t, key, s);
    list = _walk(t->buckets[hash], key, s);

    if(!_update(list, buffer, size))
        return NULL;

    return _resize(t);
}

void ht_free(ht * t){
    int i;
    for (i = 0; i < t->buckets_size; i++)
        kv_free_list(t->buckets[i]);
    free(t->buckets);
    free(t);
}

int ht_each(ht * t, ht_eachf f, void * d){
    kv * item = NULL;
    int i = 0;
    int n = 0;

    t->write_lock = 1;
    for (i = 0; i < t->buckets_size; i++){
        for (item = t->buckets[i]; !n && item != NULL; item = item->next){
            n = f(item->key, item->key_len, item->val, item->size, d);
        }
    }
    t->write_lock = 0;
    return n;
}

float ht_set_low(ht * t, float ratio){
    float old = t->low;
    if (0 < ratio && ratio < 1){
        t->low = ratio;
    }
    return old;
}

float ht_set_high(ht * t, float ratio){
    float old = t->high;
    if (0 < ratio && ratio < 1){
        t->high = ratio;
    }
    return old;
}

int ht_set_min(ht * t, size_t size){
    int old = t->min;
    if (size > 0)
        t->min = size;
    return old;
}

int ht_set_max(ht * t, size_t size){
    int old = t->max;
    if (size > 0)
        t->max = size;
    return old;
}

#undef HASH
#undef MIN
