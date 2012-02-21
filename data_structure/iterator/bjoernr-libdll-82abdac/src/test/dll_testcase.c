/*
* Copyright (c) 2008, Björn Rehm (bjoern@shugaa.de)
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/

#include <stdio.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <CUnit/Automated.h>

#include "dll_list.h"
#include "dll_util.h"

#define CU_ADD_TEST(suite, test) (CU_add_test(suite, #test, (CU_TestFunc)test))

/* Never use any value smaller than 10 for DLL_TEST_LISTSIZE */
#define DLL_TEST_LISTSIZE   (5000)
#define DLL_TEST_GENERROR   "Unknown error"

/* Test dll_append() functionality  */
static void test_append(void) 
{
    int rc, i;
    dll_list_t list;
    void *data = NULL;

    rc = dll_init(&list);
    CU_ASSERT(rc == EDLLOK);

    /* Fill the list with numbers 1..20 */
    for(i=0;i<DLL_TEST_LISTSIZE;i++) {
        rc = dll_append(&list, &data, sizeof(int));
        CU_ASSERT(rc == EDLLOK);

        if (rc == EDLLOK)
            *((int*)data) = i+1;
    }

    /* Read back and check for consistency */
    for(i=0;i<DLL_TEST_LISTSIZE;i++) {
        rc = dll_get(&list, &data, NULL, (unsigned int)i);
        CU_ASSERT(rc == EDLLOK);
        CU_ASSERT(*((int*)data) == i+1);
    }

    rc = dll_clear(&list);
    CU_ASSERT(rc == EDLLOK);
}

/* Test dll_insert() functionality  */
static void test_insert(void) 
{
    int rc, i;
    dll_list_t list;
    unsigned int count;
    void *data = NULL;

    rc = dll_init(&list);
    CU_ASSERT(rc == EDLLOK);

    /* Fill the list by appending items */
    for(i=0;i<DLL_TEST_LISTSIZE;i++) {
        rc = dll_count(&list, &count);
        CU_ASSERT(rc == EDLLOK);

        rc = dll_insert(&list, &data, sizeof(int), count);
        CU_ASSERT(rc == EDLLOK);

        if (rc == EDLLOK)
            *((int*)data) = i+1;
    }

    /* Read back and check for consistency */
    for(i=0;i<DLL_TEST_LISTSIZE;i++) {
        rc = dll_get(&list, &data, NULL, (unsigned int)i);
        CU_ASSERT(rc == EDLLOK);
        CU_ASSERT(*((int*)data) == i+1);
    }

    /* Add an item in the middle, then check if first, last and item are OK */
    rc = dll_insert(&list, &data, sizeof(int), DLL_TEST_LISTSIZE/2);
    CU_ASSERT(rc == EDLLOK);

    if (rc == EDLLOK)
        *((int*)data) = 999;

    rc = dll_get(&list, &data, NULL, 0);
    CU_ASSERT(rc == EDLLOK);
    CU_ASSERT(*((int*)data) == 1);

    rc = dll_get(&list, &data, NULL, DLL_TEST_LISTSIZE/2);
    CU_ASSERT(rc == EDLLOK);
    CU_ASSERT(*((int*)data) == 999);

    rc = dll_count(&list, &count);
    CU_ASSERT(rc == EDLLOK);

    rc = dll_get(&list, &data, NULL, count-1);
    CU_ASSERT(rc == EDLLOK);
    CU_ASSERT(*((int*)data) == DLL_TEST_LISTSIZE);

    /* Insert invalid index */
    rc = dll_count(&list, &count);
    CU_ASSERT(rc == EDLLOK);

    rc = dll_insert(&list, &data, sizeof(int), count+1);
    CU_ASSERT(rc != EDLLOK);

    /* Start afresh */
    dll_clear(&list);

    /* Fill the list by prepending items */
    for(i=0;i<DLL_TEST_LISTSIZE;i++) {
        rc = dll_insert(&list, &data, sizeof(int), 0);
        CU_ASSERT(rc == EDLLOK);

        if (rc == EDLLOK)
            *((int*)data) = i+1;
    }

    /* Read back and check for consistency */
    for(i=0;i<DLL_TEST_LISTSIZE;i++) {
        rc = dll_get(&list, &data, NULL, (unsigned int)i);
        CU_ASSERT(rc == EDLLOK);
        CU_ASSERT(*((int*)data) == DLL_TEST_LISTSIZE-i);
    }

    rc = dll_clear(&list);
    CU_ASSERT(rc == EDLLOK);
}

