#ifndef SNAKE_H
#define SNAKE_H

//initiate snake starting position, length, direction
void snake_init();

//move snake's head and update taile
void move_snake();

//check for collition
void check_self_collition();

//draw snake
void draw_snake();

extern char gameover;

#endif
