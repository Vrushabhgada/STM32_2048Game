/*
 * game_touch.c
 *
 *  Created on: Apr 21, 2025
 *      Author: vrushabh
 */

#include "game_touch.h"
#include "stdint.h"
#include "game.h"
#include "touch.h"

uint16_t Firstx = 0;
uint16_t Firsty = 0;
uint16_t Secondx = 0;
uint16_t Secondy = 0;
uint16_t Lastx = 0;
uint16_t Lasty = 0;

uint16_t xcoordinatearray[5] = {0, 0, 0, 0, 0};
uint16_t ycoordinatearray[5] = {0, 0, 0, 0, 0};
uint16_t numberOFCordinate = 0;

uint16_t count = 0;
uint16_t RestartCount = 0;

/**
 * @brief Function to detect if the restart button is pressed.
 * @param x The x-coordinate of the touch.
 * @param y The y-coordinate of the touch.
 *
 */

int DetectRestartButtonPressed(uint16_t x, uint16_t y)
{

    // Check if the point is inside the rectangle
    if (x >= RestartButtonXPressMin && x <= RestartButtonXPressMax && y >= RestartButtonYPressMin && y <= RestartButtonYPressMax && count < 2)
    {
        RestartCount++;
        if (RestartCount >= 2)
        {
            printf("Restart Button pressed\r\n");
            GamePage();
        }
        return 0;
    }
    RestartCount = 0;
    return 1;
}

/**
 * @brief Function to check for touch events and make decisions based on them.
 *
 */
void ChekTouch(void)
{
    while (1)
    {
        SwipeDecision();
        uint16_t x = ReadX();
        uint16_t y = ReadY();
        if ((x == 0) || (x == 255) || (y == 0) || (y > 4000))
        {
            continue;
        }
        printf("X: %d, Y: %d\r\n", x, y);

        switch (currentPage)
        {
        case MAIN_PAGE:
            CheckMainaPage(x, y);
            break;
        case GAME_PAGE:
            CheckGamePage(x, y);
            break;

        case WIN_PAGE:
        case GAME_OVER_PAGE:
        {
            DetectRestartButtonPressed(x, y);
            break;
        }

        default:
            break;
        }
    }
}

/**
 * @brief Function to decide the swipe direction based on the touch coordinates.
 *
 */
void SwipeDecision()
{
    if (count > 2)
    {
        count--;
    }
    else if (count == 2)
    {
        Firstx = Secondx;
        Firsty = Secondy;
//        printf("Firstx: %d, Firsty: %d\t", Firstx, Firsty);
//        printf("Lastx: %d, Lasty: %d\r\n\r\n", Lastx, Lasty);
        count--;
        uint8_t CheckStatus = 0;

        int XDifference = 0;
        int YDifference = 0;

        if (numberOFCordinate > 4)
        {
        	 printf("Firstx: %d, Firsty: %d\t", xcoordinatearray[3], ycoordinatearray[3]);
        	        printf("Lastx: %d, Lasty: %d\r\n\r\n", xcoordinatearray[numberOFCordinate - 1], ycoordinatearray[numberOFCordinate - 1]);
            XDifference = xcoordinatearray[numberOFCordinate - 1] - xcoordinatearray[3];
            YDifference = ycoordinatearray[numberOFCordinate - 1] - ycoordinatearray[3];
        }
        else
        {
        	numberOFCordinate=0;
            return;
        }

        printf("XDifference: %d, YDifference: %d\r\n", XDifference, YDifference);

        // XDifference = Lastx - Firstx;
        // YDifference = Lasty - Firsty;

        if (abs(XDifference) > abs(YDifference))
        {
        	if (abs(XDifference)<500)
        	{
        		return;
        	}
            printf("Horizontal swipe detected\r\n");
            if (XDifference > 0)
            {
                printf("new Swipe left\r\n");
                LeftShift();
                CheckStatus = 1;
            }
            else
            {
                printf("new Swipe right\r\n");
                RightShift();
                CheckStatus = 1;
            }
        }
        else
        {
        	if (abs(YDifference)<500)
        	        	{
        	        		return;
        	        	}
            printf("vertical swipe detected\r\n");
            if (YDifference > 0)
            {
                printf("new Swipe down\r\n");
                DownShift();
                CheckStatus = 1;
            }
            else
            {
                printf("new Swipe up\r\n");
                UpShift();
                CheckStatus = 1;
            }
        }

        if (CheckStatus == 1)
        {

            switch (CheckifGameOver())
            {
            case GAME_WON:
            {
                WinPage();
                currentPage = WIN_PAGE;
                break;
            }
            case GAME_OVER:
            {
                LostPage();
                break;
            }
            default:
                break;
            }
        }

        printf("update row\r\n");
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                printf("%d ", value[i][j]);
            }
            printf("\r\n"); // Move to the next row
        }
        printf("\r\n\r\n");

        numberOFCordinate = 0;
        Firstx = 0;
        Firsty = 0;
        Secondx = 0;
        Secondy = 0;
        Lastx = 0;
        Lasty = 0;
    }
}

/**
 * @brief Function to check if the play button is pressed.
 * @param x The x-coordinate of the touch.
 * @param y The y-coordinate of the touch.
 *
 */
void CheckMainaPage(uint16_t x, uint16_t y)
{
    if ((x > 1300) && (x < 3000) && (y > 1950) && (y < 2450))
    {
        printf("Play button pressed\r\n");
        GamePage();
    }
}

/**
 * @brief Function updates values to decide the swipe.
 * @param x The x-coordinate of the touch.
 * @param y The y-coordinate of the touch.
 *
 */
void CheckGamePage(uint16_t x, uint16_t y)
{

    if (DetectRestartButtonPressed(x, y) == 0)
    {
        return;
    }

    if (numberOFCordinate >= 5)
    {

        xcoordinatearray[numberOFCordinate - 1] = xcoordinatearray[numberOFCordinate];
        ycoordinatearray[numberOFCordinate - 1] = ycoordinatearray[numberOFCordinate];

        xcoordinatearray[numberOFCordinate] = x;
        ycoordinatearray[numberOFCordinate] = y;
    }
    else
    {
        xcoordinatearray[numberOFCordinate] = x;
        ycoordinatearray[numberOFCordinate] = y;
        numberOFCordinate++;
    }

    if (Firstx == 0 && Firsty == 0)
    {
        Firstx = x;
        Firsty = y;
    }
    else if (Secondx == 0 && Secondy == 0)
    {
        Secondx = x;
        Secondy = y;
    }
    else
    {
        // printf("update Lastx and Lasty\r\n");
        Lastx = x;
        Lasty = y;
    }
    count = 0XFF;
    return;
}
