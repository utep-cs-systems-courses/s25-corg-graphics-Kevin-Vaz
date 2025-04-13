/** \file lcddemo.c
 *  \brief A simple demo that draws a string and square
 */

#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"

/** Initializes everything, clears the screen, draws "hello" and a square */
int
main()
{
  configureClocks();
  lcd_init();
  u_char width = screenWidth, height = screenHeight;

  clearScreen(COLOR_RED);


  short rowMiddle = screenHeight/2;
  short colMiddle = screenWidth/2;
  short row = 0, col = 0;
  for(int j; j < 20; j++){
    row = j+rowMiddle;
    int minCol = colMiddle, maxCol = colMiddle+j;
    for(col=minCol; col<= maxCol; col++)
      drawPixel(col,row,COLOR_BLUE);
    col++;
  }
  //  drawString5x7(20,20, "Hi, HOW are you?", COLOR_ORANGE, COLOR_RED);

  // fillRectangle(30,60, 60, 20, COLOR_DEEP);  //x axis, y axis, base, height
  
}
