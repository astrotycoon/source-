#ifndef _TREE_H_
#define _TREE_H_

#include <assert.h>
#include <malloc.h>

struct _tree {
    struct _tree *l, *r, *p;
    void *val;
};

struct tree {
    struct _tree *root;
    int (*cmp)(const void *, const void *, void *);
    void *data;
};

#define islchild(t) (t == t->p->l)
#define isrchild(t) (t == t->p->r)
#define isroot(t) (!t->p)
#define isinternal(t) (t->l || t->r)
#define isleaf(t) (!isinternal(t))

static inline void _tinit(struct _tree *t)
{
    t->l = t->r = t->p = t->val = NULL;
}

static inline struct _tree *_tnew()
{
    struct _tree *t;
    t = (struct _tree *) malloc(sizeof(struct _tree));
    assert(t);
    _tinit(t);
    return t;
}

static inline void tinit(struct tree *t,
		int (*cmp)(const void *, const void *, void *), void *data)
{
    t->root = _tnew();
    t->cmp = cmp;
    t->data = data;
}

static inline struct tree *tnew(int (*cmp)(const void *, const void *, void *),
	void *data)
{
    struct tree *t;
    t = (struct tree *) malloc(sizeof(struct tree));
    assert(t);
    tinit(t, cmp, data);
    return t;
}

struct _tree *_tfind(struct tree *t, struct _tree *node, const void *elt);

static inline struct tree *tfind(struct tree *t, const void *elt)
{
    struct tree *wrapper;

    assert(t);

    wrapper = tnew(t->cmp, t->data);
    wrapper->root = _tfind(t, t->root, elt);

    return wrapper;
}

struct tree *tinsert(struct tree *t, void *elt);

struct tree *tdelete(struct tree *t, const void *elt);

void _preorder(struct _tree *node, void (*act)(const void *, void *),
       	void *data);

static inline void preorder(struct tree *t, void (*act)(const void *, void *),
       	void *data)
{
    assert(t);
    _preorder(t->root, act, data);
}

void _inorder(struct _tree *node, void (*act)(const void *, void *),
       	void *data);

static inline void inorder(struct tree *t, void (*act)(const void *, void *),
	void *data)
{
    assert(t);
    _inorder(t->root, act, data);
}

void _postorder(struct _tree *node, void (*act)(const void *, void *),
       	void *data);

static inline void postorder(struct tree *t, void (*act)(const void *, void *),
	void *data)
{
    assert(t);
    _postorder(t->root, act, data);
}

#endif  /* _TREE_H_ */
