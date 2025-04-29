#include <msp430.h>
#include "buzzer.h"
#include "led.h"
#include "snake.h"
#include "game.h"


// WARNING: LCD DISPLAY USES P1.0.  Do not touch!!!

#define LED BIT6       /* note that bit zero required for display */

#define SW1 1 //left
#define SW2 2 //down
#define SW3 4 //right
#define SW4 8 //up
#define SWITCHES 15

extern unsigned int grow_snake(unsigned int length);

int switches = 0;
extern int redrawScreen;
extern char gameover;

void switch_init();
void switch_interrupt_handler();
static char switch_update_interrupt_sense();
void wdt_c_handler();


static char switch_update_interrupt_sense(){
  char p2val = P2IN;
  P2IES |= (p2val & SWITCHES);
  P2IES &= (p2val | ~SWITCHES);
  return p2val;
}

void switch_init(){
  P2REN |= SWITCHES;
  P2IE |= SWITCHES;
  P2OUT |= SWITCHES;
  P2DIR &= ~SWITCHES;
  switch_update_interrupt_sense();
}


void switch_interrupt_handler(){
  char p2val = switch_update_interrupt_sense();
  switches = ~p2val & SWITCHES;
}




//control mechanics
void wdt_c_handler(){
  static int moveCounter =0;
  static unsigned int randomSeed =0;

  
  moveCounter++;
  if(moveCounter<5){
    return;
  }
  moveCounter =0;

  if(gameover){
    if(switches & SW2){
      snake_init();
      game_init();
      led_off();
      buzzer_set_period(0);
    }
    return;
  }
  
  if(switches & SW1){ //left
    colVelocity = -1;
    rowVelocity = 0;
  }
  if(switches & SW2){ //down
    colVelocity = 0;
    rowVelocity =1;
  }
  if (switches & SW4){ //right
    colVelocity =1;
    rowVelocity = 0;
  }
  if(switches & SW3){ //up
    colVelocity =0;
    rowVelocity = -1;
  }
}

void main(){
  
  configureClocks();
  lcd_init();
  switch_init();
  led_init();
  buzzer_init();
  snake_init();
  game_init();

 enableWDTInterrupts();
 or_sr(0x8);

 clearScreen(COLOR_BLACK);
 while(1){
   if(redrawScreen) {
    redrawScreen = 0;
    update_shape();
  }
  P1OUT &= ~BIT6;
  or_sr(0x10);
  P1OUT |= BIT6;
 }
}

void __interrupt_vec(PORT2_VECTOR) Port_2(){
  if(P2IFG & SWITCHES){
    P2IFG &= ~SWITCHES;
    switch_interrupt_handler();
  }
}
