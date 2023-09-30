/*
 * clist_test.c
 *
 * Automated test code for CLists
 */

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>
#include "clist.h"

// Some known testdata, for testing
const char *testdata[] = {"Zero", "One", "Two", "Three", "Four", "Five",
                          "Six", "Seven", "Eight", "Nine", "Ten", "Eleven", "Twelve", "Thirteen",
                          "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen",
                          "Twenty"};

const char *testdata_sorted[] = {"Eight", "Eighteen", "Eleven", "Fifteen",
                                 "Five", "Four", "Fourteen", "Nine", "Nineteen", "One", "Seven",
                                 "Seventeen", "Six", "Sixteen", "Ten", "Thirteen", "Three", "Twelve",
                                 "Twenty", "Two", "Zero"};

static const int num_testdata = sizeof(testdata) / sizeof(testdata[0]);

// Checks that value is true; if not, prints a failure message and
// returns 0 from this function
#define test_assert(value)                                         \
  {                                                                \
    if (!(value))                                                  \
    {                                                              \
      printf("FAIL %s[%d]: %s\n", __FUNCTION__, __LINE__, #value); \
      return 0;                                                    \
    }                                                              \
  }

// Checks that value == INVALID_RETURN; if not, prints a failure
// message and returns 0 from this function
#define test_invalid(value)                                        \
  {                                                                \
    if (value != INVALID_RETURN)                                   \
    {                                                              \
      printf("FAIL %s[%d]: %s\n", __FUNCTION__, __LINE__, #value); \
      return 0;                                                    \
    }                                                              \
  }

// Checks that value == expected; if not, prints a failure message and
// returns 0 from this function
#define test_compare(value, expected)                     \
  {                                                       \
    const char *v = (value);                              \
    const char *e = (expected);                           \
    if (strcmp(v, e) != 0)                                \
    {                                                     \
      printf("FAIL %s[%d] %s: expected '%s', got '%s'\n", \
             __FUNCTION__, __LINE__, #value, e, v);       \
      return 0;                                           \
    }                                                     \
  }

/*
 * Tests the CL_new, CL_push, CL_pop, and CL_free functions
 *
 * Returns: 1 if all tests pass, 0 otherwise
 */
int test_cl_push_pop()
{
  CList list = CL_new();

  test_assert(CL_length(list) == 0);
  test_invalid(CL_pop(list));

  CL_push(list, testdata[0]);

  test_assert(CL_length(list) == 1);
  test_compare(CL_pop(list), testdata[0]);
  test_assert(CL_length(list) == 0);
  test_invalid(CL_pop(list));

  for (int i = 0; i < num_testdata; i++)
  {
    CL_push(list, testdata[i]);
    test_assert(CL_length(list) == i + 1);
  }

  for (int i = num_testdata - 1; i >= 0; i--)
  {
    test_compare(CL_pop(list), testdata[i]);
    test_assert(CL_length(list) == i);
  }

  CL_free(list);

  return 1;
}

int test_cl_append()
{
  CList list = CL_new();

  // Append all the items
  for (int i = 0; i < num_testdata; i++)
  {
    CL_append(list, testdata[i]);
    test_assert(CL_length(list) == i + 1);
  }

  // ensure that the list is in the right order
  for (int i = 0; i < num_testdata; i++)
    test_compare(CL_nth(list, i), testdata[i]);

  CL_free(list);
  return 1;
}

/*
 * Tests the CL_nth function
 *
 * Returns: 1 if all tests pass, 0 otherwise
 */
int test_cl_nth()
{
  CList list = CL_new();

  // Empty list -- ensure that CL_nth returns NULL for -2, -1, 0, 1, and 2
  test_invalid(CL_nth(list, -2));
  test_invalid(CL_nth(list, -1));
  test_invalid(CL_nth(list, 0));
  test_invalid(CL_nth(list, 1));
  test_invalid(CL_nth(list, 2));

  // Add one item, then perform same tests
  CL_push(list, testdata[2]);
  test_assert(CL_length(list) == 1);
  test_invalid(CL_nth(list, -3));
  test_invalid(CL_nth(list, -2));
  test_compare(CL_nth(list, -1), testdata[2]);
  test_compare(CL_nth(list, 0), testdata[2]);
  test_invalid(CL_nth(list, 1));
  test_invalid(CL_nth(list, 2));

  // Push two more items, perform similar tests
  CL_push(list, testdata[1]);
  CL_push(list, testdata[0]);
  test_assert(CL_length(list) == 3);
  test_invalid(CL_nth(list, -5));
  test_invalid(CL_nth(list, -4));
  test_compare(CL_nth(list, -3), testdata[0]);
  test_compare(CL_nth(list, -2), testdata[1]);
  test_compare(CL_nth(list, -1), testdata[2]);
  test_compare(CL_nth(list, 0), testdata[0]);
  test_compare(CL_nth(list, 1), testdata[1]);
  test_compare(CL_nth(list, 2), testdata[2]);
  test_invalid(CL_nth(list, 3));
  test_invalid(CL_nth(list, 4));

  CL_free(list);

  return 1;
}

// Test coverage: Do your tests do a good job of covering all the interesting test cases?
// As distributed, it only has tests for the five functions I have provided. You should add additional tests so that you fully exercise the CList library, including all boundary conditions.
// implement a corresponding test function in clist_test.c, following the pattern of the test code I have already supplied.

/*
 * Tests the CL_insert function
 *
 * Returns: 1 if all tests pass, 0 otherwise
 */
int test_cl_insert()
{
  CList list = CL_new();

  // Empty list -- ensure that CL_insert returns false for -2, 1, and 2
  test_invalid(CL_nth(list, 0));
  test_assert(CL_insert(list, testdata[0], -2) == false);
  test_assert(CL_insert(list, testdata[0], 1) == false);
  test_assert(CL_insert(list, testdata[0], 2) == false);

  // Adding item at -1 position should be the same as adding at 0 position if the list is empty
  test_assert(CL_insert(list, testdata[0], -1) == true);
  test_compare(CL_nth(list, 0), testdata[0]);
  test_compare(CL_nth(list, -1), testdata[0]);

  // Adding item at 0 position
  test_assert(CL_insert(list, testdata[2], 0) == true);
  test_compare(CL_nth(list, 0), testdata[2]);
  test_compare(CL_nth(list, -2), testdata[2]);

  // Adding at unallowed positions
  test_assert(CL_insert(list, testdata[3], -4) == false);
  test_assert(CL_insert(list, testdata[3], 3) == false);

  // Adding item at allowed positions
  test_assert(CL_insert(list, testdata[3], -3) == true);
  test_compare(CL_nth(list, 0), testdata[3]);
  test_compare(CL_nth(list, -3), testdata[3]);
  test_assert(CL_insert(list, testdata[4], 3) == true);
  test_compare(CL_nth(list, 3), testdata[4]);
  test_compare(CL_nth(list, -1), testdata[4]);

  // Adding item at the end of the list
  test_assert(CL_insert(list, testdata[5], -1) == true);
  test_compare(CL_nth(list, 4), testdata[5]);
  test_compare(CL_nth(list, -1), testdata[5]);

  CL_free(list);

  return 1;
}

/*
 * Tests the CL_remove function
 * Parameters:
 *   list     The list
 *   pos      Position to perform the removal
 *
 * Returns: 1 if all tests pass, 0 otherwise
 */
int test_cl_remove()
{
  CList list = CL_new();

  // Empty list -- ensure that CL_remove returns INVALID_RETURN for -1, 0, and 1
  test_invalid(CL_remove(list, -1));
  test_invalid(CL_remove(list, 0));
  test_invalid(CL_remove(list, 1));

  // Add one item, then perform same tests
  CL_push(list, testdata[0]);
  test_assert(CL_length(list) == 1);
  test_invalid(CL_remove(list, -3));
  test_invalid(CL_remove(list, -2));
  test_compare(CL_remove(list, -1), testdata[0]);
  test_assert(CL_length(list) == 0);
  test_invalid(CL_remove(list, 0));
  test_invalid(CL_remove(list, 1));
  test_invalid(CL_remove(list, 2));

  // Push two more items, perform similar tests
  CL_push(list, testdata[1]);
  CL_push(list, testdata[2]);
  test_assert(CL_length(list) == 2);
  test_invalid(CL_remove(list, -5));
  test_invalid(CL_remove(list, -4));
  test_compare(CL_remove(list, -2), testdata[2]);
  test_assert(CL_length(list) == 1);
  test_compare(CL_remove(list, 0), testdata[1]);
  test_assert(CL_length(list) == 0);
  test_invalid(CL_remove(list, 1));

  CL_free(list);

  return 1;
}

/*
 * Tests the CL_copy function - CList CL_copy(CList list)
 * Parameters:
 *   list     The list
 *
 * Returns: 1 if all tests pass, 0 otherwise
 */
int test_cl_copy()
{
  CList list = CL_new();

  // Empty list -- ensure that CL_copy returns an empty list
  CList list_copy = CL_copy(list);
  test_assert(CL_length(list_copy) == 0);
  CL_free(list_copy);

  // Add one item, then perform same tests
  CL_push(list, testdata[0]);
  test_assert(CL_length(list) == 1);
  list_copy = CL_copy(list);
  test_assert(CL_length(list_copy) == 1);
  test_compare(CL_nth(list_copy, 0), testdata[0]);

  // Push two more items, perform similar tests
  CL_push(list, testdata[1]);
  CL_push(list, testdata[2]);
  test_assert(CL_length(list) == 3);
  CL_free(list_copy); // free the copy to be reused and replaced with a new copy

  list_copy = CL_copy(list);
  test_assert(CL_length(list_copy) == 3);
  test_compare(CL_nth(list_copy, 0), testdata[2]);
  test_compare(CL_nth(list_copy, 1), testdata[1]);
  test_compare(CL_nth(list_copy, 2), testdata[0]);

  CL_free(list);
  CL_free(list_copy);

  return 1;
}

/*
 * Tests the CL_insert_sorted function - int CL_insert_sorted(CList list, CListElementType element);
 * Parameters:
 *   list     The list
 *   element  The element to insert
 *
 *  Returns: 1 if all tests pass, 0 otherwise
 */
int test_cl_insert_sorted()
{
  CList list = CL_new();

  // Empty list -- ensure that inserting an element to empty list returns 0
  test_assert(CL_insert_sorted(list, testdata_sorted[5]) == 0);
  test_assert(CL_length(list) == 1);
  test_compare(CL_nth(list, 0), testdata_sorted[5]);

  // insert an element that is smaller than the first element
  test_assert(CL_insert_sorted(list, testdata_sorted[1]) == 0);
  test_assert(CL_length(list) == 2);
  test_compare(CL_nth(list, 0), testdata_sorted[1]);

  // insert an element that is larger than the last element
  test_assert(CL_insert_sorted(list, testdata_sorted[19]) == 2);
  test_assert(CL_length(list) == 3);
  test_compare(CL_nth(list, 2), testdata_sorted[19]);

  // empty the list - remove all elements
  while (CL_length(list) > 0)
    CL_pop(list);
  // check if the list is empty
  test_assert(CL_length(list) == 0);

  // insert all elements in the testdata_sorted array
  for (int i = 0; i < num_testdata; i++)
    CL_insert_sorted(list, testdata_sorted[i]);

  // check if the list is sorted
  for (int i = 0; i < num_testdata; i++)
    test_compare(CL_nth(list, i), testdata_sorted[i]);

  // insert an element that is smaller than the first element
  test_assert(CL_insert_sorted(list, "Aaaa") == 0);
  test_assert(CL_length(list) == 22);

  // insert an element that is larger than the last element
  test_assert(CL_insert_sorted(list, "Zzzz") == 22);
  test_assert(CL_length(list) == 23);

  // insert an element that is between the first and last element
  test_assert(CL_insert_sorted(list, "Mmmm") == 8);
  test_assert(CL_length(list) == 24);

  CL_free(list);

  return 1;
}

/*
 * Tests the CL_join function - void CL_join(CList list1, CList list2)
 * Parameters:
 *  list1     The list to be appended to
 *  list2     The list to be joined to list1
 *
 * Returns: 1 if all tests pass, 0 otherwise
 */
int test_cl_join()
{
  CList list1 = CL_new();
  CList list2 = CL_new();

  // Empty list -- ensure that CL_join returns an empty list
  CL_join(list1, list2);
  test_assert(CL_length(list1) == 0);
  test_assert(CL_length(list2) == 0);

  // Add one item, then perform same tests
  CL_push(list1, testdata[0]);
  CL_push(list2, testdata[1]);
  CL_join(list1, list2);
  test_assert(CL_length(list1) == 2);
  test_assert(CL_length(list2) == 0);
  test_compare(CL_nth(list1, 0), testdata[0]);
  test_compare(CL_nth(list1, 1), testdata[1]);

  // Push two more items, perform similar tests
  CL_append(list1, testdata[2]);
  CL_push(list2, testdata[3]);
  CL_join(list1, list2);
  test_assert(CL_length(list1) == 4);
  test_assert(CL_length(list2) == 0);
  test_compare(CL_nth(list1, 0), testdata[0]);
  test_compare(CL_nth(list1, 1), testdata[1]);
  test_compare(CL_nth(list1, 2), testdata[2]);
  test_compare(CL_nth(list1, 3), testdata[3]);

  // empty the list - remove all elements
  while (CL_length(list1) > 0)
    CL_pop(list1);

  // insert all elements in the testdata array to list2
  for (int i = 0; i < num_testdata; i++)
    CL_append(list2, testdata[i]);

  // join list2 to list1
  CL_join(list1, list2);

  // check if the first element of list1 is the first element of testdata
  test_compare(CL_nth(list1, 0), testdata[0]);

  CL_free(list1);
  CL_free(list2);

  return 1;
}

/*
 * Tests the CL_reverse function - void CL_reverse(CList list);
 * Parameters:
 *   list     The list
 *
 * Returns: 1 if all tests pass, 0 otherwise
 */
int test_cl_reverse()
{
  CList list = CL_new();

  // Empty list -- ensure that CL_reverse returns an empty list
  CL_reverse(list);
  test_assert(CL_length(list) == 0);

  // Add one item, then perform same tests
  CL_push(list, testdata[0]);
  CL_reverse(list);
  test_assert(CL_length(list) == 1);
  test_compare(CL_nth(list, 0), testdata[0]);

  // Push two more items, perform similar tests
  CL_push(list, testdata[1]);
  CL_push(list, testdata[2]);
  CL_reverse(list);
  test_assert(CL_length(list) == 3);
  test_compare(CL_nth(list, 0), testdata[0]);
  test_compare(CL_nth(list, 1), testdata[1]);
  test_compare(CL_nth(list, 2), testdata[2]);

  CL_free(list);

  return 1;
}

/*
 * Converts a string to uppercase
 * Parameters:
 *   pos       The position of the element in the list
 *   element   The element to be converted to uppercase
 *   cb_data   Caller data to pass to the function
 *
 * Returns: None
 */
void _CL_to_uppercase(int pos, CListElementType element, void *cb_data)
{
  int length = strlen(element);

  // make a copy of the element
  char *copy = (char *)malloc(length + 1); // +1 for the null terminator
  strcpy(copy, element);

  // convert the copy to uppercase
  for (int i = 0; i < length; i++)
    copy[i] = toupper(copy[i]);

  // print the position and the element in uppercase
  printf("Element: %s, at %d, Upper: %s, Length: %d\n", element, pos, copy, (int)(intptr_t)cb_data);
  free(copy);
}
/*
 * Tests the CL_foreach function - void CL_foreach(CList list, CL_foreach_callback callback, void *cb_data);
 * Parameters:
 *  list       The list
 * callback   The function to call
 * cb_data    Caller data to pass to the function
 *
 * Returns: 1 if all tests pass, 0 otherwise
 */
int test_cl_foreach()
{
  CList list = CL_new();

  // Empty list -- ensure that CL_foreach does not print anything
  CL_foreach(list, _CL_to_uppercase, (void *)(intptr_t)CL_length(list));

  // Add one item, then perform the same tests
  CL_append(list, testdata[0]);
  CL_append(list, testdata[1]);
  CL_append(list, testdata[2]);
  CL_append(list, "Ishimwe Parmenide\n");
  CL_append(list, "");
  CL_append(list, " Kigali@!");
  CL_foreach(list, _CL_to_uppercase, (void *)(intptr_t)CL_length(list));

  // MORE TEST CASES INCLUDING CORNER CASES
  // if the callback function pointer is null
  CL_foreach(list, NULL, (void *)(intptr_t)CL_length(list));
  // if the caller data pointer is null
  CL_foreach(list, _CL_to_uppercase, NULL);

  CL_free(list);
  return 1;
}

/*
 * A demonstration of how to use a CList, which also doubles as a
 * test case.
 *
 * Returns: 1 if all tests pass, 0 otherwise.
 */
int sample_clist_usage()
{
  CList list = CL_new();

  // new lists have length 0
  test_assert(CL_length(list) == 0);

  CL_push(list, "alpha");   // push 'alpha' onto front of list
  CL_push(list, "bravo");   // push 'bravo' onto front of list
  CL_push(list, "charlie"); // push 'charlie' onto front of list

  // list is now charlie, bravo, alpha
  CL_print(list); // print out the list: charlie, bravo, alpha
  test_assert(CL_length(list) == 3);

  // pop the element off the front of the list, which should be 'charlie'
  test_compare(CL_pop(list), "charlie");
  test_assert(CL_length(list) == 2);

  CL_insert(list, "delta", 2);    // insert 'delta' at position 2
  CL_append(list, "echo");        // append 'echo' at end of list
  CL_insert(list, "foxtrot", -2); // insert 'foxtrot' one before end

  // list is now: bravo, alpha, delta, foxtrot, echo
  test_assert(CL_length(list) == 5);

  // retrieve the 3rd element, numbering from 0, so it should be foxtrot
  test_compare(CL_nth(list, 3), "foxtrot");

  // list hasn't changed
  test_assert(CL_length(list) == 5);

  // now REMOVE the third element
  test_compare(CL_remove(list, 3), "foxtrot");
  test_assert(CL_length(list) == 4);

  // list is now: bravo, alpha, delta, echo

  // make a copy of the list
  CList list_copy = CL_copy(list);

  test_assert(CL_length(list_copy) == 4);

  // reverse the copy, so it is now echo, delta, alpha, bravo
  CL_reverse(list_copy);

  // remove the first item from the copy
  test_compare(CL_remove(list_copy, 0), "echo");
  test_assert(CL_length(list_copy) == 3);

  // original list should be unchanged
  test_assert(CL_length(list) == 4);

  // join the two lists; note this operation empties list_copy
  CL_join(list, list_copy);

  // list is now: bravo, alpha, delta, echo, delta, alpha, bravo
  // list_copy should be empty
  test_assert(CL_length(list) == 7);
  test_compare(CL_nth(list, 3), "echo");
  test_assert(CL_length(list_copy) == 0);

  // Lists must be destroyed
  CL_free(list);
  CL_free(list_copy);

  return 1;
}

int main()
{
  int passed = 0;
  int num_tests = 0;

  num_tests++;
  passed += test_cl_push_pop();
  num_tests++;
  passed += test_cl_append();
  num_tests++;
  passed += test_cl_nth();

  num_tests++;
  passed += test_cl_insert();

  num_tests++;
  passed += test_cl_remove();

  num_tests++;
  passed += sample_clist_usage();

  num_tests++;
  passed += test_cl_copy();

  num_tests++;
  passed += test_cl_insert_sorted();

  num_tests++;
  passed += test_cl_join();

  num_tests++;
  passed += test_cl_reverse();

  num_tests++;
  passed += test_cl_foreach();

  printf("Passed %d/%d test cases\n", passed, num_tests);
  fflush(stdout);
  return 0;
}
