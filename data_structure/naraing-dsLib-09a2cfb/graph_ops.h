/**
 * graph operation headers
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

typedef int(*BFS_FUNCPTR_T)(void*);
typedef int(*DFS_FUNCPTR_T)(void*);

typedef enum {DS_BFS_GRAY=0, DS_BFS_WHITE, DS_BFS_BLACK} DS_BFS_COLOR_E;
typedef enum {DS_DFS_GRAY=0, DS_DFS_WHITE, DS_DFS_BLACK} DS_DFS_COLOR_E;

typedef int (*SP_DJ_FP_T)(void* v);

typedef struct ds_sp_AUX {
   unsigned long spest;
   void* pred;
   unsigned long i;
} DS_SP_AUX_T;

typedef struct ds_bfs_AUX {
   DS_BFS_COLOR_E color;
   long distance;
} DS_BFS_AUX_T;

typedef struct ds_dfs_AUX {
   DS_DFS_COLOR_E color;
   struct timespec d_time;
   struct timespec f_time;
} DS_DFS_AUX_T;

/* macros to access aux information for running dfs/bfs searches */
#define D_AUX_COLOR(VTX)        ((DS_DFS_AUX_T*)((VTX_D_T*)(VTX))->aux)->color
#define D_AUX_DTIME(VTX)        ((DS_DFS_AUX_T*)((VTX_D_T*)(VTX))->aux)->d_time
#define D_AUX_FTIME(VTX)        ((DS_DFS_AUX_T*)((VTX_D_T*)(VTX))->aux)->f_time
#define UD_AUX_COLOR(VTX)       ((DS_DFS_AUX_T*)((VTX_UD_T*)(VTX))->aux)->color
#define UD_AUX_DTIME(VTX)       ((DS_DFS_AUX_T*)((VTX_UD_T*)(VTX))->aux)->d_time
#define UD_AUX_FTIME(VTX)       ((DS_DFS_AUX_T*)((VTX_UD_T*)(VTX))->aux)->f_time

/* macros for accessing aux information for running the shortest path */
#define D_SP_AUX_SPEST(VTX)     ((DS_SP_AUX_T*)((VTX_D_T*)(VTX))->aux)->spest
#define D_SP_AUX_PRED(VTX)      ((DS_SP_AUX_T*)((VTX_D_T*)(VTX))->aux)->pred
#define D_SP_AUX_I(VTX)         ((DS_SP_AUX_T*)((VTX_D_T*)(VTX))->aux)->i
#define UD_SP_AUX_SPEST(VTX)    ((DS_SP_AUX_T*)((VTX_UD_T*)(VTX))->aux)->spest
#define UD_SP_AUX_PRED(VTX)     ((DS_SP_AUX_T*)((VTX_UD_T*)(VTX))->aux)->pred

/* prototypes */
GRAPH_T* matrix_create (const unsigned long row, const unsigned long column);

GPH_ERR_E dfs (GRAPH_T* g, unsigned long vid, DFS_FUNCPTR_T func);
GPH_ERR_E bfs (GRAPH_T* g, unsigned long vid, BFS_FUNCPTR_T func);
GRAPH_T* matrix_create (const unsigned long N, const unsigned long M);
GPH_ERR_E graphviz_description (GRAPH_T* g, char* filename);
void sp_dijkstra (GRAPH_T* g, unsigned long s, SP_DJ_FP_T cb);
