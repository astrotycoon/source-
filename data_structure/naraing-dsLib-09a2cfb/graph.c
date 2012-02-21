/** @file graph.c
 * 
 * graph data structure
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <ds_types.h>
#include <graph.h>

/**
 * This routine create a duplicate of a the given graph
 *
 * @param[in] g The graph to duplicate
 *
 * @return The duplicate graph
 */
GRAPH_T* graph_duplicate
(
GRAPH_T* g
)
{
   GRAPH_T* dup_g;
   
   if (NULL == (dup_g = graph_new (g->type, g->label_type)))
   {
      return NULL;
   }
   /* XXX: todo */
   return NULL;
}

/**
 * Get the number of edges incident to given vertex in a graph.
 * 
 * @param[in] g The graph to oeprate on.
 * @param[in] v The vertex whose incident edges need to be found.
 * @return The number of edges.
 */
unsigned long graph_edges_no_get
(
GRAPH_T* g,
void* v
)
{
   unsigned long no = 0;
   
   if (g->type == GRAPH_UNDIRECTED_T)
   {
      no = ((VTX_UD_T*)v)->no;
   }
   else if (g->type == GRAPH_DIRECTED_T)
   {
      no = ((VTX_D_T*)v)->no;
   }
   return no;
}

/**
 * @brief This routine is an edge iterator for the entire graph.
 *
 * If the parameter <e> is NULL, the routine will return the first edge in the
 * list of graph edges. If it's not it will return the edge next to the edge
 * <e> in the list of edge objects maintained by the graph. This routine can
 * thus be used to iterate over all the edges in the graph. There is no
 * meaningful ordering applied to the list of edges.
 *
 * @param[in] g The graph to operate on
 * @param[in] e The edge whose next edge needs to the found.
 */
EDGE_T* graph_edge_next_get
(
GRAPH_T* g,
EDGE_T* e
)
{
   if (e == NULL)
   {
      e = g->eLst;
   }
   else
   {
      e = e->next;
   }
   return e;
}

/**
 * @brief This routine is a vertex iterator for all adjacent vertices to a *
 * given vertex.
 *
 * For directed graphs the vertices reachable through out edges are considered
 * to be the adjacent vertices
 *
 * @param[in] g The graph to operate on
 * @param[in] v The vertex whose adjacent vertices need to be found out.
 * @param[out] ctx The iterator context needed for the operation of this
 * routine.
 * @return A vertex adjacent to <v>
 * 
 */
void* graph_vertex_next_adj_get
(
GRAPH_T* g,     /* graph to operate on */
void* v,        /* vertext to operate on */
char** ctx  /* saved pointer */
)
{
   void* vtx1;
   void* vtx2;
   if (g->type == GRAPH_UNDIRECTED_T)
   {
      if (NULL == *ctx)
      {
         *ctx = (char*)((VTX_UD_T*)v)->ELst->next;
         vtx1 = ((VTX_UD_T*)v)->ELst->edge->v1;
         vtx2 = ((VTX_UD_T*)v)->ELst->edge->v2;
      }
      else
      {
         EDGE_T* e;
         e = ((VTX_EDGE*)*ctx)->edge;
         *ctx = (char*)((VTX_EDGE*)*ctx)->next;
         vtx1 = e->v1;
         vtx2 = e->v2;
      }
      return (vtx1==v)?vtx2:vtx1;      
   }
   else
   if (g->type == GRAPH_DIRECTED_T)
   {
      if (NULL == *ctx)
      {
         *ctx = (char*)((VTX_D_T*)v)->outELst->next;
         vtx1 = ((VTX_D_T*)v)->outELst->edge->v1;
         vtx2 = ((VTX_D_T*)v)->outELst->edge->v2;
      }
      else
      {
         EDGE_T* e;
         e = ((VTX_EDGE*)*ctx)->edge;
         *ctx = (char*)((VTX_EDGE*)*ctx)->next;
         vtx1 = e->v1;
         vtx2 = e->v2;
      }
      return (vtx1==v)?vtx2:vtx1;      
   }
   return NULL;
}

