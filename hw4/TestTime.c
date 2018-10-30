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
#include "resources/DynamicArrays.h"
#include "resources/LinkedLists.h"
#include "resources/Timers.h"

void ReadDataLinkedList(FILE *InputFile, LinkedLists *ListPtr);
void ReadDataDArray(FILE *InputFile, DArray *DArrayHead);

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
    LinkedLists llist;      // Linked List Definition
    DArray darray;          // Dynamic Array Definition
    int ErrorCode = 0;      // Error code variable to be updated and returned
    FILE *DataFileLL = NULL;  // Datafile used by linked list
    FILE *DataFileDA = NULL; // Datafile used by dynanmic array
 
    /* Two command line argument are required:
     * - the file name 
     * - word to search for */
    if (3 == argc) {
        
        /* attempt to open the datafile for the linked list */
        DataFileLL = fopen(argv[1], "r");

        /* Verify that file was opened correctly */
        if (NULL != DataFileLL) {

            /* Initialize the linked list */
            InitLinkedList(&llist); 

            /* Read data into linked list */
            #ifdef EN_TIME
            int lldata_reps = 10000000;
            DECLARE_TIMER(lldata);
            START_TIMER(lldata);
            DECLARE_REPEAT_VAR(lldata_r);
            BEGIN_REPEAT_TIMING(lldata_reps, lldata_r);
            #endif

            /* Read data from file into dynamic array */
            ReadDataLinkedList(DataFileLL, &llist);

            #ifdef EN_TIME
            END_REPEAT_TIMING;
            STOP_TIMER(lldata);
            PRINT_TIMER(lldata);
            PRINT_RTIMER(lldata, lldata_reps);
            #endif

            /* close datafile */
            fclose(DataFileLL); 

            /* initialize datastruct to hold a word if found */
            ElementStructs *FoundWord = NULL;

            /* Perform Search */
            #ifdef EN_TIME
            int llsearch_reps = 1000;
            DECLARE_TIMER(llsearch);
            START_TIMER(llsearch);
            DECLARE_REPEAT_VAR(llsearch_r);
            BEGIN_REPEAT_TIMING(llsearch_reps, llsearch_r);
            #endif
            FoundWord = SearchList(&llist, argv[2]);
            #ifdef EN_TIME
            END_REPEAT_TIMING;
            STOP_TIMER(llsearch);
            PRINT_TIMER(llsearch);
            PRINT_RTIMER(llsearch, llsearch_reps);
            #endif

            /* printing for the searched word */
            if (FoundWord != NULL) {
                printf("%s was found on line %d \n", FoundWord->Word, FoundWord->Position);
            } else {
                printf("Word was not found in the list \n");
            }

            /* De-allocate the linked list */
            if (llist.NumElements != 0) {
                DestroyLinkedList(&llist);
            }

        } /* NULL != DataFileLL */

        else { /* Datafile Error message */

            fprintf(stderr, "Please use a valid datafile of a list of words with one word per line.\n");
            fprintf(stderr, "Program Usage: ./TestList <datafile> <word to search for>\n"); 
            ErrorCode = DATAFILE_ERROR;

        }

        /* attempt to open datafile */
        DataFileDA = fopen(argv[1], "r");

        /* Verify that file was opened correctly */
        if (NULL != DataFileDA) {

            /* Initialize Dynamic Array */
            CreateDArray(&darray, INITIAL_SIZE);

            /* Setup Timing */
            #ifdef EN_TIME
            int dadata_reps = 10000000;
            DECLARE_TIMER(dadata);
            START_TIMER(dadata);
            DECLARE_REPEAT_VAR(dadata_r);
            BEGIN_REPEAT_TIMING(dadata_reps, dadata_r);
            #endif

            /* Read data from file into dynamic array */
            ReadDataDArray(DataFileDA, &darray);

            /* close timing information */
            #ifdef EN_TIME
            END_REPEAT_TIMING;
            STOP_TIMER(dadata);
            PRINT_TIMER(dadata);
            PRINT_RTIMER(dadata, dadata_reps);
            #endif

            /* Close the Data file */
            fclose(DataFileDA); 

            /* initialize data struct to hold found word */
            Data *FoundWordArr = NULL;

            /* begin timing for search in dynamic array */
            #ifdef EN_TIME
            int dasearch_reps = 1000;
            DECLARE_TIMER(dasearch);
            START_TIMER(dasearch);
            DECLARE_REPEAT_VAR(dasearch_r);
            BEGIN_REPEAT_TIMING(dasearch_reps, dasearch_r);
            #endif

            /* perform search */
            FoundWordArr = SearchDArray(&darray, argv[2]);

            /* close timing information */
            #ifdef EN_TIME
            END_REPEAT_TIMING;
            STOP_TIMER(dasearch);
            PRINT_TIMER(dasearch);
            PRINT_RTIMER(dasearch, dasearch_reps);
            #endif


            /* printing for the searched word */
            if (FoundWordArr != NULL) {
                printf("%s was found on line %d \n", FoundWordArr->String, FoundWordArr->Num);
            } else {
                printf("Word was not found in the list \n");
            }

            /* Destroy the dynamic array */
            DestroyDArray(&darray);

        } /* NULL != DataFileDA */

        else { /* Datafile rror message */

            fprintf(stderr, "Please use a valid datafile of a list of words with one word per line.\n");
            fprintf(stderr, "Program Usage: ./TestList <datafile> <word to search for>\n"); 
            ErrorCode = DATAFILE_ERROR;

        } /* if...else() */

    }

    else { /* Incorrect arguments error message and usage message */
        fprintf(stderr, "Please make sure that you are using 3 command lines, usage below..\n");
        fprintf(stderr, "Program Usage: ./TestList <datafile> <word to search for>\n"); 
        ErrorCode = ARG_ERROR; 
    }

     return ErrorCode;
} /* main() */


void ReadDataLinkedList(FILE *InputFile, LinkedLists *ListPtr) {
    ElementStructs TempData;   /* temp variable to hold data   */
    int lcv = 1;               /* loop control variable        */
    char String[MAX_STR_LEN+2];/* temp variable to hold string */
    char formatStr [32];       /* Used to build the dynamic length */
   
    /* Build a dynamic format string */
    sprintf(formatStr, "%c%d%c", '%', MAX_STR_LEN+1, 's');
   
    /* Read cthe data in from the file into the String buffer */
    while (EOF != fscanf(InputFile, formatStr, String)) {

        TempData.Position = lcv++;
        strncpy(TempData.Word, String, MAX_STR_LEN);
        AddToBackOfLinkedList(ListPtr, &TempData);

    } /* while() */

} /* ReadDataLinkedList() */



void ReadDataDArray(FILE *InputFile, DArray *DArrayHead) {
    Data TempData;   /* temp variable to hold data   */
    int lcv = 1;               /* loop control variable        */
    char String[MAX_STR_LEN+2];/* temp variable to hold string */
    char formatStr [32];       /* Used to build the dynamic length */
   
    /* Build a dynamic format string */
    sprintf(formatStr, "%c%d%c", '%', MAX_STR_LEN+1, 's');
   
    /* Read cthe data in from the file into the String buffer */
    while (EOF != fscanf(InputFile, formatStr, String)) {

        TempData.Num = lcv++;
        strncpy(TempData.String, String, MAX_STR_LEN);
        PushToDArray(DArrayHead, &TempData);

    } /* while() */

} /* ReadDataLinkedList() */
