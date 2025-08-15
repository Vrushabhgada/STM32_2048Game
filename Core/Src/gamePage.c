/*
 * gamePage.c
 *
 *  Created on: Apr 21, 2025
 *      Author: vrushabh
 */

#include "game.h"


Page currentPage = MAIN_PAGE;

uint8_t NumberOfCellpresent = 0;


/**
 * @brief Function to show win message.
 */
void WinPage()
{
    ILI9341_FillRoundRect(WonBoxX, WonBoxY, WonBoxW, WonBoxH, 10, wonTextBoxColour);
    ILI9341_WriteString(WonBoxX + 35, WonBoxY + 25, "You Won!", Font_11x18, 0xFFFF); // White text
}

/**
 * @brief Function to show lost message.
 */
void LostPage()
{
    ILI9341_FillRoundRect(WonBoxX, WonBoxY, WonBoxW, WonBoxH, 10, wonTextBoxColour);
    ILI9341_WriteString(WonBoxX + 30, WonBoxY + 25, "You Lost!", Font_11x18, 0xFFFF); // White text
}


/**
 * @brief Function display main page
 */
void mainPage()
{
    ILI9341_FillScreen(ILI9341_Color565(BackgroundR, BackgroundG, Backgroundb));
    ILI9341_WriteString(PlayButtonX+15 , 50, "2048", Font_16x26, 0xFFE0); // White text
    ILI9341_FillRoundRect(PlayButtonX, PlayButtonY, PlayButtonW, PlayButtonH, 20, ILI9341_Color565(buttonR, buttonG, buttonB));
    ILI9341_WriteString(PlayButtonX + 27, PlayButtonY + 15, "Play", Font_11x18, 0xFFFF); // White text
    currentPage = MAIN_PAGE;
}


/**
 * @brief Function to reset the game board.
 */
void CreatebackGround()
{
    ILI9341_FillScreen(ILI9341_Color565(BackgroundR, BackgroundG, Backgroundb));
    ILI9341_FillRoundRect(PlayBoxBorderGap, PlayBoxBorderGap, PlayBoxSize, PlayBoxSize, 10, GamePlayBackground);
}


/**
 * @brief Function to create a restart button.
 */
void CreateRestartButton()
{
    ILI9341_FillRoundRect(RestartButtonX, RestartButtonY, RestartButtonW, RestartButtonH, 10, ILI9341_Color565(buttonR, buttonG, buttonB));
    ILI9341_WriteString(RestartButtonX + 75, RestartButtonY + 15, "Restart", Font_11x18, 0xFFFF); // White text
}


/**
 * @brief Function to create the game page.
 */
void GamePage()
{
    NumberOfCellpresent = 0;

    CreatebackGround();

     ResetArray();
    UpdateAllCell();
    uint16_t temp[4][4];

    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            temp[i][j] = value[i][j];
        }
    }
    
     GenerateTwoEntry(temp);
    CreateRestartButton();

    currentPage = GAME_PAGE;
}
