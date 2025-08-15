/*
 * game.h
 *
 *  Created on: Apr 18, 2025
 *      Author: vrushabh
 */

#ifndef INC_GAME_H_
#define INC_GAME_H_

#include "ili9341.h"
#include "stdio.h"


#define BackgroundR (247)
#define BackgroundG (238)
#define Backgroundb (222)

#define buttonR 133   
#define buttonG 110   
#define buttonB 87    

#define PlayButtonX 70
#define PlayButtonY 135
#define PlayButtonW 100
#define PlayButtonH 50

#define GamePlayBackground (0x83DD)
#define CellSize (47)
#define CellGap (8)

#define RestartButtonX 6
#define RestartButtonY 252
#define RestartButtonW 228
#define RestartButtonH 48


#define PlayBoxSize (228)
#define PlayBoxBorderGap (6)
#define Totalcells (16)
#define Threshold (50)


#define WonBoxX (40)
#define WonBoxY (80)
#define WonBoxW (150)
#define WonBoxH (75)

#define wonTextBoxColour (0xDE63)
#define PlayBoxSize (228)
#define PlayBoxBorderGap (6)
#define Totalcells (16)
#define Threshold (50)

#define RestartButtonX 6
#define RestartButtonY 252
#define RestartButtonW 228
#define RestartButtonH 48
#define GamePlayBackground (0x83DD)
#define CellSize (47)
#define CellGap (8)

#define BackgroundR (247)
#define BackgroundG (238)
#define Backgroundb (222)

#define buttonR 133   
#define buttonG 110   
#define buttonB 87    

#define PlayButtonX 70
#define PlayButtonY 135
#define PlayButtonW 100
#define PlayButtonH 50




// Define the 2048 game tile colors (RGB565 format for ILI9341)
#define COLOR_2 0xFFFF    // Pale yellow
#define COLOR_4 0x2D7F    // Cyan blue
#define COLOR_8 0x07E0    // Bright green
#define COLOR_16 0xFFE0   // Bright yellow
#define COLOR_32 0xFBE0   // Deeper orange-red
#define COLOR_64 0xF800   // Red
#define COLOR_128 0xF81F  // Magenta
#define COLOR_256 0x7817  // Purple
#define COLOR_512 0x051F  // Deep blue
#define COLOR_1024 0x0310 // Dark teal
#define COLOR_2048 0xFFFF // White
#define EmptyBox (0xCDD4)

// Define text colors (for good contrast)
#define TEXT_DARK 0x0000  // Black
#define TEXT_LIGHT 0xFFFF // White


#define BackgroundR (247)
#define BackgroundG (238)
#define Backgroundb (222)

#define buttonR 133   
#define buttonG 110   
#define buttonB 87    

#define PlayButtonX 70
#define PlayButtonY 135
#define PlayButtonW 100
#define PlayButtonH 50

#define GamePlayBackground (0x83DD)
#define CellSize (47)
#define CellGap (8)

#define RestartButtonX 6
#define RestartButtonY 252
#define RestartButtonW 228
#define RestartButtonH 48


#define PlayBoxSize (228)
#define PlayBoxBorderGap (6)
#define Totalcells (16)
#define Threshold (50)



typedef enum gameStatus
{
    GAME_OVER,
    GAME_WON,
    GAME_PLAYING,
    GAME_NOT_STARTED,
} gameStatus;



extern uint16_t value[4][4];

typedef enum Page
{
    MAIN_PAGE,
    GAME_PAGE,
    WIN_PAGE,
    GAME_OVER_PAGE,
} Page; 
extern Page currentPage ;



extern uint8_t NumberOfCellpresent;



void CheckMainaPage(uint16_t x,uint16_t y);
void CheckGamePage(uint16_t x,uint16_t y);
void MakeDecision();
void GenerateTwoEntry(uint16_t temp[4][4]);
void WinPage();
void LostPage();
void mainPage();
void GamePage();

uint8_t GetRandomNumberFormto(uint8_t length);
uint16_t GetTextColor(uint32_t value);
uint16_t GetTileColor(uint32_t value);
void ResetArray();
void UpdateAllCell();
void UpdateDisplay(uint16_t temp[4][4]);
void GenerateTwoEntry(uint16_t temp[4][4]);
void UpdateDisplayBottomToTop(uint16_t temp[4][4]);
void UpdateDisplayTopToBottom(uint16_t temp[4][4]);
void UpdateDisplayRightToLeft(uint16_t temp[4][4]);

#endif /* INC_GAME_H_ */
