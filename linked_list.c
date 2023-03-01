#include "linked_list.h"
void list_init(linked_list *l)
{
	l->head = NULL;
	l->size = 0;
}
list_node *list_insert(linked_list *l, list_node *node)
{
	node->next = l->head;
	if (l->head != NULL)
	{
		l->head->prev = node;
	}
	l->head = node;
	node->prev = NULL;
	l->size++;
	return node;
}
list_node *list_insert_key(linked_list *l, char *key)
{
	list_node *node = (list_node *)malloc(sizeof(list_node));
	if (node == NULL)
	{
		return NULL;
	}
	node->key = key;

	return list_insert(l, node);
}
void list_delete(linked_list *l, list_node *p)
{
	if (p->prev != NULL)
	{
		(p->prev)->next = p->next;
	}
	else
	{
		l->head = p->next;
	}
	if (p->next != NULL)
		(p->next)->prev = p->prev;
	l->size--;
}
void list_destroy(linked_list *l)
{
	list_node *temp;
	if(l==NULL)
	{
		return;
	}
	while (l->head != NULL)
	{
		temp = l->head;
		l->head = l->head->next;
		free(temp->key);
		free(temp);
	}
}
void list_reverse(linked_list *l)
{
	list_node *prev = NULL;
	list_node *curr = l->head;
	list_node *next;
	while (curr != NULL)
	{
		next = curr->next;
		curr->next = prev;
		prev = curr;
		curr = next;
	}
	l->head = prev;
}
