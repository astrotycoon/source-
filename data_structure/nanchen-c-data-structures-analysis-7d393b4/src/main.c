/*
 ============================================================================
 Name        : main.c
 Author      :
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include "../ds-lib/testlist.h"
#include "../ds-lib/testque.h"
#include "../ds-lib/testtree.h"
#include "../ds-lib/sort.h"
#include "../ds-lib/testavl.h"

int main(void) {
//	int i;

/*
	int value;

	struct tms st_cpu;
	struct tms en_cpu;
	clock_t st_time, en_time;
	st_time = times(&st_cpu);
*/

//	printf("-----------------Test List-------------------\n");
//	List_test();

    printf("-----------------Test Sorting------------------\n");
	Sort_test();

//	printf("-----------------Test AVL Tree-------------------\n");
//	AVLTree_testPerformance();


//	printf("-----------------Test Tree-------------------\n");
//	Tree_test();


//	printf("-----------------Test Queue------------------\n");
//	Queue_test();

//	for( i = 0; i < 500000; i++ )
//		value = 5000*8000;

//	sleep(1);
//	en_time = times(&en_cpu);

	//long int elapsed = end - start;
//	printf("start %d \n", start);
//	printf("end %d \n", end);
//	printf("time elapsed: %d \n", elapsed);

//	printf("Real Time: %jd, User Time %jd, System Time %jd\n",
//	        (long int)(en_time - st_time),
//	        (long int)(en_cpu.tms_utime - st_cpu.tms_utime),
//	        (long int)(en_cpu.tms_stime - st_cpu.tms_stime));

	return EXIT_SUCCESS;
}
