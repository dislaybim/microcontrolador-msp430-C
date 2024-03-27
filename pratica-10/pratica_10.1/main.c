#include <msp430.h>


#include <ti/mcu/msp430/Grace.h>
//comunicação I2C entre o MSP430 e o PCF8574
//USCI B
//Definir no modo mestre, pois eu quero que meu MSP430 controle o microcontrolador.
//vamos precisar de interrupção.
int main(void)
{
    Grace_init();
    TA0CCTL0 = CCIE;
    TA0CTL = TASSEL_2 + MC_2;
    
    while(1){
    	__bis_SR_register(CPUOFF + GIE);
    	UCB0CTL1 &= ~UCTR;
    	UCB0CTL1 |= UCTXSTT;
    	while(UCB0CTL1 & UCTXSTT);

    	UCB0CTL1 |= UCTR + UCTXSTT;
    	__bis_SR_register(CPUOFF + GIE);
    	while(UCB0CTL1 & UCTXSTT);
    	UCB0CTL1 |= UCTXSTP;
    }
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void TA0_ISR(void){
	__bic_SR_register_on_exit(CPUOFF);
}
