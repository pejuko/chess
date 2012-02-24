#include <stdlib.h>

#include "list.h"

plist list_new(void)
{
	plist lst = malloc(sizeof(tlist));
	
	if (lst==NULL)
		return NULL;
	
	lst->first = NULL;
	lst->last = NULL;
	
	return lst;
}

pitem list_add(plist lst, void* data)
{
	pitem pom = malloc(sizeof(titem));
		
	if (lst==NULL || pom==NULL)
		return NULL;

	pom->prev = lst->last;
	pom->data = data;
	pom->next = NULL;

	if (lst->first == NULL)
		lst->first = pom;
	else
		lst->last->next = pom;

	return lst->last = pom;
}

pitem list_push(plist lst, void*data)
{
	pitem pom = malloc(sizeof(titem));

	if (lst==NULL || pom==NULL)
		return NULL;

	pom->prev = NULL;
	pom->data = data;
	pom->next = lst->first;

	if (lst->first == NULL)
		lst->last = pom;
	else
		lst->first->prev = pom;

	return lst->first = pom;
}

void* list_remove(plist lst)
{
	void* pom;
	pitem it;

	if (lst==NULL || lst->first==NULL)
		return NULL;

	pom = lst->last->data;
	it = lst->last;
	it->prev->next = NULL;
	lst->last = it->prev;
	free(it);

	return pom;
}

void* list_pop(plist lst)
{
	void* pom;
	pitem it;

	if (lst==NULL || lst->first==NULL)
		return NULL;

	pom = lst->first->data;
	it = lst->first;
	it->next->prev = NULL;
	lst->first = it->next;
	free(it);

	return pom;
}
