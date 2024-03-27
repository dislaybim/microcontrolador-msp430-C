#include "msp430g2553.h"           // Microcontroller specific header file
#include "hd44780.h"                    // HD44780 library
#include <ti/mcu/msp430/Grace.h>

unsigned int periodo = 0;
int captura = 0;
unsigned int primeira_borda;
unsigned int segunda_borda;
int tempo = 0;
int e = 0;
char display = 0;

void main(void) {
	Grace_init();
	WDTCTL = (WDTPW | WDTHOLD);                    // Stop watchdog timer
	//BCSCTL1 = CALBC1_1MHZ;                    // Set range to calibrated 1MHz
	//DCOCTL = CALDCO_1MHZ;                // Set DCO step and modulation for 1MHz
	P3DIR = 0xFF;		 // Set P1.0 (D0) to P1.7 (D7)
	P2DIR = (0x01 | 0x02 | 0x03 | 0x04);// Set P2.0 (E) and P2.1 (RS) to output
	P1OUT &= ~(BIT3); //led 1
	P1OUT &= ~(BIT4); //led 2
	P1OUT &= ~(BIT5); //led 3

	TA1CCR1 = 32768;		 // Set CCR1 value for 32.678 ms interrupt
	//TA0CCR0 = 3000;
	//TA0CCR1 = 1000;
	TA1CCTL1 = CCIE;		 // Compare interrupt enable
	TA1CTL = (TASSEL_2 | MC_2 | TACLR);		 //SMCLK, Continuous mode
	//TA1CCR2 = 30000;
	__bis_SR_register( GIE);

	hd44780_clear_screen();		 // Clear display content

	//TA0CCR2 += 66;		//33*600 us ~= 20000us ~= 20 ms
	//TA0CCTL2 &= ~CCIFG;
	while (1)		 // Endless loop - main program
	{
		//P1OUT |= BIT3;

		switch (e) {
		case 0:
			if (TA0CCTL1 & CCIFG) {		 //houve descA?
				TA0CCTL1 &= ~CCIFG;		 //TA0CCTL1 seta AJEITAR
				primeira_borda = TA0CCR1;
				captura = 1;

				e = 1;		 //passa para o proximo estado.
				TA0CCR2 += 66;		//33*600 us ~= 20000us ~= 20 ms
				TA0CCTL2 &= ~CCIFG;
			}

			if (TA0CCTL2 & CCIFG) {		 //passou o tempo de 20 ms
				e = 2;
			}
			break;
		case 1:
			if ((TA0CCTL0 & CCIFG)) { //condição lógica que verifica se esses dois eventos ja ocorreram
				TA0CCTL0 &= ~CCIFG;
				segunda_borda = TA0CCR0;  //armazena o valor da segunda captura
				//sinaliza que a minha segunda captura ocorreu
				captura = 0;  //habilita a captura do sinal 1.

				periodo = (segunda_borda - primeira_borda); //pega o período do meu sinal
				//coloquei essa operação dentro desse if, para assegurar que ela será feita
				//apos acontecer dois eventos
				//ALTERAR PARA ACIONAR OS LEDS
				if (periodo >= 25) {
					P1OUT |= BIT4;					//led2 acende
					P1OUT &= ~BIT5;					//apaga todos
					P1OUT &= ~BIT3;					//apaga todos
				} else if (periodo <= 6) {
					P1OUT |= BIT5;					//led3 acende
					P1OUT &= ~BIT4;					//apaga todos
					P1OUT &= ~BIT3;					//apaga todos
				} else {
					P1OUT &= ~BIT5;					//apaga todos
					P1OUT &= ~BIT4;					//apaga todos
					P1OUT &= ~BIT3;					//apaga todos
				}
				//
				e = 0;
				TA0CCR2 += 66;		//33*600 us ~= 20000us ~= 20 ms
				TA0CCTL2 &= ~CCIFG;		 //reseta
			}

			if (TA0CCTL2 & CCIFG) {		 //passou o tempo de 20 ms
				e = 0;
				TA0CCR2 += 66;		//33*600 us ~= 20000us ~= 20 ms
				TA0CCTL2 &= ~CCIFG;		 //reseta
				P1OUT &= ~BIT5;		 //apaga todos
				P1OUT &= ~BIT4;		 //apaga todos
				P1OUT &= ~BIT3;		 //apaga todos
			}
			break;
		case 2:
			if ((TA0CCTL0 & CCIFG)) { //condição lógica que verifica se esses dois eventos ja ocorreram
				TA0CCTL0 &= ~CCIFG;
				segunda_borda = TA0CCR0;  //armazena o valor da segunda captura
				//sinaliza que a minha segunda captura ocorreu
				captura = 0;  //habilita a captura do sinal 1.

				//periodo = (segunda_borda - primeira_borda); //pega o período do meu sinal
				//coloquei essa operação dentro desse if, para assegurar que ela será feita
				//apos acontecer dois eventos
				P1OUT |= BIT3;//ACENDE SOMENTE LED1     led dois, sinaliza que FASE B ta adiantado

				e = 0;
				TA0CCR2 += 66;		//33*600 us ~= 20000us ~= 20 ms
				TA0CCTL2 &= ~CCIFG;		 //reseta
			}
			//if (TA0CCTL1 & CCIFG) {
			//	e = 5;
			//}

			break;
		}

//periodo = 5940;
//periodo = TA0CCR1 - TA0CCR0;

		/*
		 if (TA0CCTL1 & CCIFG) { //verifica o estouro no canal de captura.
		 P2OUT ^= BIT3;
		 TA0CCTL1 &= ~CCIFG; //aguarda a captura
		 primeira_borda = TA0CCR1;  //salvo o tempo inicial na primeira borda
		 //led vai sinalizar que a captura ocorreu
		 captura = 1;  //Habilita a captura do sinal 2.
		 }

		 if ((captura) & (TA0CCTL0 & CCIFG)) { //condição lógica que verifica se esses dois eventos ja ocorreram
		 TA0CCTL0 &= ~CCIFG;
		 segunda_borda = TA0CCR0;  //armazena o valor da segunda captura
		 P2OUT ^= BIT4;  //sinaliza que a minha segunda captura ocorreu
		 captura = 0;  //habilita a captura do sinal 1.

		 periodo = (segunda_borda - primeira_borda); //pega o período do meu sinal
		 //coloquei essa operação dentro desse if, para assegurar que ela será feita
		 //apos acontecer dois eventos
		 }



		 */

		//tempo = (periodo * 43.7) / 65535;
//hd44780_write_string(" DISLAYBSON", 1, 1, NO_CR_LF );
		hd44780_write_string(" CAPTURA ", 1, 1, NO_CR_LF);
		hd44780_output_unsigned_16bit_value(captura, 1, 2, 5, 0);
//d44780_output_unsigned_16bit_value(segunda_borda, 2, 2, 5, 0);

	}
}

// Directive for timer interrupt
#pragma vector = TIMER1_A1_VECTOR
__interrupt void timer_1_a1_isr(void) // Timer 0 A1 interrupt service
{
	switch (TA1IV)                                 // Determine interrupt source
	{
	case 2:                                         // CCR1 caused the interrupt
	{
		hd44780_timer_isr();                       // Call HD44780 state machine
		break;                       // CCR1 interrupt handling done
	}
	}
}
