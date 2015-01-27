/**************************a1.c***************************
Student Name: Ethan Gagne       Student Number: 0875556
Date: Jan 16,2015               Course Name: cis2500

I have exclusive control over this submission via my password.
By including this statement in this header comment, I certify that:
     1) I have read and understood the University policy on academic integrity;
     2) I have completed the Computing with Integrity Tutorial on Moodle; and
     3) I have achieved at least 80% in the Computing with Integrity Self Test.
I assert that this work is my own. I have appropriately acknowledged any and all material
(data, images, ideas or words) that I have used, whether directly quoted or paraphrased.
Furthermore, I certify that this assignment was prepared by me specifically for this course.
****************************************************************************/


/**********************
CIS 2500 A1
Ethan Gagne 0875556
Jan 16/15
This program plays the game of obstruction
************************/


#include "a1.h"

int main()
{
    //variable declaration
    int gameGrid[6][6];
    char keyPress;
    int cursorX;
    int cursorY;
    int currentPlayer;
    int i;
    int j;
    char playName[25];

    //initializes variables
    keyPress = 32;
    cursorX = 0;
    cursorY = 0;
    currentPlayer = 1;
    for (i = 0; i < 6; i ++)
    {
        for (j = 0; j < 6; j ++)
        {
            gameGrid[i][j] = 0;
        }
    }

    // greets the players and gets their names
    printf("Welcome to Obstruction\nPlease enter your name: ");
    fgets(playName, 25, stdin);
    // gets rid of the new line character
    for (i = 1; i < 24; i ++)
    {
        if (playName[i] == '\0')
        {
            playName[i - 1] = '\0';
        }
    }
    if (playName[0] == '\n')
    {
        playName[0] = 32;
    }

    //initializes ncurses
    initscr();
    start_color();
    cbreak ();
    noecho ();

    //initializes colour pairs
    //default
    init_pair(1, 7, 0);
    //cursor
    init_pair(2, 0, 7);
    //player
    init_pair(3,4,4);
    //comp
    init_pair(4,1,1);

    //draws the 6x6 grid to play on
    drawField();
    push2Screen(gameGrid);
    drawCursor(cursorX, cursorY);
    refresh();


    //game loop
    do
    {
        //this block controls the text at the top of the screen
        //checks if the game is over
        if (gameState(gameGrid) == 1)
        {
            //prints whose turn it is
            if (currentPlayer == 1)
            {
                print2Screen(TURN, playName);
            }
        }
        else
        {
            //prints who won the game
            if (playerSwap(currentPlayer) == 1)
            {
                print2Screen(WINNER, playName);
            }
            else
            {
                print2Screen(WINNER, "Computer");
            }
        }
        //prompts the user to quit the game after it is over
        if (gameState(gameGrid) == 0)
        {
            mvwprintw(stdscr, 1, 0, "Press 'q' to quit the game");
        }

        //gets user movement and moves the cursor accordingly
            if (gameState(gameGrid) == 1)
            {
                if (currentPlayer == 1)
                {
                    keyPress = getch();
                }
                if (currentPlayer == 2)
                {
                    keyPress = 'x';
                    comPlay(gameGrid, &cursorX, &cursorY);
                }
            }
            else
            {
                keyPress = getch();
            }
        //clears the invalid move statement
        mvwprintw(stdscr, 1, 0, "                                                                    ");   	
        switch (keyPress)
        {
            case UP:
                if (cursorY > 0)
                {
                    cursorY = cursorY - 1;
                }
                else
                {
                   print2Screen(INVALID, " ");
                }
                break;

            case DOWN:
                if (cursorY < 5)
                {
                    cursorY = cursorY + 1;
                }
                else
                {
                   print2Screen(INVALID, " ");
                }
                break;

            case LEFT:
                if (cursorX > 0)
                {
                    cursorX = cursorX - 1;
                }
                else
                {
                   print2Screen(INVALID, " ");
                }
                break;

            case RIGHT:
                if (cursorX < 5)
                {
                    cursorX = cursorX + 1;
                }
                else
                {
                   print2Screen(INVALID, " ");
                }
                break;

            //case for the selection key
            case 'x':
                if (gameGrid[cursorX][cursorY] == OPEN)
                {
                    //marks the square as controlled by current player
                    gameGrid[cursorX][cursorY] = currentPlayer;
                    currentPlayer = playerSwap(currentPlayer);
                    //blocks off all the values around the selection
                    if (cursorX > 0 )
                    {
                        gameGrid[cursorX - 1][cursorY] = CLOSED;
                    }
                    if (cursorX < 5 )
                    {
                        gameGrid[cursorX + 1][cursorY] = CLOSED;
                    }
                    if (cursorY > 0 )
                    {
                        gameGrid[cursorX][cursorY - 1] = CLOSED;
                    }
                    if (cursorY < 5 )
                    {
                        gameGrid[cursorX][cursorY + 1] = CLOSED;
                    }
                    if (cursorX > 0 && cursorY > 0)
                    {
                        gameGrid[cursorX - 1 ][cursorY - 1] = CLOSED;
                    }
                    if (cursorX < 5 && cursorY > 0)
                    {
                        gameGrid[cursorX + 1 ][cursorY - 1] = CLOSED;
                    }
                    if (cursorX < 5 && cursorY < 5)
                    {
                        gameGrid[cursorX + 1 ][cursorY + 1] = CLOSED;
                    }
                    if (cursorX > 0 && cursorY < 5)
                    {
                        gameGrid[cursorX - 1 ][cursorY + 1] = CLOSED;
                    }

                }
                else
                {
                   print2Screen(INVALID, " ");
                }
                break;

            case 'h':
                displayHelp();
                drawField();
                break;

            default:
                print2Screen(INVALID, " ");
                break;

        }
        //reads grid and makes changes to the screen for the user to see
        push2Screen(gameGrid);
        drawCursor(cursorX, cursorY);

        refresh();

    }while(keyPress != 'q');

    endwin();

    return 0;
}

