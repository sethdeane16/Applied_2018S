/********************************************************************
 * Applied Programming:                                             
 *    Solution of Overdetermined System of Equations Ax=b arising   
 *    in least square problems via QR factorizations using the GSL                                                 *                                                                  
 * Compilation:  gcc -ansi -g -lgsl -lgslcblas  hw8.c DynamicArrays.c -o  hw8
 *                                                                  
 * Tested in Ubuntu 12.04 LTS                                       
 * Revised: Juan C. Cockburn, April 9, 2015 (juan.cockburn@rit.edu) 
 * 10/10/2015 R. Repka - Minor clean up
 * 10/26/2016 R. Repka - Major re-write, added QR and norm solution modes
 * 11/12/2017 R. Repka - Minor comment change for norm of residuals, removed
 *                       "c" in GE_FindPoint
 * 11/28/2016 R. Repka - Added Pearson function
 * 07/11/2017 R. Repka  - Switched to getopt_long_only
 ********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <getopt.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_linalg.h>
#include "resources/ClassErrors.h"
#include "resources/DynamicArrays.h"

/*****************************************************************************
 Function prototypes 
*****************************************************************************/
void readPoints (FILE *file, DArray *DArrayPtr);
void Norm_FindPoint(int nr, int nc, const DArray *points, gsl_vector *x_ls, int verbose);
void GE_FindPoint(int nr, int nc, const DArray *points, gsl_vector *x_ls, int verbose);
double RSquareError(int nr, int nc, const DArray *points, const gsl_vector *x_ls);
double normOfResiduals(int nr, int nc, const DArray *points, const gsl_vector *x_ls);
double pearson_correl(int nr, int nc, const DArray *points, const gsl_vector *x_ls);
double evalPoly(int nc, double x, const gsl_vector *x_ls);


/*****************************************************************************
 This program uses least squares to generate approximate functions
    usage: hw8 (-ge | -norm) -order  num   -points  file  [-verbose] \n");
 
 Returns: 0 for success, non-zero for errors
 *****************************************************************************/
