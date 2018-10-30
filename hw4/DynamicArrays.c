/**
 ** DynamicArrays.c  - private implementations - student file
 **
 ** Implementation of a (constant growth) Dynamic Array data type             
 **   Modify struc Data in DynamicArrays.h for payload according application  
 **   To change growth policy modify PushToDArray as needed                   
 **                                                                           
 ** Written by: Greg Semeraro,Ph.D.                                           
 ** Modified: Dr. Juan C. Cockburn (jcck@ieee.org)                            
 ** Revised: 02/01/2014 JCCK     
 **          10/02/2015 R Repka 
 **          03/03/2017 R Repka
 **          07/21/2017 R.Repka - added class errors include file
 **          09/25/2017 R.Repka - Minor psuedo code correction
 **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "resources/ClassErrors.h"
#include "resources/DynamicArrays.h"

#define INITIAL_SIZE (1000)

/***************************************************************************
 CreateDArray -- Allocates memory and initializes DArray state             
        void CreateDArray(DArray *DArrayHead, unsigned int InitialSize)
 Where:
    DArray *DArrayHead       - Pointer to a storage block which holds the
                               data structure entry 
    unsigned int InitialSize - The initial data block size, could be 0
    returns: void            - nothing is returned.
    errors:                  - This routine will print an error message to 
                               stderror and exit with an error code
 **************************************************************************/
void CreateDArray(DArray *DArrayHead, unsigned int InitialSize) {
    /************************************************************************
     Pseudo code: 
     Initialize array with 0 entries used 
     Initialize capacity (you decide, could be zero)
     Based on capacity allocate memory storage, if necessary 
        If initial capacity is zero, no allocation needed 
        else Allocate storage space for payload according to struc Data
     if memory not allocated print error to stderr and exit with a code 
    ***********************************************************************/
    int ErrorCode = 0;
    DArrayHead->EntriesUsed = 0; 
    DArrayHead->Capacity = InitialSize;

    if (DArrayHead->Capacity != 0) {
        DArrayHead->Payload = (Data*) malloc(InitialSize * sizeof(Data));

        // Error checking for Malloc
        if (NULL == DArrayHead->Payload) {
            ErrorCode = MALLOC_ERROR;
            fprintf(stderr, "MALLOC_ERROR");
        }
    }

    else {
        DArrayHead->Payload = NULL;
        printf("Error in creation of Darray");
    }

} /* CreateDArray() */


/***************************************************************************
  PushToDArray -- Adds data at the end of the dynamic array and updates its 
                  state in the array header.  If full, reallocates memory 
                  space according to growth policy                          
        unsigned int PushToDArray(DArray *DArrayHead, Data *PayloadPtr)     
Where:
    DArray *DArrayHead      - Pointer to a storage block which holds the
                              data structure entry 
    Data * PayloadPtr       - 
    returns: unsigned int   - The index of the last element inserted
    errors:                 - This routine will print an error message to 
                              stderror and exit with an error code 
 **************************************************************************/
unsigned int PushToDArray(DArray *DArrayHead, Data *PayloadPtr)
{
   /************************************************************************
    Pseudo code: 
    Check if the array needs to be made larger 
    If yes, increase the size of the dynamic array 
    Re-allocate storage for the array elements using larger size 
    if memory not re-allocated print error to stderr and exit 
    Copy the Data in PlayloadPtr into the Darray
    Increment the number of elements used in Darray header 
    Return the array index of last element inserted 
   ************************************************************************/
    if (DArrayHead->EntriesUsed >= DArrayHead->Capacity) {
        DArrayHead->Capacity = DArrayHead->Capacity + INITIAL_SIZE;
        DArrayHead->Payload = realloc(DArrayHead->Payload, DArrayHead->Capacity * sizeof(Data));
        if (NULL == DArrayHead->Payload) {
            fprintf(stderr, "REALLOC_ERROR");
            return REALLOC_ERROR;
        }
    }
    memcpy(&DArrayHead->Payload[DArrayHead->EntriesUsed], PayloadPtr, sizeof(Data));
    DArrayHead->EntriesUsed ++;
    
    return DArrayHead->EntriesUsed;

} /* PushToDArray() */


/*************************************************************************
 DestroyDArray -- Returns the memory back to the heap and updates the   
                  state in the array header      
        void DestroyDArray(DArray *DArrayHead)
  Where:
    DArray *DArrayHead  - Pointer to a storage block which holds the
                          data structure entry 
    returns: void       - nothing is returned.
    errors:             - No errors are reported
*************************************************************************/
void DestroyDArray(DArray *DArrayHead)
{
   /**********************************************************************
    Pseudo code: 
    Set the number of entries used to zero in Darray header 
    Set the capacity to zero in Darray header
    De-allocate the storage for the array elements 
 *************************************************************************/
    DArrayHead->EntriesUsed = 0;
    DArrayHead->Capacity = 0;
    free(DArrayHead->Payload);
} /* DestroyDArray() */



Data *SearchDArray(DArray *DArrayHead, char *String) {

    int i = 0;
    
    while (i <= DArrayHead->EntriesUsed) {
        if (!strcmp(DArrayHead->Payload[i].String, String)) {
            return &DArrayHead->Payload[i];
        }
        i++;
    }
    return NULL;
}
