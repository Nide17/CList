/*
 * clist.c
 *
 * Linked list implementation for ISSE Assignment 5
 *
 * Author: <YOUR NAME HERE!>
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "clist.h"

#define DEBUG

struct _cl_node
{
  CListElementType element;
  struct _cl_node *next;
};

struct _clist
{
  struct _cl_node *head;
  int length;
};

/*
 * Create (malloc) a new _cl_node and populate it with the supplied
 * values
 *
 * Parameters:
 *   element, next  the values for the node to be created
 *
 * Returns: The newly-malloc'd node, or NULL in case of error
 */
static struct _cl_node *
_CL_new_node(CListElementType element, struct _cl_node *next)
{
  struct _cl_node *new = (struct _cl_node *)malloc(sizeof(struct _cl_node));

  assert(new);

  new->element = element;
  new->next = next;

  return new;
}

// Documented in .h file
CList CL_new()
{
  CList list = (CList)malloc(sizeof(struct _clist));
  assert(list);

  list->head = NULL;
  list->length = 0;

  return list;
}

// Documented in .h file
void CL_free(CList list)
{
  assert(list);

  // free all nodes in the list
  struct _cl_node *node = list->head;
  while (node != NULL)
  {
    struct _cl_node *next = node->next;
    free(node);
    node = next;
  }

  // free the list itself
  free(list);
}

// Documented in .h file
int CL_length(CList list)
{
  assert(list);
#ifdef DEBUG
  // In production code, we simply return the stored value for
  // length. However, as a defensive programming method to prevent
  // bugs in our code, in DEBUG mode we walk the list and ensure the
  // number of elements on the list is equal to the stored length.

  int len = 0;
  for (struct _cl_node *node = list->head; node != NULL; node = node->next)
    len++;

  assert(len == list->length);
#endif // DEBUG

  return list->length;
}

// Documented in .h file
void CL_print(CList list)
{
  assert(list);
  int num = 0;
  for (struct _cl_node *node = list->head; node != NULL; node = node->next)
    printf("  [%d]: %s\n", num++, node->element);
}

// Documented in .h file
void CL_push(CList list, CListElementType element)
{
  assert(list);
  list->head = _CL_new_node(element, list->head);
  list->length++;
}

// Documented in .h file
CListElementType CL_pop(CList list)
{
  assert(list);
  struct _cl_node *popped_node = list->head;

  if (popped_node == NULL)
    return INVALID_RETURN;

  CListElementType ret = popped_node->element;

  // unlink previous head node, then free it
  list->head = popped_node->next;
  free(popped_node);
  // we cannot refer to popped node any longer

  list->length--;

  return ret;
}

// Documented in .h file
void CL_append(CList list, CListElementType element)
{
  assert(list);

  // CREATE A NEW NODE
  struct _cl_node *new_node = _CL_new_node(element, NULL);

  // IF LIST IS EMPTY, NEW NODE IS THE HEAD
  if (list->head == NULL)
  {
    list->head = new_node;
    list->length++;
    return;
  }

  // OTHERWISE, WALK THE LIST UNTIL WE FIND THE LAST NODE
  struct _cl_node *node = list->head;
  while (node->next != NULL)
    node = node->next;

  // NOW NODE POINTS TO THE LAST NODE IN THE LIST
  node->next = new_node;

  list->length++;

  return;
}

// Documented in .h file
CListElementType CL_nth(CList list, int pos)
{
  assert(list);

  // Check if position is within bounds
  if (pos < -list->length || pos >= list->length)
    return INVALID_RETURN;

  // If pos is negative, count from the end of the list
  if (pos < 0)
    pos = list->length + pos;

  // Walk the list until we find the node at position pos
  struct _cl_node *node = list->head;
  while (pos > 0 && node != NULL)
  {
    node = node->next;
    pos--;
  }

  // If node is NULL, we walked off the end of the list
  if (node == NULL)
    return INVALID_RETURN;

  // Otherwise, node points to the node at position pos
  return node->element;
}

