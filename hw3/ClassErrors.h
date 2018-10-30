#ifndef _CLASS_ERRORS_H_
#define _CLASS_ERRORS_H_
/******************************************************************************
* Standard class errors list - professor file
  07/23/2017 - R. Repka     Initial version
  11/20/2017 - R. Repka     Added pivot errors
******************************************************************************/

/* Standard error return codes */
#define MALLOC_ERROR        (-99)
#define CALLOC_ERROR        (-100)
#define REALLOC_ERROR       (-101)
#define STR_LENGTH_ERROR    (100)

#define PGM_SUCCESS           (0)        /* Program was successful  */
#define PGM_SYNTAX_ERROR     (20)       /* Program had a syntax error */
#define PGM_FILE_NOT_FOUND   (10)       /* Input file not found */
#define PGM_INTERNAL_ERROR   (99)       /* Internal logic error */


#define CMD_ARGUMENT_ERROR    (1)       /* Incorrect number of arguments*/
#define FILE_ERROR            (2)       /* File could not be opened */
#define NOPIVOTFOUNT          (3)       /* Invalid pivot value */
#define TOOMANYCOLS           (4)       /* Too many columns in the data */
#define TOOMANYROWS           (5)       /* Too many rows in the data */
#define MATRIXNOTSQUARE       (6)

/* Define NAN if this compiler does not */ 
 #ifndef NAN
   #define NAN (0x7f38d30d9000)
#endif
 #ifndef iNAN
   #define iNAN (0x80000000)
#endif

/* Clever macros */
#ifdef MDEBUG
   #define MALLOC_DEBUG(P)                                               \
         fprintf(stderr, "debug: malloc() of %p at line %d in %s\n",     \
                                         (void *)P, __LINE__, __FILE__)

   #define FREE_DEBUG(P)   do { free(P);                                  \
   fprintf(stderr, "debug: free()   of %p at line %d in %s\n",            \
                         (void *)P, __LINE__, __FILE__);                  \
   fflush(stderr);} while(0) 

#else
   #define MALLOC_DEBUG(P) 
   #define FREE_DEBUG(P)   free(P)
#endif




#endif /* _CLASS_ERRORS_H_ */
