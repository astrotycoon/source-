#include <stdio.h>
#include "list.h"
#include "resourcetrack.h"

void PrintList(const List L) {
	ListPosition P = List_header(L);

	if (List_isEmpty(L))
		printf("Empty list\n");
	else {
		do {
			P = List_advance(P);
			printf("%d ", List_retrieve(P));
		} while (!List_isLast(P, L));
		printf("\n");
	}
}

List createTestList(int size){
    List l = List_makeEmpty(NULL);
	ListPosition p = List_header(l);
    int i;

    for (i = 0; i < size; i++) {
		List_insert(&i, l, p);
		p = List_advance(p);
	}
	return l;
}


int List_test(void) {
    const int START=10;
    const int END=1000;
    const int STEP=10;

	List L, retL;
	ListPosition P;
	int i;

    Resource_initilizeOperationArray();

    Resource_startTrack("list","List_makeEmpty");
	for(i=START;i<=END;i+=STEP){
        L = createTestList(i);
        Resource_clearData();
        retL = List_makeEmpty(L);
        Resource_storeData(i);
	}
	Resource_analyseSequence();

    L = List_makeEmpty(NULL);
    Resource_startTrack("list","List_isEmpty");
	for(i=START;i<=END;i+=STEP){
        L = createTestList(i);
        Resource_clearData();
        List_isEmpty(L);
        Resource_storeData(i);
	}
	Resource_analyseSequence();

    L = List_makeEmpty(NULL);
    Resource_startTrack("list","List_isLast");
	for(i=START;i<=END;i+=STEP){
        L = createTestList(i);
        P = List_header(L);
        Resource_clearData();
        List_isLast(P,L);
        Resource_storeData(i);
	}
	Resource_analyseSequence();

    L = List_makeEmpty(NULL);
    Resource_startTrack("list","List_find-(worst-case)");
	for(i=START;i<=END;i+=STEP){
        L = createTestList(i);
        P = List_header(L);
        ListPosition last = List_last(L);
        Resource_clearData();
        List_find(last, L);
        Resource_storeData(i);
	}
	Resource_analyseSequence();

    L = List_makeEmpty(NULL);
    Resource_startTrack("list","List_delete");
	for(i=START;i<=END;i+=STEP){
        L = createTestList(i);
        P = List_header(L);
        ListElement last = List_retrieve(List_last(L));
        Resource_clearData();
        List_delete(last, L);
        Resource_storeData(i);
	}
	Resource_analyseSequence();

    L = List_makeEmpty(NULL);
    Resource_startTrack("list","List_findPrevious-(worst-case)");
	for(i=START;i<=END;i+=STEP){
        L = createTestList(i);
        P = List_header(L);
        ListElement last = List_retrieve(List_last(L));
        Resource_clearData();
        List_findPrevious(last, L);
        Resource_storeData(i);
	}
	Resource_analyseSequence();

    L = List_makeEmpty(NULL);
    Resource_startTrack("list","List_insert-(onto-first)"); // first position
	for(i=START;i<=END;i+=STEP){
        L = createTestList(i);
        P = List_header(L);
        ListPosition first = List_first(L);
        Resource_clearData();
        int value = 30;
        List_insert(&value,L,first);
        Resource_storeData(i);
	}
	Resource_analyseSequence();

    L = List_makeEmpty(NULL);
    Resource_startTrack("list","List_deleteList");
	for(i=START;i<=END;i+=STEP){
        L = createTestList(i);
        Resource_clearData();
        List_deleteList(L);
        Resource_storeData(i);
	}
	Resource_analyseSequence();

    L = List_makeEmpty(NULL);
    Resource_startTrack("list","List_header");
	for(i=START;i<=END;i+=STEP){
        L = createTestList(i);
        Resource_clearData();
        P = List_header(L);
        Resource_storeData(i);
	}
	Resource_analyseSequence();

    L = List_makeEmpty(NULL);
    Resource_startTrack("list","List_first");
	for(i=START;i<=END;i+=STEP){
        L = createTestList(i);
        Resource_clearData();
        P = List_first(L);
        Resource_storeData(i);
	}
	Resource_analyseSequence();

    L = List_makeEmpty(NULL);
    Resource_startTrack("list","List_advance");
	for(i=START;i<=END;i+=STEP){
        L = createTestList(i);
        Resource_clearData();
        P = List_advance(P);
        Resource_storeData(i);
	}
	Resource_analyseSequence();

    L = List_makeEmpty(NULL);
    Resource_startTrack("list","List_retrieve");
	for(i=START;i<=END;i+=STEP){
        L = createTestList(i);
        Resource_clearData();
        List_retrieve(P);
        Resource_storeData(i);
	}
	Resource_analyseSequence();

    // --------------Time------------------
    char* list0[] = {"List_makeEmpty","List_find-(worst-case)"};
    Resource_writePlotScript("List0",list0,2,0,0);

    char* list1[] = {"List_deleteList"};
    Resource_writePlotScript("List_deleteList", list1,1,0,0);

    char* opConstant[] = {"List_isEmpty","List_isLast", "List_delete", "List_findPrevious-(worst-case)","List_insert-(onto-first)","List_header","List_first","List_advance","List_retrieve"};
    Resource_writePlotScript("List_constant",opConstant,9,0,0);

    //---------------space----------------
    char* list2[] = {"List_makeEmpty"};
    Resource_writePlotScript("List_makeEmpty",list2,1,1,0);

    char* deleteList[] = {"List_deleteList"};
    Resource_writePlotScript("List_deleteList",deleteList,1,1,0);

    char* list3[] = {"List_isEmpty","List_isLast","List_find-(worst-case)","List_delete","List_findPrevious-(worst-case)","List_insert-(onto-first)","List_header","List_first","List_advance","List_retrieve"};
    Resource_writePlotScript("List_constant",list3,10,1,0);

    Resource_writeTableData("List");

/*
	L = List_makeEmpty(NULL);
	P = List_header(L);
	PrintList(L);

	for (i = 0; i < 10; i++) {
		List_insert(&i, L, P);
		PrintList(L);
		P = List_advance(P);
	}
	for (i = 0; i < 10; i += 2)
		List_delete(&i, L);

	for (i = 0; i < 10; i++)
		if ((i % 2 == 0) == (List_find(&i, L) != NULL))
			printf("Find fails\n");

	printf("Finished deletions\n");

	PrintList(L);

	List_deleteList(L);
*/

	return 0;
}
