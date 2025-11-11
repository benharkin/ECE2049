#include "delay.h"

void swDelay(int numLoops)
{
    volatile unsigned int i, j;  // volatile to prevent removal in optimization
                                 // by compiler. Functionally this is useless code

    for (j = 0; j < numLoops; j++)
    {
        i = 1000;                 // SW Delay
        while (i > 0)   {            // could also have used while (i)
            i--;
            if (buttonsPressed()){
                return;
            }
        }
    }

}
