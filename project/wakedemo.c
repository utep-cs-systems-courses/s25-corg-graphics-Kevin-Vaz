#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"


// WARNING: LCD DISPLAY USES P1.0.  Do not touch!!!

#define LED BIT6       /* note that bit zero required for display */

#define SW1 1 //left
#define SW2 2 //down
#define SW3 4 //right
#define SW4 8 //up
#define SWITCHES 15

#define MAX_LENGTH 100
//snake body
short snakeX[MAX_LENGTH];
short snakeY[MAX_LENGTH];
short snakeLength = 5;

//food
short foodX[2] = {20,50};
short foodY[2] = {30, 50};
char foodExists[2] = {1,0};


// Snake setup
short drawPos[2] = {20, 20}; //starting position
short controlPos[2] = {20,20}; //new position

short colVelocity = 0; //initial movement speed
short rowVelocity = 0; //moving sideways

int switches = 0;

char blue = 31, green = 0, red = 31; //not needed for now

short redrawScreen = 1;

void draw_ball(int col, int row, unsigned short color){
  fillRectangle(col-1, row-1,3,3,color);
}


void screen_update_ball(){
  draw_ball(snakeX[snakeLength-1], snakeY[snakeLength-1], COLOR_BLACK);
  draw_ball(snakeX[0], snakeY[0], COLOR_WHITE); //DRAW NEW BALL
}


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


//draw the food
void draw_food(){
  for(int i=0; i<2;i++){
    if(foodExists[i]){
      fillRectangle(foodX[i]-1, foodY[i]-1,3,3, COLOR_RED);
    }
  }
}


void update_shape(){
  screen_update_ball();
  draw_food();
}

//control mechanics
void wdt_c_handler(){
  static int moveCounter =0;

  moveCounter++;
  if(moveCounter<5){
    return;
  }
  moveCounter =0;
    
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

  //move ball

  controlPos[0] += colVelocity;
  controlPos[1] += rowVelocity;

  //keep snake inside bounds
  if(controlPos[0] < 1) controlPos[0] = 1;
  if(controlPos[0] > screenWidth-2) controlPos[0] = screenWidth-2;
  if(controlPos[1] < 1) controlPos[1] = 1;
  if(controlPos[1] > screenHeight-2) controlPos[1] = screenHeight-2;

  redrawScreen =1;

  for(int i=snakeLength -1; i >0; i--){
    snakeX[i] = snakeX[i-1];
    snakeY[i] = snakeY[i-1];
      }
  snakeX[0] = controlPos[0];
  snakeY[0] = controlPos[1];

  for(int i = 0; i<2; i++){
    if(foodExists[i] && controlPos[0] == foodX[i] && controlPos[1] == foodY[i]){
      snakeLength++;
      foodExists[i] = 0;
    }
  }
  for(int i = 0; i < (snakeLength >= 10 ? 2: 1); i++){
  if(!foodExists[i] && controlPos[0] == foodX[i] && controlPos[1] == foodY[i]){
    foodX[i] = (foodX[i] + 17) % (screenWidth -5) + 2; //randomizer
    foodY[i] = (foodY[i] + 23) % (screenHeight -5) + 2;
    foodExists[i] =0;
  }
 }

}

void main(){
  P1DIR |= LED;
  P1OUT |= LED;
  configureClocks();
lcd_init();
switch_init();
enableWDTInterrupts();
or_sr(0x8);

clearScreen(COLOR_BLACK);
while(1){
  if(redrawScreen) {
    redrawScreen = 0;
    update_shape();
  }
  P1OUT &= ~LED;
  or_sr(0x10);
  P1OUT |= LED;
 }
}

void __interrupt_vec(PORT2_VECTOR) Port_2(){
  if(P2IFG & SWITCHES){
    P2IFG &= ~SWITCHES;
    switch_interrupt_handler();
  }
}
