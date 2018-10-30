/*---------------------------------------------------------------------------
  05/08/2017    R. Repka    Make derivative clearer
---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------
  Student framework equations to be solved to help with the root finder 
---------------------------------------------------------------------------*/
#include "resources/rootfinding.h"
#include <math.h>

/*---------------------------------------------------------------------------
  This function implements the equation to be solved.  
  
  Where: double x - the value to evaluate
  Returns: double - the value of the function at the point
  Errors:  none
---------------------------------------------------------------------------*/
double func1(double x){
    
    double one = (float)30 / (float)52;
    double two = (float)10 / (float)47;
    double y = .76 * x * sin(one * x) * tan(two * x) + 2.9 * cos(x + 2.5) * sin(0.39 * (1.5 + x));

    return y;
}


/*---------------------------------------------------------------------------
  This function implements the first derivative equation, which is calculated
  via any off-line process you wish (e.g  Wolfram Alpha)
  There is NO requirement to programmatically generate the derivative.
  
  Where: double x - the value to evaluate
  Returns: double - the value of the function at the point
  Errors:  none
---------------------------------------------------------------------------*/
double func1Deriv(double x){

    double two = (float)10 / (float)47;
    double thr = (float)15 / (float)26;
    double y = -2.9 * sin(x + 2.5) * sin(0.39 * (1.5 + x)) + 1.131 * cos(x + 2.5) * cos(0.39 * (1.5 + x)) + 0.76 * sin(thr * x) * sin(two * x) + 0.438462 * x * cos(thr * x) * tan (two * x) + 0.161702 * x * sin(thr * x) * pow((1/cos(two * x)), 2);
    return y;
}
