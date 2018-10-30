 /*********************************************************
 * APMATRIX Module - student file
 *
 * 10/24/2013,  Juan C. Cockburn               
 * 09/13/2015,  R. Repka - added error checking
 * 11/15/2015   R. Repka - changed Real to double
 * 04/10/2016   R. Repka - Minor change to headers
 * 05/25/2016   R. Repka - Added feature to m_free to 
 *                         detect pointer rearrangement. 
 * 11/01/2016   R. Repka - Removed m_ident(), and made
 *                         minor comment changes
 * 04/06/2017   R. Repka    Minor commend modification G to A
 *********************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "resources/ClassErrors.h"
#include "resources/apmatrix.h"


void swap_row(Matrix *A, int i, int j, iVector *p);
void upper(Matrix *A, Matrix *U);
void lower(Matrix *A, Matrix *L);

/*---------------------------------------------------------------------------
  Implements in place Gaussian elimination 
  
  Where: Matrix  *A - Pointers to the matrix to solve
         iVector *p - Pointer to the permutation vector
  Errors: none
  Returns: int - 0 = success
               - 3 = No pivot point found, not solvable
---------------------------------------------------------------------------*/
int PLU_factor(Matrix *A, iVector *p)
{
    int err = 0;

    // each row in the array
    for (int k = 0; k < A->nr; k++)
    {
        double piv = A->mat[k][k];

        // each row below pivot (rbp)
        for (int i = k+1; i < A->nr; i++)
        {
            // if pivot of row is greater than rows below it, we must swap
            if ( fabs(A->mat[i][k]) > fabs(piv) )
            {
                piv = A->mat[i][k];
                swap_row(A, k, i, p);
            }
        } // for i

        // perform steps of gaussian elimination
        for (int i = k+1; i < A->nr; i++)
        {
            A->mat[i][k] = (double) (A->mat[i][k] / A->mat[k][k]);

            // subtract factor * pivot row from each row
            for (int n = k+1; n < A->nc; n++)
            {
                A->mat[i][n] -= (double) (A->mat[k][n] * A->mat[i][k]);
            }
            
        } // for i
    } // for k
    
    // check if matrix is solvable
    if (fabs(A->mat[A->nr-1][A->nc-2]) <= (double) 1E-9) {
        err = NOPIVOTFOUND;
    }
    if ( isnan(A->mat[A->nr-1][A->nc-2]) ) {
        err = NOPIVOTFOUND;
    }

    return err;
}

/*---------------------------------------------------------------------------
  Solves Ax=b using PLU factorization
  
  Where: Matrix  *A - Pointer to the PLU factored matrix
         iVector *p - Pointer to the permutation vector 
         rVector *b - Pointer to the b matrix
         rVector *x - Pointer to the vector to hold the answer
  Errors: none
  Returns: nothing
---------------------------------------------------------------------------*/
void PLU_solve(Matrix *A, iVector *p, rVector *b, rVector *x)
{
    Matrix *U = m_alloc(A->nr, A->nc);
    Matrix *L = m_alloc(A->nr, A->nc);
    rVector *c = rv_alloc(A->nr);

    // get upper and lower triangular vectors
    upper(A, U);
    lower(A, L);

    // find the c vector with forward substitution
    for (int i = 0; i < A->nr; i++) {
        c->vec[i] = b->vec[ p->ivec[i] ];

        for (int j = 0; j < i; j++) {
            c->vec[i] -= L->mat[i][j] * c->vec[j];
        }

        c->vec[i] = c->vec[i] / L->mat[i][i];
    }

    // find the x vector with backward substitution
    for (int i = A->nr-1; i >= 0; i--) {
        x->vec[i] = c->vec[i];

        for (int j=i+1; j < A->nr; j++) {
            x->vec[i] -= U->mat[i][j] * x->vec[j];
        }

        x->vec[i] = x->vec[i] / U->mat[i][i];
    }

    rv_free(c);
    m_free(U);
    m_free(L);
}

/*---------------------------------------------------------------------------
  Allocate memory space for matrix, initialized to zero.  
  Note: This allocates one big block of RAM and then adjusts the pointer table
  
  Where: int nr - The number of rows and columns in the matrix
         int nc -
  Errors: exits
  Returns Matrix * - Pointer to the resulting matrix space
---------------------------------------------------------------------------*/
Matrix* m_alloc(int nr, int nc) {
    int i;
    double* ptr; /* pointer to head memory */
    Matrix* M;
    
    /* Allocate memory for matrix "header" structure */
    M = malloc(sizeof(Matrix));
    if (NULL == M)
    {
        printf("Error: Malloc() failed at line %d of %s\n", __LINE__, __FILE__);
        exit(MALLOC_ERROR);
    }
    M->nr = nr;
    M->nc = nc;
    
    /* Allocate space for matrix data */
    M->mat = malloc( nr * sizeof(double *)); /* row pointers */
    if (NULL == M->mat)
    {
        printf("Error: Malloc() failed at line %d of %s\n", __LINE__, __FILE__);
        exit(MALLOC_ERROR);
    }
    
    /* The data needs to be set to zero */
    ptr = calloc( nr*nc, sizeof(double) );   /* matrix data   */
    if (NULL == ptr)
    {
        printf("Error: calloc() failed at line %d of %s\n", __LINE__, __FILE__);
        exit(MALLOC_ERROR);
    }
    
    for (i=0; i<nr; i++) {                /* set row pointers */
         M->mat[i] = ptr + nc*i;
    }
    return M;
}


