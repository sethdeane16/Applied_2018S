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
    if (3 == argc) { /* note that argc 3 means two argument given */
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

            #ifdef SEARCHWORD
            char SearchStr[MAX_STR_LEN];
            strcpy(SearchStr,argv[2]);

            ElementStructs *FoundStr = NULL;
            FoundStr = SearchList(&llist, SearchStr);
            if (FoundStr != NULL){
                printf("%s was found on line %d \n", FoundStr->Word, 
                        FoundStr->Position);
            } else {
                printf("'%s' was not found \n", SearchStr);
            }
            #endif

            /* De-allocate the linked list */
            fprintf(stdout, "Destroying the linked list\n"); 

            if (llist.NumElements != 0) {
                DestroyLinkedList(&llist);
            }

        /* Datafile error */
        } else { 
        
            fprintf(stderr, "Please use a valid data file of a list of words with one word per line. \n");
            fprintf(stderr, "Progam Usage: ./TestSearch <datafile> <word to search for> \n");
            ErrorCode = DATAFILE_ERROR;  
   
        } /* if...else() */
    } /* if() */
    else { /* Usage message */
        
        fprintf(stderr, "This program requires two arguments. See usage below. \n");
        fprintf(stderr, "Progam Usage: ./TestSearch <datafile> <word to search for> \n");

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
