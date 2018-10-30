/***********************************************************************
 * Student C framework to calculate the roots and evaluate polynomials
 * static functions are not REQURED, you are free to implement they way
 * you wish
 * Course: Applied Programming 
 * Revised: 2015
 *
 * 10/16/2015 R. Repka  - Removed evalPoly, extern
 * 04/03/2015 R. Repka  - Added cpow() and Horner comments, 
 * 10/24/2016 R. Repka  - Updated evalDerivs comments
 * 10/26/2016 R. Repka  - Changed createPoly to initPoly
 ***********************************************************************/ 
#include </usr/include/complex.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "resources/ClassErrors.h"
#include "resources/poly.h"

#define ZERO 0.00000001

/*---------------------------------------------------------------------------
  Define local functions
---------------------------------------------------------------------------*/
static double complex* quadraticRoots( polynomial *p);
static double complex laguerre(polynomial *p, double tol, int verb);
static polynomial* deflPoly(polynomial *p, double complex root);
static double complex* evalDerivs(polynomial *p, double complex point);
static void printComplex(double complex x);
int comp (const void * elem1, const void * elem2);

/*---------------------------------------------------------------------------
  Initializes a polynomial data structure with nterms.  This allocates storage
  for the actual polynomial.
  
  Where: polynomial *p       - Pointer to polynomial data structure to create
         unsigned int nterms - The number of elements to create
  Returns: nothing
  Errors:  prints an error and exits
---------------------------------------------------------------------------*/
void initPoly(polynomial *p, unsigned int nterms){
    p->nterms = nterms;
    p->polyCoef = malloc(sizeof(double complex) * nterms);
} 


/*---------------------------------------------------------------------------
  Destroys/frees a polynomial
  
  Where: polynomial *p - Pointer to polynomial data structure to destroy
  Returns: nothing
  Errors:  none
---------------------------------------------------------------------------*/
void freePoly(polynomial *p){
    p->nterms = 0;
    free(p->polyCoef);
    p->polyCoef = NULL;
    free(p);
    p = NULL;
} 


/*---------------------------------------------------------------------------
  This function evaluates a polynomial at a complex point z.  
  Don't use the cpow() function, it generates round off errors.  You are 
  required to use Horner's factorization 
   
  Were: polynomial *p    - Pointer to the polynomial to evaluate
        double complex z - Point to evaluate 
  Returns: double complex- Complex solution
  Errors:  none
---------------------------------------------------------------------------*/
double complex cevalPoly(polynomial *p, double complex z){
    double complex out = 0.0 + 0.0*I;
    for (int i = 0; i < p->nterms; i++) {
        out = out * z + p->polyCoef[i];
    }
    return out; 
} 


/*---------------------------------------------------------------------------
 This finds all the roots (real and complex) of a real polynomial.  If there
 is a single root, it solves for it.  If there are two roots then it uses
 the quadratic formula.  If there are more than two roots, it uses Laguerre.
 If a complex root is found then BOTH the + and - roots are deflated from the
 polynomial.
 
 Where: polynomial *poly - Pointer to an array of polynomial coefficients
        double tolerance - Termination tolerance
        int verb         - Verbose flag indicating whether to print intermediate
                           results or not ( 1 = TRUE, 0 = FALSE 
 
 Returns: double complex *roots - A pointer to a complex root array
 Errors:  prints an error and exits
---------------------------------------------------------------------------*/
double complex* roots(polynomial *poly, double tolerance, int verb){

    // in the case of a linear equation
    if (poly->nterms == 2) {
        double complex *root = (double complex *)malloc(sizeof(double complex));
        *root = poly->polyCoef[1] / (- poly->polyCoef[0]);

        return root;
    }

    // in the case of quadratic or greater powered equation
    double complex *root = malloc(sizeof(double complex) * poly->nterms);
    for (int i = poly->nterms; i > 1; i--){

        // quadratic case
        if (i == 3) {
            printf("        Found final two roots through Quadratic Formula\n");
            double complex *temp = quadraticRoots(poly);
            root[0] = temp[0];
            root[1] = temp[1];
            free(temp);
            temp = NULL;
            return root;

        // case where we need laguerres
        } else {
            root[i-2] = laguerre(poly, tolerance, verb);
            
            polynomial *ptemp;
            ptemp = deflPoly(poly, root[i-2]);

            free(poly->polyCoef);
            poly->polyCoef = NULL;

            poly->nterms -= 1;
            poly->polyCoef = ptemp->polyCoef;
            free(ptemp);


            if (verb) {
                printf("        Deflated: P(x) = ");
                printPoly(poly);
            }
        } // if ... else
    }
    return root;
}