/*---------------------------------------------------------------------------
  Release memory space used by a matrix  
  
  Where: Matrix *m - The matrix to free
  Errors: none
  Returns nothing
---------------------------------------------------------------------------*/
void m_free(Matrix* M) {

    int i;
    void *smallP;

    /* The pointer might have been rearranged, so find the smallest */
    smallP = (void *)M->mat[0];
    for (i=1; i<M->nr; i++) {
        if (smallP > (void *)M->mat[i]) { smallP = (void *)M->mat[i];}
    }

    free(smallP);    /* free data */
    free(M->mat);    /* free row pointers */
    free(M);         /* free matrix header */
}

 
 
/*---------------------------------------------------------------------------
  Print the matrix elements 
  
  Where: Matrix *m - The matrix to print
         char *fs  - pointer to a printf format string
  Errors: none
  Returns nothing
---------------------------------------------------------------------------*/
void m_print(const Matrix* M, const char* fs) {
  int i,j;
  for (i=0; i<M->nr; ++i) 
  { 
    for (j=0; j<M->nc; ++j)
    {        
        printf(fs, M->mat[i][j]);
    }
    putchar('\n');
  }
  putchar('\n');
}


/*---------------------------------------------------------------------------
  Allocates a real vector of size n, uninitialized
  
  Where: int n - The size of the vector to create
  Errors: none
  Returns rVector * - Pointer to the resulting reals vector
---------------------------------------------------------------------------*/
rVector* rv_alloc(int n)
{
    rVector *r = malloc(sizeof(rVector));
    if (NULL == r)
    {
        printf("Error: Malloc() failed at line %d of %s\n", __LINE__, __FILE__);
        exit(MALLOC_ERROR);
    }

    r->n = n;

    r->vec = malloc(sizeof(double) * n);
    if (NULL == r->vec)
    {
        printf("Error: Malloc() failed at line %d of %s\n", __LINE__, __FILE__);
        exit(MALLOC_ERROR);
    }

    return r;
} 


/*---------------------------------------------------------------------------
  Allocates an integer vector of size n
  
  Where: int n - The size of the integer vector to create
  Errors: exits
  Returns iVector * - Pointer to the resulting integer vector
---------------------------------------------------------------------------*/
iVector* iv_alloc(int n)
{
    iVector *i = malloc(sizeof(rVector));
    if (NULL == i)
    {
        printf("Error: Malloc() failed at line %d of %s\n", __LINE__, __FILE__);
        exit(MALLOC_ERROR);
    }

    i->n = n;

    i->ivec = malloc(sizeof(double) * n);
    if (NULL == i->ivec)
    {
        printf("Error: Malloc() failed at line %d of %s\n", __LINE__, __FILE__);
        exit(MALLOC_ERROR);
    }
    for (int j = 0; j < n; j++)
    {
        i->ivec[j] = j;
    }

    return i;
} 


/*---------------------------------------------------------------------------
  Frees a reals vector 
  
  Where: rVector *V - The real vector to release
  Errors: none
  Returns: nothing
---------------------------------------------------------------------------*/
void rv_free(rVector* V)
{
    free(V->vec);    /* free row pointers */
    free(V);         /* free matrix header */
} 


/*---------------------------------------------------------------------------
  Frees an integer vector 
  
  Where: iVector *V - The integer vector to release
  Errors: none
  Returns: nothing
---------------------------------------------------------------------------*/
void iv_free(iVector* V)
{
    free(V->ivec);    /* free row pointers */
    free(V);         /* free matrix header */
} 


/*---------------------------------------------------------------------------
  Prints a real vector 
  
  Where: rVector *V - The reals vector to print
         char *fs  - Pointer to a printf format string
  Errors: none
  Returns: nothing
---------------------------------------------------------------------------*/
void rv_print(const rVector* V, const char* fs)
{
    int i;
    for (i = 0; i < V->n; ++i) 
    { 
        printf(fs, V->vec[i]);
        putchar('\n');
    }
} 

/*---------------------------------------------------------------------------
  Prints an integer vector 
  
  Where: iVector *V - The integer vector to print
         char *fs  - Pointer to a printf format string
  Errors: none
  Returns: nothing
---------------------------------------------------------------------------*/
/* Prints an ivector */
void iv_print(const iVector* V, const char* fs)
{
    int i;
    for (i = 0; i < V->n; ++i) 
    { 
        printf(fs, V->ivec[i]);
        putchar('\n');
    }
} 




/*############################################################################
 * Helper functions
############################################################################*/

/*---------------------------------------------------------------------------
  Swap row i and row j in A matrix 
  Swap element i and element j in p vector
---------------------------------------------------------------------------*/
void swap_row(Matrix *A, int i, int j, iVector *p)
{
    // swap the p vector elements
    int t = p->ivec[i];
    p->ivec[i] = p->ivec[j];
    p->ivec[j] = t;

    // swap the A matrix rows
    for (int s = 0; s < A->nc; s++)
    {
        double temp = A->mat[i][s];
        A->mat[i][s] = A->mat[j][s];
        A->mat[j][s] = temp;
    }
}

/*---------------------------------------------------------------------------
 * upper traingular matrix of A
---------------------------------------------------------------------------*/
void upper(Matrix *A, Matrix *U)
{
    for (int i = 0; i < A->nr; i++)
    {
        for (int j = 0; j < A->nr; j++)
        {
            if (i<=j) {
                U->mat[i][j] = A->mat[i][j];
            } else {
                U->mat[i][j] = 0;
            }
        }
    }
}

/*---------------------------------------------------------------------------
 * lower traingular matrix of A
---------------------------------------------------------------------------*/
void lower(Matrix *A, Matrix *L)
{
    for (int i = 0; i < A->nr; i++)
    {
        for (int j = 0; j < A->nr; j++)
        {
            if (i>j) {
                L->mat[i][j] = A->mat[i][j];
            } if (i==j) {
                L->mat[i][j] = 1;
            } if (i<j) {
                L->mat[i][j] = 0;
            }
        }
    }
}
