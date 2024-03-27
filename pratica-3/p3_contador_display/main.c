#include <msp430.h>
#include <ti/mcu/msp430/Grace.h>


const char vet[] =
		{ 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F };
int indiceParaIncrementar = 0; //variálvel global, para incrementar
int main(void) {
	// ativar o WDT
	WDTCTL = WDTPW;
	// Configurar P2 como saída para os segmentos do display
	P2DIR |= BIT0 + BIT1 + BIT2 + BIT3 + BIT4 + BIT5 + BIT6 + BIT7;

	// Inicializar configurações do Grace
	Grace_init();
	// Configurar o Timer A0 no modo de contagem ascendente (Up Mode)
	P3DIR |= BIT0 + BIT1 + BIT2 + BIT3 + BIT4 + BIT5 + BIT6 + BIT7;
	TA0CTL |= MC_1;

	while (1) {
		WDTCTL = WDTPW + WDTCNTCL; // reseta o WDT
		if (TA0CTL & TAIFG) {
			TA0CTL &= ~TAIFG;
			indiceParaIncrementar++;
		}
		if (indiceParaIncrementar > 9) {
			indiceParaIncrementar = 0; // Reiniciar o contador quando atingir 9
		}
		P3OUT = vet[indiceParaIncrementar];
		// Atualizar os segmentos do display usando a Porta 1
	}
}