/**
 *
 * This routine returns the next edge in the list of edges incident to vertex
 * <v>. This routine is designed to work inside loop constructs where you
 * would want to iterate over all the vertex edges. <ctx> maintains the
 * context between each iteration. The contents of <ctx> need to be
 * initialied to NULL  before the first invocation of this routine. For
 * directed graphs the out edge list of a vertex are iterated over.
 * 
 * @param[in] g The graph to operate on
 * @param[in] v The vertex whose adjacent vertices need to be found out.
 * @param[out] ctx The iterator context needed for the operation of this routine.
 * @return Next edge in the list of vertex edges
 */
EDGE_T* graph_vertex_next_edge_get
(
GRAPH_T* g,     /* graph to operate on */
void* v,        /* vertext to operate on */
char** ctx  /* saved pointer */
)
{
   if (g->type == GRAPH_UNDIRECTED_T)
   {
      if (NULL == *ctx)
      {
         *ctx = (char*)((VTX_UD_T*)v)->ELst->next;
         return ((VTX_UD_T*)v)->ELst->edge;
      }
      else
      {
         EDGE_T* e;
         e = ((VTX_EDGE*)*ctx)->edge;
         *ctx = (char*)((VTX_EDGE*)*ctx)->next;
         return e;
      }
   }
   else
   if (g->type == GRAPH_DIRECTED_T)
   {
      if (NULL == *ctx)
      {
         *ctx = (char*)((VTX_D_T*)v)->outELst->next;
         return ((VTX_D_T*)v)->outELst->edge;
      }
      else
      {
         EDGE_T* e;
         e = ((VTX_EDGE*)*ctx)->edge;
         *ctx = (char*)((VTX_EDGE*)*ctx)->next;
         return e;
      }
   }
   return NULL;
}

/**
 * @brief This routine returns the next in edge in the list of edges incident to
 * vertex <v> in a directed graph.
 *
 * This routine is designed to work inside loop constructs where you would
 * want to iterate over all the vertex edges. <ctx> maintains the context
 * between each iteration. The contents of <ctx> need to be initialied to NULL
 * before the first invocation of this routine. 
 * 
 * @param[in] g The graph to operate on
 * @param[in] v The vertex whose adjacent vertices need to be found out.
 * @param[out] ctx The iterator context needed for the operation of this routine.
 * @return Next edge in the list of vertex edges
 */
EDGE_T* graph_vertex_next_in_edge_get
(
GRAPH_T* g,     /* graph to operate on */
void* v,        /* vertext to operate on */
char** ctx  /* saved pointer */
)
{
   if (g->type == GRAPH_UNDIRECTED_T)
   {
      if (NULL == *ctx)
      {
         *ctx = (char*)((VTX_UD_T*)v)->ELst->next;
         return ((VTX_UD_T*)v)->ELst->edge;
      }
      else
      {
         EDGE_T* e;
         e = ((VTX_EDGE*)*ctx)->edge;
         *ctx = (char*)((VTX_EDGE*)*ctx)->next;
         return e;
      }
   }
   else
   if (g->type == GRAPH_DIRECTED_T)
   {
      /* XXX: todo */
   }
   return NULL;
}

/**
* @brief Get the next vertex in the list of graph vertices
*
* This routine returns the next vertex in the list of vertices maintained by
* the graph. It is useful as a iterator to go over all the graph vertices.
*
* @param[in] g The graph to operate on
* @param[in] vtx Find the vertex next to this vertex. If NULL, then the
* routine returns the first vertex in the list.
* @return The valid next vertex object or NULL if reached the end of the list
*/
void* graph_vertex_next_get
(
GRAPH_T* g, 
void* vtx  
)
{
   if (g->type == GRAPH_UNDIRECTED_T)
   {
      if (NULL == vtx)
      {
         return g->vLst;
      }
      else
      {
         return ((VTX_UD_T*)vtx)->next;
      }
   }
   
   if (g->type == GRAPH_DIRECTED_T)
   {
      if (NULL == vtx)
      {
         return g->vLst;
      }
      else
      {
         return ((VTX_D_T*)vtx)->next;
      }
   }
   /* should never reach here */
   return NULL;
}
   
/**
 * Remove a vertex from the graph.
 *
 */
