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
  int prevX = snakeX[0];
  int prevY = snakeY[0];
  int prev2X, prev2Y;
  snakeX[0] = controlPos[0];
  snakeY[0] = controlPos[1];

  //move each tail section
  for(int i = 1; i <snakeLength; i++){
    prev2X = snakeX[i];
    prev2Y = snakeY[i];
    snakeX[i] = prevX;
    snakeY[i] = prevY;
    prevX = prev2X;
    prevY = prev2Y;
  }

  controlPos[0] += colVelocity;
  controlPos[1] += rowVelocity;

  //keep snake inside the limits
  if(controlPos[0] < 1) controlPos[0] = 1;
  if(controlPos[0] > screenWidth -1) controlPos[0] = screenWidth-1;
  if(controlPos[1] < 1) controlPos[1] = 1;
  if(controlPos[1] > screenHeight -1) controlPos[1] = screenHeight -1;

  //check if food was eaten
  if(controlPos[0] >= foodX-2 && controlPos[0] <= foodX + 2 &&
     controlPos[1] >= foodY-2 && controlPos[1] <= foodY + 2) {
    snakeLength = grow_snake(snakeLength);
    foodExists = 0;
}

void check_self_collision(){
  for(int i = 1; i < snakeLength; i++){
    if(snakeX[i] == controlPos[0] && snakeY[i] == controlPos[1]){
      game_over();
    }
  }
}
}
void draw_snake(){
  for(int i = 0; i < snakeLength; i++){
    draw_ball(snakeX[0], snakeY[0], COLOR_WHITE); //draw head
  }
    draw_ball(snakeX[snakeLength-1], snakeY[snakeLength-1], COLOR_BLACK);
}
