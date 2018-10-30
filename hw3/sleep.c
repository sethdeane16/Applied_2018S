/*---------------------------------------------------------------------------
  sleep.c - student file
  01/01/2016    R. Repka    This code is only used to verify your timing macros
  02/202016     R. Repka    Added include file hint
---------------------------------------------------------------------------*/
#include <unistd.h>
#include <stdio.h>
#include "Timers.h"
/* add other include files as necessary */
DECLARE_TIMER(Function_Timer)

int main()
{
    #ifdef EN_TIME
    clock_t end_t;
    int delay;
    int reps = 2;
    #endif


    printf("Start\n");
    /* Your macro stuff here */
    START_TIMER(Function_Timer);
    DECLARE_REPEAT_VAR(repeat_var);
    BEGIN_REPEAT_TIMING(reps, repeat_var);
    
    /* wait for 60 seconds */
    #ifdef EN_TIME
    end_t = clock() + 60 * CLOCKS_PER_SEC;
    while (end_t > clock())
        {
        /* Consume CPU time */
        delay = 1<<19;
        while (delay) 
            {
                delay--;
            }
        }
    #endif

   
    /* more of your macro stuff */   
    END_REPEAT_TIMING;
    STOP_TIMER(Function_Timer);
    PRINT_TIMER(Function_Timer);
    PRINT_RTIMER(Function_Timer, reps);
    
   
    printf("End\n");
    return 0;
}
