/**
 * linked list data structure test cases
 * Copyright (c) 2010, Gaurav Mathur <narainmg@gmail.com>
 *   
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *   
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *   
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * See README and COPYING for more details.
 */

#include <stdio.h>

#include <ll.h>
#include <utils.h>

/*******************************************************************************
 * sll_new - create a new SSL
 *
 * RETURNS: SLL_T* if ok; NULL if error
 */

int post_iterator (int ignore1, int ignore2, int ignore3)
{
   fprintf (stdout, "\n");
   fprintf (stdout, "TC [LL]: [END]\n");
   return 0;
}

/*******************************************************************************
 * sll_new - create a new SSL
 *
 * RETURNS: SLL_T* if ok; NULL if error
 */
int pre_iterator (int ignore1, int ignore2, int ignore3)
{
   printf ("[==]\n");
   return 0;
}

/*******************************************************************************
 * sll_new - create a new SSL
 *
 * RETURNS: SLL_T* if ok; NULL if error
 */
static int iter_cb_int (SLL_NODE* node)
{
   fprintf (stdout, "%ld ", node->FIDATA);
   return 0;
}

/*******************************************************************************
* test_case_int_1 - test core linked list operations
*
* RETURNS: void
*/
static void test_case_int_4 (void)
{
    SLL_T* head;
    int i;

    fprintf (stdout, "TC[LL]: test case int#1\n");
    head = sll_new (IDATA, iter_cb_int, pre_iterator, post_iterator);

    fprintf (stdout, "TC[LL]: inserting 1...19\n");
    for (i = 1; i < 20; i++)
       sll_insert (head, i);
    sll_iter (head);
    
    fprintf (stdout, "TC[LL]: deleting 10\n");
    sll_delete (head, 10);
    fprintf (stdout, "TC[LL]: deleting 15\n");
    sll_delete (head, 15);
    fprintf (stdout, "TC[LL]: deleting 19\n");
    sll_delete (head, 19);
    sll_iter (head);
    fprintf (stdout, "TC[LL]: inserting 19\n");    
    sll_insert (head, 19);
    fprintf (stdout, "TC[LL]: inserting 10\n");    
    sll_insert (head, 10);
    sll_iter (head);
    if (NULL != sll_find (head, 5))
        fprintf (stdout, "TC[LL]: OK found 5\n");
    else
        fprintf (stdout, "TC[LL]: ERROR Not found 5\n");
    if (NULL != sll_find (head, 67))
        fprintf (stdout, "TC[LL]: ERROR found 67\n");
    else
        fprintf (stdout, "TC[LL]: OK Not found 67\n");

    
}

/*******************************************************************************
* test_case_int_2 - test routines to expand a linked list
*
* RETURNS: void
*/
static void test_case_int_2 (void)
{
    SLL_T* first;
    SLL_T* second;
    int i;

    fprintf (stdout, "TC[LL]: test case int#2\n");
    first = sll_new (IDATA, iter_cb_int, pre_iterator, post_iterator);
    second = sll_new (IDATA, iter_cb_int, pre_iterator, post_iterator);

    for (i = 10; i <= 20; i++)
       sll_insert (first, i);
    for (i = 20; i < 30; i++)
       sll_insert (second, i);

    sll_iter (first);
    sll_iter (second);
    fprintf (stdout, "TC[LL]: concatenating\n");
    sll_concat (first, second);
    sll_iter (first);
}

/*******************************************************************************
 * test_case_int_3 - test routines sll_next_get and sll_prev_get
 *
 * RETURNS: n.a.
 */

static void test_case_int_3 (void)
    {
    SLL_T* sll;
    SLL_NODE* node = NULL;
    int i;

    fprintf (stderr, "test case int#3\n");
    sll = sll_new (IDATA, iter_cb_int, pre_iterator, post_iterator);

    fprintf (stderr, "inserting elements from 10 - 20 in the list\n");
    for (i = 10; i <= 20; i++)
        sll_insert (sll, i);

    while (NULL != (node = sll_next_get (sll, node)))
        {
        fprintf (stderr, "%ld ", node->FIDATA);        
        }
    fprintf (stderr, "\n");
    
    node = sll_last_get(sll);
    while (NULL != (node = sll_prev_get (sll, node)))
        {
        fprintf (stderr, "%ld ", node->FIDATA);        
        }
    
    fprintf (stderr, "\n");
    }

/*******************************************************************************
 * sll_new - create a new SSL
 *
 * RETURNS: SLL_T* if ok; NULL if error
 */

static int iter_cb_char (SLL_NODE* node)
{
   printf ("%c ", node->FCDATA);
   return 0;
}

/*******************************************************************************
 * sll_new - create a new SSL
 *
 * RETURNS: SLL_T* if ok; NULL if error
 */

static void test_case_char (void)
{
    SLL_T* head;
    int i;
    
    head = sll_new (CDATA, iter_cb_char, pre_iterator, post_iterator);

    for (i = 'a'; i <= 'z'; i++)
       sll_insert (head, i);
    sll_iter (head);
    printf ("\n");
    fprintf (stderr, "deleting a\n");
    sll_delete (head, 'a');
    fprintf (stderr, "deleting f\n");
    sll_delete (head, 'f');
    fprintf (stderr, "deleting z\n");
    sll_delete (head, 'z');
    sll_iter (head);
    fprintf (stderr, "inserting a \n");    
    sll_insert (head, 'a');
    fprintf (stderr, "inserting z\n");    
    sll_insert (head, 'z');
    fprintf (stderr, "inserting z\n");    
    sll_insert (head, 'z');
    sll_iter (head);
    fprintf (stderr, "deleting z\n");
    sll_delete (head, 'z');
    sll_iter (head);
}

