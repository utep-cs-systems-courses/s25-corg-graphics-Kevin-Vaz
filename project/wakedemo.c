#include <msp430.h>

#include <libTimer.h>

#include "lcdutils.h"

#include "lcddraw.h"



// WARNING: LCD DISPLAY USES P1.0.  Do not touch!!!



#define LED BIT6       /* note that bit zero required for display */



#define SW1 1
#define SW2 2
#define SW3 4
#define SW4 8



#define SWITCHES 15

// Snake setup

#define CELL_SIZE 8    // Size of each snake cell
#define MAX_SNAKE_LENGTH 50



typedef struct {
  short col;
  short row;
} SnakeSegment;



SnakeSegment snake[MAX_SNAKE_LENGTH];  // Snake body
int snakeLength = 5;  // Starting length of snake


// Variables for movement
short colVelocity = 1, rowVelocity = 0;
short colLimits[2] = {1, screenWidth / CELL_SIZE};  // Screen width in terms of cells
short rowLimits[2] = {1, screenHeight / CELL_SIZE}; // Screen height in terms of cells


// Button interrupt setup
static char switch_update_interrupt_sense() {
  char p2val = P2IN;
  /* update switch interrupt to detect changes from current buttons */
  P2IES |= (p2val & SWITCHES);    /*if switch up, sense down */
  P2IES &= (p2val | ~SWITCHES);    /* if switch down, sense up */
  return p2val;
}

void switch_init() {            /* setup switch */
  P2REN |= SWITCHES;        /* enables resistors for switches */
  P2IE |= SWITCHES;         /* enable interrupts from switches */
  P2OUT |= SWITCHES;        /* pull-ups for switches */
  P2DIR &= ~SWITCHES;       /* set switches' bits for input */
  switch_update_interrupt_sense();
}

int switches = 0;

void switch_interrupt_handler() {
  char p2val = switch_update_interrupt_sense();
  switches = ~p2val & SWITCHES;
}

// Draw the snake
void draw_snake() {
  clearScreen(COLOR_BLACK); // Clear the screen before redrawing the snake

  for (int i = 0; i < snakeLength; i++) {
    int x = snake[i].col * CELL_SIZE;
    int y = snake[i].row * CELL_SIZE;
    fillRectangle(x, y, CELL_SIZE, CELL_SIZE,
		  (i == 0) ? COLOR_GREEN : COLOR_WHITE);  // Head = Green, Body = White
  }
}

// Update the position of the snake and redraw it
static void update_shape() {
  // Move the snake
  for (int i = snakeLength - 1; i > 0; i--) {
    snake[i] = snake[i - 1];  // Shift the body segments
  }

  // Update the head position
  snake[0].col += colVelocity;
  snake[0].row += rowVelocity;

  // Check for boundary collisions
  if (snake[0].col <= 0 || snake[0].col >= colLimits[1] || snake[0].row <= 0 || snake[0].row >= rowLimits[1]) {

    // Handle the collision (for now, reset the snake position)

    snakeLength = 3;

    snake[0].col = 5;

    snake[0].row = 5;

    colVelocity = 1;

    rowVelocity = 0;

  }



  draw_snake();  // Redraw the snake with its new position

}



// Watchdog timer handler

void wdt_c_handler() {

  static int secCount = 0;



  secCount++;

  if (secCount >= 25) {  // 10 times per second

    if (switches & SW1) rowVelocity = -1; // Up

    if (switches & SW2) rowVelocity = 1;  // Down

    if (switches & SW3) colVelocity = -1; // Left

    if (switches & SW4) colVelocity = 1;  // Right



    update_shape();  // Update the snake position and redraw
  }

}



void main() {

  P1DIR |= LED;    /**< Green LED on when CPU on */

  P1OUT |= LED;

  configureClocks();

  lcd_init();

  switch_init();



  enableWDTInterrupts();  /**< enable periodic interrupt */

  or_sr(0x8);             /**< GIE (enable interrupts) */



  // Initialize the snake

  snake[0].col = 5;

  snake[0].row = 5;

  snake[1].col = 4;

  snake[1].row = 5;

  snake[2].col = 3;

  snake[2].row = 5;



  while (1) {

    P1OUT &= ~LED;  // LED off

    or_sr(0x10);    /**< CPU OFF */

    P1OUT |= LED;   // LED on

  }

}



// Button interrupt service routine

void __interrupt_vec(PORT2_VECTOR) Port_2() {

  if (P2IFG & SWITCHES) {      /* did a button cause this interrupt? */

    P2IFG &= ~SWITCHES;         /* clear pending switch interrupts */

    switch_interrupt_handler();  /* single handler for all switches */

  }

}
