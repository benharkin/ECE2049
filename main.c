#include <msp430.h>
#include "peripherals.h"

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;    // Stop watchdog timer
}
