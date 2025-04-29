#ifndef led_included
#define led_included
#include <msp430.h>

#define LED_RED BIT6
#define LED_GREEN BIT0
#define LEDS (BIT0 | BIT6)

void led_init();

#endif
