/**
 * test case driver
 * Copyright (c) 2010, Gaurav Mathur <narainmg@gmail.com>
 *   
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *   
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *   
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * See README and COPYING for more details.
 */

#include <tc_graph.h>
#include <tc_heap.h>

/* todo - signal handler to flush buffers on seg fault */
int main (int argc, char** argv)
{
   //graph_main();
   tc_sll_main ();
   //tc_skl_main();
   //isort_main ();
   //queue_main ();
   //matrix_main();
   /* bfs_main (); */
   //tc_directed_main();
   /*
   tc_heap_iterator();
   
   tc_heap_max_main();
   tc_heap_min_main();
   tc_heap_min_ii_main();
   
   tc_dj_i_main();
   tc_dj_ii_main();
   tc_dj_iii_main();
   tc_dj_iv_main();
   tc_dj_v_main();
   heapsort_main();
   */   
   return 0;
}
