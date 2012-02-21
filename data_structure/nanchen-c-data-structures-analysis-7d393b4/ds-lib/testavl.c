#include "avltree.h"
#include "testavl.h"
#include "resourcetrack.h"
#include "sort.h";

#include <stdio.h>

int data[10000];

AvlTree createTestTree(int size){
    int i;

    AvlTree T = AVLTree_makeEmpty( NULL );
    for( i = 0; i < size; i++)
        T = AVLTree_insert( data[i], T );
    return T;
}

void AVLTree_testPerformance(){
    int START=100;
    int END=2000;
    int STEP=10;
    int i;

    permute(data, 10000);
    int inserted = data[5000];
    //quicksort(data, 10000);

    Resource_initilizeOperationArray();

    Resource_startTrack("avltree","insert");
	for(i=START;i<END;i+=STEP){
        AvlTree tree = createTestTree(i);
        //int ret = AVLTree_retrieve(AVLTree_findMax(tree));
        Resource_clearData();
        tree = AVLTree_insert(data[i], tree);
        Resource_storeData(i);
        AVLTree_makeEmpty(tree);
	}
	Resource_analyseSequence();

    Resource_startTrack("avltree","make-empty");
	for(i=START;i<END;i+=STEP){
        AvlTree tree = createTestTree(i);
        Resource_clearData();
        AVLTree_makeEmpty(tree);
        Resource_storeData(i);
	}
	Resource_analyseSequence();

    Resource_startTrack("avltree","find");
	for(i=START;i<END;i+=STEP){
        AvlTree tree = createTestTree(i);
        Resource_clearData();
        AVLTree_find( data[i], tree);
        Resource_storeData(i);
        AVLTree_makeEmpty(tree);
	}
	Resource_analyseSequence();

    Resource_startTrack("avltree","find-min");
	for(i=START;i<END;i+=STEP){
        AvlTree tree = createTestTree(i);
        Resource_clearData();
        int ret = AVLTree_retrieve(AVLTree_findMin(tree));
        //printf("min = %d\n", ret);
        Resource_storeData(i);
        AVLTree_makeEmpty(tree);
	}
	Resource_analyseSequence();

    Resource_startTrack("avltree","find-max");
	for(i=START;i<END;i+=STEP){
        AvlTree tree = createTestTree(i);
        Resource_clearData();
        int ret = AVLTree_retrieve(AVLTree_findMax(tree));
    //    printf("max= %d\n", ret);
        Resource_storeData(i);
        AVLTree_makeEmpty(tree);
	}
	Resource_analyseSequence();

    Resource_startTrack("avltree","retrieve");
	for(i=START;i<END;i+=STEP){
        AvlTree tree = createTestTree(i);
        Position p = AVLTree_find( data[i-1], tree);
      //  printf("try to retrieve %d result pointer = %d\n", data[i],p);
        Resource_clearData();
        int ret = AVLTree_retrieve(p);
        //printf("retrived: %d\n", ret);
        Resource_storeData(i);
        AVLTree_makeEmpty(tree);
	}
	Resource_analyseSequence();

    // --------------Time------------------
    char* time0[] = {"insert"};
    Resource_writePlotScript("insert",time0,1,0,1);
    char* time1[] = {"make-empty"};
    Resource_writePlotScript("make-empty",time1,1,0,0);
    char* find[] = {"find"};
    Resource_writePlotScript("find",find,1,0,1);
    char* time2[] = {"find-min","find-max"};
    Resource_writePlotScript("find-min-max",time2,2,0,1);
    char* time3[] = {"retrieve"};
    Resource_writePlotScript("retrieve",time3,1,0,0);


    // ------------space-------------
    Resource_writePlotScript("insert",time0,1,1,1);
    Resource_writePlotScript("make-empty",time1,1,1,0);
    char* space0[] = {"find", "find-min", "find-max", "retrieve"};
    Resource_writePlotScript("constant",space0,4,1,0);

    // table
    Resource_writeTableData("AVLTree");
}


AVLTree_test()
{
    AvlTree T;
    Position P;
    int i;
    int j = 0;

    T = AVLTree_makeEmpty( NULL );
    for( i = 0; i < 50; i++, j = ( j + 7 ) % 50 )
        T = AVLTree_insert( j, T );
    for( i = 0; i < 50; i++ )
        if( ( P = AVLTree_find( i, T ) ) == NULL || AVLTree_retrieve( P ) != i )
            printf( "Error at %d\n", i );

 /* for( i = 0; i < 50; i += 2 )
        T = AVLTree_delete( i, T );

    for( i = 1; i < 50; i += 2 )
        if( ( P = AVLTree_find( i, T ) ) == NULL || AVLTree_retrieve( P ) != i )
            printf( "error at %d\n", i );
    for( i = 0; i < 50; i += 2 )
        if( ( P = AVLTree_find( i, T ) ) != NULL )
            printf( "error at %d\n", i );
*/
    printf( "Min is %d, Max is %d\n", AVLTree_retrieve( AVLTree_findMin( T ) ),
               AVLTree_retrieve( AVLTree_findMax( T ) ) );

    return 0;
}