/* Test dll_extend() functionality  */
static void test_extend(void) 
{
    int rc, i;
    unsigned int count;
    dll_list_t list, lext;
    void *data = NULL;

    rc = dll_init(&list);
    CU_ASSERT(rc == EDLLOK);

    rc = dll_init(&lext);
    CU_ASSERT(rc == EDLLOK);

    /* Fill the first list with numbers 1..DLL_TEST_LISTSIZE/2 */
    for(i=0;i<DLL_TEST_LISTSIZE/2;i++) {
        rc = dll_append(&list, &data, sizeof(int));
        CU_ASSERT(rc == EDLLOK);

        if (rc == EDLLOK)
            *((int*)data) = i+1;
    }

    /* Fill the second list with numbers (DLL_TEST_LISTSIZE/2)+1..DLL_TEST_LISTSIZE */
    for(i=DLL_TEST_LISTSIZE/2;i<DLL_TEST_LISTSIZE;i++) {
        rc = dll_append(&lext, &data, sizeof(int));
        CU_ASSERT(rc == EDLLOK);

        if (rc == EDLLOK)
            *((int*)data) = i+1;
    }

    /* Append lext to list */
    rc = dll_extend(&list, &lext);
    CU_ASSERT(rc == EDLLOK);

    rc = dll_count(&list, &count);
    CU_ASSERT(rc == EDLLOK);
    CU_ASSERT(count == DLL_TEST_LISTSIZE);

    rc = dll_count(&lext, &count);
    CU_ASSERT(rc == EDLLOK);
    CU_ASSERT(count == DLL_TEST_LISTSIZE/2);

    /* Read back and check for consistency */
    for(i=0;i<DLL_TEST_LISTSIZE;i++) {
        rc = dll_get(&list, &data, NULL, (unsigned int)i);
        CU_ASSERT(rc == EDLLOK);
        CU_ASSERT(*((int*)data) == i+1);
    }

    rc = dll_clear(&list);
    CU_ASSERT(rc == EDLLOK);

    rc = dll_clear(&lext);
    CU_ASSERT(rc == EDLLOK);
}

/* Test dll_deepcopy() functionality  */
static void test_deepcopy(void) 
{
    int rc, i;
    unsigned int count;
    dll_list_t from, to;
    void *data = NULL;

    rc = dll_init(&from);
    CU_ASSERT(rc == EDLLOK);

    rc = dll_init(&to);
    CU_ASSERT(rc == EDLLOK);

    /* Fill the first list with numbers 1..DLL_TEST_LISTSIZE */
    for(i=0;i<DLL_TEST_LISTSIZE;i++) {
        rc = dll_append(&from, &data, sizeof(int));
        CU_ASSERT(rc == EDLLOK);

        if (rc == EDLLOK)
            *((int*)data) = i+1;
    }

    /* Make a deep copy of from */
    rc = dll_deepcopy(&from, &to);
    CU_ASSERT(rc == EDLLOK);

    rc = dll_count(&to, &count);
    CU_ASSERT(rc == EDLLOK);
    CU_ASSERT(count == DLL_TEST_LISTSIZE);

    /* Read back and check for consistency */
    for(i=0;i<DLL_TEST_LISTSIZE;i++) {
        rc = dll_get(&to, &data, NULL, (unsigned int)i);
        CU_ASSERT(rc == EDLLOK);
        CU_ASSERT(*((int*)data) == i+1);
    }

    rc = dll_clear(&from);
    CU_ASSERT(rc == EDLLOK);

    rc = dll_clear(&to);
    CU_ASSERT(rc == EDLLOK);
}

/* Test dll_remove() functionality  */
static void test_remove(void) 
{
    int rc, i;
    unsigned int count;
    dll_list_t list;
    void *data = NULL;

    rc = dll_init(&list);
    CU_ASSERT(rc == EDLLOK);

    /* Fill the list with numbers 1..DLL_TEST_LISTSIZE */
    for(i=0;i<DLL_TEST_LISTSIZE;i++) {
        rc = dll_append(&list, &data, sizeof(int));
        CU_ASSERT(rc == EDLLOK);

        if (rc == EDLLOK)
            *((int*)data) = i+1;
    }

    /* Remove item in the middle */
    rc = dll_remove(&list, DLL_TEST_LISTSIZE/2);
    CU_ASSERT(rc == EDLLOK);

    /* Remove the last item (value DLL_TEST_LISTSIZE) */
    rc = dll_count(&list, &count);
    CU_ASSERT(rc == EDLLOK);

    rc = dll_remove(&list, count-1);
    CU_ASSERT(rc == EDLLOK);

    /* Remove the first item (value 1) */
    rc = dll_remove(&list, 0);
    CU_ASSERT(rc == EDLLOK);

    /* Check for correct number of items in the list */
    rc = dll_count(&list, &count);
    CU_ASSERT(rc == EDLLOK);
    CU_ASSERT(count == (DLL_TEST_LISTSIZE-3));

    /* First item must be 2 now */
    rc = dll_get(&list, &data, NULL, (unsigned int)0);
    CU_ASSERT(rc == EDLLOK);
    CU_ASSERT(*((int*)data) == 2);

    /* Last item must be DLL_TEST_LISTSIZE-1 */
    rc = dll_get(&list, &data, NULL, count-1);
    CU_ASSERT(rc == EDLLOK);
    CU_ASSERT(*((int*)data) == (DLL_TEST_LISTSIZE-1));

    rc = dll_clear(&list);
    CU_ASSERT(rc == EDLLOK);
}

