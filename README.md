## CList

__INTRODUCTION__

This repository contains a CList library that provides a set of functions for working with linked lists in the C programming language. The library includes functionalities for creating, manipulating, and managing linked lists. The implementation is inspired by the Python list class that is a built-in data type, and Python provides numerous powerful ways to interact with lists including that lists can grow to any length, Insertion and deletion operations execute quickly and are easy to use. Lists can be
copied, printed, merged, sorted, and so forth, all with easy to use built-in Python functions.

__DESCRIPTION__

The CList library includes the following functions:

1. CList CL_new(): Creates a new empty linked list.

Returns a new empty linked list.

2. void CL_push(CList list, CListElementType element): Adds an element to the front of the list.

Parameters:
- list: The list to which the element is added.
- element: The element to be added to the front of the list.

Modifies the list by adding the specified element to the front.

3. CListElementType CL_pop(CList list): Removes and returns the element from the front of the list.

Parameters:
- list: The list from which the element is removed.

Returns the element removed from the front of the list.

4. void CL_append(CList list, CListElementType element): Appends an element to the end of the list.

Parameters:
- list: The list to which the element is appended.
- element: The element to be appended to the end of the list.

Modifies the list by adding the specified element to the end.

5. CListElementType CL_nth(CList list, int pos): Returns the element at the specified position in the list.

Parameters:
- list: The list from which the element is retrieved.
- pos: The position (index) of the element to be retrieved.

Returns the element at the specified position in the list.

6. bool CL_insert(CList list, CListElementType element, int pos): Inserts an element at the specified position in the list.

Parameters:
- list: The list in which the element is inserted.
- element: The element to be inserted.
- pos: The position (index) at which the element is to be inserted.

Returns true if the insertion is successful; otherwise, returns false.

7. CListElementType CL_remove(CList list, int pos): Removes and returns the element at the specified position in the list.

Parameters:
- list: The list from which the element is removed.
- pos: The position (index) of the element to be removed.

Returns the element removed from the specified position in the list.

8. CList CL_copy(CList list): Creates a copy of the list.

Parameters:
- list: The list to be copied.
- Returns a new linked list that is a copy of the original list.

9. int CL_insert_sorted(CList list, CListElementType element): Inserts an element into a sorted list while maintaining the sorted order.

Parameters:
- list: The sorted list in which the element is to be inserted.
- element: The element to be inserted.

Returns the position (index) at which the element is inserted in the sorted list.

10. void CL_join(CList list1, CList list2): Appends the elements of list2 to the end of list1.

Parameters:
- list1: The first list to which elements are appended.
- list2: The second list whose elements are appended to list1.

Modifies list1 by adding the elements from list2 to the end.

11. void CL_reverse(CList list): Reverses the order of elements in the list.

Parameters:
- list: The list to be reversed.

Modifies the list by reversing the order of its elements.

12. void CL_foreach(CList list, CL_foreach_callback callback, void *cb_data): Applies a callback function to each element in the list.

Parameters:
- list: The list on which the callback function is applied.
- callback: A user-defined callback function to be applied to each element.
- cb_data: Caller data to be passed to the callback function.

Invokes the callback function on each element in the list.

__IMPORTANCE__

The CList library is a fundamental tool that can be used for handling linked lists in C programming. It can offer essential data manipulation capabilities, making it important for various applications that require linked lists such as data organization, data storage, algorithms, and so forth.

__GETTING STARTED__

To use the CList library:

* Clone this repository to access the library code.
* Compile your program with the CList library code using the make command.
```
make
```
* You can now use the CList functions in your program by testing typing:
```
./clist_test
```
  
__TESTING__

This program is implemented and tested against multiple input and test cases implemented in the clist_test.c file.
  
 __KEYWORDS__

<mark>ISSE</mark>     <mark>CMU</mark>     <mark>Assignment5</mark>     <mark>CList</mark>     <mark>C Programming</mark>     <mark>Linked Lists</mark>

  __AUTHOR__

 Written by parmenin (Niyomwungeri Parmenide ISHIMWE) at CMU-Africa - MSIT

 __DATE__

 October 01, 2023