/*---------------------------------------------------------------------------
  This function evaluates the polynomial derivatives at a point using the
  compact method or another equivalent method.
     
  If you decide not use the compact all-in-one p,p',p'' evaluation code, 
  you can implement this function:
        As separate p, p' and p'' derivatives
        Then evaluate each function separately
        Still returning 3 values
  OR
  You can choose to create functions of your own and not implement this function
  at all
  
  Where: polynomial *p        - Pointer to a polynomial data to evaluate
         double complex point - The point to evaluate
  Returns: double complex*    - Pointer to an array of evaluated 
                                derivatives d, d' and d'' 
  Errors:  prints an error and exits
---------------------------------------------------------------------------*/
static double complex* evalDerivs(polynomial *p, double complex point){

    // initialize output structure
    double complex *out = malloc(sizeof(double complex) * 3); ;

    // create polynomials
    polynomial *pf = malloc(sizeof(polynomial));
    polynomial *pdf = malloc(sizeof(polynomial));
    polynomial *pddf = malloc(sizeof(polynomial));

    // initialize polynomials
    initPoly(pf, p->nterms);
    initPoly(pdf, p->nterms-1);
    initPoly(pddf, p->nterms-2);

    // setup f
    for (int i = 0; i < p->nterms; i++) {
        pf->polyCoef[i] = p->polyCoef[i];}

    // take derivative
    for (int i=1; i < p->nterms; i++) {
        pdf->polyCoef[i-1] = (p->nterms - i)*pf->polyCoef[i-1];}

    // take second derivative
    for (int i=1; i < p->nterms-1; i++) {
        pddf->polyCoef[i-1] = (p->nterms - 1 - i) *pdf->polyCoef[i-1];}

    // calculate values of derivatives at given point
    out[0] = cevalPoly(pf, point); 
    out[1] = cevalPoly(pdf, point); 
    out[2] = cevalPoly(pddf, point); 

    // free polynomial pointers and array pointers
    freePoly(pddf);
    freePoly(pdf);
    freePoly(pf);

    return out;
} 

/*---------------------------------------------------------------------------
  Returns the two roots from a quadratic polynomial
  
  Where: polynomial *p - Pointer to a 2nd order polynomial
  Returns: double complex* - A pointer to a complex root pair.
  Errors:  prints an error and exits
---------------------------------------------------------------------------*/
static double complex* quadraticRoots( polynomial *p){
    double complex *out = (double complex *)malloc(sizeof(double complex) *2);
    double complex a = p->polyCoef[0];
    double complex b = p->polyCoef[1];
    double complex c = p->polyCoef[2];
    double complex four = 4.0 + 0.0*I;
    out[0] = (-(b) + csqrt( (b * b)  - (four * a * c))) / (2.0 * a);
    out[1] = (-(b) - csqrt( (b * b)  - (four * a * c))) / (2.0 * a);
    return out;
} 