/* Test dll_indexof() functionality  */
static void test_indexof(void) 
{
    int rc, i, cmpitem;
    unsigned int index;
    dll_list_t list;
    void *data = NULL;

    rc = dll_init(&list);
    CU_ASSERT(rc == EDLLOK);

    /* Fill the list with numbers 1..DLL_TEST_LISTSIZE */
    for(i=0;i<DLL_TEST_LISTSIZE;i++) {
        rc = dll_append(&list, &data, sizeof(int));
        CU_ASSERT(rc == EDLLOK);

        if (rc == EDLLOK)
            *((int*)data) = i+1;
    }

    /* Try to find a specific item */
    cmpitem = DLL_TEST_LISTSIZE/2;
    rc = dll_indexof(&list, dll_compar_int, (void*)&cmpitem, &index);
    CU_ASSERT(rc == EDLLOK);
    CU_ASSERT(index == (unsigned int)((DLL_TEST_LISTSIZE/2)-1));

    rc = dll_clear(&list);
    CU_ASSERT(rc == EDLLOK);
}

/* Test dll_reverse() functionality  */
static void test_reverse(void) 
{
    int rc, i;
    dll_list_t list;
    void *data = NULL;

    rc = dll_init(&list);
    CU_ASSERT(rc == EDLLOK);

    /* Fill the list with numbers DLL_TEST_LISTSIZE..1 */
    for(i=0;i<DLL_TEST_LISTSIZE;i++) {
        rc = dll_append(&list, &data, sizeof(int));
        CU_ASSERT(rc == EDLLOK);

        if (rc == EDLLOK)
            *((int*)data) = DLL_TEST_LISTSIZE-i;
    }

    /* Sort the list */
    rc = dll_reverse(&list);
    CU_ASSERT(rc == EDLLOK);

    /* Make sure the list is in right order now */
    for(i=0;i<DLL_TEST_LISTSIZE;i++) {
        rc = dll_get(&list, &data, NULL, (unsigned int)i);
        CU_ASSERT(rc == EDLLOK);
        CU_ASSERT(*((int*)data) == i+1);
    }

    rc = dll_clear(&list);
    CU_ASSERT(rc == EDLLOK);
}

/* Test dll_sort() functionality  */
static void test_sort(void) 
{
    int rc, i;
    dll_list_t list;
    void *data = NULL;

    rc = dll_init(&list);
    CU_ASSERT(rc == EDLLOK);

    /* Fill the list with numbers DLL_TEST_LISTSIZE..1 */
    for(i=0;i<DLL_TEST_LISTSIZE;i++) {
        rc = dll_append(&list, &data, sizeof(int));
        CU_ASSERT(rc == EDLLOK);

        if (rc == EDLLOK)
            *((int*)data) = DLL_TEST_LISTSIZE-i;
    }

    /* Sort the list */
    rc = dll_sort(&list, dll_compar_int);
    CU_ASSERT(rc == EDLLOK);

    /* Make sure the list is in right order now */
    for(i=0;i<DLL_TEST_LISTSIZE;i++) {
        rc = dll_get(&list, &data, NULL, (unsigned int)i);
        CU_ASSERT(rc == EDLLOK);
        CU_ASSERT(*((int*)data) == i+1);
    }

    rc = dll_clear(&list);
    CU_ASSERT(rc == EDLLOK);
}