int main(int argc, char *argv[])
{
    DArray points;
    FILE *pointsFile;
    gsl_vector *x_ls; // least squares solution

    /*------------------------------------------------------------------------
      UI variables with sentential values
    ------------------------------------------------------------------------*/
    int verbose = 0;
    int ge = 0;
    int norm = 0;
    int order = 0;
    int err = 0; // Error value set during the factorization

    /*------------------------------------------------------------------------
      These variables are used to control the getopt_long_only command line 
      parsing utility.  
    ------------------------------------------------------------------------*/
    int rc;
    /* getopt_long stores the option index here. */
    int option_index = 0;
  
    /* This contains the short command line parameters list */
    char *getoptOptions = "gno:p:v";
  
    /* This contains the long command line parameter list */
    struct option long_options[] = {
        {"ge",          no_argument,       0, 'g'},
        {"norm",        no_argument,       0, 'n'},
        {"order",       required_argument, 0, 'o'},
        {"points",      required_argument, 0, 'p'},
        {"verbose",     no_argument,       0, 'v'},
        {"verb",        no_argument,       0, 'v'},
        {0, 0, 0, 0}    /* Termination */
    };
  
    opterr = 1;           /* Enable automatic error reporting */
    while ((rc = getopt_long_only(argc, argv, getoptOptions, long_options, &option_index)) != -1) {
      
        /* Detect the end of the options. */
        switch (rc)
        {
            case 'g':
                ge = 1;
                break;

            case 'n':
                norm = 1;
                break;

            case 'o':
                order = atoi(optarg) + 1;
                if (order < 1) {
                    printf("The Order has to be greater than or equal to 1\n");
                    exit(PGM_INTERNAL_ERROR);
                }
                break;

            case 'v':                    /* Verbose */
                verbose = 1;
                break;

            case 'p':
                pointsFile = fopen(optarg, "r");
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
        // TODO: update to hw8 usage information
        fprintf(stderr, "Makes a correction factor that makes a pretty picture\n");
        fprintf(stderr, "Usage: ./hw8 -[norm|ge] -o number -p file <-verbose>\n");
        fflush(stderr);
        return(PGM_INTERNAL_ERROR);
    } else {
        CreateDArray(&points, 100);
        readPoints(pointsFile, &points);

        x_ls  = gsl_vector_alloc(order);
        
        if ( ge &&  norm) { printf("ERROR, you have both the ge and norm flags set\n"); }
        if ( ge && !norm) { GE_FindPoint(points.EntriesUsed, order, &points, x_ls, verbose); }
        if (!ge &&  norm) { Norm_FindPoint(points.EntriesUsed, order, &points, x_ls, verbose); }
        if (!ge && !norm) { printf("ERROR, Neither ge or norm were set\n"); }
        
        // print out finding equation finding things
        printf("\nx_ls(%i x 1)\n", order);
        for (int i = 0; i < order; i++) {
            printf("%i:  %g \n", i, gsl_vector_get(x_ls, i));
        }
        double nom = normOfResiduals(points.EntriesUsed, order, &points, x_ls);
        printf("Norm of Residuals error = %f\n", nom);

        double rsq = RSquareError(points.EntriesUsed, order, &points, x_ls);
        printf("R**2 determination coef = %f\n", rsq);

        double pcl = pearson_correl(points.EntriesUsed, order, &points, x_ls);
        printf("Pearson Correlation = %f\n", pcl);

        // Clean up
        gsl_vector_free(x_ls);  
        DestroyDArray(&points);
    }
   
  return PGM_SUCCESS; /* main */
}


/*---------------------------------------------------------------------------
  Find the least squares approximation to data "points" of order "nc" using
  the Gaussian Elimination approach.
  
  Where: int nr           - The number of points (rows) of the input file
         int nc           - The number of columns (order) of the solution
         DArray *points   - Pointer to the x,y data
         gsl_vector *x_ls - The solution is returned here
         int verbose      - Verbose flag

  Returns: nothing
  Errors: Assumes the standard GSL error handler
---------------------------------------------------------------------------*/
void GE_FindPoint(int nr, int nc, const DArray *points, gsl_vector *x_ls, int verbose) {
    int i, j;

    gsl_matrix *A;    /* coefficient matrix       */
    gsl_vector *b;    /* coefficient vector       */
    gsl_vector *tau;  /* Householder coefficients */
    gsl_vector *res;  /* vector of residuals      */
    
    /* Allocate space for Matrices and vectors */
    A   = gsl_matrix_alloc(nr, nc); /* Data matrix */
    b   = gsl_vector_alloc(nr);     /* Data vector */
    tau = gsl_vector_alloc(nc);     /* required place holder for GSL */
    res = gsl_vector_alloc(nr);     /* Contains the residual */


    // set first col to 1, set second column to first col of datafile
    for (i = 0; i < nr; i++) {
        gsl_matrix_set(A, i, 0, 1.0);
        gsl_matrix_set(A, i, 1, (double) points->Payload[i].a);
    }

    // set rest of A matrix 
    for (i = 0; i < nr; i++) {
        for (j = 2; j < nc; j++) {
            double curr = gsl_matrix_get(A, i, j-1);
            gsl_matrix_set(A, i, j, curr * curr);
        }
    }



    // set b vector
    for (i = 0; i < nr; i++) {
        gsl_vector_set(b, i, (double) points->Payload[i].b);
    }

    if (verbose) {
        // print A
        printf("A(%i x %i) \n", nr, nc);
        for (i = 0; i < 2; i++) {
            printf("%i:  ", i);
            for (j = 0; j < nc; j++) {
                printf("%f ", gsl_matrix_get(A, i, j));
            }
            printf("\n");
        }
        printf("...\n");
        for (i = nr-2; i < nr; i++) {
            printf("%i:  ", i);
            for (j = 0; j < nc; j++) {
                printf("%f ", gsl_matrix_get(A, i, j));
            }
            printf("\n");
        }
        printf("\n");

        // print b
        printf("b(%i x 1) \n", nr);
        for (i = 0; i < 2; i++) {
            printf("%i:  %2.0f \n", i, gsl_vector_get(b, i));
        }
        printf("...\n");
        for (i = nr-2; i < nr; i++) {
            printf("%i:  %2.0f \n", i, gsl_vector_get(b, i));
        }
    }


    /*  Solve Ax=b directly via QR factorization */
    /*  Find QR decomposition: (note that gls_linalg_QR_decomp overwrites A )
     *  On return, the vector tau and the columns of the lower triangular part of
     *  the matrix A have the Householder coefficients and vectors */
    gsl_linalg_QR_decomp(A, tau);

    /* Solve R x_ls = Q^T b, R is upper triangular */
    /* Note that we pass the "overwritten A", tau and b as input arguments
     * On return x_ls has the least square solution and res the residual vector Ax-b  */
    gsl_linalg_QR_lssolve(A, tau, b, x_ls, res);


    /* Free memory  */
    gsl_matrix_free(A);
    gsl_vector_free(b);
    gsl_vector_free(tau);  
    gsl_vector_free(res);
} /* End GE_FindPoint() */

/*---------------------------------------------------------------------------
  Find the least squares approximation to data "points" of order "nc" using
  the "Normal equations" approach.
        
                        A'Az = A'b
  
  Where: int nr           - The number of points (rows) of the input file
         int nc           - The number of columns (order) of the solution
         DArray *points   - Pointer to the x,y data
         gsl_vector *x_ls - The solution is returned here
         int verbose      - Verbose flag

  Returns: nothing
  Errors: Assumes the standard GSL error handler
---------------------------------------------------------------------------*/
void Norm_FindPoint(int nr, int nc, const DArray *points, gsl_vector *x_ls, int verbose) {
    int i, j;         /* counters                 */
    gsl_matrix *A;    /* coefficient matrix A     */
    gsl_matrix *AT;   /* coefficient matrix A'    */
    gsl_matrix *ATA;  /* coefficient matrix A'A   */
    gsl_vector *b;    /* coefficient vector b     */
    gsl_vector *ATB;  /* coefficient vector A'b   */
    gsl_vector *tau;  /* Householder coefficients */
    gsl_vector *res;  /* vector of residuals      */

    /* Allocate space for Matrices and vectors */
    ATA  = gsl_matrix_alloc(nc, nc); /* Data matrix */
    AT   = gsl_matrix_alloc(nc, nr); /* Data matrix */
    A    = gsl_matrix_alloc(nr, nc); /* Data matrix */
    b    = gsl_vector_alloc(nr);     /* Data vector */
    ATB  = gsl_vector_alloc(nc);     /* Data vector */
    tau  = gsl_vector_alloc(nc);     /* required place holder for GSL */
    res  = gsl_vector_alloc(nc);     /* Contains the residual */

  
    // set first col to 1, set second column to first col of datafile
    for (i = 0; i < nr; i++) {
        gsl_matrix_set(A, i, 0, 1.0);
        gsl_matrix_set(A, i, 1, (double) points->Payload[i].a);
    }

    // set rest of A matrix 
    for (i = 0; i < nr; i++) {
        for (j = 2; j < nc; j++) {
            double curr = gsl_matrix_get(A, i, j-1);
            gsl_matrix_set(A, i, j, curr * curr);
        }
    }

    // set b vector
    for (i = 0; i < nr; i++) {
        gsl_vector_set(b, i, (double) points->Payload[i].b);
    }

    // transpose matrix A to AT
    gsl_matrix_transpose_memcpy(AT, A);

    // multiply AT and A
    gsl_blas_dgemm (CblasNoTrans, CblasNoTrans, 1.0, AT, A, 0.0, ATA);

    // multiply AT and b
    gsl_blas_dgemv (CblasNoTrans, 1.0, AT, b, 0.0, ATB);

    int s;
    gsl_permutation *p = gsl_permutation_alloc(nc);

    // solve (ATA)x=(ATB)
    gsl_linalg_LU_decomp(ATA, p, &s);
    gsl_linalg_LU_solve(ATA, p, ATB, x_ls);
    gsl_permutation_free(p);
 

    if (verbose) {
        // print A
        printf("A(%i x %i) \n", nr, nc);
        for (i = 0; i < 2; i++) {
            printf("%i:  ", i);
            for (j = 0; j < nc; j++) {
                printf("%f ", gsl_matrix_get(A, i, j));
            }
            printf("\n");
        }
        printf("...\n");
        for (i = nr-2; i < nr; i++) {
            printf("%i:  ", i);
            for (j = 0; j < nc; j++) {
                printf("%f ", gsl_matrix_get(A, i, j));
            }
            printf("\n");
        }
        printf("\n");

        // print b
        printf("b(%i x 1) \n", nr);
        for (i = 0; i < 2; i++) {
            printf("%i:  %2.0f \n", i, gsl_vector_get(b, i));
        }
        printf("...\n");
        for (i = nr-2; i < nr; i++) {
            printf("%i:  %2.0f \n", i, gsl_vector_get(b, i));
        }
        printf("\n");

        // print AT
        printf("AT(%i x %i) \n", nc, nr);
        for (i = 0; i < nc; i++) {
            printf("%i:  ", i);
            for (j = 0; j < 2; j++) {
                printf("%f ", gsl_matrix_get(AT, i, j));
            }
            printf(" ...\n");
        }
        printf("\n");

        // print ATA
        printf("ATA(%i x %i) \n", nc, nc);
        for (i = 0; i < nc; i++) {
            printf("%i:  ", i);
            for (j = 0; j < nc; j++) {
                printf("%f ", gsl_matrix_get(ATA, i, j));
            }
            printf("\n");
        }
        printf("\n");
 
        // print ATB
        printf("ATB(%i x 1) \n", nc);
        for (i = 0; i < nc; i++) {
            printf("%i:  %2.0f \n", i, gsl_vector_get(ATB, i));
        }
        printf("\n");
 
    }


    /* Free memory  */
    gsl_matrix_free(A);
    gsl_matrix_free(AT);
    gsl_matrix_free(ATA);
    gsl_vector_free(b);
    gsl_vector_free(ATB);
    gsl_vector_free(tau); 
    gsl_vector_free(res);
} /* end Norm_FindPoint() */



/****************************************************************************
  This calculate the norm of residuals given the points and the solution
  
                   normR = squareRoot [sum ( yi - f(x)i}**2 ]

  Where: int nr           - The number of points (rows) of the input file
         int nc           - The number of columns (order) of the solution
         DArray *points   - Pointer to the x,y data
         gsl_vector *x_ls - The solution vector, small power first

  Errors: Assumes the standard GSL error handler
  
  Returns: double norm of residuals
****************************************************************************/
double normOfResiduals(int nr, int nc, const DArray *points, const gsl_vector *x_ls) {
    double out = 0.0;
    double dif = 0.0;
    double corrected = 0.0;
    for (int i = 0; i < nr; i++) {
        corrected = points->Payload[i].a - evalPoly(nc, points->Payload[i].a, x_ls);
        dif = points->Payload[i].a - points->Payload[i].b - corrected;
        out += dif * dif;
    }
    return sqrt(out);
} /* normOfResiduals */


/****************************************************************************
  This calculate the R2 coefficient of Determination error between the points  
  and the solution
                   
  Where: int nr           - The number of points (rows) of the input file
         int nc           - The number of columns (order) of the solution
         DArray *points   - Pointer to the x,y data
         gsl_vector *x_ls - The solution vector, small power first
 
  Errors: Assumes the standard GSL error handler
  
  Returns: R squared error
****************************************************************************/
double RSquareError(int nr, int nc, const DArray *points, const gsl_vector *x_ls) {
    int i = 0;
    double dif = 0.0;
    double corrected = 0.0;
    double ideal;

    // calculate numerator
    double num = 0.0;
    for (i = 0; i < nr; i++) {
        ideal = points->Payload[i].a - points->Payload[i].b;
        corrected = ideal - evalPoly(nc, ideal, x_ls);
        dif = ideal - corrected;
        num += dif * dif;
    }
   
    // calculate u
    double u = 0.0;
    for (i = 0; i < nr; i++) {
        ideal = points->Payload[i].a - points->Payload[i].b;
        u += ideal;
    }
    u /= nr;

    // calculate denominator
    double den = 0.0;
    for (i = 0; i < nr; i++) {
        ideal = points->Payload[i].a - points->Payload[i].b;
        dif = (ideal - u);
        den += dif * dif;
    }
    
    return 1 - (num / den);
} /* End RSquareError */


/*****************************************************************************
 This calculates the Pearson's Correlation, or the excel function correl()
                    
  Where: int nr           - The number of points (rows) of the input file
         int nc           - The number of columns (order) of the solution
          DArray *points   - Pointer to the x,y data
         gsl_vector *x_ls - The solution vector, small power first

  Errors: Assumes the standard GSL error handler       
       
 Returns: double pearson_srq 
*****************************************************************************/
double pearson_correl(int nr, int nc, const DArray *points, const gsl_vector *x_ls) {
    int i = 0;
    double corrected = 0.0;
    double ideal = 0.0;

    double num = 0.0;
    for (int i = 0; i < nr; i++) {
        ideal = points->Payload[i].a - points->Payload[i].b;
        corrected = points->Payload[i].a - evalPoly(nc, ideal, x_ls);
        num += ideal * corrected;
    }
    num *= nr;

    double sy = 0.0;
    double sf = 0.0;
    for (i = 0; i < nr; i++) {
        ideal = points->Payload[i].a - points->Payload[i].b;
        corrected = points->Payload[i].a - evalPoly(nc, ideal, x_ls);
        sy += ideal;
        sf += corrected;
    }
    num = num - (sy * sf);
    // numerator is fully calculated

    double ll = 0.0; // lower left side of equation
    for (i = 0; i < nr; i++) {
        ideal = points->Payload[i].a - points->Payload[i].b;
        ll += ideal * ideal;
    }
    ll = (nr * ll) - (sy * sy);

    double lr = 0; // lower right side of equation
    for (i = 0; i < nr; i++) {
        ideal = points->Payload[i].a - points->Payload[i].b;
        corrected = points->Payload[i].a - evalPoly(nc, ideal, x_ls);
        lr += corrected * corrected;
    }
    lr = (nr * lr) - (sf * sf);

    return num / sqrt(ll * lr);
} /* End pearson_correl */


/***************************************************************************************
 Evaluates a polynomial at a point, assumes low order term first.  Must use Horner's 
 factorization 
 
 Where: int nc           - The number of columns in the solution
        double x         - Point at which splines should be evaluated
        gsl_vector *x_ls - The solution vector, small power first
          
 Returns: double - The value at the desired point
 Errors:  none
*****************************************************************************************/
double evalPoly(int nc, double x, const gsl_vector *x_ls) {

    double result = 0.0;
    for (int i = nc - 1; i >= 0; i--) {
        result = gsl_vector_get(x_ls, i) + result * x ;
    } 
    return result;

} /* End evalPoly */


/***************************************************************************************
 Reads the points data from file and returns it in a Darray
 
 Where: FILE *file     - open handle to read from
                         of the form:     22.0      6.7
                                          23.4      18.8
        DArray *DArrayPtr - Pointer to a dynamic array to store the data
  Returns: nothing
  Errors:  none
*****************************************************************************************/
void readPoints(FILE *file, DArray *DArrayPtr) {
    Data TempData;
    char String[256];
    
    while ( NULL != fgets(String, 256, file) ) {

        int spot = 0; // decides if data will go in a or b of TempData
        char *token = strtok(String, " "); // strtok things

        // for every element in the file
        while (token != NULL) {

            // first column = a
            if (spot == 0) {
                TempData.a = atof(token); 

            // second column = b
            } else {
                TempData.b = atof(token); 
            }

            // increment the spot in TempData where data will go
            spot ++;
            token = strtok(NULL, " ");
        }

        // add the data from the temp array to the d array
        PushToDArray(DArrayPtr, &TempData);
    }
     
} /* readPoints */
