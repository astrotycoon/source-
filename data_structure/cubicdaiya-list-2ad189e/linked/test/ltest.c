#include <stdio.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "../list.h"

#define NUM 10

static void test_list_add(void);
static void test_list_addfront(void);
static void test_list_add_addfront(void);
static void test_list_get(void);
static void test_list_get_negative_idx(void);
static void test_list_check_head(void);
static void test_list_join(void);
static void test_list_insert(void);
static void test_list_remove(void);

static void test_list_add(void) {
    list_t *l = list_create(sizeof(int));
    
    for (int i=0;i<NUM;++i) {
        int e = i + 1;
        l = list_add(l, &e);
    }
    
    {
        int i = 1;
        for (list_t *p=list_head(l);p!=NULL;p=p->next,++i) {
            CU_ASSERT(list_entry_eq(p, &i));
        }
    }
    
    list_destroy(l);
}

static void test_list_addfront(void) {
    list_t *l = list_create(sizeof(int));
    for (int i=0;i<NUM;++i) {
        int e = i + 1;
        l = list_addfront(l, &e);
    }
    
    {
        int i = NUM;
        for (list_t *p=list_head(l);p!=NULL;p=p->next,--i) {
            CU_ASSERT(list_entry_eq(p, &i));
        }
    }
    list_destroy(l);
}

static void test_list_add_addfront(void) {
    list_t *l   = list_create(sizeof(int));
    int array[] = {3,4,5,6,2,1,7,8,9,10};
    l = list_add(l,      &array[0]);
    l = list_add(l,      &array[1]);
    l = list_add(l,      &array[2]);
    l = list_add(l,      &array[3]);
    l = list_addfront(l, &array[4]);
    l = list_addfront(l, &array[5]);
    l = list_add(l,      &array[6]);
    l = list_add(l,      &array[7]);
    l = list_add(l,      &array[8]);
    l = list_add(l,      &array[9]);
    
    {
        int i = 1;
        for (list_t *p=list_head(l);p!=NULL;p=p->next,++i) {
            CU_ASSERT(list_entry_eq(p, &i));
        }
    }
    list_destroy(l);
}


static void test_list_get(void) {
    list_t *l = list_create(sizeof(int));
    for (int i=0;i<NUM;++i) {
        int e = i + 1;
        l = list_add(l, &e);
    }
    for (int i=0;i<NUM;++i) {
        list_t *p = list_get(l, i);
        int e = i + 1;
        CU_ASSERT(list_entry_eq(p, &e));
    }
    list_destroy(l);
}

static void test_list_get_negative_idx(void) {
    list_t *l = list_create(sizeof(int));
    for (int i=0;i<NUM;++i) {
        int e = i + 1;
        l = list_add(l, &e);
    }
    list_t *p = list_get(l, -1);
    CU_ASSERT(p == NULL);
    list_destroy(l);
}

static void test_list_check_head(void) {
    list_t *l = list_create(sizeof(int));
    for (int i=0;i<NUM;++i) {
        int e = i + 1;
        l = list_add(l, &e);
    }
    list_t *h = list_head(l);
    for (list_t *p=list_head(l);p!=NULL;p=p->next) {
        CU_ASSERT(p->head == h);
    }
    list_destroy(l);
}

static void test_list_join(void) {
    list_t *l1 = list_create(sizeof(int));
    list_t *l2 = list_create(sizeof(int));
    int array1[] = {2,3,1};
    int array2[] = {4,5,6};
    l1 = list_add(l1,      &array1[0]);
    l1 = list_add(l1,      &array1[1]);
    l1 = list_addfront(l1, &array1[2]);
    l2 = list_add(l2,      &array2[0]);
    l2 = list_add(l2,      &array2[1]);
    l2 = list_add(l2,      &array2[2]);
    list_t *l3 = list_join(l1, l2);
    {
        int i = 1;
        for (list_t *p=list_head(l3);p!=NULL;p=p->next,++i) {
            CU_ASSERT(list_entry_eq(p, &i));
        }
    }
    list_destroy(l3);
}

static void test_list_insert(void) {
    list_t *l = list_create(sizeof(int));
    for (int i=0;i<NUM;++i) {
        int e = i + 1;
        l = list_add(l, &e);
    }
    int inserts[] = {11,99,100};
    l = list_insert(l, 0,  &inserts[0]);
    l = list_insert(l, 5,  &inserts[1]);
    l = list_insert(l, 12, &inserts[2]);
    list_t *p = list_head(l);
    int answers[] = {11,1,2,3,4,99,5,6,7,8,9,10,100};
    for (int i=0;i<sizeof(answers)/sizeof(answers[0]);++i) {
        CU_ASSERT(list_entry_eq(p, &answers[i]));
        p = p->next;
    }
    list_destroy(l);
}

static void test_list_remove(void) {
    list_t *l = list_create(sizeof(int));
    for (int i=0;i<NUM;++i) {
        int e = i + 1;
        l = list_add(l, &e);
    }
    l = list_remove(l, 0);
    l = list_remove(l, 3);
    l = list_remove(l, 7);
    list_t *p = list_head(l);
    int answers[] = {2,3,4,6,7,8,9};
    for (int i=0;i<sizeof(answers)/sizeof(answers[0]);++i) {
        CU_ASSERT(list_entry_eq(p, &answers[i]));
        p = p->next;
    }
    list_destroy(l);
}

int main (int argc, char *argv[]) {
    CU_pSuite suite_list;
    CU_initialize_registry();
    suite_list = CU_add_suite("list", NULL, NULL);
    CU_add_test(suite_list, "test_list_add",              test_list_add);
    CU_add_test(suite_list, "test_list_addfront",         test_list_addfront);
    CU_add_test(suite_list, "test_list_add_addfront",     test_list_add_addfront);
    CU_add_test(suite_list, "test_list_get",              test_list_get);
    CU_add_test(suite_list, "test_list_get_negative_idx", test_list_get_negative_idx);
    CU_add_test(suite_list, "test_list_check_head",       test_list_check_head);
    CU_add_test(suite_list, "test_list_join",             test_list_join);
    CU_add_test(suite_list, "test_list_insert",           test_list_insert);
    CU_add_test(suite_list, "test_list_remove",           test_list_remove);
    CU_basic_run_tests();
    CU_cleanup_registry();
    
    return 0;
}