/*
 comPlay: moves the cursor for the computer player
 Pre: Pass in a 2D array of int as a gameboard, an int for the x-position of the cursor, and an int for the y-position of the cursor
 Post: moves the cursor to the nearest open box
*/
void comPlay(int comGrid[6][6], int * xPos,int * yPos)
{
    //variable declaration
    int i;
    int j;

    //finds and open space to move the cursor to
    for (i = 0; i < 6; i ++)
    {
        for (j = 0; j < 6; j ++)
        {
            if (comGrid[i][j] == OPEN)
            {
                *xPos = i;
                *yPos = j; 
            }
        }
    }
}

/*
 displayHelp: displays a help page
 Pre: no values required
 Post: prints a page of help to the screen
*/
void displayHelp(void)
{
    //variable declaration
    int i;
    int j;

    //clears the screen
    for (i = 0; i <= 62; i ++)
    {
        for (j = 0; j <=46 ; j ++)
        {
            mvaddch(j,i, 32);
        }
    }
    //help info
    mvwprintw(stdscr, 0, 0, "OBSTRUCTION");
    mvwprintw(stdscr, 2, 0, "*******GAME RULES*******");
    mvwprintw(stdscr, 3, 0, "The objective of the game is to be the person\n to place the last marker");
    mvwprintw(stdscr, 5, 0, "When a marker is placed, you create obstructions\n all around it and future markers cannot be \n placed here");
    mvwprintw(stdscr, 8, 0, "The two players take turns placing markers until\n there are no empty spaces left and a \n winner is declared");
    mvwprintw(stdscr, 12, 0, "*******CONTROLS*******");
    mvwprintw(stdscr, 13, 0, "i - UP            j - LEFT");
    mvwprintw(stdscr, 14, 0, "k - DOWN          l - RIGHT");
    mvwprintw(stdscr, 15, 0, "x - SELECT        q - QUIT");
    mvwprintw(stdscr, 17, 0, "*******VISUAL*******");
    mvwprintw(stdscr, 18, 0, "Cursor - this is a white outline at the current postion");
    mvwprintw(stdscr, 19, 0, "Your Marker - these are the blue x's");
    mvwprintw(stdscr, 20, 0, "Comp Marker - these are the red +'s");
    mvwprintw(stdscr, 21, 0, "Obstructions - these are white characters in the shape of");
    mvwprintw(stdscr, 22, 0, "               a square in the middle of a box");

    //cursor
    attron(COLOR_PAIR(2));
    for (i = 0; i < 8; i ++)
    {
        mvaddch(25, 0 + i, '%');
        mvaddch(30, 0 + i, '%');
    }
    for (i = 0; i < 5; i ++)
    {
        mvaddch(25 + i, 0, '%');
        mvaddch(25 + i, 7, '%');
    }
    //player marker
    attron(COLOR_PAIR(3));
    mvaddch(29, 10, 'x');
    mvaddch(27, 10, 'x');
    mvaddch(28, 11, 'x');
    mvaddch(28, 11, 'x');
    mvaddch(29, 12, 'x');
    mvaddch(27, 12, 'x');
    //computer marker
    attron(COLOR_PAIR(4));
    mvaddch(27, 15, '+');
    mvaddch(28, 15, '+');
    mvaddch(29, 15, '+');
    mvaddch(28, 14, '+');
    mvaddch(28, 16, '+');
    //closed
    attron(COLOR_PAIR(1));
    for (i = 1; i < 4; i ++)
    {
        for (j = 1; j < 7; j ++)
        {
            mvaddch( 26 + i, 18 + j, '@');
        }
    }

    //quit message
    mvwprintw(stdscr, 32, 0, "press any key to exit this help screen");

    getch();
    //clears the screen
    for (i = 0; i <= 62; i ++)
    {
        for (j = 0; j <=46 ; j ++)
        {
            mvaddch(j,i, 32);
        }
    }

    return;
}

