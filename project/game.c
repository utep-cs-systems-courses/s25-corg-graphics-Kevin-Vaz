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

extern char growing;
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
  buzzer_set_period(800);
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

    if(foodExists &&
       controlPos[0] >= foodX -4 && controlPos[0] <= foodX + 4 &&
       controlPos[1] >= foodY -4 && controlPos[1] <= foodY + 4) {
      fillRectangle(foodX-2, foodY -2, 5, 5, COLOR_BLACK);
      snakeLength += 7;
      growing =1;
      foodExists =0;
    }

    
  if(!foodExists){
    foodX = (foodX + 17) % (screenWidth -10);
    foodY = (foodY + 11) % ( screenHeight - 10);
      if(foodX < 5);
      if(foodY < 5);
      foodExists =1;
    }
  draw_snake();
  draw_food();
}

void restart_game(){
  game_init();
  snake_init();
  buzzer_set_period(0);
  redrawScreen =1;
}
