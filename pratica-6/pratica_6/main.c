#include <msp430.h>

//atrazar um sinal em 180, utilizando o módulo ccp

#include <ti/mcu/msp430/Grace.h>

char pri_cap = 1;
unsigned int T, ti, tar_ant;

int main(void) {
	Grace_init();
	P1OUT = ~(BIT2);// Activate Grace-generated configuration
	tar_ant = TA0R;
	TACCR1 = TACCR0 + (TACCR0 / 2);  // Compensa 180 graus
	while (1) {
		if((unsigned int)(TA0R - tar_ant)>60000){
			tar_ant = TA0R;
			P1OUT ^= BIT2;
		}
		if (TA0CCTL0 & CCIFG) {
			TA0CCTL0 &= ~CCIFG;//aguarda a captura
			if (pri_cap) {//se for a primeira captura
				pri_cap = 0;//dizer que meu ti é 0
				ti = TA0CCR0;          //salvo o tempo inicial na primeira borda
			} else {
				T = TA0CCR0 - ti;
				ti = TA0CCR0;       //salvo o tempo inicial para a proxima borda
				P1OUT ^= BIT7;
			}
		}

	}
}