/* Test dll_iterator_*() functionality  */
static void test_iterator(void) 
{
    int rc, i;
    dll_list_t list;
    dll_iterator_t it;
    void *data = NULL;
    size_t datasize;

    rc = dll_init(&list);
    CU_ASSERT(rc == EDLLOK);

    /* Fill the list with numbers DLL_TEST_LISTSIZE..1 */
    for(i=0;i<DLL_TEST_LISTSIZE;i++) {
        rc = dll_append(&list, &data, sizeof(int));
        CU_ASSERT(rc == EDLLOK);

        if (rc == EDLLOK)
            *((int*)data) = i+1;
    }

    /* Iterate forward */
    rc = dll_iterator_init(&it, &list);
    CU_ASSERT(rc == EDLLOK);
    
    i = 1;
    while ((rc = dll_iterator_next(&it, &data, &datasize)) == EDLLOK) {
        CU_ASSERT(i == *((int*)data));
        CU_ASSERT(datasize == sizeof(int));
        i++;
    }
    CU_ASSERT(rc == EDLLTILT);

    /* Iterate backward */
    rc = dll_iterator_init(&it, &list);
    CU_ASSERT(rc == EDLLOK);
    
    i = DLL_TEST_LISTSIZE;
    while ((rc = dll_iterator_prev(&it, &data, &datasize)) == EDLLOK) {
        CU_ASSERT(i == *((int*)data));
        CU_ASSERT(datasize == sizeof(int));
        i--;
    }
    CU_ASSERT(rc == EDLLTILT);

    rc = dll_clear(&list);
    CU_ASSERT(rc == EDLLOK);
}

static void test_strerror(void)
{
    int rc;
    
    rc = strcmp(dll_strerror(EDLLERROR), DLL_TEST_GENERROR);
    CU_ASSERT(rc != 0);

    rc = strcmp(dll_strerror(EDLLOK), DLL_TEST_GENERROR);
    CU_ASSERT(rc != 0);

    rc = strcmp(dll_strerror(EDLLTILT), DLL_TEST_GENERROR);
    CU_ASSERT(rc != 0);

    rc = strcmp(dll_strerror(EDLLNOMEM), DLL_TEST_GENERROR);
    CU_ASSERT(rc != 0);

    rc = strcmp(dll_strerror(EDLLINV), DLL_TEST_GENERROR);
    CU_ASSERT(rc != 0);

    rc = strcmp(dll_strerror(-1), DLL_TEST_GENERROR);
    CU_ASSERT(rc == 0);
}

int main(int argc, char *argv[]) 
{
    int ret = 0;
    CU_ErrorCode cu_rc;
    CU_pSuite cu_suite01;
    CU_pTest cu_test;

    /* Initialize test registry */
    cu_rc = CU_initialize_registry();
    if (cu_rc != CUE_SUCCESS) {
        ret = 1;
        goto finish;
    }

    /* Add a suite to the registry */
    cu_suite01 = CU_add_suite("dlltest", NULL, NULL);
    if (cu_suite01 == NULL) {
        ret = 2;
        goto finish;
    }

    /* Add tests to suite */
    cu_test = CU_ADD_TEST(cu_suite01, test_append);
    if (cu_test == NULL) {
        ret = 3;
        goto finish;
    }
    cu_test = CU_ADD_TEST(cu_suite01, test_insert);
    if (cu_test == NULL) {
        ret = 3;
        goto finish;
    }
    cu_test = CU_ADD_TEST(cu_suite01, test_extend);
    if (cu_test == NULL) {
        ret = 3;
        goto finish;
    }
    cu_test = CU_ADD_TEST(cu_suite01, test_deepcopy);
    if (cu_test == NULL) {
        ret = 3;
        goto finish;
    }
    cu_test = CU_ADD_TEST(cu_suite01, test_remove);
    if (cu_test == NULL) {
        ret = 3;
        goto finish;
    }
    cu_test = CU_ADD_TEST(cu_suite01, test_indexof);
    if (cu_test == NULL) {
        ret = 3;
        goto finish;
    }
    cu_test = CU_ADD_TEST(cu_suite01, test_reverse);
    if (cu_test == NULL) {
        ret = 3;
        goto finish;
    }
    cu_test = CU_ADD_TEST(cu_suite01, test_sort);
    if (cu_test == NULL) {
        ret = 3;
        goto finish;
    }
    cu_test = CU_ADD_TEST(cu_suite01, test_iterator);
    if (cu_test == NULL) {
        ret = 3;
        goto finish;
    }
    cu_test = CU_ADD_TEST(cu_suite01, test_strerror);
    if (cu_test == NULL) {
        ret = 3;
        goto finish;
    }

    /* Be verbose */
    CU_basic_set_mode(CU_BRM_VERBOSE);

    /* Run all tests in our suite */
    cu_rc = CU_basic_run_tests();
    if (cu_rc != CUE_SUCCESS) {
        ret = 4;
        goto finish;
    }

finish:
    switch (ret) {
        case 1:
            printf("CU_initialize_registry() failed\n");
            break;
        case 2:
            printf("CU_add_suite() failed\n");
            break;
        case 3:
            printf("CU_ADD_TEST() failed\n");
            break;
        case 4:
            printf("CU_basic_run_tests() failed\n");
            break;
        default:
            break;
    }

    CU_cleanup_registry();
    return ret;
}