/*
 drawCursor: draws the cursor position
 Pre: Pass in an int for the x-position of the cursor and an int for the y-position of the cursor (both are relative to a 6x6 grid)
 Post: draws the cursor on the selected box but does not refresh
*/
void drawCursor(int xValue, int yValue)
{
    //variable declaration
    int i;

    // draws a border of '%' on the box
    attron(COLOR_PAIR(2));
    for (i = 0; i < 8; i ++)
    {
        mvaddch(((yValue) * 7 + 4), ((xValue) * 10 + 2 + i), '%');
        mvaddch(((yValue) * 7 + 8), ((xValue) * 10 + 2 + i), '%');
    }
    for (i = 0; i < 5; i ++)
    {
        mvaddch(((yValue) * 7 + 4 + i), ((xValue) * 10 + 2), '%');
        mvaddch(((yValue) * 7 + 4 + i), ((xValue) * 10 + 9), '%');
    }
    attron(COLOR_PAIR(1));
}

/*
 drawField: draws the grid for the gameboard
 Pre: no values required
 Post: gameboard is drawn and refreshed
*/
void drawField(void)
{
    //variable declaration
    int i;
    int j;

    //creates the horizontal lines for the grid that are 2 characters thick
    for (i=2; i <= 46; i = i + 7)
    {
        for (j = 0; j < 60; j ++)
        {
            mvaddch(i,j,'#');
            mvaddch(i + 1,j,'#');
        }
    }
    //creates the vertical lines for the grid that are 2 characters thick
    for (i=0; i <= 62; i = i + 10)
    {
        for (j = 2; j < 46; j ++)
        {
            mvaddch(j,i,'#');
            mvaddch(j, i + 1,'#');
        }
    }

    refresh();

    return;
}

/*
 drawPattern: holds all the drawing patterns used for the box states
 Pre: Pass in an int for the x-position, an int for the y-position, and an int for the item to be drawn
 Post: desired shap is drawn but not refreshed
*/
void drawPattern(int xGrid, int yGrid, int item)
{
    //variable declaration
    int i;
    int j;

    //stored layouts to be used for each type of marking
    switch (item)
    {
        case PLAY1:
            //draws an x made of 'x'
            attron(COLOR_PAIR(3));
            mvaddch(((yGrid) * 7 + 5), ((xGrid) * 10 + 4), 'x');
            mvaddch(((yGrid) * 7 + 7), ((xGrid) * 10 + 4), 'x');
            mvaddch(((yGrid) * 7 + 6), ((xGrid) * 10 + 5), 'x');
            mvaddch(((yGrid) * 7 + 6), ((xGrid) * 10 + 6), 'x');
            mvaddch(((yGrid) * 7 + 5), ((xGrid) * 10 + 7), 'x');
            mvaddch(((yGrid) * 7 + 7), ((xGrid) * 10 + 7), 'x');
            //places spaces around the border of the box to remove the cursor if it was there
            attron(COLOR_PAIR(1));
            for (i = 0; i < 8; i ++)
            {
                mvaddch(((yGrid) * 7 + 4), ((xGrid) * 10 + 2 + i), 32);
                mvaddch(((yGrid) * 7 + 8), ((xGrid) * 10 + 2 + i), 32);
            }
            for (i = 0; i < 5; i ++)
            {
                mvaddch(((yGrid) * 7 + 4 + i), ((xGrid) * 10 + 2), 32);
                mvaddch(((yGrid) * 7 + 4 + i), ((xGrid) * 10 + 9), 32);
            }
            break;

        case PLAY2:
            //draws a + made of '+'
            attron(COLOR_PAIR(4));
            mvaddch(((yGrid) * 7 + 5), ((xGrid) * 10 + 5), '+');
            mvaddch(((yGrid) * 7 + 6), ((xGrid) * 10 + 5), '+');
            mvaddch(((yGrid) * 7 + 7), ((xGrid) * 10 + 5), '+');
            mvaddch(((yGrid) * 7 + 6), ((xGrid) * 10 + 4), '+');
            mvaddch(((yGrid) * 7 + 6), ((xGrid) * 10 + 6), '+');
            //places spaces around the border of the box to remove the cursor if it was there
            attron(COLOR_PAIR(1));
            for (i = 0; i < 8; i ++)
            {
                mvaddch(((yGrid) * 7 + 4), ((xGrid) * 10 + 2 + i), 32);
                mvaddch(((yGrid) * 7 + 8), ((xGrid) * 10 + 2 + i), 32);
            }
            for (i = 0; i < 5; i ++)
            {
                mvaddch(((yGrid) * 7 + 4 + i), ((xGrid) * 10 + 2), 32);
                mvaddch(((yGrid) * 7 + 4 + i), ((xGrid) * 10 + 9), 32);
            }
            break;

        case CLOSED:
            //makes a filled square of '@'
            for (i = 1; i < 4; i ++)
            {
                for (j = 1; j < 7; j ++)
                {
                    mvaddch(((yGrid) * 7 + 4 + i), ((xGrid) * 10 + 2 + j), '@');
                }
            }
            //places spaces around the border of the box to remove the cursor if it was there
            for (i = 0; i < 8; i ++)
            {
                mvaddch(((yGrid) * 7 + 4), ((xGrid) * 10 + 2 + i), 32);
                mvaddch(((yGrid) * 7 + 8), ((xGrid) * 10 + 2 + i), 32);
            }
            for (i = 0; i < 5; i ++)
            {
                mvaddch(((yGrid) * 7 + 4 + i), ((xGrid) * 10 + 2), 32);
                mvaddch(((yGrid) * 7 + 4 + i), ((xGrid) * 10 + 9), 32);
            }
            break;

        case OPEN:
            // fills the square with spaces
            for (i = 0; i < 5; i ++)
            {
                for (j = 0; j < 8; j ++)
                {
                    mvaddch(((yGrid) * 7 + 4 + i), ((xGrid) * 10 + 2 + j), 32);
                }
            }
            break;

    }
    attron(COLOR_PAIR(1));
}

