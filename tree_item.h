#ifndef __TREE_ITEM_H__
#define __TREE_ITEM_H__

#include "tree.h"

typedef struct stree_item {
	struct stree_item* prev;
	void* data;
	ptree tree;
	struct stree_item* next;
} ttree_item, *ptree_item;

#endif
