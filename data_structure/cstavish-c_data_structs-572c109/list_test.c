/*
Copyright (c) 2011, Coleman Stavish
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
  * Redistributions of source code must retain the above copyright
	notice, this list of conditions and the following disclaimer.
  * Redistributions in binary form must reproduce the above copyright
	notice, this list of conditions and the following disclaimer in the
	documentation and/or other materials provided with the distribution.
  * Neither the name of Coleman Stavish nor the
	names of contributors may be used to endorse or promote products
	derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL COLEMAN STAVISH BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdio.h>
#include "cs_linked_list.h"

static void print_sll(cs_sll *list) {
    for (int i = 0; i < list->size; i++)
        puts(cs_sll_get(list, i));
}

static void print_dll(cs_dll *list) {
    for (int i = 0; i < list->size; i++)
        puts(cs_dll_get(list, i));
}

int main() {
    
    {
        cs_dll *list = cs_dll_create(NULL, "lorem", "ipsum", "dolor", "sit", "amet", NULL);
        
        puts("Starting contents:\n\n");
        print_dll(list);
            
        puts("\n\nSwapping 0 with 2\n\n");
        cs_dll_swap(list, 0, 2);
        print_dll(list);
    
        
        puts("\n\nInserting 'pseudo-Latin' at index 3\n\n");
        cs_dll_ins(list, "psuedo-Latin", 3);
        print_dll(list);
    
            
        puts("\n\nRemoving element at index 2\n\n");
        cs_dll_del(list, 2);
        print_dll(list);
    
        puts("\n\nSetting 2 to 'Cicero?'\n\n");
        cs_dll_set(list, "Cicero?", 2);
        print_dll(list);
            
        cs_dll_destroy(list);
    }
    {
        cs_sll *list = cs_sll_create(NULL, "lorem", "ipsum", "dolor", "sit", "amet", NULL);
        
        puts("\n\nStarting contents:\n\n");
        print_sll(list);
            
        puts("\n\nSwapping 0 with 2\n\n");
        cs_sll_swap(list, 0, 2);
        print_sll(list);
    
        
        puts("\n\nInserting 'pseudo-Latin' at index 3\n\n");
        cs_sll_ins(list, "psuedo-Latin", 3);
        print_sll(list);
    
            
        puts("\n\nRemoving element at index 2\n\n");
        cs_sll_del(list, 2);
        print_sll(list);
    
        puts("\n\nSetting 2 to 'Cicero?'\n\n");
        cs_sll_set(list, "Cicero?", 2);
        print_sll(list);
            
        cs_sll_destroy(list);
    }
    
    return 0;
}