static GPH_ERR_E vertex_remove
(
GRAPH_T* g,
void* vtx
)
{
   void* iter_vtx;
   void* prev_iter_vtx = NULL;
   void* tmp_vtx;
   
   iter_vtx = g->eLst;
   while (iter_vtx)
   {
      if (iter_vtx == vtx)
      {
         if (NULL == prev_iter_vtx)
         {
            tmp_vtx = g->eLst;
            g->eLst = g->eLst->next;
            free (tmp_vtx);
         }
         else
         {
            if (g->type == GRAPH_UNDIRECTED_T)
            {
               prev_iter_vtx = ((VTX_UD_T*)iter_vtx)->next;
               free (iter_vtx);
            }
            if (g->type == GRAPH_DIRECTED_T)
            {
               prev_iter_vtx = ((VTX_D_T*)iter_vtx)->next;
               free (iter_vtx);
            }
         }
         break;
      }
      if (g->type == GRAPH_UNDIRECTED_T)
      {
         iter_vtx = ((VTX_UD_T*)iter_vtx)->next;
      }
      if (g->type == GRAPH_DIRECTED_T)
      {
         iter_vtx = ((VTX_D_T*)iter_vtx)->next;
          }
   }       
   return GPH_ERR_OK;
}

/*******************************************************************************
* graph_vertex_find_i - find a vertex object given it's integer identifier
*
* This routine returns the vertex object for the integer vertex identifier
* <vid> in graph <g>.
*
* RETURNS: A valid vertex object pointer or NULL if no vertex found
*/

void* graph_vertex_find_i
(
GRAPH_T* g,             /* graph to operate on */
unsigned long vid       /* interger vertex identifier */
)
{
   if (g->type == GRAPH_UNDIRECTED_T)
   {
      VTX_UD_T* vtx = g->vLst;
      
      while (NULL != vtx)
      {
         if (vtx->id.iid == vid)
         {
            return vtx;
         }
         vtx = vtx->next;
      }
   }
   
   if (g->type == GRAPH_DIRECTED_T)
   {
      VTX_D_T* vtx = g->vLst;
      
      while (NULL != vtx)
      {
         if (vtx->id.iid == vid)
            return vtx;
         vtx = vtx->next;
      }
   }
   return NULL;
}

/*******************************************************************************
* graph_vertex_find_c - find a vertex identifier given it's string label  
*
* RETURNS: 1 if the vertex exists, 0 if not
*/

void* graph_vertex_find_c
    (
    GRAPH_T* g,
    char* vid
    )
{
   if (g->type == GRAPH_UNDIRECTED_T)
   {
      VTX_UD_T* vtx = g->vLst;
      while (NULL != vtx)
      {
         if (strcmp (vtx->id.cid, vid))
            return vtx;
      }
   }
   
   if (g->type == GRAPH_DIRECTED_T)
   {
      VTX_D_T* vtx = g->vLst;
      while (NULL != vtx)
      {
         if (strcmp (vtx->id.cid, vid))
            return vtx;
      }
   }
   return NULL;
}

/**
 * @brief 
 */
static EDGE_T* graph_edge_find_undirected
(
GRAPH_T* g,     /* graph to operate on */
VTX_UD_T* v1,   /* edge vertex #1 */
VTX_UD_T* v2    /* edge vertex #2 */
)
{
   unsigned long no;
   char* ctx = NULL;
   EDGE_T* e;

   /* Look up all the edges adjacent to a vertex. For each edge, we check if
    * it's and edge from <v1> to <v2> or not. We choose the vertex with the
    * smaller number of incident edges to minimize the lookups.
    */      
   if (v1->no < v2->no)
   {
      no = 0;
      while (no < v1->no)
      {
         e = graph_vertex_next_edge_get (g, v1, &ctx);
         if (e->v1 == v2 || e->v2 == v2)
            return e;
         no++;
      }
   }
   else /* optimized to only search in the shorter of the two edge lists */
   {
      no = 0;
      while (no < v2->no)
      {
         e = graph_vertex_next_edge_get (g, v2, &ctx);
         if (e->v1 == v1 || e->v2 == v1)
            return e;
         no++;
      }
   }
   return NULL;
}

/**
 * @brief 
 */
static EDGE_T* graph_edge_find_directed
(
GRAPH_T* g,     /* graph to operate on */
VTX_D_T* v1,    /* edge vertex #1 */
VTX_D_T* v2     /* ed */
)
{
   unsigned long no;
   char* ctx = NULL;
   EDGE_T* e;

   /* Look up all the edges adjacent to a vertex. For each edge, we check if
    * it's an edge from <v1> to <v2> or not. We choose the vertex with the
    * smaller number of incident edges to minimize the lookups.
    */      
   if (v1->no < v2->no)
   {
      no = 0;
      while (no < v1->no)
      {
         e = graph_vertex_next_edge_get (g, v1, &ctx);
         if (e->v2 == v2)
            return e;
         no++;
      }
   }
   else
   {
      no = 0;
      while (no < v2->no)
      {
         e = graph_vertex_next_edge_get (g, v2, &ctx);
         if (e->v1 == v1)
            return e;
         no++;
      }
   }
   return NULL;
}

