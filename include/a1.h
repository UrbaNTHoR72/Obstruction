/**************************a1.h***************************
Student Name: Ethan Gagne       Student Number: 0875556
Date: Jan 16, 2015              Course Name: cis2500

I have exclusive control over this submission via my password.
By including this statement in this header comment, I certify that:
     1) I have read and understood the University policy on academic integrity;
     2) I have completed the Computing with Integrity Tutorial on Moodle; and
     3) I have achieved at least 80% in the Computing with Integrity Self Test.
I assert that this work is my own. I have appropriately acknowledged any and all material
(data, images, ideas or words) that I have used, whether directly quoted or paraphrased.
Furthermore, I certify that this assignment was prepared by me specifically for this course.
****************************************************************************/
#include <stdio.h>
#include <ncurses.h>
#include <string.h>

//values for the grid
#define OPEN 0
#define PLAY1 1
#define PLAY2 2
#define CLOSED 3
//values for the keys
#define UP 'i' 
#define LEFT 'j'
#define DOWN 'k'
#define RIGHT 'l'
//print2Screen() item values
#define TURN 1
#define WINNER 2
#define INVALID 3

//function prototypes
void comPlay(int comGrid[6][6], int * xPos,int * yPos);
void displayHelp(void);
void drawCursor(int xValue, int yValue);
void drawField(void);
void drawPattern(int xGrid, int yGrid, int item);
int gameState(int playGrid[6][6]);
int playerSwap (int swap);
void print2Screen(int item, char input[25]);
void push2Screen(int screenGrid[6][6]);



