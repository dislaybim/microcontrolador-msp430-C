#include <msp430.h>
#include <ti/mcu/msp430/Grace.h>

//ignore essa quantidade exagerada de variáveis, fiz ela por precaução.

volatile unsigned int segundos = 0; //cada estouro de contagem é 1s
volatile unsigned int unidade_segundos = 0;
volatile unsigned int dezena_segundo = 0;
volatile unsigned int minutos = 0; //cada 60 estouro de contagem é 1minuto
volatile unsigned int unidade_minuto = 0;
volatile unsigned int dezena_minuto = 0;
volatile unsigned int horas = 0; //cada 3600 estouros de contagem, uma hora.
volatile unsigned int unidade_hora = 0;
volatile unsigned int dezena_hora = 0;
//volatile unsigned int ponto_minuto = 0; //se estivermos controlando os minutos, o ponto vai piscar em 100ms
//volatile unsigned int ponto_hora = 0; //se estivermos controlando a hora, o ponto piscará em 500ms

//essas são as variáveis globais cruciais do funcionamento do programa
//primeiramente, será feito o incremento da contagem de segundos, minutos e horas
//depois, será feito a contagem da amostragem

const char vet_segundos[] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
		0x7F, 0x6F }; //vetor com os números.

//PINOS DAS UNIDADES P3OUT
//PINOS DAS DEZENAS P1OUT
//PARA FICAR MELHOR DE PERCORRER A MATRIZ
char passa = 0;
char anterior = 0;
int controle = 0;
int ruido = 0;
int ant = 0;
void main(void) {
	//P3OUT = 0x00;//inicia pinos em nivel lógico 0

	Grace_init();
	WDTCTL = WDTPW;
	// Configurar Timer0 para operar no modo de contagem ascendente
	TA0CTL |= MC_1; //ativa timer 0
	TA1CTL = TASSEL_2 + ID_3 + MC_2; // Timer1
	TA1CTL |= MC_1;//ativar o timer 1
	TA1R = 0;
	P2OUT &= ~BIT0;//controlar as dezenas do Display
	P2OUT &= ~BIT1;//controlar as unidades do Display
	P2OUT &= ~BIT4;//piscar o ponto

	while (1) {
		WDTCTL = WDTPW + WDTCNTCL;//desativa o WDT
		if (TA0CTL & TAIFG) {//verifica o estouro da flag, do controle do Timer0
			TA0CTL &= ~TAIFG; // Limpar o flag de estouro

			segundos++;//a cada estouro incrementa a variável segundos, e as demais variáveis
			unidade_segundos = segundos % 10;
			dezena_segundo = segundos / 10;
			unidade_minuto = minutos % 10;
			dezena_minuto = minutos / 10;
			unidade_hora = horas % 10;
			dezena_hora = horas / 10;

			if (segundos > 59) { //se passar de  60 segundos
				segundos = 0; //segundo volta a contar
				unidade_segundos = 0;
				dezena_segundo = 0;
				minutos++; // incrementa a variável minutos
				if (minutos >= 60) {//se passar 60 minutos
					minutos = 0;//zera a variável minutos
					unidade_minuto = 0;
					dezena_minuto = 0;
					horas++;//incrementa a variável hora
					if (horas >= 24) {//se passar de 24hrs
						horas = 0;//zera horas
						unidade_hora = 0;
						dezena_hora = 0;

					}
				}
			}

		}
		char botao = (P2IN & BIT2);//definir o p2.2 como entrada para incrementar a variavel segundos
		if ((P2IN & BIT2)== 0){//se apertar o botão, incrementa a variável segundos e cria um efeito cascata
			//ruido++;
			//controle++;
			//if (ruido == 500) {
			//	ruido = 1;
			//}
			while (TA1R <= 500) {
				//espera 4ms
			}
			TA1R = 0;
			segundos++;
			unidade_segundos = segundos % 10;
			dezena_segundo = segundos / 10;
			unidade_minuto = minutos % 10;
			dezena_minuto = minutos / 10;
			unidade_hora = horas % 10;
			dezena_hora = horas / 10;

			if (segundos > 59) { //coloca o ciclo de contagem em 60 segundos
				segundos = 0; //se ultrapassar 0
				unidade_segundos = 0;
				dezena_segundo = 0;
				minutos++;
				if (minutos >= 60) {
					minutos = 0;
					unidade_minuto = 0;
					dezena_minuto = 0;
					horas++;
					if (horas >= 24) {
						horas = 0;
						unidade_hora = 0;
						dezena_hora = 0;

					}
				}
			}

		}

		/*char botao2 = (P2IN & BIT3);//aqui é so a ideia de implementar um botão e para controlar o display
		if ((P2IN & BIT3)== 0){
			if (TA1CTL & TAIFG) {
				TA1CTL &= ~TAIFG;
				controle++;
				if (controle >= 2) {
					controle = 0;
				}

			}
			if (TA1CTL & TAIFG) {
			 if (TA1CTL != ant) {
			 if (botao2 == 0) {

			 TA1R = 0;
			 TA1CTL &= ~TAIFG;

			 }
			 }
			 ant = botao2;
			 controle++;
			 if (controle > 2) {
			 controle = 0;
			 }

			//ruido++;
			if (ruido > 500) {
			 botao2 = 1;
			 ruido = 0;
			 }

			//while ((P2IN & BIT3)== 0); // Aguardar até que o botão seja liberado
		char botao2 = (P2IN & BIT3);

		 if ((P2IN & BIT3) == 1) { // Alteração aqui
		 if (TA1CTL & TAIFG) {
		 if (TA1CTL != ant) {
		 if (botao2 == 0) { // Alteração aqui
		 TA1R = 0;
		 TA1CTL &= ~TAIFG;
		 }
		 }
		 ant = botao2;
		 controle++;
		 if (controle > 2) {
		 controle = 0;
		 }
		 }
		 }
	}*/
		if(TA0R == 2){//alternar o display entre segundos/minutos e horas a cada 2segundos
			controle++;
			if(controle > 2){
				controle = 0;
			}
		}



		switch (controle) {
		case 0:
			// Exibir segundos
			P2OUT &= ~BIT0; // Desativa o TBJ do dígito das dezenas
			P1OUT = vet_segundos[unidade_segundos];
			P2OUT |= BIT1; // Ativa o TBJ do dígito das unidades
			P2OUT &= ~BIT1; // Desativa o TBJ do dígito das unidades
			P1OUT = vet_segundos[dezena_segundo];
			P2OUT |= BIT0; // Ativa o TBJ do dígito das dezenas
			P2OUT &= ~BIT4;
			break;
		case 1:
			if (TA1CTL & TAIFG) {
				TA1CTL &= ~TAIFG;
				P2OUT ^= BIT4;
			}
			// Exibir minutos
			P2OUT &= ~BIT0; // Desativa o TBJ do dígito das dezenas
			P1OUT = vet_segundos[unidade_minuto];
			P2OUT |= BIT1; // Ativa o TBJ do dígito das unidades
			P2OUT &= ~BIT1; // Desativa o TBJ do dígito das unidades
			P1OUT = vet_segundos[dezena_minuto];
			P2OUT |= BIT0; // Ativa o TBJ do dígito das dezenas
			//while (TA1R == 500000) {
			//P2OUT |= BIT4;
			//}
			break;
		case 2:
			if (TA1CTL & TAIFG) {
				TA1CTL &= ~TAIFG;
				if (TA1R == 8) {
					P2OUT ^= BIT4;
				}
			}
			// Exibir horas
			P2OUT &= ~BIT0; // Desativa o TBJ do dígito das dezenas
			P1OUT = vet_segundos[unidade_hora];
			P2OUT |= BIT1; // Ativa o TBJ do dígito das unidades
			P2OUT &= ~BIT1; // Desativa o TBJ do dígito das unidades
			P1OUT = vet_segundos[dezena_hora];
			P2OUT |= BIT0; // Ativa o TBJ do dígito das dezenas
			break;
		default:
			break;
		}
	}
}
