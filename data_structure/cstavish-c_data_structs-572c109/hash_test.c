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
#include <stdlib.h>
#include "cs_hash_tab.h"

static void clean_up(const char *key, void *val) {
    printf("cleaning up value for key \"%s\"\n", key);
    // nothing to clean up in this case (string constants)
}

static void for_each(cs_hash_tab *tab, const char *key, void *val, size_t count) {
    printf("%s => %s\n", key, (const char *)val);
}   

int main() {
    cs_hash_tab *t = cs_hash_create_kv("Pink", "Floyd", "Jimi", "Hendrix", "Grateful", "Dead", "Titus", "Andronicus", NULL);
    t->cleanup = clean_up;
    cs_hash_set(t, "Steely", "Dan");
    cs_hash_del(t, "Titus");
    puts(cs_hash_get(t, "Pink"));
    puts(cs_hash_get(t, "Jimi"));
    puts(cs_hash_get(t, "Grateful"));
    puts(cs_hash_get(t, "Steely"));
    cs_hash_iterate(t, for_each);
    cs_hash_destroy(t);
}