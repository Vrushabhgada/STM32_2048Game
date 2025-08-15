/*
 * game_swipe_logic.c
 *
 *  Created on: Apr 18, 2025
 *      Author: vrushabh
 */

#include "game.h"
extern uint16_t value[4][4];


/**
 * @brief Function to move the row values to the right.
 * @param row The row to be moved.
 * 
 */
void MoveRowToRight(uint16_t *row)
{
    int Fillposition = 3;
    int x = 3;

    while (Fillposition >= 0)
    {
        if (row[Fillposition] != 0)
        {
            Fillposition--;
            x = Fillposition;
            continue;
        }

        while (x >= 0 && row[x] == 0)
        {
            x--;
        }

        if (x < 0)
        {
            break;
        }
        else
        {
            row[Fillposition] = row[x];
            row[x] = 0;
            Fillposition--;
        }
    }
}


/**
 * @brief Right swipe logic.
 */
void RightShift()
{
    uint16_t temp[4][4];
    int x = 0;

    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            temp[i][j] = value[i][j];
        }
    }

    for (int y = 0; y < 4; y++)
    {
        MoveRowToRight(temp[y]);
        UpdateDisplay(temp);
        x = 3;
        while (x >= 1)
        {
            if ((temp[y][x] == temp[y][x - 1]) && (temp[y][x] != 0))
            {
                temp[y][x] = temp[y][x] + temp[y][x - 1];
                temp[y][x - 1] = 0;
                x -= 2;
                NumberOfCellpresent--;
            }
            else
            {
                x--;
            }
        }
        UpdateDisplay(temp);
        MoveRowToRight(temp[y]);
        UpdateDisplay(temp);
    }

    GenerateTwoEntry(temp);
}


/**
 * @brief Function to move the column values down.
 * @param temp The temporary matrix.
 * @param col The column index.
 * 
 */
void MoveColumnDown(uint16_t temp[4][4], int col)
{
    uint16_t column[4];
    // Extract column
    for (int row = 0; row < 4; row++)
        column[row] = temp[row][col];
        
    int Fillposition = 3;
    int x = 3;
    while (Fillposition >= 0)
    {
        if (column[Fillposition] != 0)
        {
            Fillposition--;
            x = Fillposition;
            continue;
        }
        while (x >= 0 && column[x] == 0)
        {
            x--;
        }
        if (x < 0)
        {
            break;
        }
        else
        {
            column[Fillposition] = column[x];
            column[x] = 0;
            Fillposition--;
        }
    }
    
    // Write column back to temp
    for (int row = 0; row < 4; row++)
        temp[row][col] = column[row];
}



/**
 * @brief Down swipe logic.
 */
void DownShift()
{
    uint16_t temp[4][4];
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            temp[i][j] = value[i][j];
            
    for (int col = 0; col < 4; col++)
    {
        MoveColumnDown(temp, col);
        UpdateDisplayTopToBottom(temp);
        
        int x = 3;
        while (x > 0)
        {
            if (temp[x][col] != 0 && temp[x][col] == temp[x-1][col])
            {
                temp[x][col] += temp[x-1][col];
                temp[x-1][col] = 0;
                x -= 2;
                NumberOfCellpresent--;
            }
            else
            {
                x--;
            }
        }
        
        UpdateDisplayTopToBottom(temp);
        MoveColumnDown(temp, col);
        UpdateDisplayTopToBottom(temp);
    }
    
    GenerateTwoEntry(temp);
}



/**
 * @brief Function to move the column values up.
 * @param temp The temporary matrix.
 * @param col The column index.
 * 
 */
void MoveColumnUp(uint16_t temp[4][4], int col)
{
    uint16_t column[4]; // Temporary array to hold the column
    
    // Extract the column
    for (int row = 0; row < 4; row++)
        column[row] = temp[row][col];
    
    // Move values up (similar to your MoveRowToRight logic)
    int fillPosition = 0;
    int x = 0;
    while (fillPosition < 4)
    {
        if (column[fillPosition] != 0)
        {
            fillPosition++;
            x = fillPosition;
            continue;
        }
        while (x < 4 && column[x] == 0)
        {
            x++;
        }
        if (x >= 4)
        {
            break;
        }
        else
        {
            column[fillPosition] = column[x];
            column[x] = 0;
            fillPosition++;
        }
    }
    
    // Write the column back to temp
    for (int row = 0; row < 4; row++)
        temp[row][col] = column[row];
}


/**
 * @brief Up swipe logic.
 */
void UpShift()
{
    uint16_t temp[4][4];
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            temp[i][j] = value[i][j];
    
    for (int col = 0; col < 4; col++)
    {
        MoveColumnUp(temp, col);
        UpdateDisplayBottomToTop(temp);
        
        // Merge similar adjacent cells
        int x = 0;
        while (x < 3)
        {
            if (temp[x][col] != 0 && temp[x][col] == temp[x + 1][col])
            {
                temp[x][col] += temp[x + 1][col];
                temp[x + 1][col] = 0;
                x += 2;
                NumberOfCellpresent--;
            }
            else
            {
                x++;
            }
        }
        
        UpdateDisplayBottomToTop(temp);
        MoveColumnUp(temp, col);
        UpdateDisplayBottomToTop(temp);
    }
    
    GenerateTwoEntry(temp);
}


/**
 * @brief Function to move the row values to the left.
 * @param row The row to be moved.
 * 
 */
void MoveRowToLeft(uint16_t *row)
{
    int Fillposition = 0;
    int x = 0;

    while (Fillposition < 4)
    {
        if (row[Fillposition] != 0)
        {
            Fillposition++;
            x = Fillposition;
            continue;
        }

        while (x < 4 && row[x] == 0)
        {
            x++;
        }

        if (x >= 4)
        {
            break;
        }
        else
        {
            row[Fillposition] = row[x];
            row[x] = 0;
            Fillposition++;
        }
    }
}


/**
 * @brief Left swipe logic.
 */
void LeftShift()
{
    uint16_t temp[4][4];
    int x = 0;

    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            temp[i][j] = value[i][j];
        }
    }

    for (int y = 0; y < 4; y++)
    {
        MoveRowToLeft(temp[y]);
        UpdateDisplayRightToLeft(temp);
        x = 0;
        while (x < 3)
        {
            if ((temp[y][x] == temp[y][x + 1]) && (temp[y][x] != 0))
            {
                temp[y][x] = temp[y][x] + temp[y][x + 1];
                temp[y][x + 1] = 0;
                x += 2;
                NumberOfCellpresent--;
            }
            else
            {
                x++;
            }
        }
        UpdateDisplayRightToLeft(temp);
        MoveRowToLeft(temp[y]);
        UpdateDisplayRightToLeft(temp);
    }

    GenerateTwoEntry(temp);
}
