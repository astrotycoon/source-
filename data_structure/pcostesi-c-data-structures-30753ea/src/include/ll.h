/*
 *      ll.h
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


#ifndef __LLH
#define __LLH 1

#include <stddef.h>

typedef struct  List llist;

typedef int (*ll_filter_f)(const void *, size_t);
typedef int (*ll_eachf)(size_t s, int item, const void * v, void * d);


llist * ll_append(llist * list, const void * val, size_t size);
llist * ll_remove(llist * list);
llist * ll_new(const void * val, size_t size);
llist * ll_insert(llist * list, const void * val, size_t size);
llist * ll_update(llist * list, const void * val, size_t size);
llist * ll_appendl(llist * list, llist * n);
llist * ll_next(llist * list);
llist * ll_prev(llist * list);
llist * ll_tail(llist * list);
llist * ll_head(llist * list);
size_t  ll_get(llist * list, void * buffer, size_t size);
void    ll_free(llist * list);
llist * ll_filter(llist * list, ll_filter_f * f);
llist * ll_split(llist * list, ll_filter_f * f);
int     ll_each(llist * list, ll_eachf f, void * d);
#endif
