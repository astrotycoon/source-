/*  Copyright 2010 Gaurav Mathur

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef DS_SORT_H
#define DS_SORT_H

#include <stdlib.h>
#include <heap.h>

int heapsort (HEAP_T* h);

int isort_i (int* arr, size_t num);

#endif /* DS_SORT_H */
