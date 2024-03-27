/*
 * ======== Standard MSP430 includes ========
 */
#include <msp430.h>

#include <ti/mcu/msp430/Grace.h>
#define LED BIT5

int main(void)
{
    Grace_init();
    while(1){// Activate Grace-generated configuration
    	LPM3;//entra em baixo consumo de energia

    }

}
