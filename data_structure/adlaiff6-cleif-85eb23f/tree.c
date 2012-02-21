#include <tree.h>

struct _tree *_tfind(struct tree *t, struct _tree *node, const void *elt)
{
    int c;
    struct _tree *found;

    assert(node);

    if (!node->val) {
	found = node;
	goto out;
    }

    c = t->cmp(elt, node->val, t->data);

    if (c < 0) {
	if (!node->l) {
	    found = node->l = _tnew();
	} else {
	    found = _tfind(t, node->l, elt);
	}
    } else if (c > 0) {
	if (!node->r) {
	    found = node->r = _tnew();
	} else {
	    found = _tfind(t, node->r, elt);
	}
    } else {
	found = node;
    }

out:
    return found;
}

struct tree *tinsert(struct tree *t, void *elt)
{
    struct _tree *loc;
    struct tree *wrapper;

    assert(t);

    loc = _tfind(t, t->root, elt);
    loc->val = elt;

    wrapper = tnew(t->cmp, t->data);
    wrapper->root = loc;
    return wrapper;
}

struct tree *tdelete(struct tree *t, const void *elt)
{
    struct _tree *loc, *repl, *repr, *rep;
    struct tree *wrapper;

    assert(t);

    loc = _tfind(t, t->root, elt);

    rep = NULL;
    if (isinternal(loc)) {
	repl = loc->l;
	repr = loc->r;

	while (repl && repr) {
	    repl = repl->r;
	    if (repl) {
		repr = repr->l;
	    }
	}

	rep = (struct _tree *) (repl || repr);

	rep->p = loc->p;
	rep->l = loc->l;
	rep->l->p = rep;
	rep->r = loc->r;
	rep->r->p = rep;
    }

    if (!isroot(loc)) {
	if (islchild(loc)) {
	    loc->p->l = rep;
	} else if (isrchild(loc)) {
	    loc->p->r = rep;
	}
    }

    wrapper = tnew(t->cmp, t->data);
    wrapper->root = loc;
    return wrapper;
}

void _preorder(struct _tree *node, void (*act)(const void *, void *),
       	void *data)
{
    assert(node);
    if (node->val) {
	act(node->val, data);
    }
    if (node->l) {
	_preorder(node->l, act, data);
    }
    if (node->r) {
	_preorder(node->r, act, data);
    }
}

void _inorder(struct _tree *node, void (*act)(const void *, void *),
       	void *data)
{
    assert(node);
    if (node->l) {
	_inorder(node->l, act, data);
    }
    if (node->val) {
	act(node->val, data);
    }
    if (node->r) {
	_inorder(node->r, act, data);
    }
}

void _postorder(struct _tree *node, void (*act)(const void *, void *),
       	void *data)
{
    assert(node);
    if (node->l) {
	_postorder(node->l, act, data);
    }
    if (node->r) {
	_postorder(node->r, act, data);
    }
    if (node->val) {
	act(node->val, data);
    }
}
