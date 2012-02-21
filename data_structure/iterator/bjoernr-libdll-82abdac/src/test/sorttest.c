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
#include <stdlib.h>
#include <dll_list.h>
#include <dll_util.h>

int main(int argc, char *argv[])
{
        dll_list_t list;
        dll_list_t copy;
        dll_iterator_t it;
        int *data = NULL;
        int i;

        dll_init(&list);
        dll_init(&copy);

        for(i=0;i<50;i++) {
                dll_append(&list, (void**)&data, sizeof(int));
                *data = (int)(((double)random()/(double)RAND_MAX)*1000.0);
        }

        dll_deepcopy(&list, &copy);
        dll_sort(&list, dll_compar_int);
        dll_reverse(&list);

        dll_iterator_init(&it, &list);
        while(dll_iterator_next(&it, (void**)&data, NULL) == EDLLOK) {
                printf("data: %d\n", *data);
        }

        printf("--\n");

        dll_iterator_init(&it, &copy);
        while(dll_iterator_next(&it, (void**)&data, NULL) == EDLLOK) {
                printf("data: %d\n", *data);
        }

        dll_clear(&list);
        dll_clear(&copy);

        return 0;
}

