//
// Created by johan on 2025-06-05.
//

#ifndef LIST_H
#define LIST_H
#include <stddef.h>

#define List_count(L) ((L)->count)
#define List_is_empty(L) (List_count(L) == 0)
#define List_first(L) ((L)->first != NULL ? (L)->first->value : NULL)
#define List_last(L) ((L)->last != NULL ? (L)->last->value : NULL)

#define LIST_FOREACH(LIST, FIRST, NEXT, CURR)                               \
ListNode *_node = NULL;                                                     \
ListNode *CURR = NULL;                                                      \
for (CURR = _node = LIST->FIRST; _node != NULL; CURR = _node = _node->NEXT)

#define LIST_FOR_EACH(list) for (ListNode *cur = (list)->first; cur != NULL; cur = cur->next)


typedef struct ListNode {
    struct ListNode *next;
    void *value;
} ListNode;

typedef struct List {
    int count;
    ListNode *first;
    ListNode *last;
} List;

List *List_create();
void List_destroy(List *list);
void List_clear(const List *list);
void List_clear_destroy(List *list);
void List_push(List *list, void *value);
void *List_pop(List *list);
void *List_remove(List *list, ListNode *node_to_remove);
List *List_copy(const List *list_to_copy, size_t s);
void List_join(List *base, List *additional);



#endif //LIST_H
