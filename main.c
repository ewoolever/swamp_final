#include <msp430.h>

#define Duty_Cycle 32768; // this value is 0.5 second
#define Delay_Time 260; // this is how long we'll idle in 0.5 seconds
int delay;

void Setup_GPIO(void);
unsigned int i;

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    Setup_GPIO();
    Setup_Timers();
    __enable_interrupt();
    delay = 0;

    while(1){
        if(delay >= 260){
            P2OUT |= BIT7;
            P1OUT |= BIT6;
            TB1CTL |= MC__STOP;
        }
    }
}

void Setup_GPIO(void){
    //Setup LEDS
    P1DIR |= BIT6;
    P1OUT |= BIT6;

    //Setup Relay pin
    P2DIR |= BIT7;
    P2OUT &= ~BIT7;

    //Setup input Switches
    P2DIR &= ~BIT0;
    P2IFG &= ~BIT0;
    P2IE |= BIT0;

    //Enable IO (take out of low power mode)
    PM5CTL0 &= ~LOCKLPM5;
}

void Setup_Timers(void){
    TB1CTL |= TBCLR;
    TB1CTL |= TBSSEL__ACLK;
    TB1CTL |= MC__STOP;
    TB1CCR0 |= Duty_Cycle;               //Set to .5 seconds interrupt to trigger.....

    TB1CCTL0 |= CCIE;
    TB1CCTL0 &= ~CCIFG;
}

#pragma vector = PORT2_VECTOR
__interrupt void ISR_PORT2_Switches(void){
    for(i=0; i<10000;i++);
    for(i=0; i<10000;i++);

    P2IFG &= ~BIT0;
    P1OUT ^= BIT6;
    TB1CTL |= MC__UP;
}

#pragma vector = TIMER1_B0_VECTOR
__interrupt void Pump_Timer(void)
{
    TB0CCTL0 &= ~CCIFG;
    delay++;

}
