#include <msp430.h>
#include "switches.h"

char switches = 0;

// update interrupt sense for edge detection
static char switch_update_interrupt_sense() {
  char p2val = P2IN;
  P2IES |= (p2val & SWITCHES);   // high-to-low edge
  P2IES &= (p2val | ~SWITCHES);  // low-to-high edge
  return p2val;
}

// initialize the switches
void switch_init() {
  P2REN |= SWITCHES;   // enable resistors
  P2IE  |= SWITCHES;   // enable interrupts
  P2OUT |= SWITCHES;   // pull-ups
  P2DIR &= ~SWITCHES;  // set as input
  switch_update_interrupt_sense();
}

// handle switch interrupt
void switch_interrupt_handler() {
  char p2val = switch_update_interrupt_sense();
  switches = ~p2val & SWITCHES;  // store which switches are pressed
}

