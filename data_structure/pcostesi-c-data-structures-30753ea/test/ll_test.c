/*
 *      ll_test.c
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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ll.h"
#include "CUnit/Basic.h"

#define BUFFER_SIZE 64

int init_ll_suite(void) {
    return 0;
}


int clean_ll_suite(void) {
    return 0;
}

int print_content(size_t size, int i, const void * v, void * d){
    printf("[%d]>> %*s\n", i, (int) size, (const char *) v);
    return 0;
}

void test_ll(void)
{
    /* TODO: CU_ASSERT me */

    llist * list = NULL;
    llist * list2 = NULL;
    char *haiku[] = {"this is unix foo",
                     "ll_get the data",
                     "buffer explodes."};
    list = ll_insert(list, haiku[0], strlen(haiku[0]) + 1);
    ll_append(list, haiku[1], strlen(haiku[1]) + 1);
    ll_append(list, haiku[2], strlen(haiku[2]) + 1);

    ll_update(ll_prev(ll_tail(list)), "ll_update tests", \
            strlen("ll_update tests") + 1);

    list2 = list;
    list = ll_remove(list);

    ll_free(list2);
    ll_free(list);
}
