#include <msp430.h>

#include <ti/mcu/msp430/Grace.h>
unsigned int ti, tempo, tempo_w;
char m;
char rv;
//p2.5 A
//p2.6 B
//p2.7 C

int main(void) {
	Grace_init();
	WDTCTL = WDTPW;
	TA0CCTL1 |= CCIS0; //saída do comparador
	rv = 1;
	P2OUT = 0; //deixar o capacitor inicialmente descarregado

	do {
		if (rv) {
			P2OUT |= BIT6; //COLOCANDO APENAS B EM ALTO => RV
			P2OUT &= ~(BIT5); //COLOCANDO APENAS B EM ALTO => RV
		} else {
			P2OUT |= BIT5; //COLOCANDO APENAS A EM ALTO =>
			P2OUT &= ~BIT6;
		}
		P2OUT |= BIT5; //CARREGA O CAPACITOR
		TA0R = 0; //armazena a contagem

		if (TA0R > 1000) {
			P2OUT &= ~(BIT5); //DESCARREGA O CAPACITOR
			if (TA0CCTL1 & CCIFG) { //verificar quando o valor do comparador é 1
				if (rv) {
					tempo = TA0CCR1 - ti;
				} else {

					tempo_w = TA0CCR1 - ti;
				}
				if (rv) {
					P3OUT = tempo_w / tempo;
				}
			}

		}

		WDTCTL = WDTPW + WDTCNTCL;
	} while (1);

}
//m=tempo_w / tempo;