/*
 gameState: finds whether the game is over or still going
 Pre: Pass in a 2D-array of int that contains the values of what is to be placed in the coresponding squares
 Post: returns a 1 if the game is still going and a 0 if the game is over
*/
int gameState(int playGrid[6][6])
{
    //variable declaration
    int playing;
    int i;
    int j;

    //variable initialization
    playing = 0;

    //checks for any empty spaces on th grid
    for (i = 0; i < 6; i ++)
    {
        for (j = 0; j < 6; j ++)
        {
            if (playGrid[i][j] == OPEN)
            {
                playing = 1;
            }
        }
    }

    return playing;
}

/*
 playerSwap: switches beteen player one and two
 Pre: Pass an int to be switched
 Post: returns a 2 if given 1 and returns a  if given 2
*/
int playerSwap (int swap)
{
    if (swap == 1)
    {
        swap = 2;
    }
    else
    {
        swap = 1;
    }
    return swap;
}

/*
 print2Screen: prints messages to the screen based on the selected phrase
 Pre: Pass in int for the desired phrase and an array of char to add a personalized name
 Post: Message gets printed to the screen
*/
void print2Screen(int item, char input[25])
{
    //selects which message to print out
    switch (item)
    {
        //turn
        case TURN:
           mvwprintw(stdscr, 0, 0, "                                                                                 ");
           mvwprintw(stdscr, 0, 0, "It is %s's turn", input);
           refresh();
            break;

        //winning
        case WINNER:
            mvwprintw(stdscr, 0, 0, "                                                                                ");
            mvwprintw(stdscr, 0, 0, "%s has won the game", input);
            refresh();
            break;

        //invalid
        case INVALID:
            mvwprintw(stdscr, 1, 0, "That move is invalid");
            refresh();
            break;

    }
    return;
}

/*
 PrintMsg: puts all the correct box values to the screen
 Pre: Pass in a 2D-array of int that contains the values of what is to be placed in the coresponding squares
 Post: all game movement is updates to the screen
*/
void push2Screen(int screenGrid[6][6])
{
    //variable declaration
    int i;
    int j;

    //goes through every element of the array and displays the propper item int thier box based on their held value
    for (i = 0; i < 6; i ++)
    {
        for (j = 0; j < 6; j ++)
        {
           switch (screenGrid[j][i])
           {
              case PLAY1:
                    drawPattern(j, i, PLAY1);
                    break;

                case PLAY2:
                    drawPattern(j, i, PLAY2);
                    break;

                case CLOSED:
                    drawPattern(j, i, CLOSED);
                    break;

                case OPEN:
                    drawPattern(j, i, OPEN);
                    break;

            }
        }
    }
}
