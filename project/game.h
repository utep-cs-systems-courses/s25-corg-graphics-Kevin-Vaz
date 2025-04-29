#ifndef GAME_H
#define GAME_H

//starting game state
void game_init();

//gameOver Event
void game_over();

//Draw the food
void draw_food();

//redraw snake
void update_shape();

//allow other files to modify redraw flag
extern int redrawScreen;
extern char gameover;
extern short foodX, foodY;
extern char foodExists;

#endif
