#ifndef SNAKE_H
#define SNAKE_H

//initiate snake starting position, length, direction
void snake_init();

//move snake's head and update taile
void move_snake();

//check for collition
void check_self_collision();

//draw snake
void draw_snake();

extern char gameover;
extern short colVelocity, rowVelocity;
extern short snakeX[100], snakeY[100];
extern short snakeLength;
extern short controlPos[2];


#endif
