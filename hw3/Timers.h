/*************************************************************************
* Timers.h - Instrumentation Macros Module  - Student file
*
* The timing instrumentation macros can be globally enabled by setting 
* the variable EN_TIME (ENable TIMErs) in the compiler command line, e.g., 
* use "gcc -DEN_TIME...." to enable the timers when needed.
* A compiler warning will be issued when the timers are enabled.  
* Note: All timing output should be directed to stderr.
*
*  10/08/2016   R. Repka    Fixed C++ struct warnings 
*  12/03/2016   R. Repka    Added semicolon to DECLARE_TIMER
**************************************************************************/
#ifndef _TIMERS_H_
#define _TIMERS_H_


#if defined(EN_TIME)
    #include <stdio.h>
    #include <time.h>
    #if defined(WARNING_MSG)
        #warning Timers enabled! Execution could be adversly effected.
    #endif
#endif


#if defined(EN_TIME)
    struct timmerDetails {                                                    
        clock_t Start;    /* Start Time   - set when the timer is started */
        clock_t Stop;     /* Stop Time    - set when the timer is stopped */
        clock_t Elapsed;  /* Elapsed Time - Accumulated when the timer is stopped */
        int State;        /* Timer State  - 0=stopped / 1=running */
    }; /* Elapsed Time and State must be initialized to zero */


    #define DECLARE_TIMER(A)                                            \
        struct timmerDetails                                            \
        A = /* Elapsed Time and State must be initialized to zero */    \
        {                                                               \
        /* Start   = */ 0,                                              \
        /* Stop    = */ 0,                                              \
        /* Elapsed = */ 0,                                              \
        /* State   = */ 0,                                              \
        }; /* Timer has been declared and defined,   ;  IS required */

          
    #define START_TIMER(A) {                                            \
        if (1 == A.State) {                                             \
            fprintf(stderr, "Error, running timer "#A" started.\n");    \
        } /* end if */                                                  \
        A.State = 1;                                                    \
        A.Start = clock();                                              \
    } /* START_TIMER() */
    

    #define RESET_TIMER(A) {                                            \
        A.Elapsed = 0;                                                  \
    } /* RESET_TIMER */
    

    #define STOP_TIMER(A) {                                             \
        A.Stop = clock();                                               \
                                                                        \
        if (0 == A.State) {                                             \
            fprintf(stderr, "Error, stopped timer "#A" stopped again.\n");\
        } /* end if */                                                  \
        else {                                                          \
            A.Elapsed += A.Stop - A.Start;                              \
        } /* end if */                                                  \
        A.State = 0;                                                    \
    } /* STOP_TIMER */
    

    #define PRINT_TIMER(A) {                                            \
        if (1 == A.State) {                                             \
            STOP_TIMER(A);                                              \
        } /* end if */                                                  \
        fprintf(stderr, "Elapsed CPU time ("#A") = %g sec. \n",         \
                (double)A.Elapsed / (double)CLOCKS_PER_SEC);            \
    } /* PRINT_TIMER */



    #define PRINT_RTIMER(A,R) {                                         \
        if (1 == A.State) {                                             \
            STOP_TIMER(A);                                              \
        } /* end if */                                                  \
        fprintf(stderr, "Elapsed CPU Time per Iteration ("#A", %d) = % .2e sec.\n",\
                 R, ((double)A.Elapsed / (double)CLOCKS_PER_SEC)/(double) R);                            \
    } /* PRINT_RTIMER */

    
    #define DECLARE_REPEAT_VAR(V)                                       \
        int _t_ ## V = 0;  


    #define BEGIN_REPEAT_TIMING(R,V)                                  \
        for (_t_ ## V ; _t_ ## V < R; _t_ ## V++) {                      \
    /* BEGIN_REPEAT_TIMING */


    #define END_REPEAT_TIMING                                           \
       } 
         


#else
    #define DECLARE_TIMER(A)
    #define START_TIMER(A)
    #define RESET_TIMER(A)
    #define STOP_TIMER(A)
    #define PRINT_TIMER(A)
    #define PRINT_RTIMER(A,R)

    #define DECLARE_REPEAT_VAR(V)
    #define BEGIN_REPEAT_TIMING(R,V)
    #define END_REPEAT_TIMING
#endif /* end if */

        

#endif /* _TIMERS_H_ */
