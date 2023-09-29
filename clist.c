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

  // deallocate all the nodes in the list
  struct _cl_node *this_node = list->head;

  // traverse the list, deallocating each node
  while (this_node != NULL)
  {
    struct _cl_node *next_node = this_node->next;

    // deallocate the current node
    free(this_node);

    // move on to the next node
    this_node = next_node;
  }

  // deallocate the list structure itself
  free(list);
}

// Documented in .h file
int CL_length(CList list)
{
  assert(list);
#ifdef DEBUG
  // In production code, we simply return the stored value for
  // length. However, as a defensive programming method to prevent
  // bugs in our code, in DEBUG mode we traverse the list and ensure the
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

  // new node to append - its next pointer should be NULL
  struct _cl_node *new_node = _CL_new_node(element, NULL);

  // when appending to an empty list, the new node becomes the head
  if (list->head == NULL)
    list->head = new_node;

  else
  {
    // otherwise, traverse the list until we find the last node
    struct _cl_node *this_node = list->head;
    while (this_node->next != NULL)
      this_node = this_node->next;

    // then append the new node to the end of the list
    this_node->next = new_node;
  }

  // increment the length of the list
  list->length++;
}

// Documented in .h file
CListElementType CL_nth(CList list, int pos)
{
  assert(list);

  // bounds check - if pos is negative or out of bounds, it's an error
  if (pos < -list->length || pos >= list->length)
    return INVALID_RETURN;

  // convert negative pos to positive by counting from the end of the list
  if (pos < 0)
    pos = list->length + pos;

  // traverse the list until we find the node at position pos
  struct _cl_node *this_node = list->head;
  while (pos > 0 && this_node != NULL)
  {
    this_node = this_node->next;
    pos--;
  }

  // if this_node is NULL, we are at the end of the list
  if (this_node == NULL)
    return INVALID_RETURN;

  // otherwise, this_node points to the node at position pos
  return this_node->element;
}

// Documented in .h file
bool CL_insert(CList list, CListElementType element, int pos)
{
  assert(list);

  // bounds check - if pos is negative or out of bounds, it's an error
  if (pos < -list->length - 1 || pos > list->length)
    return false;

  // convert negative pos to positive by counting from the end of the list
  if (pos < 0)
    pos = list->length + pos + 1;

  // If pos is 0, just push the element onto the list
  if (pos == 0)
  {
    CL_push(list, element);
    return true;
  }

  struct _cl_node *this_node = list->head;

  //
  while (pos > 1 && this_node != NULL)
  {
    this_node = this_node->next;
    pos--;
  }

  // If this_node is NULL, we are at the end of the list
  if (this_node == NULL)
    return false;

  // Otherwise, this_node points to the this_node at position pos-1
  this_node->next = _CL_new_node(element, this_node->next);

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

  // traverse the list until we find the node at position pos-1
  struct _cl_node *this_node = list->head;
  while (pos > 1 && this_node != NULL)
  {
    this_node = this_node->next;
    pos--;
  }

  // If this_node is NULL, we are at the end of the list
  if (this_node == NULL)
    return INVALID_RETURN;

  // Otherwise, this_node points to the this_node at position pos-1
  struct _cl_node *rm_node = this_node->next;
  if (rm_node == NULL)
    return INVALID_RETURN;

  // remove the node at position pos-1 - point current node to the node after the one we are removing
  this_node->next = rm_node->next;

  // Save the element to return
  CListElementType rm_element = rm_node->element;

  // Decrement the length of the list
  list->length--;

  // deallocate the node we are removing
  free(rm_node);

  return rm_element;
}

// Documented in .h file
CList CL_copy(CList list)
{
  assert(list);

  // create a new list
  CList list_copy = CL_new();

  // traverse the list, appending each element to the new list
  for (struct _cl_node *node = list->head; node != NULL; node = node->next)
    CL_append(list_copy, node->element);

  // return the newly-created copy
  return list_copy;
}

// Documented in .h file
int CL_insert_sorted(CList list, CListElementType element)
{
  assert(list);

  // if list is empty, just push the element onto front of list
  if (list->head == NULL)
  {
    CL_push(list, element);
    return 0;
  }

  // otherwise, traverse the list until we find the first element that is
  // greater than or equal to the element we are inserting
  struct _cl_node *this_node = list->head;
  int position = 0;
  while (this_node->next != NULL && strcmp(this_node->element, element) < 0)
  {
    this_node = this_node->next;
    position++;
  }

  // now this_node points to the first element that is greater than or equal to
  if (strcmp(this_node->element, element) >= 0)
    CL_insert(list, element, position);

  // now this_node points to the last element in the list, so append the new element
  else
  {
    CL_append(list, element);
    position = CL_length(list) - 1;
  }

  // return the position of the newly-inserted element
  return position;
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
  }

  // otherwise, traverse list1 until we find the last node
  else
  {
    struct _cl_node *this_node = list1->head;
    while (this_node->next != NULL)
      this_node = this_node->next;

    // pointing the last node at the head of list2
    this_node->next = list2->head;
    list1->length = list1->length + list2->length;

    // empty list2
    list2->head = NULL;
    list2->length = 0;
  }
}

// Documented in .h file
void CL_reverse(CList list)
{
  assert(list);

  // reverse if list is not empty
  if (list->head != NULL)
  {
    struct _cl_node *prev_node = NULL;
    struct _cl_node *this_node = list->head;
    struct _cl_node *next_node = NULL;

    while (this_node != NULL)
    {
      next_node = this_node->next;
      this_node->next = prev_node;
      prev_node = this_node;
      this_node = next_node;
    }

    // update head of list
    list->head = prev_node;
  }
}

// Documented in .h file
void CL_foreach(CList list, CL_foreach_callback callback, void *cb_data)
{
  assert(list);

  // traverse the list, calling the callback function for each element
  int position = 0;
  for (struct _cl_node *this_node = list->head; this_node != NULL; this_node = this_node->next)
  {
    callback(position, this_node->element, cb_data);
    position++;
  }
}