/**
 * @brief This routine finds out if an edge exists between two vertices.
 *
 * For a directed graph it takes into account direction of the edge too.
 *
 * @param[in] g The graph to operate on
 * @param[in] v1 The source vertex in a directed graph or one of the edge
 * vertices in an undirected graph.
 * @param[in] v2 The destination vertex in a directed graph or one of the edge
 * vertices in an undirected graph.
 * @return A valid edge if edge exists, NULL if it does not
 */
EDGE_T* graph_edge_find
(
GRAPH_T* g,
void* v1,
void* v2
)
{
   EDGE_T* e = NULL;
   
   if (g->type == GRAPH_DIRECTED_T)
   {
      e = graph_edge_find_directed (g, v1, v2);
   }
   else if (g->type == GRAPH_UNDIRECTED_T)
   {
      e = graph_edge_find_undirected (g, v1, v2);
   }
   /* should never reach here */
   return e;
}

/*******************************************************************************
* graph_v_insert - insert a new edge into the graph
*
* RETURNS: A valid vertex node if created or present, NULL on error 
*/

static GPH_ERR_E graph_add_edge_to_vertices
(
GRAPH_T* g,
EDGE_T* edge,    
void* v1,
void* v2
)
{
   VTX_EDGE* ve;
   
   if (g->type == GRAPH_UNDIRECTED_T)
   {
      VTX_UD_T* vtx = v1;

      ve = malloc (sizeof (VTX_EDGE));
      if (NULL == ve)
         return GPH_ERR_ERR;
      ve->edge = edge;
      ve->next = NULL;
      
      if (NULL == vtx->ELst)
      {
         vtx->ELst = ve;
         vtx->last_edge = ve;
      }
      else
      {
         vtx->last_edge->next = ve;
         vtx->last_edge = ve;
      }
      vtx->no++;
      
      ve = malloc (sizeof (VTX_EDGE));
      if (NULL == ve)
         return GPH_ERR_ERR;
      ve->edge = edge;
      ve->next = NULL;

      
      vtx = v2;
      if (NULL == vtx->ELst)
      {
         vtx->ELst = ve;
         vtx->last_edge = ve;
      }
      else
      {
         vtx->last_edge->next = ve;
         vtx->last_edge = ve;
      }
      vtx->no++;
   }
   
   if (g->type == GRAPH_DIRECTED_T)
   {
      VTX_D_T* vtx1 = v1;
      VTX_D_T* vtx2 = v2;
      
      ve = malloc (sizeof (VTX_EDGE));
      if (NULL == ve)
         return GPH_ERR_ERR;
      ve->edge = edge;
      ve->next = NULL;

      
      if (NULL == vtx1->outELst)
      {
         vtx1->outELst = ve;
         vtx1->last_out_edge = ve;
      }
      else
      {
         vtx1->last_out_edge->next = ve;
         vtx1->last_out_edge = ve;
      }
      vtx1->no++;
      
      ve = malloc (sizeof (VTX_EDGE));
      if (NULL == ve)
         return GPH_ERR_ERR;
      ve->edge = edge;
      ve->next = NULL;
      
      if (NULL == vtx2->inELst)
      {
         vtx2->inELst = ve;
         vtx2->last_in_edge = ve;
      }
      else
      {
         vtx2->last_in_edge->next = ve;
         vtx2->last_in_edge = ve;
      }

   }
   return GPH_ERR_OK;
}

/**
 * Insert a vertex in the list of vertices of the graph. There is no
 * intelligence built-in the ordering of these vertices. They are inserted as
 * they are created so they are in first-created-first-in order
 *
 * @param[in] g The graph to operate on
 * @param[in] vtx The vertex to insert in the graph
 */
