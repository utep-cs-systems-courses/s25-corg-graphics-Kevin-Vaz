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
  gameover = 1;
  clearScreen(COLOR_RED);

  drawString5x7(25, 60, "GAME OVER", COLOR_WHITE, COLOR_BLUE);
  drawString5x7(10, 80, "PRESS any key", COLOR_BLUE, COLOR_GREEN);
  
  P1OUT |= LED_GREEN;
  buzzer_set_period(100);
  redrawScreen =0;
  
}

void draw_food(){
  if(foodExists){
      fillRectangle(foodX-2, foodY-2, 5, 5, COLOR_RED);
    }
}

//REDRAW SNAKE
void update_shape(){
  if(gameover){
     return;
  }
 
  //    clearScreen(COLOR_BLACK);
   
    move_snake();
    check_self_collision();

    
    if(foodExists &&
       controlPos[0] >= foodX -4 && controlPos[0] <= foodX + 4 &&
       controlPos[1] >= foodY -4 && controlPos[1] <= foodY + 4) {
      fillRectangle(foodX-2, foodY -2, 5, 5, COLOR_BLACK);
      snakeLength = grow_snake(snakeLength);
      growing =1;
      foodExists =0;
    }

    
  if(!foodExists){
    foodX = (foodX + 17) % (screenWidth -10);
    foodY = (foodY + 11) % ( screenHeight - 10);
      if(foodX / 5) * 5;
      if(foodY / 5) * 5;
      foodExists =1;
    }
  draw_snake();
  draw_food();
}

void restart_game(){
  game_init();
  snake_init();
  clearScreen(COLOR_BLACK);
  
  redrawScreen =1;
}
