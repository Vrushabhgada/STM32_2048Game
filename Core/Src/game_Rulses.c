/*
 * game_Rulses.c
 *
 *  Created on: Apr 21, 2025
 *      Author: vrushabh
 */

#include "game.h"
/**
 * @brief Matrix to store the values of the game board.
 */
uint16_t value[4][4] = {
    {2, 4, 8, 0},
    {8, 64, 2, 0},
    {128, 8, 4, 0},
    {2, 8, 8, 0},
};


/**
 * @brief Function to generate a random number between 0 and length-1.
 * @param length The upper limit for the random number generation.
 * 
 * @return A random number between 0 and length-1.
 */
uint8_t GetRandomNumberFormto(uint8_t length)
{
    uint32_t random_value = SysTick->VAL;
    return (random_value % length); // Modulo 4 gives range 0-3
}


/**
 * @brief Function to get the color of a tile based on its value.
 * @param value The value of the tile.
 * 
 * @return The color associated with the tile value.
 */
uint16_t GetTileColor(uint32_t value)
{
    switch (value)
    {
    case 0:
        return EmptyBox; // Black for empty tiles
    case 2:
        return COLOR_2;
    case 4:
        return COLOR_4;
    case 8:
        return COLOR_8;
    case 16:
        return COLOR_16;
    case 32:
        return COLOR_32;
    case 64:
        return COLOR_64;
    case 128:
        return COLOR_128;
    case 256:
        return COLOR_256;
    case 512:
        return COLOR_512;
    case 1024:
        return COLOR_1024;
    case 2048:
        return COLOR_2048;
    default:
        return 0x0000; // Black for unknown values
    }
}

/**
 * @brief Function to get the string representation of a tile value.
 * @param value The value of the tile.
 * 
 * @return The string representation of the tile value.
 */
const char *GetTileString(uint16_t value)
{
    switch (value)
    {
    case 0:
        return ""; // Empty string for empty tiles
    case 2:
        return "  2 ";
    case 4:
        return "  4 ";
    case 8:
        return "  8 ";
    case 16:
        return " 16 ";
    case 32:
        return " 32 ";
    case 64:
        return " 64 ";
    case 128:
        return " 128";
    case 256:
        return " 256";
    case 512:
        return " 512";
    case 1024:
        return "1024";
    case 2048:
        return "2048";
    default:
        return ""; // Return empty string for 0 or unknown
    }
}


/**
 * @brief Function to get the text color based on the tile value.
 * @param value The value of the tile.
 * 
 * @return The text color associated with the tile value.
 */
uint16_t GetTextColor(uint32_t value)
{
    // Use white text on darker backgrounds, black text on lighter backgrounds
    switch (value)
    {
    case 2:
    case 4:
    case 8:
    case 16:
    case 32:
    case 2048:
        return TEXT_DARK;
    case 64:
    case 128:
    case 256:
    case 512:
    case 1024:
        return TEXT_LIGHT;
    default:
        return TEXT_LIGHT;
    }
}


/**
 * @brief Function to check if the game is over.
 * 
 * @return The status of the game (GAME_OVER, GAME_WON, or GAME_PLAYING).
 */
gameStatus CheckifGameOver()
{
    // Check for adjacent tiles with the same value
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            if (value[y][x] == 0)
            {
                return GAME_PLAYING; // Game is not over
            }
            else if (value[y][x] == 2048)
            {
                return GAME_WON; // Game is not over
            }
            if ((x < 3) && (value[y][x] == value[y][x + 1]))
            {
                return GAME_PLAYING; // Game is not over
            }

            // Check below neighbor (but not at the bottom edge)
            if ((y < 3) && (value[y][x] == value[y + 1][x]))
            {
                return GAME_PLAYING; // Game is not over
            }
        }
    }
    return GAME_OVER; // Game is over
}


/**
 * @brief Function to reset the game board.
 */
void ResetArray()
{
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            value[i][j] = 0;
        }
    }
}

/**
 * @brief Function to update all cells on the game board.
 */
void UpdateAllCell()
{
    int xlocaiton = 0;
    int ylocaiton = 0;
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
    
            xlocaiton = PlayBoxBorderGap + CellGap * (x + 1) + CellSize * (x);
            ylocaiton = PlayBoxBorderGap + CellGap * (y + 1) + CellSize * (y);
            ILI9341_FillRoundRect(xlocaiton, ylocaiton, CellSize, CellSize, 10, GetTileColor(value[y][x]));
            ILI9341_WriteString(xlocaiton + 10, ylocaiton + 20, GetTileString(value[y][x]), Font_7x10, GetTextColor(value[y][x])); // White text
        }
    }
}

/**
 * @brief Function to update the cell whose values have changed when swiping from right and update the matrix.
 * @param temp The new values to be displayed.
 * 
 */
