//comunica��o modo SPI com o CI 74HC595.
//USCI A trabalha no modo spi quanto USART
//USCI B, trabalha tanto em modo spi quanto usart.
//UCOE
//UCxRXIFG      s�o flags  que indicam a transmiss�o de dados
#include <msp430.h>
#include <inttypes.h>

#include <ti/mcu/msp430/Grace.h>

int main(void) {
	Grace_init();                   // Activate Grace-generated configuration
	uint8_t contador = 0; //criamos uma vari�vel contador m�dulo 256 de contagem
	P2OUT &= ~BIT1; //zera o valor armazenado no 595 que possui flip-flops com entrada clear: flip-flops em cascatas
	P2OUT |= BIT1; //zera o valor armazenado no 595, pois o clr do flip-flop necessita do n�vel l�gico 1 para resetar.
	P2OUT &= ~BIT5;
	TA0CTL |= MC_1;
	while (1) { //la�o infinito
		if (TA0CTL & TAIFG) { //passou um segundo?
			P2OUT ^= BIT5; //comuta o estado l�gico do p2.5
			TA0CTL &= ~TAIFG; //zera a flag do timer0, para aguardar mais um segundo
			contador++; //incrmenta o contador
			UCB0TXBUF = contador; //atribui o valor do registrador de transmi��o ao valor do contador.
			while (!(IFG2 & UCB0RXIFG))
				; //aguarda transmitir o dado de 8 bits
			IFG2 &= ~UCB0RXIFG; //como  � SPI o correto � checar se 8 bits  foram recebidos
								//o qual indica quw o dado de 8 bits foi transmitido
								//chegando a flag de recep��o n�o precisar� de delay
			P2OUT |= BIT0; //seta o pino RCLpara que o dado seja guardado no 595
			P2OUT &= ~BIT0;

		}

	}

}
