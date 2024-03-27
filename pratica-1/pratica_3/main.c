#include <msp430.h>

/*
 * ======== Grace related includes ========
 */
#include <ti/mcu/msp430/Grace.h>

/*
 *  ======== main ========
 */
int main(void) {
	Grace_init();
	// Activate Grace-generated configuration
	P2OUT = 0;
	while (1) {
		if (P2IN & BIT0) {
			P2OUT |= BIT1;
		} else {
			P2OUT &= ~(BIT1);
		}

		if (TA0CTL & TAIFG) {
			TA0CTL &= ~TAIFG;
		    P2OUT ^= BIT2;  // Limpa o bit 2
		}

	}
}