static GPH_ERR_E vertex_insert
(
GRAPH_T* g,
void* vtx
)
{
   if (NULL == g)
      return GPH_ERR_ERR;
   
   if (NULL == g->vLst)
   {
      g->last_vertex = vtx;
      g->vLst = vtx;
   }
   else
   {
      if (g->type == GRAPH_UNDIRECTED_T)
      {         
         VTX_UD_T* lvtx = g->last_vertex;
         lvtx->next = vtx;
         g->last_vertex = vtx;
      }
      if (g->type == GRAPH_DIRECTED_T)
      {
         VTX_D_T* lvtx = g->last_vertex;
         lvtx->next = vtx;
         g->last_vertex = vtx;
      }
   }
   g->v_no++;
   return GPH_ERR_OK;
}

/**
 * Creates a new graph vertex given an integer vertex identifier. If a vertex
 * with the specified vertex identifier exists, the routine will return a
 * vertex object for that identifier.
 *
 * @param[in] g    The graph to operate on
 * @param[in] vid  An integer vertex identifier
 * @param[in] info User-specified information that can be attached to the
 *                 vertex
 * @return A valid vertex node if created or present, NULL on error 
 */
void* graph_v_create_i
(
GRAPH_T* g,         
unsigned long vid,  
void* info          
)
{
   void* v;

   /* Find out of a vertex object exists with the given vid */
   if (NULL != (v = graph_vertex_find_i (g, vid)))
   {
      return v;
   }
   
   if (g->type == GRAPH_UNDIRECTED_T)
   {
      VTX_UD_T* vtx;
      
      if (NULL == (vtx = malloc (sizeof (VTX_UD_T))))
         return NULL;
      vtx->ELst = NULL; 
      vtx->next = NULL;
      vtx->no = 0;
      vtx->id.iid = vid;
      vtx->aux = info;
      vertex_insert (g, vtx);
      return vtx;
   }
   
   if (g->type == GRAPH_DIRECTED_T)
   {
      VTX_D_T* vtx;
      if (NULL == (vtx = malloc (sizeof (VTX_D_T))))
         return NULL;

      vtx->outELst = NULL;
      vtx->inELst = NULL;
      vtx->next = NULL;
      vtx->no = 0;
      vtx->id.iid = vid;
      vtx->aux = info;
      vertex_insert (g, vtx);
      return vtx;
   }
   return NULL;
}

/*******************************************************************************
* graph_v_create_c - create a vertex given a textual vertex identifier
*
* This routine creates a new vertext given a string vertex identifier
* <v>. <info> specifies the aux. information that we might need to attach to
* the graph.
*
* RETURNS: Created vertext if successful, NULL otherwise
*/

static void* graph_v_create_c
(
GRAPH_T* g,
char* v,
void* info
)
{
   void* vtx;
   if (NULL != (vtx = graph_vertex_find_c (g, v)))
      return vtx;
   
   if (g->type == GRAPH_UNDIRECTED_T)
   {
      VTX_UD_T* vtx;
      
      if (NULL == (vtx = malloc (sizeof (VTX_UD_T))))
         return NULL;
      vtx->ELst = NULL;
      vtx->next = NULL;
      vtx->no = 0;
      vtx->id.cid = malloc (strlen (v) + 1);
      strcpy (vtx->id.cid, v);
      vtx->aux = info;
      vertex_insert (g, vtx);
      return vtx;
   }
   
   if (g->type == GRAPH_DIRECTED_T)
   {
      VTX_D_T* vtx;
      if (NULL == (vtx = malloc (sizeof (VTX_UD_T))))
         return NULL;
      
      vtx->outELst = NULL;
      vtx->inELst = NULL;
      vtx->next = NULL;
      vtx->no = 0;
      vtx->id.cid = malloc (strlen (v) + 1);
      strcpy (vtx->id.cid, v);
      vtx->aux = info;
      vertex_insert (g, vtx);
   }
   
   return NULL;    
}

/*******************************************************************************
* graph_v_remove_i - remove a integer lable vertex from the graph 
*
* RETURNS: GPH_ERR_E 
*/

static GPH_ERR_E graph_v_remove_i
    (
    GRAPH_T* g,         /* graph to add vertex to */
    unsigned long v     /* vertext identifier */
    )
{
    return GPH_ERR_OK;    
    }

/*******************************************************************************
* edge_remove - 
*
* RETURNS: 
*/

