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

#include "dll_list.h"
#include "dll_util.h"

/* ######################################################################### */
/*                            TODO / Notes                                   */
/* ######################################################################### */

/* ######################################################################### */
/*                            Types & Defines                                */
/* ######################################################################### */

typedef struct {
        int error_code;
        const char *desc;
} dll_error_description_t;

/* ######################################################################### */
/*                           Private interface (Module)                      */
/* ######################################################################### */

/* ######################################################################### */
/*                           Implementation                                  */
/* ######################################################################### */

static const dll_error_description_t dll_errdesc[] = {
        {EDLLERROR, "Unspecified error"},
        {EDLLOK,    "No error"},
        {EDLLTILT,  "Iterator turnaround"},
        {EDLLNOMEM, "Not enough memory"},
        {EDLLINV,   "Invalid argument"},
};
static const char *dll_errdesc_unknown = "Unknown error";


char const* dll_strerror(int errnum)
{
        int i;
        int items = sizeof(dll_errdesc)/sizeof(dll_error_description_t);

        /* Go through the list and try to find a matching error code */
        for (i=0;i<items;i++) {
                if (dll_errdesc[i].error_code == errnum) {
                        return (char*)dll_errdesc[i].desc;
                }
        }

        /* Not found, return a generic string */
        return (char*)dll_errdesc_unknown;
}

int dll_compar_int(const void *item1, const void *item2)
{
        int int1 = *((int*)item1);
        int int2 = *((int*)item2);

        if (int1 > int2)
                return 1;

        if (int1 < int2)
                return -1;

        return 0;
}

