#include "game.h"
#include "snake.h"
#include "buzzer.h"
#include "led.h"
#include "lcdutils.h"
#include "lcddraw.h"

short foodX, foodY;
char foodExists = 1;

//redraw flag
int redrawScreen =1;

//game over flag
char gameover =0;

extern unsigned int grow_snake(unsigned int);

//setup of initial values
void game_init(){
  foodX = 30;
  foodY = 30;
  foodExists =1;
  gameover =0
}

//handle game over
void game_over(){
  gameover =1;
  P1OUT |= LED_GREEN;
  buzzer_set_period(500);
}

void draw_food(){
  if(foodExists){
      fillRectangle(foodX-2, foodY-2, 5, 5, COLOR_RED);
    }
}

//REDRAW SNAKE
void update_shape(){
  if(!gameover){
    move_snake();
    check_self_collision();
    draw_snake();
    draw_food;
  }
}
