/**************************************************************************
  Student frame work.   Add and remove as you see fit.
  
  07/14/2017    R. Repka    Initial release
 *************************************************************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "resources/ClassErrors.h"
#include "resources/apmatrix.h"
#include "resources/Timers.h"
#include "apmatrix.c"

#define ELEMENT_SIZE 20

Matrix* readfile(FILE *DataFile);

int main(int argc, char* argv[])
{
    /*------------------------------------------------------------------------
      UI variables with sentential values
    ------------------------------------------------------------------------*/
    int verbose = 0;
    FILE *DataFile = NULL;
    int err = 0; // Error value set during the factorization

    /*------------------------------------------------------------------------
      These variables are used to control the getopt_long_only command line 
      parsing utility.  
    ------------------------------------------------------------------------*/
    int rc;
    /* getopt_long stores the option index here. */
    int option_index = 0;
  
    /* This contains the short command line parameters list */
    char *getoptOptions = "vi:";
  
    /* This contains the long command line parameter list */
    struct option long_options[] = {
        {"verbose",     no_argument,       0, 'v'},
        {"verb",        no_argument,       0, 'v'},
        {"input",       required_argument, 0, 'i'},
        {"in",          required_argument, 0, 'i'},
        {0, 0, 0, 0}    /* Termination */
    };
  
    opterr = 1;           /* Enable automatic error reporting */
    while ((rc = getopt_long_only(argc, argv, getoptOptions, long_options, &option_index)) != -1) {
      
        /* Detect the end of the options. */
        switch (rc)
        {
            case 'v':                    /* Verbose */
                verbose = 1;
                break;

            case 'i':
                DataFile = fopen(optarg, "r");
                break;

            case '?':  /* Handled by the default error handler */
                break;

            default:
                printf ("Internal error: undefined option %0xX\n", rc);
                exit(PGM_INTERNAL_ERROR);

        } // End switch 
    } /* end while */

    /*------------------------------------------------------------------------
     Check for command line syntax errors
    ------------------------------------------------------------------------*/
    if ((optind < argc)){
        fprintf(stderr, "Tests matrix operations parallel GSL\n");
        fprintf(stderr, "Usage: ./hw7 -input file <-verbose>\n");
        fprintf(stderr, "e.g: ./hw7 -input rand.txt -verbose\n");
        fflush(stderr);
        return(PGM_INTERNAL_ERROR);
    } else {

        // timing
        #ifdef EN_TIME
        DECLARE_TIMER(timer);
        START_TIMER(timer);
        #endif
    

        // read datafile and execute things
        if (NULL != DataFile) {

            /*******************************************
             * My Solution
             ******************************************/
            // read file
            Matrix *M = readfile(DataFile);
            iVector *p = iv_alloc(M->nr);
            rVector *x = rv_alloc(M->nr);
            rVector *b = rv_alloc(M->nr);

            // if you want a different b matrix, comment out this line
            for (int i = 0; i < M->nr; i++) {
                b->vec[i] = M->mat[i][M->nr];
            }

            if (verbose) {
                printf("A = \n");
                m_print(M, "%f ");
                printf("B = \n");
                rv_print(b, "%f ");
                printf("\n");
            }

            // perform various matrix operations here
            err = PLU_factor(M, p);

            if (verbose) {
                printf("P = \n");
                iv_print(p, "%d ");
                printf("\n");
                printf("LU Matrix = \n");
                m_print(M, "%f ");
            }

            if (err == 0) {
                PLU_solve(M, p, b, x);
                if (verbose) {
                    printf("X = \n");
                    rv_print(x, "%f ");
                }
            } else {
                printf("No pivot was found in bottom row, matrix not solvable.\n");
            }

            // close file and free matrix
            rv_free(b);
            rv_free(x);
            iv_free(p);
            m_free(M);
            fclose(DataFile);
        } 
        #ifdef EN_TIME
        STOP_TIMER(timer);
        PRINT_TIMER(timer);
        #endif
    }
    return PGM_SUCCESS;
}

/*-----------------------------------------------------------------------
 * Reads the file and creates the matrix
 *
 * Where:  *Datafile - pointer to the file to open and read
 * Error:  Nothing
 * Return: The pointer to the matrix
 *----------------------------------------------------------------------*/
Matrix* readfile(FILE *DataFile) {

    int nr = 0;
    int nc = 0;

    // read first line to get size of array
    char *FirstLine = malloc(ELEMENT_SIZE * 2);
    if ( NULL != fgets(FirstLine, ELEMENT_SIZE * 2, DataFile) ) {
        char *token = strtok(FirstLine, " ");
        int i = 0;
        while (token != NULL) {
            if (i == 0) { nr = atof(token); }
            else        { nc = atof(token); }

            token = strtok(NULL, " ");
            i++;
        } // while != NULL
    } // end of first line
    free(FirstLine);

    // put array from file into matrix
    Matrix *M = NULL;
    M = m_alloc(nr, nc);
    char *String = malloc(nr * nc * ELEMENT_SIZE);

    // read row from file
    int lcv = 0;
    while ( NULL != fgets(String, nr * nc * ELEMENT_SIZE, DataFile) && lcv < nr ) {
        char *token = strtok(String, " \t\r\n");
        int i = 0;
        while (token != NULL && i < nc) {
            M->mat[lcv][i] = atof(token);
            token = strtok(NULL, " \t\r\n");
            i++;
        } // while != NULL

        lcv ++;
    } // while != EOF
    free(String);

    return M;
}

