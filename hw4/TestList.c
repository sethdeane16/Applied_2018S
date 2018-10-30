/******************************************************************************
 An student framework implementation to test the LinkedLists module.  
 Reads in a large list of words and puts them into the data structure, 
 then prints out the first and last six elements in the data structure as 
 well as the total number of words.
 Note: This is only a framework, it does not include ALL the functions or 
       code you will need.
******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "resources/ClassErrors.h"
#include "resources/LinkedLists.h"

void ReadData(FILE *InputFile, LinkedLists *ListPtr);

/* Local functions */
/******************************************************************************
 Program to test the Data structure by reading the Data file provided on
 the command line into the structure, printing out the first and last five
 elements and total number of elements.

 Where: int argc     - the number of parameters on the command line
        char *argv[] - Pointer to the command line parameters
 Returns: 0 - success, 
          2 - can't open input file
          1 - Invalid number of parameters
******************************************************************************/
int main(int argc, char* argv[]) {
    /* declare local variables */
    LinkedLists llist; // Linked List Definition
    int ErrorCode = 0;
    FILE *DataFile = NULL;
 
    /* One command line argument is required: the file name     */
    if (2 == argc) { /* note that argc 2 means one argument given */
        /* Print out an opening message with the file name 
        then try to open the Data file for input (read mode)   */
        DataFile = fopen(argv[1], "r");
 

        /* Verify that file was opened correctly */
        if (NULL != DataFile) {

            /* Initialize the linked list */
            InitLinkedList(&llist); 

            /* Read all Data from text file */
            ReadData(DataFile, &llist);

            /* Close the Data file */
            fclose(DataFile); 


            /* Create pointer that we can use to get data from llist */ 
            LinkedListNodes *printPtr;

            /* Print the first 6 entries */
            printPtr = llist.FrontPtr; // set the printptr to the front of the list
            fprintf(stdout, "First 6 words in linked list:\n");
            for (int i = 0; i < 6; i++) {
                if (printPtr != NULL) {
                    printf("%s \n", printPtr->ElementPtr->Word);
                    printPtr = printPtr->Previous;
                }
            }
             
            /* Print the last 6 entries */
            printPtr = llist.BackPtr; // set the printptr to the back of the list
            fprintf(stdout, "Last 6 words in linked list:\n");
            for (int i = 0; i < 6; i++) {
                if (printPtr != NULL) {
                    printf("%s \n", printPtr->ElementPtr->Word);
                    printPtr = printPtr->Next;
                }
            }
            
            /* Print total number of words read */
            fprintf(stdout, "Number of words: %d \n", llist.NumElements);
            
            /* Removal Things */
            ElementStructs *removedElement;

            /* Remove from front of list, print a message */
            if (NULL != llist.FrontPtr) {
                int elementsBefore = llist.NumElements;
                removedElement = RemoveFromFrontOfLinkedList(&llist);
                if (elementsBefore >= 2) {
                    fprintf(stdout, "Remove from front of list, new front is: %s \n", llist.FrontPtr->ElementPtr->Word);
                    fprintf(stdout, "Removed value is: %s \n", removedElement->Word);
                    free(removedElement);
                    FREE_DEBUG(removedElement);
                }
                else if (elementsBefore == 1) {
                    fprintf(stdout, "Removed value is: %s \n", removedElement->Word);
                    fprintf(stdout, "There are no more elements left in the list \n");
                    free(removedElement);
                    FREE_DEBUG(removedElement);
                }
                else if (elementsBefore == 0) {
                    fprintf(stdout, "Nothing was removed. \n");
                    fprintf(stdout, "There are no more elements left in the list. \n");
                }
            }
         
            /* Remove from back of list, print a message */
            if (NULL != llist.BackPtr) {
                int elementsBefore = llist.NumElements;
                removedElement = RemoveFromBackOfLinkedList(&llist);
                if (elementsBefore >= 2) {
                    fprintf(stdout, "Remove from back of list, new back is: %s \n", llist.BackPtr->ElementPtr->Word);
                    fprintf(stdout, "Removed value is: %s \n", removedElement->Word);
                    free(removedElement);
                    FREE_DEBUG(removedElement);
                }
                else if (elementsBefore == 1) {
                    fprintf(stdout, "Removed value is: %s \n", removedElement->Word);
                    fprintf(stdout, "There are no more elements left in the list \n");
                    free(removedElement);
                    FREE_DEBUG(removedElement);
                }
                else if (elementsBefore == 0) {
                    fprintf(stdout, "Nothing was removed. \n");
                    fprintf(stdout, "There are no more elements left in the list. \n");
                }
            }

            /* De-allocate the linked list */
            fprintf(stdout, "Destroying the linked list\n"); 
            
            if (llist.NumElements != 0) {
                DestroyLinkedList(&llist);
            }

        } else { /* Error message */
        
            fprintf(stderr, "Please use a valid datafile of a list of words with one word per line. \n");
            fprintf(stderr, "Program Usage: ./TestList <datafile> \n"); 
            ErrorCode = DATAFILE_ERROR;
        
        } /* if...else() */

    } else { /* Usage message */
        fprintf(stderr, "Please use a valid datafile of a list of words with one word per line. \n");
        fprintf(stderr, "Program Usage: ./TestList <datafile> \n"); 
        ErrorCode = ARG_ERROR; 
  
    } /* if...else() */

    return ErrorCode;
} /* main() */


void ReadData(FILE *InputFile, LinkedLists *ListPtr)
{
    ElementStructs TempData;   /* temp variable to hold data   */
    int lcv = 1;               /* loop control variable        */
    char String[MAX_STR_LEN+2];/* temp variable to hold string */
    char formatStr [32];       /* Used to build the dynamic length */
   
    /* Build a dynamic format string */
    sprintf(formatStr, "%c%d%c", '%', MAX_STR_LEN+1, 's');
   
    /* Read cthe data in from the file into the String buffer */
    while (EOF != fscanf(InputFile, formatStr, String))
    {

        TempData.Position = lcv++;
        strncpy(TempData.Word, String, MAX_STR_LEN);
        AddToBackOfLinkedList(ListPtr, &TempData);

    } /* while() */

} /* ReadData() */