/*******************************************************************************
 * sll_new - create a new SSL
 *
 * RETURNS: SLL_T* if ok; NULL if error
 */

int tc_sll_main (int argc, char** argv)
    {
//test_case_int_1 ();
//  test_case_char ();
// test_case_int_2 ();
// test_case_int_3 ();
    test_case_int_4();
   return 0;
}


/**
 * @brief 
 * 
 * @return A new linked list (SLL_T*)
 */
static void tc_skl_2 (void)
{
    SKL_T* head;
    unsigned long item;
    int i;

    fprintf (stdout, "TC[SKL]: test case int#2\n");
    head = skl_new (IDATA, 16, iter_cb_int, pre_iterator, post_iterator);

    for (i = 1; i <= 30; i++)
    {
       fprintf (stdout, "inserting %d\n", i);
       skl_insert_idata (head, i);
    }
    fprintf (stdout, "\n");
    fprintf (stderr, "deleting 10\n");
    skl_delete_idata (head, 10);
    fprintf (stderr, "deleting 30\n");
    skl_delete_idata (head, 30);
    fprintf (stderr, "deleting 1\n");
    skl_delete_idata (head, 1);
    fprintf (stderr, "inserting 12\n");    
    skl_insert_idata (head, 12);
    fprintf (stderr, "inserting 1\n");    
    skl_insert_idata (head, 1);
    skl_iter (head);
}

/**
 * @brief 
 * 
 * @return A new linked list (SLL_T*)
 */
static void tc_perf_1 (void)
{
    SKL_T* skl;
    SLL_T* sll;
    void* st;
    void* et;
    unsigned long item;
    unsigned long diff=0;
    int i;
    int iter;

    fprintf (stdout, "TC[%5s]: test case int#1\n", "skl");
    for (iter = 0; iter < 100; iter++)
        {
        fprintf (stdout, "TC[%5s]: iteration %d\n", "skl", iter+1);
        skl = skl_new (IDATA, 16, iter_cb_int, pre_iterator, post_iterator);
        sll = sll_new (IDATA, iter_cb_int, pre_iterator, post_iterator);
        
        for (i = 1; i <= 10000; i++)
            {
            item = dslib_random (30000);
            //fprintf (stdout, "inserting %lu \n", item);
            skl_insert_idata (skl, item);
            sll_insert (sll, item);
            }
        //fprintf (stdout, "\n");
        //sll_iter (sll);
        //skl_iter (skl);
        
        st = dslib_record_time();
        if (NULL != skl_find_idata (skl, 1000))
            fprintf (stdout, "TC[%5s]: found 1000\n", "skl");
        else
            fprintf (stdout, "TC[%5s]: did not find 1000\n", "skl");
        et = dslib_record_time();
        if (ERR_UTILS_OK != dslib_time_diff (st, et, &diff))
            fprintf (stdout, "TC[%5s]: dslib_time_diff() error\n", "skl");
        
        fprintf (stdout, "TC[%5s]: find took %ldns\n", "skl", diff);
        
        st = dslib_record_time();
        if (NULL != sll_find (sll, 1000))
            fprintf (stdout, "TC[%5s]: found 1000\n", "sll");
        else
            fprintf (stdout, "TC[%5s]: did not find 1000\n", "sll");
        et = dslib_record_time();
        if (ERR_UTILS_OK != dslib_time_diff (st, et, &diff))
            fprintf (stdout, "TC[%5s]: dslib_time_diff() error\n", "skl");
        
        fprintf (stdout, "TC[%5s]: find took %ldns\n\n", "skl", diff);
        }
}

/**
 * @brief 
 * 
 * @return A new linked list (SLL_T*)
 */
static void tc_skl_1 (void)
{
    SKL_T* head;
    void* st;
    void* et;
    unsigned long item;
    unsigned long diff=0;
    int i;

    fprintf (stdout, "TC[%5s]: test case int#1\n", "skl");
    head = skl_new (IDATA, 16, iter_cb_int, pre_iterator, post_iterator);

    for (i = 1; i <= 2000; i++)
    {
       item = dslib_random (10000);
       //fprintf (stdout, "inserting %lu \n", item);
       skl_insert_idata (head, item);
    }
    //fprintf (stdout, "\n");
    skl_iter (head);
    st = dslib_record_time();
    if (NULL != skl_find_idata (head, 1000))
        fprintf (stdout, "TC[%5s]: found 1000\n", "skl");
    else
        fprintf (stdout, "TC[%5s]: did not find 1000\n", "skl");
    et = dslib_record_time();
    if (ERR_UTILS_OK != dslib_time_diff (st, et, &diff))
        fprintf (stdout, "TC[%5s]: dslib_time_diff() error\n", "skl");
        
    fprintf (stdout, "TC[%5s]: find took %ldns\n", "skl", diff);
    /*
    fprintf (stderr, "deleting 1000\n");
    sll_delete (head, 1000);      
    fprintf (stderr, "deleting 15\n");
    sll_delete (head, 15);
    fprintf (stderr, "deleting 19\n");
    sll_delete (head, 19);
    sll_iter (head);
    fprintf (stderr, "inserting 19\n");    
    sll_insert (head, 19);
    fprintf (stderr, "inserting 10\n");    
    sll_insert (head, 10);
    sll_iter (head);
    */
}

int tc_skl_main (int argc, char** argv)
{
   /* to-do seed PRNG */
   dslib_srandom ();
   //tc_skl_1 ();
   // tc_skl_2 ();
   tc_perf_1 ();
   return 0;
}
