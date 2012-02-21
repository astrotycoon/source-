#ifndef LIBHX_MAP_INTERNAL_H
#define LIBHX_MAP_INTERNAL_H 1

#include <libHX/list.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @type:	actual type of map (%HX_MAPTYPE_*), used for virtual calls
 * @ops:	function pointers for key and data management
 * @flags:	bitfield of map flags
 */
struct HXmap_private {
	/* from struct HXmap */
	unsigned int items, flags;

	/* private: */
	enum HXmap_type type;
	size_t key_size, data_size;
	struct HXmap_ops ops;
};

/**
 * @bk_array:	bucket pointers
 * @power:	index into HXhash_primes to denote number of buckets
 * @max_load:	maximum number of elements before table gets enlarged
 * @min_load:	minimum number of elements before table gets shrunk
 * @tid:	transaction ID, used to track relayouts
 */
struct HXhmap {
	struct HXmap_private super;

	struct HXlist_head *bk_array;
	unsigned int power, max_load, min_load, tid;
};

/**
 * @anchor:	anchor point in struct HXhmap_node
 * @key:	data that works as key
 * @data:	data that works as value
 */
struct HXhmap_node {
	struct HXlist_head anchor;
	/* HXmap_node */
	union {
		void *key;
		const char *const skey;
	};
	union {
		void *data;
		char *sdata;
	};
};

struct HXmap_trav {
	enum HXmap_type type;
	unsigned int flags;
};

struct HXhmap_trav {
	struct HXmap_trav super;
	const struct HXhmap *hmap;
	const struct HXlist_head *head;
	unsigned int bk_current, tid;
};

enum {
	RBT_LEFT = 0,
	RBT_RIGHT = 1,
	RBT_RED = 0,
	RBT_BLACK,
	/* Allows for at least 16 million objects (in a worst-case tree) */
	RBT_MAXDEP = 48,
	/*
	 * Height h => pow(2, h/2)-1 nodes minimum. 1: 1, 2: 2, 4: 4, 6: 8,
	 * 8: 16, 10: 32, 12: 64, 14: 128, 16: 256, 18: 512, 20: 1K, 22: 2K,
	 * 24: 4K, 26: 8K, 28: 16K, 30: 32K, 32: 64K, 34: 128K, 36: 256K,
	 * 38: 512K, 40: 1M, 42: 2M, 44: 4M. 46: 8M, 48: 16M, 50: 32M, 52: 64M,
	 * 54: 128M, 56: 256M, 58: 512M, 60: 1G, 62: 2G, 64: 4G.
	 */
};

/**
 * @sub:	leaves
 * @color:	RBtree-specific node color
 */
struct HXrbtree_node {
	struct HXrbtree_node *sub[2];
	/* HXmap_node */
	union {
		void *key;
		const char *const skey;
	};
	union {
		void *data;
		char *sdata;
	};
	unsigned char color;
};

struct HXrbtree {
	struct HXmap_private super;
	struct HXrbtree_node *root;
	unsigned int tid;
};

struct HXrbtrav {
	struct HXmap_trav super;
	unsigned int tid; /* last seen btree transaction */
	const struct HXrbtree *tree;
	struct HXrbtree_node *current; /* last visited node */
	char *checkpoint;
	struct HXrbtree_node *path[RBT_MAXDEP]; /* stored path */
	unsigned char dir[RBT_MAXDEP];
	unsigned char depth;
};

typedef bool (*qfe_fn_t)(const struct HXmap_node *, void *);

extern const unsigned int HXhash_primes[];

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* LIBHX_MAP_INTERNAL_H */