static GPH_ERR_E edge_remove
(
GRAPH_T* g,
EDGE_T* edge
)
{
   EDGE_T* iter_edge;
   EDGE_T* prev_iter_edge = NULL;
   EDGE_T* tmp_edge;
   
   iter_edge = g->eLst;
   while (iter_edge)
   {
      if (iter_edge == edge)
      {
         if (NULL == prev_iter_edge)
         {
            tmp_edge = g->eLst;
            g->eLst = g->eLst->next;
            free (tmp_edge);
         }
         else
         {
            prev_iter_edge = iter_edge->next;
            free (iter_edge);
         }
         break;
      }
      iter_edge = iter_edge->next;
   }       
   return GPH_ERR_OK;
}

/*******************************************************************************
* graph_v_add_c - add a new 
*
* RETURNS: 
*/

static GPH_ERR_E graph_v_remove_c
    (
    GRAPH_T* g,
    char* v
    )
    {

    return GPH_ERR_OK;
    }

/*******************************************************************************
* graph_remove_i - 
*
*
* RETURNS: GPH_ERR_E
*/

GPH_ERR_E graph_remove_i
(
GRAPH_T* g,
unsigned long v1,       /* integer vertex identifier. */
unsigned long v2       /* integer vertex identifier. */
)
{
   return GPH_ERR_ERR;   
}

/**
 * @brief Insert an edge in the graph
 *
 * @param[in] g The graph to operate on
 * @param[in] edge The edge to insert into the graph
 * @returns GPH_ERR_OK if unsuccessful, GPH_ERR_ERR othewise
 */

static GPH_ERR_E edge_insert
(
GRAPH_T* g,
EDGE_T* edge
)
{
   if (NULL == g)
      return GPH_ERR_ERR;
   
   if (NULL == g->eLst)
   {
      g->last_edge = edge;
      g->eLst = edge;
   }
   else
   {
      g->last_edge->next = edge;
      g->last_edge = edge;
   }
   g->e_no++;
   return GPH_ERR_OK;
}

/**
 * @brief This routine either adds an edge and it's two vertices, or a *
 * stand-alone vertext to the graph.
 *
 * The vertex identifier are integer. If <is_edge> is TRUE, then this API
 * inserts an edge defined by the two vertices <v1> and <v2>. If <g> is a
 * directed graph then <v1> is considered to be the source vertex and <v2> the
 * destination vertex. If <is_edge> is FALSE, then this API inserts a
 * stand-alone vertext defined by <v1>.
 *
 * @param[in] g         graph to add the edge or vertices
 * @param[in] info      edge label
 * @param[in] v1        integer vertex identifier
 * @param[in] v1_info   auxilliary vertex information
 * @param[in] v2        integer vertex identifier. Ignored if <is_edge> is FALSE
 * @param[in] v2_info   auxilliary vertex information. Ignored if <is_edge> is FALSE
 * @param[in] weight    edge weight. Ignored if <is_edge> is FALSE
 * @param[in] is_edge   TRUE if inserting an edge, FALWSE if inserting a stand-alone vertex
 * @return GPH_ERR_OK if successful, GPH_ERR_ERR otherwise
 */
GPH_ERR_E graph_add_i
(
GRAPH_T* g,             
void* info,             
unsigned long v1,       
void* v1_info,          
unsigned long v2,       
void* v2_info,          
unsigned int weight,    
BOOL_E is_edge          
)
{
   EDGE_T* e;
   void *v1o, *v2o;

   v1o = graph_v_create_i (g, v1, info);
   v2o = graph_v_create_i (g, v2, info);
   
   if (NULL  != graph_edge_find (g, v1o, v2o))
   {
      return GPH_ERR_EDGE_EXISTS;
   }
   
   if (NULL == (e = malloc (sizeof (EDGE_T))))
      return GPH_ERR_MALLOC_FAIL;   
   //fprintf (stderr, "v1=%lu v2=%lu\n", ((VTX_UD_T*)v1o)->id.iid, ((VTX_UD_T*)v2o)->id.iid);
   e->next = NULL;
   e->v1 = v1o;
   e->v2 = v2o;
   e->weight = weight;
   e->aux = info;
   edge_insert (g, e);
   
   /* update the edge lists for the vertices */
   if (GPH_ERR_ERR == graph_add_edge_to_vertices (g, e, v1o, v2o))
   {
      return GPH_ERR_EDGE_ADD_FAIL;
   }
   return GPH_ERR_OK;   
}

