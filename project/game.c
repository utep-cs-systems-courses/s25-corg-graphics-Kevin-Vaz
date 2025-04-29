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

extern unsigned int grow_snake(unsigned int length);

//setup of initial values
void game_init(){
  foodX = 30;
  foodY = 30;
  foodExists =1;
  gameover =0;
}

//handle game over
void game_over(){
  gameover =1;
  P1OUT |= LED_GREEN;
  buzzer_set_period(500);
  clearScreen(COLOR_RED);
  redrawScreen =1;
}

void draw_food(){
  if(foodExists){
      fillRectangle(foodX-2, foodY-2, 5, 5, COLOR_RED);
    }
}

//REDRAW SNAKE
void update_shape(){
  if(gameover){
    clearScreen(COLOR_RED);
    return;
  }
  
    move_snake();
    check_self_collision();
      
  if(!foodExists){
      foodX = (foodX + 37) % screenWidth;
      foodY = (foodY + 61) % screenHeight;
      if(foodX < 5) foodX += 5;
      if(foodY < 5) foodY += 5;
      foodExists =1;
    }
  clearScreen(COLOR_BLACK);
  draw_snake();
  draw_food();
}
