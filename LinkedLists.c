/******************************************************************************
 An student framework implementation of doubly linked lists that holds 
 elements containing a 256 character string and a sequence number.
 12/24/2017 - R. Repka     Removed AddToFrontOfLinkedList()
 03/12/2018 - R. Repka     Added pseudo code 
 09/17/2019 - R. Repka     fixed minor prototype errors 
 09/26/2019 - R. Repka     Added comments to RemoveFromFrontOfLinkedList()
 10/01/2019 - R. Repka     Changed search function to int
 9/21/2026 - Neo Morocho   Added implementations for linked list module functions
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ClassErrors.h"
#include "LinkedLists.h"


/******************************************************************************
  Use the following pseudo code to help with all the functions in this module
  
  add 1st node
    LinkedList front = points to node 1,  LinkedList back = points to node 1
      Node 1 next     = NULL  (at the end)
      Node 1 previous = NULL (at the start)
          
  add 2nd node
    LinkedList front = points to node 1,  LinkedList back = points to node 2
      Node 1 next points to node 2             Node 2 next   = NULL  (at the end)
      Node 1 previous = NULL (at the start)    Node 2 previous points to node 1

  add 3rd node
  LinkedList front = points to node 1,  LinkedList back = points to node 3
      Node 1 next points to node 2           Node 2 next points to node 3      Node 3 next   = NULL  (at the end)
      Node 1 previous = NULL (at the start)  Node 2 previous points to node 1  Node 3 previous points to node 2
      
   Note: It is possible to implement with only 2 special cases 
      
   Remove from front of 3 node list 
    save the pointer to the data and return it at the end
    LinkedList front = points to node 2,  LinkedList back = points to node 3
      Node 2 next points to node 3           Node 3 next   = NULL  (at the end)
      Node 2 previous = NULL (at the start)  Node 3 previous points to node 2
      free Node but NOT the data
******************************************************************************/

/******************************************************************************
 Initialize the linked list data structure.  Points to nothing, no entries.

 Where: LinkedLists *ListPtr - Pointer to the linked list to create
 Returns: nothing
 Errors: none
******************************************************************************/
void InitLinkedList(LinkedLists *ListPtr)
{
  ListPtr->NumElements = 0;
  ListPtr->FrontPtr = NULL;
  ListPtr->BackPtr = NULL;
} /* InitLinkedList() */


/******************************************************************************
 Adds a node to the back of the list.

 Where: LinkedLists *ListPtr    - Pointer to the linked list to add to
        char *DataPtr - Pointer to the data to add to the list
 Returns: nothing
 Errors: Prints to stderr and exits
******************************************************************************/
void AddToBackOfLinkedList(LinkedLists *ListPtr, char *DataPtr)
{
    /* Checks if pointer given is NULL or empty list to prevent errors */
  if (ListPtr == NULL || DataPtr == NULL) {
      fprintf(stderr, "Null pointer error in AddToBackOfLinkedList \n");
      exit(MALLOC_ERROR);
  }
  /* Creates and allocates memory for a new node and check for malloc failure */
  LinkedListNodes *added_Node = (LinkedListNodes*)malloc(sizeof(LinkedListNodes));
  if (added_Node == NULL){
      fprintf(stderr, "Memory allocation failed at AddToBackOfLinkedList\n");
      exit(MALLOC_ERROR);
  }
  /* Copy data to the new node safely using strncpy*/
  strncpy(added_Node->String, DataPtr, sizeof(added_Node->String) -1);
  added_Node->String[sizeof(added_Node->String) - 1] = '\0';
  /* Increment the number of elements in the list */
  ListPtr->NumElements++;
  added_Node->Next = NULL;
  added_Node->Previous = NULL;
  /* Adds the node to the list based on 2 cases */
  if (ListPtr->FrontPtr == NULL){
     /* Adds to front/back of list if list is empty */
      ListPtr->FrontPtr = added_Node;
      ListPtr->BackPtr = added_Node;
  } else {
      /* Adds to back of list if list is not empty */
      added_Node->Previous = ListPtr->BackPtr;
      ListPtr->BackPtr->Next = added_Node;
      ListPtr->BackPtr = added_Node;
  }
} /* AddToBackOfLinkedList */

