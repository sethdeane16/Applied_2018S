/******************************************************************************
 An student framework implementation of doubly linked lists that holds 
 elements containing a 256 character string and a sequence number.
 12/24/2017 - R. Repka     Removed AddToFrontOfLinkedList()
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "resources/ClassErrors.h"
#include "resources/LinkedLists.h"



/******************************************************************************
 Initialize the linked list data structure

 Where: LinkedLists *ListPtr - Pointer to the linked list to create
 Returns: nothing
 Errors: none
******************************************************************************/
void InitLinkedList(LinkedLists *ListPtr)
{
    // Initialize variables
    ListPtr->NumElements = 0;
    ListPtr->FrontPtr = NULL;
    ListPtr->BackPtr = NULL;
}


/******************************************************************************
 Adds a node to the back of the list.

 Where: LinkedLists *ListPtr    - Pointer to the linked list to add to
        ElementStructs *DataPtr - Pointer to the data to add to the list
 Returns: nothing
 Errors: Prints to stderr and exits
******************************************************************************/
void AddToBackOfLinkedList(LinkedLists *ListPtr, ElementStructs *DataPtr)
{
    /*Declare a pointer to a node*/
    LinkedListNodes*NodePtr;

    /*Allocate the node*/
    NodePtr = (LinkedListNodes*)malloc(sizeof(LinkedListNodes));
    MALLOC_DEBUG(NodePtr);

    /*Attach the *data* to the node*/
    NodePtr->ElementPtr = malloc(sizeof(ElementStructs)); 
    MALLOC_DEBUG(NodePtr->ElementPtr);
    *NodePtr->ElementPtr = *DataPtr;

    // there is no data in the linked list
    if (ListPtr->NumElements == 0) {
        // set the only element in the linked list to the NodePtr
        ListPtr->FrontPtr = NodePtr;
        ListPtr->BackPtr = NodePtr;
        NodePtr->Next = NULL;
    }

    // there is already data in the linked list
    else {

        /*Link new node to the*current*back of the list*/
        NodePtr->Next = ListPtr->BackPtr;
        /*Attach the*current*back of the list to the new node*/
        ListPtr->BackPtr->Previous = NodePtr;
        /*Update the pointer to the back of the list to the new node*/
        ListPtr->BackPtr = NodePtr;
    }

    /*Terminate the backward pointing list*/
    NodePtr->Previous = NULL;

    // inrement the number of nodes in the linked list
    ListPtr->NumElements += 1;

}

/******************************************************************************
 Removes a node from the front of the list and returns a pointer to the node
 data removed. On empty lists should return a NULL pointer.
 Note: This will destroy the node but not the associated ElementStruct data element.
  
 Where: LinkedLists *ListPtr    - Pointer to the linked list to remove from
 Returns: Pointer to the node removed or NULL for none
 Errors: none
******************************************************************************/
ElementStructs *RemoveFromFrontOfLinkedList(LinkedLists *ListPtr)
{
    if (ListPtr->NumElements != 0) {
        /*Declare a pointer to a node*/
        LinkedListNodes *NodePtr;
    
        /*Declare a pointer to the list element (to be returned)*/
        ElementStructs *DataPtr;
    
        /*Assign the pointer to the node being removed*/
        NodePtr = ListPtr->FrontPtr;
    
        if (NULL == NodePtr) {
            return NULL;
        } else {
            /*Assign the pointer to the data element being removed/returned*/
            DataPtr = NodePtr->ElementPtr;
    
    
            if (ListPtr->NumElements == 1) {
                /*Advance the front to the next element*/
                ListPtr->FrontPtr = NULL;
    
            } else {
                /*Advance the front to the next element*/
                ListPtr->FrontPtr = NodePtr->Previous;
    
                /*Detach the node being removed from the front of the list*/
                ListPtr->FrontPtr->Next = NULL;
            }
    
    
            /*De-allocate the node storage*/
            free(NodePtr);
            FREE_DEBUG(NodePtr);
    
            /*Decrement the number of elements counter*/
            ListPtr->NumElements--;
            return DataPtr;

        }
    } else {
        return NULL;
    }
}

/******************************************************************************
 Removes a node from the back of the list and returns a pointer to the node
 data removed. On empty lists should return a NULL pointer.
 Note: This will destroy the node but not the associated ElementStruct data element.
  
 Where: LinkedLists *ListPtr    - Pointer to the linked list to remove from
 Returns: Pointer to the node removed or NULL for none
 Errors: none
******************************************************************************/
ElementStructs *RemoveFromBackOfLinkedList(LinkedLists *ListPtr)
{
    if (ListPtr->NumElements != 0) {
        /*Declare a pointer to a node*/
        LinkedListNodes *NodePtr;

        /*Declare a pointer to the list element (to be returned)*/
        ElementStructs *DataPtr;

        /*Assign the pointer to the node being removed*/
        NodePtr = ListPtr->BackPtr;

        if (NULL == NodePtr) {
            return NULL;
        } else {
            /*Assign the pointer to the data element being removed/returned*/
            DataPtr = NodePtr->ElementPtr;

            if (ListPtr->NumElements == 1) {
                /*Advance the front to the next element*/
                ListPtr->BackPtr = NULL;

            } else {
                /*Advance the front to the next element*/
                ListPtr->BackPtr = NodePtr->Next;

                /*Detach the node being removed from the front of the list*/
                ListPtr->BackPtr->Previous = NULL;
            }

            /*De-allocate the node storage*/
            free(NodePtr);
            FREE_DEBUG(NodePtr);

            /*Decrement the number of elements counter*/
            ListPtr->NumElements--;
            return DataPtr;
        }
    } else {
        return NULL;
    }
}


/******************************************************************************
 De-allocates the linked list and resets the struct fields (in the header) 
 to indicate that the list is empty.

 Where: LinkedLists *ListPtr    - Pointer to the linked list to destroy
 Returns: nothing
 Errors: none
******************************************************************************/
void DestroyLinkedList(LinkedLists *ListPtr)
{
    LinkedListNodes* current = ListPtr->FrontPtr;
    while (current != NULL) {
        free(current->ElementPtr);
        FREE_DEBUG(current->ElementPtr);
        free(current);
        FREE_DEBUG(current);
        current = current->Previous;
    }
    ListPtr->NumElements = 0; // reflect that there are 0 elements in LL 
    ListPtr = NULL;
}


/******************************************************************************
 Searches the linked list for a provided word. If found, returns the pointer
 to the element struct. If not found, returns a NULL pointer
 
 Where: LinkedLists *ListPtr - Pointer to the linked list to search
        char *String         - Pointer to the string to search
 Returns: Pointer to the element if found, NULL otherwise
 Errors: none
 * ***************************************************************************/
ElementStructs *SearchList(LinkedLists *ListPtr, char *String)
{

    LinkedListNodes* current = ListPtr->FrontPtr;
    while (current != NULL) {

        if (!strcmp(current->ElementPtr->Word, String))  {
            return current->ElementPtr;
        } 
        current = current->Previous;
    }
    return NULL;
}

