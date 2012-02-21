/*
 *      bst_test.c
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
#include "bst.h"
#include "CUnit/Basic.h"

#define BUFFER_SIZE 64


int init_bst_suite(void) {
    return 0;
}


int clean_bst_suite(void) {
    return 0;
}

void test_bst (void) {

    char * test = "this is a test. Let's see if it works!";
    char buffer[BUFFER_SIZE];
    size_t sbuffer;
    bstnode * root = NULL;

    /* set a few bstnodes to test the tree:
     *  - Add adds an element, and fails when it already exists;
     *  - Set adds an element, updating it if already exists;
     *  - Update only updates existing elements. */

    char * expected = "1234567890";

    root = bst_set(root, "test", test, strlen(test) + 1);
    bst_set(root, "test2", expected, 5);
    bst_set(root, "test3", "aucdef", 2);

    /* add should fail with existing keys */
    CU_ASSERT(bst_add(root, "test", NULL, 0) == NULL);

    bst_update(root, "test3", "abcdef", 4);

    bst_nearest(root, "test2111", buffer, 2);
    CU_ASSERT( root != NULL );

    root = bst_delete(root, "test");
    bst_free(root);
}