/******************************************************************************
 Removes a node from the front of the list and returns a pointer to the node
 data removed. On empty lists should return a NULL pointer.  
 Note: You will need to malloc a string buffer and copy the data from the
       linked list node before destroying the node.  The calling routine was
       written to free the string so there are no memory leaks

 
   Linked lists contain nodes, which contain data strings
 
 Where: LinkedLists *ListPtr    - Pointer to the linked list to remove from
 Returns: Pointer to the data removed or NULL for none
 Errors: none
******************************************************************************/
char *RemoveFromFrontOfLinkedList(LinkedLists *ListPtr)
{
  /* Checks if pointer given is NULL or empty list to prevent errors */
  if (ListPtr == NULL || ListPtr->FrontPtr == NULL) {
    return NULL;
  }
  /* Temp variable to hold the data being removed */
  LinkedListNodes *removed_Node =NULL;
  size_t data_length = 0;
  char *removed_Data = NULL;
  removed_Node = ListPtr->FrontPtr;
  data_length = strlen(removed_Node->String) + 1;
  removed_Data = (char *)malloc(data_length);
  /* Check for malloc error when allocating data */
  if (removed_Data == NULL){
      fprintf(stderr, "Memory allocation failed at RemoveFromFrontOfLinkedList\n");
      exit(MALLOC_ERROR);
  }
  strncpy(removed_Data, removed_Node->String, data_length);
  /* Update the front pointer of the list */
  ListPtr->FrontPtr = removed_Node->Next;
  /* Check if the list is empty */
  if (ListPtr->FrontPtr == NULL)
  {
    /* Update the back pointer */
    ListPtr->BackPtr = NULL;
  } else {
    /* Update the previous pointer */
    ListPtr->FrontPtr->Previous = NULL;
  }
  /* Reduces the number of elements in the list and frees up the temporary node*/
  ListPtr->NumElements--;
  free(removed_Node);
  /* Returns the data from the removed node */
  return removed_Data;
} /* RemoveFromFrontOfLinkedList() */


/******************************************************************************
 De-allocates the linked list and resets the struct fields (in the header) 
 to indicate that the list is empty.  
 
 If you choose to use the RemoveFromFrontOfLinkedList() function, remember 
 the calling routine must free the returned pointer so there are no memory leaks
       
 Where: LinkedLists *ListPtr    - Pointer to the linked list to destroy
 Returns: nothing
 Errors: none
******************************************************************************/
void DestroyLinkedList(LinkedLists *ListPtr)
{
  /* Temp variable to hold the data being removed */
  char *tempData = NULL;
  while (ListPtr->FrontPtr != NULL){
    /* Removes nodes from the front of list then returns the string buffers from it */
    tempData = RemoveFromFrontOfLinkedList(ListPtr);
      if (tempData != NULL){
        free(tempData);
        tempData = NULL;
    }
  }
  /* De-allocate the ListPtr and reset its fields */
  ListPtr->FrontPtr = NULL;
  ListPtr->BackPtr = NULL;
  ListPtr->NumElements = 0;
} /* DestroyLinkedList() */


/******************************************************************************
 Searches the linked list for a provided word. If found, returns 1 otherwise 0
 
 Where: LinkedLists *ListPtr - Pointer to the linked list to search
        char *String         - Pointer to the string to search
 Returns: 1 if found, 0 otherwise
 Errors: none
 * ***************************************************************************/
int SearchList(LinkedLists *ListPtr, char *String)
{
  /* Return value */
  int foundWord = 0;
  /* Temporary node thats set to the first node of the linked list */
  LinkedListNodes *currentNode = ListPtr->FrontPtr;
    /* Runs until all nodes have been checked for their string */
    while (currentNode != NULL){
        /* Checks if string in node matches with string inputted */
        if (strcmp(currentNode->String, String) == 0){
            /* Sets return value to 1 */
            foundWord = 1;
            break;
            }
        /* Moves onto the next node */
        currentNode = currentNode->Next;
        }
        return foundWord;
} /* SearchList() */




