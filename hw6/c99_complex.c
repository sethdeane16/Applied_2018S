/**************************************************************
  Example of C99 complex module usage

  Revised: Juan C. Cockburn - Adapted to C99
  Last Modified: 9/4/2014
  Compile with: gcc .... -std=c99
***************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include </usr/include/complex.h> /* note explicit path */

/*************************************************************/
int main ( void ) {
/* double precision complex numbers */
 double complex z,a;     /* complex numbers */
 double complex b[3];    /* complex vector */
 double complex c[2][2]; /* complex matrix */
 int i,j;   /* temporary counter variables */

 printf ( "\n" );
 printf ( "C99 complex module: declarations and assignments\n" );

/* Note that capital i denotes the imaginary unit */
  a = 1.0 + 2.0*I;

  b[0] = 1.0 + 2.0*I;
  b[1] = 3.0 + 4.0*I;
  b[2] = 5.0 + 6.0*I;
  
  c[0][0] = 1.0 + 0.1*I;
  c[0][1] = 1.0 + 0.2*I;
  c[1][0] = 2.0 + 0.1*I;
  c[1][1] = 2.0 + 0.2*I;

/* output results to stdio */
  printf ( "\n  Scalar a:\n" );
  printf ( "  % g %+gi\n", creal(a), cimag(a));

  printf ( "\n  Vector v:\n" );
  for (i = 0;i < 3;i++ )  {
    printf ( " [ % g %+gi ] \n", creal(b[i]), cimag(b[i]) );
  }

  printf ( "\n  Matrix c:\n" );
  for (i = 0;i < 2;i++ )  {
    printf ( " [" );
    for ( j = 0; j < 2; j++ )  {
      printf ( "  %g %+gi ", creal(c[i][j]), cimag(c[i][j]) );
    }
    printf ( "]\n" );
  }
  
  printf ( "\n" );

  printf ("** Working with Complex Number:\n" );    
  printf ("       a = % g %+gi\n", creal(a), cimag(a));
  printf (" cabs(a) = % g\n",cabs(a) );
  printf (" carg(a) = % g\n",carg(a) );             /* the angle */

  printf (" creal(a) = % g\n",creal(a) );
  printf (" cimag(a) = % g\n",cimag(a) );

  z=conj(a);                                        /* complex conjugates */
  printf (" conj(a) = % g %+gi\n",creal(z),cimag(z) );
  
  printf ( "** Complex Arithmetic:\n" );      
  printf ("  a      = % g %+gi\n",creal(a),cimag(a) );
  
  z = a + 3.0;
  printf ("  a + 3  = % g %+gi\n",creal(z),cimag(z) );
  
  z = a + 5.0*I;
  printf ("  a + 5i = % g %+gi\n",creal(z),cimag(z) );
  
  z = a * 4.0;
  printf ("  4 * a  = % g %+gi\n",creal(z),cimag(z) );
  
  z= a / 8.0;
  printf ("  a / 8  = % g %+gi\n",creal(z),cimag(z) );
  
  z= a * a;
  printf ("  a * a  = % g %+gi\n",creal(z),cimag(z) );
  
  z= 1.0 / a;
  printf ("  1/a    = % g %+gi\n",creal(z),cimag(z) );

  printf ( "** Complex Math Functions:\n" );      
  z=cpow(a,3);                                      /* power, not accurate */
  printf ("     a^3 = % g %+gi\n",creal(z),cimag(z) );
  
  z=csqrt(a);
  printf (" sqrt(a) = % g %+gi\n",creal(z),cimag(z) );

  return 0;
}
