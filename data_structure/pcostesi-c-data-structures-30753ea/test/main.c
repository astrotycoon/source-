/*
 *      main.c
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

#include "CUnit/Basic.h"

int init_bst_suite(void);
int init_ll_suite(void);
int init_ht_suite(void);

void test_bst(void);
void test_ht(void);
void test_ll(void);

int clean_bst_suite(void);
int clean_ll_suite(void);
int clean_ht_suite(void);

int main(int argc, char **argv)
{
    /* Initialize the CUnit test registry */
    if ( CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* add a suite to the registry */
    CU_pSuite bstSuite = CU_add_suite("BST Suite", init_bst_suite, clean_bst_suite);
    CU_pSuite llSuite = CU_add_suite("LinkedList Suite", init_ll_suite, clean_ll_suite);
    CU_pSuite htSuite = CU_add_suite("HashTable Suite", init_ht_suite, clean_ht_suite);

    if ( NULL == bstSuite || NULL == llSuite || NULL == htSuite ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /*add the test cases to the suites */
    if ((NULL == CU_add_test(bstSuite, "test_bst", test_bst)) || 
        (NULL == CU_add_test(llSuite, "test_ll", test_ll)) ||
        (NULL == CU_add_test(htSuite, "test_ht", test_ht)) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
