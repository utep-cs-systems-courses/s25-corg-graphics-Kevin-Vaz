#include <msp430.h>
#include "buzzer.h"
#include "led.h"
#include "snake.h"
#include "game.h"
#include "lcdutils.h"
#include "lcddraw.h"
#include "libTimer.h"

// WARNING: LCD DISPLAY USES P1.0.  Do not touch!!!

#define LED BIT6       /* note that bit zero required for display */

#define SW1 1 //left
#define SW2 2 //down
#define SW3 4 //right
#define SW4 8 //up
#define SWITCHES 15

extern unsigned int grow_snake(unsigned int length);
extern char switches;

extern int redrawScreen;
extern char gameover;
extern short foodX, foodY;
extern char foodExists;

void switch_init();
void switch_interrupt_handler();
static char switch_update_interrupt_sense();
void wdt_c_handler();

//control mechanics
void wdt_c_handler(){
  static int moveCounter = 0;
  moveCounter++;

  if(moveCounter <15) return;
  moveCounter = 0;
  
  if(gameover){
    redrawScreen = 1;
    return;
  }
  

  if (switches & SW1 && colVelocity != 1) { // left
    colVelocity = -1;
    rowVelocity = 0;
  }
  if (switches & SW2 && rowVelocity != -1) { // down
    colVelocity = 0;
    rowVelocity = 1;
  }
  if (switches & SW4 && colVelocity !=-1) { // right
    colVelocity = 1;
    rowVelocity = 0;
  }
  if (switches & SW3 && rowVelocity != 1) { // up
    colVelocity = 0;
    rowVelocity = -1;
  }
  
  /*move head
  controlPos[0] += colVelocity;
  controlPos[1] += rowVelocity;
  */
  move_snake();
  // Keep snake inside bounds
  if (controlPos[0] < 1) controlPos[0] = 1;
  if (controlPos[0] > screenWidth - 2) controlPos[0] = screenWidth - 2;
  if (controlPos[1] < 1) controlPos[1] = 1;
  if (controlPos[1] > screenHeight - 2) controlPos[1] = screenHeight - 2;

  //check food collision
  if(!gameover && foodExists &&
     controlPos[0] >= foodX - 2 && controlPos[0] <= foodX-2 &&
     controlPos[1] >= foodY - 2 && controlPos[1] <= foodY +2){

    snakeLength += 5;
    foodExists=0;

    for(int i = snakeLength -5; i < snakeLength; i++){
      snakeX[i] = snakeX[i-1];
      snakeY[i] = snakeY[i-1];
    }
  }
  
  //set redraw flag
  redrawScreen = 1;
}

int main(){
  
  configureClocks();
  lcd_init();
  clearScreen(COLOR_BLACK);
  switch_init();
  led_init();
  buzzer_init();
  snake_init();
  game_init();

  enableWDTInterrupts();
  or_sr(0x8);

 while(1){
   if(redrawScreen) {
    redrawScreen = 0;
    update_shape();
  }
  P1OUT &= ~BIT6;
  or_sr(0x10);
  P1OUT |= BIT6;
 }
 return 0;
}

void __interrupt_vec(PORT2_VECTOR) Port_2(){
  if(P2IFG & SWITCHES){
  P2IFG &= ~SWITCHES;
  switch_interrupt_handler();
  }
}