/**
 * @brief This routine adds either edge and it's two vertices, or a
 * stand-alone vertext to the graph. The vertex identifiers are text.
 *
 * If 'is_edge' is TRUE, then this API inserts an edge defined by the two
 * vertices 'v1' and 'v2'. If <g> is a directed graph then <v1> is considered
 * to be the source vertex and <v2> the destination vertex. If <is_edge> is
 * FALSE, then this API inserts a stand-alone vertext defined by <v1>.
 *
 * @param[in] g         graph to add the edge or vertices
 * @param[in] info      edge label
 * @param[in] v1        text vertex identifier
 * @param[in] v1_info   auxilliary vertex information
 * @param[in] v2        text vertex identifier. Ignored if <is_edge> is FALSE
 * @param[in] v2_info   auxilliary vertex information. Ignored if <is_edge> is FALSE
 * @param[in] weight    edge weight. Ignored if <is_edge> is FALSE
 * @param[in] is_edge   TRUE if inserting an edge, FALWSE if inserting a stand-alone vertex
 * @return GPH_ERR_OK if successful, GPH_ERR_ERR otherwise
 */
GPH_ERR_E graph_add_c
(
GRAPH_T* g,
void* info,     
char* v1,       
void* v1_info,       
char* v2,               
void* v2_info, 
unsigned int weight,    
BOOL_E is_edge       
)
{
   EDGE_T* e;
   void *v1o, *v2o;

   if (NULL == (e = malloc (sizeof (EDGE_T))))
      return GPH_ERR_MALLOC_FAIL;

   v1o = graph_v_create_c (g, v1, info);
   v2o = graph_v_create_c (g, v2, info);

   e->next = NULL;   
   e->v1 = v1o;
   e->v2 = v2o;
   e->weight = weight;
   e->aux = info;
   edge_insert (g, e);
   
   /* update the edge lists for the vertices */
   if (GPH_ERR_ERR == graph_add_edge_to_vertices (g, e, v1o, v2o))
   {
      return GPH_ERR_EDGE_ADD_FAIL;
   }
   return GPH_ERR_OK;   
}

/**
 * @brief This routine deletes a graph.
 *
 * It will also free up all the edge and vertex objects maintained within the
 * graph.
 *
 * @param[in] g The graph to delete
 * @return GPH_ERR_OK if successfull, GPH_ERR_OK otherwise
 */
GPH_ERR_E graph_delete
(
GRAPH_T* g
)
{
   EDGE_T* ep = g->eLst;
   EDGE_T* en = NULL;

#define NEXT_AND_FREE(PREV, NEXT)                                       \
   do                                                                   \
   {                                                                    \
      while (PREV)                                                      \
      {                                                                 \
         NEXT = PREV->next;                                             \
         free (PREV);                                                   \
         PREV = NEXT;                                                   \
      }                                                                 \
   }while(0)
   
   /* free the edge list */
   NEXT_AND_FREE (ep, en);
   if (g->type == GRAPH_UNDIRECTED_T)
   {
      VTX_UD_T* vp = g->vLst;
      VTX_UD_T* vn = NULL;
      
/* free the vertex list */
      NEXT_AND_FREE (vp, vn);
   }
   else if (g->type == GRAPH_DIRECTED_T)
   {
      VTX_D_T* vp = g->vLst;
      VTX_D_T* vn = NULL;
/* free the out edge list */
      NEXT_AND_FREE (vp, vn);
   }
   
   return GPH_ERR_OK;   
}

/**
 * @brief This routine creates a new graph
 *
 * @param[in] type The type of graph to create - directed or undirected
 * @param[in] label_id_type The type of vertex identifiers the graph will use
 * - integer or string identifiers
 * @return A valid GRAPH_T* if successfull or NULL if unsuccessful
 */
GRAPH_T* graph_new
(
GRAPH_TYPE_E type,              /* directed or undirected graph */
GRAPH_LABEL_E label_id_type     /* integer or string vertex identifiers */
)
{
   GRAPH_T* gph = NULL;
   
   if (NULL == (gph = malloc (sizeof (GRAPH_T))))
      goto graph_new_err;
   gph->eLst = NULL;
   gph->vLst = NULL;
   gph->v_no = 0;
   gph->e_no = 0;
   gph->type = type;
   gph->label_type = label_id_type;
   return gph;
   
graph_new_err:
   if (gph)
      free (gph);
   
   return NULL;
}

