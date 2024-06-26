/*
 *  This file is automatically generated and does not require a license
 *
 *  ==== WARNING: CHANGES TO THIS GENERATED FILE WILL BE OVERWRITTEN ====
 *
 *  To make changes to the generated code, use the space between existing
 *      "USER CODE START (section: <name>)"
 *  and
 *      "USER CODE END (section: <name>)"
 *  comments, where <name> is a single word identifying the section.
 *  Only these sections will be preserved.
 *
 *  Do not move these sections within this file or change the START and
 *  END comments in any way.
 *  ==== ALL OTHER CHANGES WILL BE OVERWRITTEN WHEN IT IS REGENERATED ====
 *
 *  This file was generated from
 *      C:/ti/grace_2_20_01_12/packages/ti/mcu/msp430/csl/interrupt_vectors/InterruptVectors_init.xdt
 */
#include <msp430.h>

/* USER CODE START (section: InterruptVectors_init_c_prologue) */
/* User defined includes, defines, global variables and functions */
/* USER CODE END (section: InterruptVectors_init_c_prologue) */

/*
 *  ======== InterruptVectors_graceInit ========
 */
void InterruptVectors_graceInit(void)
{
}


/* Interrupt Function Prototypes */



/*
 *  ======== PORT2 Interrupt Service Routine ========
 *
 * Here are several important notes on using PORTx interrupt Handler:
 *
 * 1. User must explicitly clear the port interrupt flag before exiting
 *
 *    PxIFG &= ~(BITy);
 *
 * 2. User could also exit from low power mode and continue with main
 *    program execution by using the following instruction before exiting
 *    this interrupt handler.
 *
 *    __bic_SR_register_on_exit(LPMx_bits);
 *
 */
#pragma vector=PORT2_VECTOR
__interrupt void PORT2_ISR_HOOK(void)
{
    /* USER CODE START (section: PORT2_ISR_HOOK) */
	P2IE &= ~BIT4;//desabillitar int. porta 2
	P2IFG &= ~BIT4;

	if (!(P2IES & BIT4))
	P2OUT ^= BIT5;//e=1

	TA0CTL  |= MC_1 | TAIE;//imer ligado e interrupção habilitadatimerA0/* replace this comment with your code */

	/* USER CODE END (section: PORT2_ISR_HOOK) */
}

/*
 *  ======== Timer0_A3 Interrupt Service Routine ======== 
 */
#pragma vector=TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR_HOOK(void)
{
    /* USER CODE START (section: TIMER0_A1_ISR_HOOK) */
    TA0CTL &= ~TAIE;//desabilita interrup. timerA0/* replace this comment with your code */
    TA0CTL &= ~(MC1+MC0);//timer parado
    //TA0CTL |=TACLR;

    P2IES ^= BIT4;//configura para a borda oposta
    P2IFG &= ~BIT4;

    P2IE |= BIT4;//habillitar int. porta 2

    //P2OUT ^= BIT5;//e=1
	TA0CTL &= ~TAIFG;

    /* USER CODE END (section: TIMER0_A1_ISR_HOOK) */
}
