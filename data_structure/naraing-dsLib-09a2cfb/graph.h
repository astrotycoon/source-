/*
 * graph data structure headers
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

typedef enum {
   GRAPH_DIRECTED_T=0,
   GRAPH_UNDIRECTED_T=1
}GRAPH_TYPE_E;

/**
 * @enum graph vertex identifier types
 */
typedef enum {
   GRAPH_INT_T=0,
   GRAPH_CHAR_T=1
}GRAPH_LABEL_E;

/* vertex identifier */
typedef union
    {        
    char* cid;          /* vertex identified by a textual label, or */
    unsigned long iid;  /* vertex identified by an integer label */
    }VTXID_U;

/* edge identifier */
typedef union
    {             
    char* cid;          /* edge identified by a textual label, or */
    unsigned long iid;  /* edge identified by an integer label*/
    }EDGID_U;

/**< edge node */
typedef struct _dsedge {
   struct _dsedge* next;/**< next edge in the list */
   void* v1;            /**< edge vertex #1. This is the source vertex in a directed graph */
   void* v2;            /**< edge vertext #2. This is the destination vertex in a directed graph */
   unsigned long weight;/**< edge weight */
   void* aux;           /**< auxilliary data to attach to the vertex */
   char label[128];     /**< textual edge label */
   EDGID_U id;          /**< edge identifier */
}EDGE_T;

/* list for list of edges incident on a node */
typedef struct _vtxedge {
   struct _vtxedge* next;
   EDGE_T* edge;   
} VTX_EDGE;

/* common vertex node fields */
#define COMMON_VERTEX_FIELDS                                            \
   struct _dsvtx* next;                                                 \
   unsigned long no;   /* total number of edges incident on the vertex */ \
   void* aux;                                                           \
   VTXID_U id;         /* vertex identifier */                          \

/* vertex node for a directed graph */
typedef struct _dsvtxd {
   struct _dsvtxd* next; /* next vertex in the list */
   VTX_EDGE* outELst;  /* list of outward directed edges incident on the vertex */
   VTX_EDGE* inELst;   /* list of inward directed edges incident on the vertex */
   unsigned long no;   /* total number of edges incident on the vertex */
   void* aux;          /* auxilliary data to attach to the vertex */
   VTXID_U id;         /* vertex identifier */
   VTX_EDGE* last_out_edge;
   VTX_EDGE* last_in_edge;
}VTX_D_T;

/* vertex node for an un-directed graph */
typedef struct _dsvtxud {
   struct _dsvtxud* next; /* next vertex in the list */
   VTX_EDGE* ELst;     /* list of edges incident on the vertex */
   unsigned long no;   /* total number of edges incident on the vertex */
   void* aux;          /* auxilliary data to attach to the vertex */
   VTXID_U id;         /* vertex identifier */
   VTX_EDGE* last_edge;
}VTX_UD_T;

/* A graph */
typedef struct _dsgraph {
   EDGE_T* eLst;       /* list of edges in the graph */
   void* vLst;         /* list of vertices in the graph */
   unsigned long v_no; /* total number of vertices in the graph */
   unsigned long e_no; /* total number of edges in the graph */
   VTXID_U last_vid;
   EDGID_U last_eid;
   GRAPH_TYPE_E type;
   EDGE_T* last_edge;
   void* last_vertex;
   GRAPH_LABEL_E label_type;
}GRAPH_T;

/* graph operation result code  */
typedef enum {
   GPH_ERR_ERROR_HIGH = -512,   /* fencepost */
   GPH_ERR_MALLOC_FAIL,
   GPH_ERR_EDGE_ADD_FAIL,
   GPH_ERR_ERR = -1,            /* generic operational error */
   GPH_ERR_OK = 0,              /* routine returned without any errors */
   GPH_ERR_EDGE_EXISTS = 1,
   GPH_ERR_VERTEX_EXISTS = 2   
} GPH_ERR_E;

#define GRAPH_NO_VERTICES(G)    ((G)->v_no)
#define GRAPH_NO_EDGES(G)       ((G)->e_no)

#define DEBUG_EDGE_PRINT_I(G,E)                                         \
   do                                                                   \
   {                                                                    \
      if (G->type == GRAPH_UNDIRECTED_T)                                \
      {                                                                 \
         fprintf (stdout, "*edge (%s) v1=%lu v2=%lu\n",                 \
                  (char*)E->aux,                                        \
                  ((VTX_UD_T*)E->v1)->id.iid,                           \
                  ((VTX_UD_T*)E->v2)->id.iid);                          \
      }                                                                 \
                                                                        \
      if (G->type == GRAPH_DIRECTED_T)                                  \
      {                                                                 \
         fprintf (stdout, "*edge (%s) v1=%lu v2=%lu\n",                 \
                  (char*)E->aux,                                        \
                  ((VTX_D_T*)E->v1)->id.iid,                            \
                  ((VTX_D_T*)E->v2)->id.iid);                           \
      }                                                                 \
                                                                        \
   }while (0)

#define DEBUG_EDGE_PRINT_C(G,E)                                         \
   do                                                                   \
   {                                                                    \
      if (G->type == GRAPH_UNDIRECTED_T)                                \
      {                                                                 \
         fprintf (stdout, "*edge (%s) v1=%s v2=%s\n",                   \
                  (char*)E->aux,                                        \
                  ((VTX_UD_T*)E->v1)->id.cid,                           \
                  ((VTX_UD_T*)E->v2)->id.cid);                          \
      }                                                                 \
                                                                        \
      if (G->type == GRAPH_DIRECTED_T)                                  \
      {                                                                 \
         fprintf (stdout, "*edge (%s) v1=%s v2=%s\n",                   \
                  (char*)E->aux,                                        \
                  ((VTX_D_T*)E->v1)->id.cid,                            \
                  ((VTX_D_T*)E->v2)->id.cid);                           \
      }                                                                 \
                                                                        \
   }while (0)


/* get the next edge in the list of graph edges */
EDGE_T* graph_edge_next_get (GRAPH_T* g, EDGE_T* e);
/* get the vertex next in the list of graph vertices  */
void* graph_vertex_next_get (GRAPH_T* g, void* v);
/* get the number of edges incident on a vertex */
unsigned long graph_edges_no_get (GRAPH_T* g, void* v);
/* get the vertex adjacent to a given vertex in the graph */
void* graph_vertex_next_adj_get (GRAPH_T* g, void* v, char** ctx);
/* get the vertex edge next to given edge */
EDGE_T* graph_vertex_next_edge_get (GRAPH_T* g, void* v, char** saveptr);
/* delete a graph */
GPH_ERR_E graph_delete (GRAPH_T* g);
/* create a new graph */
GRAPH_T* graph_new (GRAPH_TYPE_E type, GRAPH_LABEL_E label_id_type);
/* add a new edge to the graph */
GPH_ERR_E graph_add_i (GRAPH_T* g, void* info, unsigned long v1,
                       void* v1_info, unsigned long v2, void* v2_info,
                       unsigned int weight, BOOL_E is_edge);
/* find if an edge (u, v) exists */
EDGE_T* graph_edge_find (GRAPH_T* g, void* u, void* v);
/* return the vertex identified by the integer label */
void* graph_vertex_find_i (GRAPH_T* g, unsigned long vid);
/* diagnostic print */
void graph_edge_print (GRAPH_T* g);
/* diagnostic print */
void graph_vertex_print (GRAPH_T* g);
