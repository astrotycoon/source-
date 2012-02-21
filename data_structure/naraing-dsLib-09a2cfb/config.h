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

#define TEST_CASE
#define TEST_CASE_GRAPH
#define TEST_CASE_QUEUE
#define TEST_CASE_SLL
#define TEST_CASE_HEAP
#define TEST_CASE_BTREE
#undef DEBUG_PRINT

#ifdef DEBUG
#define DEBUG_PRINT(fmt, args...) fprintf(stderr, fmt, ## args)
#else
#define DEBUG_PRINT(fmt, args...)
#endif