/*---------------------------------------------------------------------------
  Uses Laguerre's method to compute a root of a real polynomial
  
  Where: polynomial *p - Pointer to a polynomial structure to evaluate
         double tol    - Termination tolerance 
         int verb      - Verbose flag indicating whether to print intermediate
                         results or not ( 1 = TRUE, 0 = FALSE 
 Returns: double complex - The complex root or (NAN + 0.0*I) if  none is found
 Errors:  none
---------------------------------------------------------------------------*/
static double complex laguerre(polynomial *p, double tol, int verb){
    double complex x = 0;
    double complex *dereval; 
    double complex nn = p->nterms-1;
    double complex gx = 0;
    double complex hx = 0;
    double complex cf = 0; // correction factor
    double complex d1 = 0, d2 = 0; // denominator variables

    if (verb) {
        printf("    Laguerre's Alrogithm( tol = %g )\n", tol); 
    }

    int maxitt = 10000;
    for (int i = 0; i < maxitt; i++) {
        dereval = evalDerivs(p, x);

        // make sure we don't divide by zero
        if (cabs(dereval[0]) < ZERO) {
            if (verb) {
                printf("        Found root at ");
                printComplex(x);}
            free(dereval);
            return x;
        }

        // bulk of laguerres calculations
        gx = dereval[1] / dereval[0];
        hx = (gx*gx) - (dereval[2] / dereval[0]);
        free(dereval);
        dereval = NULL;
        d1 = gx + csqrt( (nn - 1) * ((nn * hx) - (gx * gx)) );
        d2 = gx - csqrt( (nn - 1) * ((nn * hx) - (gx * gx)) );
        if (cabs(d1) > cabs(d2)) {
            cf = nn / d1;
        } else {
            cf = nn / d2;
        }

        // friendly output
        if (verb) {
            printf("        it: %d, x: %g + %gI\n", i, creal(x), cimag(x));
            printf("            G(x): "); printComplex(gx);
            printf("            H(x): "); printComplex(hx);
            printf("            aplha: "); printComplex(cf);
            printf("\n");
        }
        
        // condition where we can exit safely
        if (cabs(cf) < ZERO) {
            if (verb) {
                printf("        Found root at ");
                printComplex(x);}
            return x;
        
        // continue looking for a root
        } else {
            x -= cf;
        }
    }
    fprintf(stderr, "Laguerres finished without reaching a conclusion\n");
    return(NAN);
}

/*---------------------------------------------------------------------------
  Deflates a root from the polynomial, returning the new polynomial 
  
  Where: polynomial *p       - Pointer to the polynomial to deflate
         double complex root - The root to use 
  Returns: polynomial *      - Pointer to the deflated polynomial
  Errors:  none
---------------------------------------------------------------------------*/
static polynomial* deflPoly(polynomial *p, double complex root){
    polynomial *out = malloc(sizeof(polynomial));
    initPoly(out, p->nterms);

    double complex temp = 0;
    for (int i = 0; i < p->nterms; i++) {
        if (i == 0) {
            out->polyCoef[i] = p->polyCoef[i];
        } else {
            temp = out->polyCoef[i-1] * root;
            out->polyCoef[i] = p->polyCoef[i] + temp;
        }
    }

    return out;
} 


/*---------------------------------------------------------------------------
  The function prints out complex data
  
  Where: double complex x - the complex data to print out
  returns:  nothing
  errors:   none
---------------------------------------------------------------------------*/
static void printComplex(double complex x){
    printf("%6.4f%+6.4fi\n", creal(x), cimag(x)); 
} 

/*---------------------------------------------------------------------------
  Prints a polynomial
  Where: polynomial *p - Pointer to polynomial data structure to print
  Errors: none
  Returns: nothing
---------------------------------------------------------------------------*/
void printPoly (polynomial *p){
    for (int i = 0; i < p->nterms; i++) {
        printf("%6.4fx^%d, ", creal(p->polyCoef[i]), p->nterms-i-1); 
    }
    printf("\n"); 
} 

/*---------------------------------------------------------------------------
  Prints the roots of a polynomial from largest (in magnitude) to smallest
  
  Where: polynomial *p - Pointer to polynomial data structure to print
  Returns: nothing
  Errors:  none
---------------------------------------------------------------------------*/
void printRoots (polynomial *p, double tolerance, int verb){
    printf("Roots:\n");

    // sort array
    for (int i = 0; i < p->nterms; i++){
        for (int j = i+1; j < p->nterms; j++) {
            if ( cabs(p->polyCoef[i]) > cabs(p->polyCoef[j]) ) {
                double complex temp = p->polyCoef[i];
                p->polyCoef[i] = p->polyCoef[j];
                p->polyCoef[j] = temp;
            }
        }
    }

    for (int i = p->nterms-1; i >= 0; i--){
        printf("        ");
        printComplex(p->polyCoef[i]);
    }
}

