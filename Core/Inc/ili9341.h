/*
 * ili9341.h
 *
 *  Created on: Apr 21, 2025
 *      Author: vrushabh
 * 
 * Reference:
 * This code is inspired by / based on the work of Aleksander Alekseev
 * GitHub Repo : https://github.com/afiskon/stm32-ili9341
 */
#ifndef __ILI9341_H__
#define __ILI9341_H__

#include "stm32f0xx_hal.h"
#include "fonts.h"


#define ILI9341_CS_Pin       GPIO_PIN_6
#define ILI9341_CS_GPIO_Port GPIOB
#define ILI9341_DC_Pin       GPIO_PIN_9
#define ILI9341_DC_GPIO_Port GPIOA
#define ILI9341_RST_Pin      GPIO_PIN_7
#define ILI9341_RST_GPIO_Port GPIOC

void ILI9341_SetRotation(uint8_t rotation);
void ILI9341_Init(void);
void ILI9341_FillScreen(uint16_t color);
void ILI9341_SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void ILI9341_FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
uint32_t ILI9341_ReadID(void);
void ILI9341_DrawPixel(uint16_t x, uint16_t y, uint16_t color);
void ILI9341_DrawChar(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t color);
void ILI9341_WriteString(uint16_t x, uint16_t y, const char* str, FontDef font, uint16_t color);
void ILI9341_FillRoundRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, uint16_t color);
uint16_t ILI9341_Color565(uint8_t r, uint8_t g, uint8_t b);
uint16_t ILI9341_ColorHex(const char *hex);

#endif
