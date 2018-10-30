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
#include <gsl/gsl_linalg.h>


#define ELEMENT_SIZE 20

Matrix* readfile(FILE *DataFile);

int main(int argc, char* argv[])
{
    /*------------------------------------------------------------------------
      UI variables with sentential values
    ------------------------------------------------------------------------*/
    int verbose = 0;
    FILE *DataFile = NULL;

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

        #ifdef EN_TIME
        DECLARE_TIMER(timer);
        START_TIMER(timer);
        #endif
        // read datafile and execute things
        if (NULL != DataFile) {

            /*******************************************
             * Setup GSL Solution
             ******************************************/
            // read file
            Matrix *M = readfile(DataFile);

            int nr = M->nr;     /* Matrix dimensions, rows    */
            int nc = M->nc;     /* Matrix dimensions, columns */

            // setup a_data for gsl
            double *a_data = malloc(sizeof(double) * M->nr * M->nr);
            double *b_data = malloc(sizeof(double) * M->nr);
            int count = 0;
            for (int i = 0; i < M->nr; i++) {
                for (int j = 0; j < M->nr; j++) {
                    a_data[count] = M->mat[i][j];
                    count++; 
                }
            }

            // setup b_data for gsl
            for (int i = 0; i < M->nr; i++) {
                b_data[i] = M->mat[i][M->nr];
            }

            nr = M->nr;
            nc = M->nr;

            m_free(M);
            fclose(DataFile);



            /*******************************************
             * GSL Solution
             ******************************************/
            /* Local variables */
            int i, j, k, l; /* Indices and counters       */
            int s;          /* Sign of the permutation    */
            
            /* Declare and allocate matrix and vector variables */
            gsl_matrix *A_gsl = gsl_matrix_calloc (nr,nc) ; /* A */
            gsl_vector *b_gsl = gsl_vector_calloc (nr) ;    /* b */
            gsl_vector *x_gsl = gsl_vector_calloc (nc);     /* x */
            
            gsl_permutation *p_gsl = gsl_permutation_alloc (nr); /* Permutation Vector  for LU */

            /* Initialize coefficient matrix A and vector b */
            /* use gsl_matrix_set  and gsl_vector_set */
            k = 0 ; l = 0 ; /* set counters  to zero */
            for (i=0;i<nr;i++) {
                for (j=0;j<nc;j++) {
                    gsl_matrix_set(A_gsl,i,j,a_data[k]) ; 
                    k++ ;
                } /* for j */
              
                gsl_vector_set(b_gsl,i,b_data[l]) ; 
                l++ ;
            } /* for i */

            if (verbose) {
                /* Print entries of A use gsl_matrix_get and printf */
                /* do not use gsl_matrix_fprintf */
                printf("Solution of the system Ax=b via PLU factorizations\n");
                printf("Matrix A:\n");
                for (i = 0; i < nr; i++) {
                    for (j = 0; j < nc; j++)
                        printf ("%7.2g ", gsl_matrix_get (A_gsl, i, j));
                    putchar('\n');
                } /* for i */
          
                /* Print entries of vector b */
                printf("Vector b:\n");
                gsl_vector_fprintf (stdout, b_gsl,"%7.2g") ;
            }

            /* Perform (in place) PLU factorization */
            gsl_linalg_LU_decomp (A_gsl, p_gsl, &s); /* A is overwritten, p is permutation     */
          
            if (verbose) {
                /* Print out the P and LU matrix */
                fprintf(stdout, "\nP = [");
                gsl_permutation_fprintf (stdout, p_gsl, " %u");
                fprintf(stdout, " ] \n");
                
                fprintf(stdout, "LU matrix = \n");
                printf("Matrix LU:\n");
                for (i = 0; i < nr; i++) {
                    for (j = 0; j < nc; j++) {
                        printf ("%7.2g ", gsl_matrix_get (A_gsl, i, j));
                    } 
                    putchar('\n');
                } /* End for i */               
            }
            
            /* Find solution using the PLU factors found above and b */
            gsl_linalg_LU_solve (A_gsl, p_gsl, b_gsl, x_gsl);

            if (verbose) {
                /* Print solution x */
                printf("Solution x:\n");
                gsl_vector_fprintf (stdout, x_gsl, "%7.2g");
            }
     
            
            /* Clean up - free heap memory */
            free(b_data);
            free(a_data);
            gsl_matrix_free (A_gsl);
            gsl_vector_free (b_gsl);
            gsl_permutation_free (p_gsl);
            gsl_vector_free (x_gsl);
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

