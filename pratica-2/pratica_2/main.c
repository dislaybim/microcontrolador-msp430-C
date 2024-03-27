#include <msp430.h>
#include <ti/mcu/msp430/Grace.h>

#define LINHAS 4
#define COLUNAS 4

char keys[LINHAS][COLUNAS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};
enum EstadoTeclaAsterisco {
    AGUARDANDO,
    PISCAR_TUDO
};

enum EstadoTeclaAsterisco estadoAsterisco = AGUARDANDO;
//------------------------------funções de configução----------------------------------------------------------
void liga1() {

    P1OUT |= BIT4;  // Acende o LED em P1.4
}

void liga2() {

    P1OUT |= BIT5;  // Acende o LED em P1.5
}

void liga3() {

    P1OUT |= BIT6;  // Acende o LED em P1.6
}

void liga4() {

    P1OUT |= BIT7;  // Acende o LED em P1.7
}

void liga5() {

    P1OUT |= BIT2;  // Acende o LED em P2.0
}

void liga6() {

    P1OUT |= BIT3;  // Acende o LED em P2.1
}

void ligaTudo() {
	P1OUT |= BIT4 + BIT5 + BIT6 + BIT7; // Acende tudo
	P1OUT |= BIT3 + BIT2;
}

void apagaTudo() {
	P1OUT &= ~(BIT4 + BIT5 + BIT6 + BIT7); // Apaga tudo
	P1OUT &= ~(BIT2 + BIT3);
}

void piscaTudo(){
	if (TA0CTL & TAIFG){
		TA0CTL &= ~TAIFG;
		P1OUT ^= BIT4 + BIT5 + BIT6 + BIT7; // Pisca tudo
		P1OUT ^= BIT2 + BIT3;
	}
}

void apaga1(){
	P1OUT &= ~(BIT4); // Apaga o LED em P1.4
}

void apaga2(){
	P1OUT &= ~(BIT5); // Apaga o LED em P1.5
}

void apaga3(){
	P1OUT &= ~(BIT6); // Apaga o LED em P1.6
}

void apaga4(){
	P1OUT &= ~(BIT7); // Apaga o LED em P1.7
}

void apaga5(){
	P2OUT &= ~(BIT0); // Apaga o LED em P2.0
}

void apaga6(){
	P2OUT &= ~(BIT1); // Apaga o LED em P2.1
}

void piscaLEDs() {
    P1OUT ^= BIT4 + BIT5 + BIT6 + BIT7; // Pisca LEDs em P1
    P2OUT ^= BIT0 + BIT1;               // Pisca LEDs em P2
}
//------------------------------função principal-----------------------------------------------------
int main(void) {
	int piscando = 0;
	int i, j;
    Grace_init();

    while (1) {

        // Verifica cada linha
        for (i = 0; i < LINHAS; i++) {
            P3OUT &= ~(1 << i);  // Coloca a linha atual em 0


            // Verifica cada coluna na linha atual
            for (j = 0; j < COLUNAS; j++) {
                if (!(P2IN & (1 << (j + LINHAS)))) {
                	// Se a tecla está pressionada
                    switch (keys[i][j]) {
                        case '1':
                            liga1();
                            break;
                        case '2':
                            liga2();
                            break;
                        case '3':
                            liga3();
                            break;
                        case '4':
                        	liga4();
                        	break;
                        case '5':
                            liga5();
                            break;
                        case '6':
                            liga6();
                            break;
                        case 'A':
                            ligaTudo();
                            break;
                        case 'B':
                            apagaTudo();
                            break;
                        case 'C':
						if (estadoAsterisco == AGUARDANDO) {
							estadoAsterisco = PISCAR_TUDO;
							piscando = 1;
						} else {
							estadoAsterisco = AGUARDANDO;
							piscando = 0;
							apagaTudo();
						}
                            break;

                        // Adicione casos para outras teclas conforme necessário
                        default:
                            break;
                    }
                }
            }

            P3OUT |= (1 << i);  // Coloca a linha de volta em 1
        }
        if (piscando) {
                   switch (estadoAsterisco) {
                       case PISCAR_TUDO:
                    	   piscaTudo();
                           break;
                       // Adicione casos para outros estados conforme necessário
                   }
               }
        __delay_cycles(1000);  // Aguarda um curto período para evitar ruídos
    }
}
