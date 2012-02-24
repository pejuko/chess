#ifndef __LIST_H__
#define __LIST_H__

typedef struct sitem {
	struct sitem* prev;
	void* data;
	struct sitem* next;
} titem, *pitem;

typedef struct slist {
	pitem first;
	pitem last;
} tlist, *plist;

extern plist list_new(void);
extern pitem list_add(plist lst, void* data);
extern pitem list_push(plist lst, void* data);
extern void* list_remove(plist lst);
extern void* list_pop(plist lst);

#endif
