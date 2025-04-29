#include "snake.h"
#include "game.h"
#include "lcdutils.h"
#include "lcddraw.h"

//snake body
short snakeX[100], snakeY[100];
short snakeLength = 5;

//snake head
short controlPos[2] = {20,20};

//snake velocity
short colVelocity = 0, rowVelocity = 0;

extern unsigned int grow_snake(unsigned int);

extern short foodX, foodY;
extern char foodExists;

//snake values for gameOver
void snake_init(){
  snakeLength = 5;
  controlPos[0] = 20;
  controlPos[1] = 20;
  colVelocity = 0;
  rowVelocity = 0;
}

//Update snake body and head
void move_snake(){

  for(int i = snakeLength -1; i> 0; i--){
    snakeX[0] = snakeX[i-1];
    snakeY[0] = snakeY[i-1];
  }
  //update head
  controlPos[0] += colVelocity;
  controlPos[1] += rowVelocity;

  //keep snake inside the limits
  if(controlPos[0] < 1) controlPos[0] = 1;
  if(controlPos[0] > screenWidth -2) controlPos[0] = screenWidth-2;
  if(controlPos[1] < 1) controlPos[1] = 1;
  if(controlPos[1] > screenHeight -2) controlPos[1] = screenHeight -2;

  snakeX[0] = controlPos[0];
  snakeY[1] = controlPos[1];
}

void check_self_collision(){
  for(int i = 1; i < snakeLength; i++){
    if(snakeX[i] == controlPos[0] && snakeY[i] == controlPos[1]){
      game_over();
    }
  }
}
void draw_snake(){
  //erase tail
    fillRectangle(snakeX[snakeLength-1], snakeY[snakeLength-1], 5, 5, COLOR_WHITE); //draw head
    fillRectangle(snakeX[0], snakeY[0],5,5, COLOR_BLACK);
    
}
