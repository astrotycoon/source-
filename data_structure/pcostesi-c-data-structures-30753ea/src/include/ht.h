/*
 *      ht.h
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


#ifndef __HTH
#define __HTH 1

#include <stddef.h>

typedef unsigned (*ht_hashf)(void *s, size_t size);
typedef int (*ht_eachf)(const void * k, size_t ks, const void * v, size_t vs, void * d);
typedef struct Hashtable ht;

void    ht_free(ht * t);
ht *    ht_new(ht_hashf f);
size_t  ht_get(ht * t, void * key, size_t s, void * buffer, size_t size);
void *  ht_aget(ht * t, void * key, size_t s, size_t * size);
ht *    ht_set(ht * t, void * key, size_t s, void * buffer, size_t size);
ht *    ht_del(ht * t, void * key, size_t s);
ht *    ht_update(ht * t, void * key, size_t s, void * buffer, size_t size);
float   ht_set_low(ht * t, float ratio);
float   ht_set_high(ht * t, float ratio);
int     ht_set_min(ht * t, size_t size);
int     ht_set_max(ht * t, size_t size);
int     ht_each(ht * t, ht_eachf f, void * d);
#endif
