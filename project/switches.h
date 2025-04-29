#ifndef switches_include
#define switches_include


#define SW1 BIT0 //button s1
#define SW2 BIT1 //s2
#define SW3 BIT2 //s3
#define SW4 BIT3 //s4
#define SWITCHES (SW1|SW2|SW3|SW4)



extern char switches;



void switch_init();
void switch_interrupt_handler();





#endif
