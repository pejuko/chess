#ifndef __TREE_H__
#define __TREE_H__

typedef struct tree {
	struct tree *root;
	void *data;

	struct tree *first;
	struct tree *actual;
	struct tree *last;

	struct tree *next;
	struct tree *prev;
} tree;

extern tree *tree_new(void *data);
extern tree *tree_add_first(tree *node, tree *data);
extern tree *tree_push(tree *node, void *data);
extern tree *tree_pop(tree *node);
extern tree *tree_rem(tree *item);
/* extern tree *tree_rem_tree(tree *node); */
extern void tree_del(tree *node);
extern void tree_del_nodes(tree *node);

extern void (*data_free)(void*);

#endif