void UpdateDisplay(uint16_t temp[4][4])
{
    int xlocaiton, ylocaiton;

    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            if (value[y][x] == temp[y][x])
            {
                continue;
            }

            value[y][x] = temp[y][x];

            xlocaiton = PlayBoxBorderGap + CellGap * (x + 1) + CellSize * x;
            ylocaiton = PlayBoxBorderGap + CellGap * (y + 1) + CellSize * y;

            ILI9341_FillRoundRect(xlocaiton, ylocaiton, CellSize, CellSize, 10, GetTileColor(value[y][x]));
            ILI9341_WriteString(xlocaiton + 10, ylocaiton + 20, GetTileString(value[y][x]), Font_7x10, GetTextColor(value[y][x]));
        }
    }
}


/**
 * @brief Function to generate two new entries on the game board.
 * @param temp The current state of the game board.
 * 
 */
void GenerateTwoEntry(uint16_t temp[4][4])
{
    if (NumberOfCellpresent == 16)
    {
        UpdateDisplay(temp);
        return;
    }

    int cell1 = GetRandomNumberFormto(Totalcells - NumberOfCellpresent);
    int cell2 = cell1;
    while ((cell2 == cell1) && (NumberOfCellpresent != 15))
    {
        cell2 = GetRandomNumberFormto(Totalcells - NumberOfCellpresent);
    }
    if (NumberOfCellpresent == 15)
    {
        NumberOfCellpresent = 16;
    }
    else
    {
        NumberOfCellpresent += 2;
    }

    int cellNumber = 0;

    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            if (temp[y][x] == 0)
            {
                if ((cellNumber == cell1) || (cellNumber == cell2))
                {
                    temp[y][x] = 2;
                }
                cellNumber++;
            }
        }
    }

    UpdateDisplay(temp);
}


/**
 * @brief Function to update the cell whose values have changed when swiping from left and update the matrix.
 * @param temp The new values to be displayed.
 * 
 */
void UpdateDisplayRightToLeft(uint16_t temp[4][4])
{
    int xlocaiton, ylocaiton;
    for (int y = 0; y < 4; y++)
    {
        for (int x = 3; x >= 0; x--)  // Start from rightmost column
        {
            if (value[y][x] == temp[y][x])
            {
                continue;
            }
            value[y][x] = temp[y][x];
            xlocaiton = PlayBoxBorderGap + CellGap * (x + 1) + CellSize * x;
            ylocaiton = PlayBoxBorderGap + CellGap * (y + 1) + CellSize * y;
            ILI9341_FillRoundRect(xlocaiton, ylocaiton, CellSize, CellSize, 10, GetTileColor(value[y][x]));
            ILI9341_WriteString(xlocaiton + 10, ylocaiton + 20, GetTileString(value[y][x]), Font_7x10, GetTextColor(value[y][x]));
        }
    }
}


/**
 * @brief Function to update the cell whose values have changed when swiping from top to bottom and update the matrix.
 * @param temp The new values to be displayed.
 * 
 */
void UpdateDisplayTopToBottom(uint16_t temp[4][4])
{
    int xlocaiton, ylocaiton;
    for (int x = 0; x < 4; x++)  // Process columns first
    {
        for (int y = 0; y < 4; y++)  // Then rows (top to bottom)
        {
            if (value[y][x] == temp[y][x])
            {
                continue;
            }
            value[y][x] = temp[y][x];
            xlocaiton = PlayBoxBorderGap + CellGap * (x + 1) + CellSize * x;
            ylocaiton = PlayBoxBorderGap + CellGap * (y + 1) + CellSize * y;
            ILI9341_FillRoundRect(xlocaiton, ylocaiton, CellSize, CellSize, 10, GetTileColor(value[y][x]));
            ILI9341_WriteString(xlocaiton + 10, ylocaiton + 20, GetTileString(value[y][x]), Font_7x10, GetTextColor(value[y][x]));
        }
    }
}



/**
 * @brief Function to update the cell whose values have changed when swiping from bottom to top and update the matrix.
 * @param temp The new values to be displayed.
 * 
 */
void UpdateDisplayBottomToTop(uint16_t temp[4][4])
{
    int xlocaiton, ylocaiton;
    for (int x = 0; x < 4; x++)  // Process columns first
    {
        for (int y = 3; y >= 0; y--)  // Then rows (bottom to top)
        {
            if (value[y][x] == temp[y][x])
            {
                continue;
            }
            value[y][x] = temp[y][x];
            xlocaiton = PlayBoxBorderGap + CellGap * (x + 1) + CellSize * x;
            ylocaiton = PlayBoxBorderGap + CellGap * (y + 1) + CellSize * y;
            ILI9341_FillRoundRect(xlocaiton, ylocaiton, CellSize, CellSize, 10, GetTileColor(value[y][x]));
            ILI9341_WriteString(xlocaiton + 10, ylocaiton + 20, GetTileString(value[y][x]), Font_7x10, GetTextColor(value[y][x]));
        }
    }
}