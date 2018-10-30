/**************************************************************************
  Student frame work.   Add and remove as you see fit.
  
  07/14/2017    R. Repka    Initial release
 * ***********************************************************************/
#include </usr/include/complex.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "resources/ClassErrors.h"
#include "resources/poly.h"

/************************************************************************
  Tests three types of root finding, secant, newton, and bisection,
  based on user input and prints out the timing results.
************************************************************************/
int main(int argc, char* argv[])
{
    /*------------------------------------------------------------------------
      UI variables with sentential values
    ------------------------------------------------------------------------*/
    int verbose = 0;
    double tol = .000001;
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

            // TODO: setup reading from file?
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
    // TODO: add error checking for syntax
    if (optind < argc) {
        fprintf(stderr, "Tests complex root finding method\n");
        fprintf(stderr, "Usage: ./hw6 -input file <-verbose>\n");
        fprintf(stderr, "e.g: ./hw6 -input polynomials.txt -verbose\n");
        fflush(stderr);
        return(PGM_INTERNAL_ERROR);
    } /* End if error */

    // read datafile and execute things
    if (NULL != DataFile) {

        // read row from file
        char Stringlen[255];
        char String[255];
        while ( NULL != fgets(String, 255, DataFile) ) {

            // print what will be in the array
            printf("=========================================================\n");

            // copy to get the length of the string
            strcpy(Stringlen, String);

            // get length of line
            char *tokenlen = strtok(Stringlen, " ");
            int len = 0;
            while (tokenlen != NULL) {
                tokenlen = strtok(NULL, " ");
                len ++;
            }

            // setup the polynomial
            polynomial *poly = malloc(sizeof(polynomial));
            initPoly(poly, len);

            // read character from file into array
            char *token = strtok(String, " ");
            int c = 0;
            while (token != NULL) {
                double complex tokendouble = atof(token);
                poly->polyCoef[c] = tokendouble;
                // necessary closings for the itteration
                token = strtok(NULL, " ");
                c++;
                
            }

            // setup roots polynomial
            polynomial *r = malloc(sizeof(polynomial));
            initPoly(r, poly->nterms);

            // perform various mathematical functions here
            printf("P(x) = "); printPoly(poly);

            free(r->polyCoef);
            r->polyCoef = NULL;

            r->polyCoef = roots(poly, tol, verbose);
            r->nterms -=1;
            printRoots(r, tol, verbose);

            
            // free the polynomial and the array
            freePoly(r);
            freePoly(poly);

        }

        // close file
        fclose(DataFile);
    }

    return PGM_SUCCESS;
}

