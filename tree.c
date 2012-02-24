#include <stdio.h>
#include <stdlib.h>

#include "tree.h"

void (*data_free)(void*)=free;

tree *tree_new(void* data)
{
	tree *node = (tree*)malloc(sizeof(tree));

	if (node==NULL)
		return NULL;

	node->root = NULL;
	node->data = data;
	node->first = NULL;
	node->last = NULL;
	node->next = NULL;
	node->prev = NULL;
	node->actual = NULL;

	return node;
}

tree *tree_add_first(tree *node, tree *data)
{
	if (node==NULL || data==NULL) {
		return NULL;
	}

	tree_rem(data);
	data->root = node;
	data->prev = NULL;
/*	data->data=data; */
	data->next = node->first;

	if (node->first == NULL) {
		node->last = data;
		node->actual = data;
	} else
		node->first->prev = data;
	node->first = data;

	return data;
}

tree *tree_push(tree *node, void* data)
{
	return tree_add_first(node, tree_new(data));
}

tree *tree_pop(tree *node)
{
	tree *pom;
	
	if (node==NULL || node->first==NULL)
		return NULL;

	pom = node->first;
	node->first = pom->next;
	if (node->first == NULL)
		node->last = NULL;
	else
		node->first->prev = NULL;
	if (node->actual == pom)
		node->actual = node->first;
	pom->root = NULL;

	return pom;
}

tree *tree_rem(tree *item)
{
	tree *node;
	
	if (item == NULL)
		return NULL;

	node = item->root;
	if (node == NULL)
		return item;

	if (item->next == NULL) {
		node->last = item->prev;
	} else
		item->next->prev = item->prev;

	if (item->prev == NULL) {
		node->first = item->next;
	} else
		item->prev->next = item->next;
	
	if (item == node->actual) {
		if (item->next != NULL)
			node->actual = item->next;
		else
			node->actual = item->prev;
	}
	item->root = NULL;
	item->next = NULL;
	item->prev = NULL;

	return item;
}

/*
tree *tree_rem_tree(tree *node)
{
	tree *root;
	ptree_item pom;

	if (node==NULL || node->root==NULL)
		return NULL;
	
	root=node->root;
	pom=root->first;
	
	while (pom!=NULL && pom->tree!=node)
		pom=pom->next;

	if (pom==NULL)
		return NULL;
	
    if (tree_rem(root, pom)!=node){
		fprintf(stderr,"ERROR -- tree_rem_tree(): remove other tree\n");
		return NULL;
	}

	return node;
}
*/

void tree_del(tree *node)
{
	if (node==NULL)
		return;

	data_free(node->data);

	tree_rem(node);
	while (node->first != NULL) {
		tree_del(node->first);
	}
	tree_del(node->next);

	free(node);
}

void tree_del_nodes(tree *node)
{
	if (node==NULL)
		return;
	while (node->first != NULL)
		tree_del(node->first);
}
