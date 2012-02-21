/*
 * sort.h
 *
 *  Created on: 12 Mar 2011
 *      Author: nan
 */

#ifndef SORT_H_
#define SORT_H_

typedef int ElementType;

void insertionSort(ElementType A[], int N);
void shellsort(ElementType A[], int N);
void heapsort(ElementType A[], int N);
void mergesort(ElementType A[], int N);
void quicksort(ElementType A[], int N);
void radixExchangeSort(ElementType a[], int bit, int left, int right);

void Sort_test();

#endif /* SORT_H_ */
