/******************************************************************************
 * TestDarray_hw.c - skeleton of code to test DynamicArray module
 *
 * Program to test the DynamicArrays module.  Reads in a large list of words
 * and puts them into the data structure, then prints out the first and last 
 * five elements in the data structure as well as the total number of words.
 * R. Repka   9/27/2017 - Minor comment fix
 * R. Repka  12/23/2017 - Safer fscanf() and strncpy()
 ******************************************************************************/
#include <stdio.h>
#include <string.h>
#include "ClassErrors.h"
#include "DynamicArrays.h"

/* Initial Size of Dynamic Array */
#define INITIAL_SIZE (1000)

/* Local function definitions */
 void ReadData(FILE *InputFile, DArray *DynamicArrayPtr);


/******************************************************************************
 This program to test the Data structure by reading the Data file provided on
 the command line into the structure, printing out the first and last N
 elements and total number of elements.  
        int main(int argc, char* argv[])
 Where:
    int argc        - The number of command line parameters
    char *argv[]    - An array of pointers to the parameters
    returns: in     - 0 for success, non-zero for error
    errors:         - Error message printed to stderr and exits with an error 
                      code 
******************************************************************************/
int main(int argc, char* argv[]) {
    /* declare local variables */
    int ErrorCode = 0;               /* Application error code - 0 is OK */
    DArray TestDynamicArray;         /* Dynamic Array for Data */
    FILE *DataFile = NULL;           /* Pointer to Data file name (from cmd line) */

    /* One command line argument is required: the file name     */
    if (2 == argc) {/* note that argc 2 means one argument given */
        /* Try to open the Data file for input (read mode)    
          argv[1] is a string which contains the file name*/
        DataFile = fopen(argv[1], "r"); 

        /* Verify that file was opened correctly */
        if (NULL != DataFile) {
            /********************************************************************
             Pseudo code: 
             Initialize the dynamic array 
             Read all Data from text file 
             Close the Data file - use fclose(DataFile)
             Print the first 6 entries 
             Print the last 6 entries 
             Print total number of words read 
             De-allocate the dynamic array 
            ********************************************************************/
            /* WRITE YOUR CODE HERE */
            CreateDArray(&TestDynamicArray, INITIAL_SIZE);
            ReadData(DataFile, &TestDynamicArray);
            
            fclose(DataFile);
            printf("\r\nFirst 6 elements of file \r\n");
            for (int i = 0; i < 6; i++) {
                printf("%s \r\n", TestDynamicArray.Payload[i].String);
            }

            printf("\r\nLast 6 elements of file \r\n");
            for (int i = TestDynamicArray.EntriesUsed - 6; i < TestDynamicArray.EntriesUsed; i++) {
                printf("%s \r\n", TestDynamicArray.Payload[i].String);
            }

            printf("\r\nEntries Used: %d \r\n", TestDynamicArray.EntriesUsed);
            DestroyDArray(&TestDynamicArray);

        } /* if() */
        else {
            /* WRITE YOUR CODE HERE */
            /* Print error messages */
            fprintf(stderr, "ERROR: the file '%s' could not be opened.\r\n", argv[1]);
            ErrorCode = FILE_ERROR;
        } /* if...else() */
    } /* if() */
    else{
        /* WRITE YOUR CODE HERE    */
        /* Print usage information */
        fprintf(stderr, "ERROR:\n\
                \rThis command reads words into a dynamic array and prints the first 6\n\
                \rand last 6 words from the input file. Usage information is below\n\
                \rUsage: ./TestDarray_hw <file.txt>\r\n");
        ErrorCode = CMD_ARGUMENT_ERROR;
    } /* if...else() */

    return ErrorCode;
} /* main() */
  
 
/******************************************************************************
 Read the data file (assumed to be open) one word at a time placing each
 word into the data structure.  This routine will verify that the input string
 has the correct length.  Any string that is too long will generate a warning
 to stderr, not be added but processing will continue
        void ReadData(FILE *InputFile, DArray *DynamicArrayPtr)
  Where:
    FILE *InputFile         - Pointer to an open input file
    DArray *DynamicArrayPtr - Pointer to a storage block which holds the
                              data structure entry 
    returns: voi            - nothing is returned.
    errors:                 - This routine will print an error message to 
                              stderror and exit with an error code
******************************************************************************/
void ReadData(FILE *InputFile, DArray *DynamicArrayPtr)
{
    Data TempData;             /* temp variable to hold data   */
    int lcv = 1;               /* loop control variable        */
    int checkfollowing = 0;    /* Check the line length following the perp. */
    char String[MAX_STR_LEN+2];/* temp variable to hold string */
    char formatStr [32];       /* Used to build the dynamic length */
   
    /* Build a dynamic format string */
    sprintf(formatStr, "%c%d%c", '%', MAX_STR_LEN+1, 's');
   
    /* Read cthe data in from the file into the String buffer */
    while (EOF != fscanf(InputFile, formatStr, String))
    {
        /* Insert code here to make sure the input data is not too long
          hint:  use strlen(String)   */
        if ((strlen(String) > 255) || (checkfollowing == 1)) {
            if (checkfollowing == 0) {
                checkfollowing = 1;
            }
            else {
                fprintf(stderr, "ERROR: A word in the file is too long! \r\n");
                checkfollowing = 0;
            }
            continue;
        }

        TempData.Num = lcv++;
        strncpy(TempData.String, String, MAX_STR_LEN);

        PushToDArray(DynamicArrayPtr, &TempData);

    } /* while() */
} /* ReadData() */