// Documented in .h file
bool CL_insert(CList list, CListElementType element, int pos)
{
  assert(list);

  // Check if position is within bounds
  if (pos < -list->length - 1 || pos > list->length)
    return false;

  // If pos is negative, count from the end of the list
  if (pos < 0)
    pos = list->length + pos + 1;

  // If pos is 0, just push the element onto the list
  if (pos == 0)
  {
    CL_push(list, element);
    return true;
  }

  // Walk the list until we find the node at position pos-1
  struct _cl_node *node = list->head;

  while (pos > 1 && node != NULL)
  {
    node = node->next;
    pos--;
  }

  // If node is NULL, we walked off the end of the list
  if (node == NULL)
    return false;

  // Otherwise, node points to the node at position pos-1
  node->next = _CL_new_node(element, node->next);

  // Increment the length of the list
  list->length++;

  return true;
}

// Documented in .h file
CListElementType CL_remove(CList list, int pos)
{
  assert(list);

  // If pos is negative, count from the end of the list
  if (pos < 0)
    pos = list->length + pos;

  // If pos is still negative or out of bounds, it's an error
  if (pos < 0 || pos >= list->length)
    return INVALID_RETURN;

  // If pos is 0, just pop the head of the list
  if (pos == 0)
    return CL_pop(list);

  // Walk the list until we find the node at position pos-1
  struct _cl_node *node = list->head;
  while (pos > 1 && node != NULL)
  {
    node = node->next;
    pos--;
  }

  // If node is NULL, we walked off the end of the list
  if (node == NULL)
    return INVALID_RETURN;

  // Otherwise, node points to the node at position pos-1
  struct _cl_node *removed_node = node->next;
  if (removed_node == NULL)
    return INVALID_RETURN;

  // Unlink the node at position pos-1
  node->next = removed_node->next;

  // Save the element to return
  CListElementType ret = removed_node->element;

  // Free the removed node
  free(removed_node);

  // Decrement the length of the list
  list->length--;

  return ret;
}

// Documented in .h file
CList CL_copy(CList list)
{
  assert(list);

  // create a new list
  CList new_list = CL_new();

  // walk the list, appending each element to the new list
  for (struct _cl_node *node = list->head; node != NULL; node = node->next)
    CL_append(new_list, node->element);

  return new_list;
}

// Documented in .h file
int CL_insert_sorted(CList list, CListElementType element)
{
  assert(list);

  // if list is empty, just push the element onto the list
  if (list->head == NULL)
  {
    CL_push(list, element);
    return 0;
  }

  // otherwise, walk the list until we find the first element that is
  // greater than or equal to the element we are inserting
  struct _cl_node *node = list->head;
  int pos = 0;
  while (node->next != NULL && strcmp(node->element, element) < 0)
  {
    node = node->next;
    pos++;
  }

  // now node points to the first element that is greater than or
  // equal to the element we are inserting
  node->next = _CL_new_node(element, node->next);
  list->length++;

  return pos;
}

// Documented in .h file
void CL_join(CList list1, CList list2)
{
  assert(list1);
  assert(list2);

  // if list1 is empty, just point it at list2
  if (list1->head == NULL)
  {
    list1->head = list2->head;
    list1->length = list2->length;
    list2->head = NULL;
    list2->length = 0;
    return;
  }

  // otherwise, walk list1 until we find the last node
  struct _cl_node *node = list1->head;
  while (node->next != NULL)
    node = node->next;

  // now node points to the last node in list1
  node->next = list2->head;
  list1->length += list2->length;

  // list2 is now empty
  list2->head = NULL;
  list2->length = 0;

  return;
}

// Documented in .h file
void CL_reverse(CList list)
{
  assert(list);

  // if list is empty, there's nothing to do
  if (list->head == NULL)
    return;

  // otherwise, walk the list, reversing the next pointers
  struct _cl_node *prev = NULL;
  struct _cl_node *node = list->head;

  while (node != NULL)
  {
    struct _cl_node *next = node->next;
    node->next = prev;
    prev = node;
    node = next;
  }

  // now prev points to the last node in the list
  list->head = prev;

  return;
}

// Documented in .h file
void CL_foreach(CList list, CL_foreach_callback callback, void *cb_data)
{
  assert(list);

  // typedef void (*CL_foreach_callback)(int pos, CListElementType element, void *cb_data);
  int pos = 0;
  for (struct _cl_node *node = list->head; node != NULL; node = node->next)
  {
    callback(pos, node->element, cb_data);
    pos++;
  }

  return;
}
