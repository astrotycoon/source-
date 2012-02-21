/*
 * testutil.c
 *
 *  Created on: Apr 5, 2011
 *      Author: nan
 */
#include "testutil.h"

void permute(int a[], int n) {
	int i;

	for (i = 0; i < n; i++)
		a[i] = i;
	for (i = 1; i < n; i++)
		Swap(&a[i], &a[rand() % (i + 1)]);
}

