/*
 * touch.c
 *
 *  Created on: Apr 18, 2025
 *      Author: vrushabh
 * 
 * Reference:
 * This code is inspired by / based on the work of Aleksander Alekseev
 * GitHub Repo : https://github.com/afiskon/stm32-ili9341
 * 
 */

#include "touch.h"
#include "stm32f0xx_hal.h"
#include "stdio.h"
#include "game.h"

extern SPI_HandleTypeDef hspi1;

#define TSC2046_CS_LOW() HAL_GPIO_WritePin(ILI9341_TOUCH_CS_GPIO_Port, ILI9341_TOUCH_CS_Pin, GPIO_PIN_RESET)
#define TSC2046_CS_HIGH() HAL_GPIO_WritePin(ILI9341_TOUCH_CS_GPIO_Port, ILI9341_TOUCH_CS_Pin, GPIO_PIN_SET)

/**
 * @brief Write data to the TSC2046 touch controller via SPI.
 * @param data The byte to transmit.
 * @return The byte received during transmission.
 * 
 */

uint8_t TSC2046_SPI_Write(uint8_t data)
{
    uint8_t rx;
    HAL_SPI_TransmitReceive(&hspi1, &data, &rx, 1, HAL_MAX_DELAY);
    return rx;
}


/**
 * @brief Read a coordinate from the TSC2046 touch controller.
 * @param cmd The command byte to determine which coordinate to read.
 * @return The 9-bit coordinate value.
 * 
 */

uint16_t TSC2046_ReadCoord(uint8_t cmd)
{
    uint16_t result = 0;

    TSC2046_CS_LOW();
    TSC2046_SPI_Write(cmd);
    result = TSC2046_SPI_Write(0x00) << 8;
    result |= TSC2046_SPI_Write(0x00);
    TSC2046_CS_HIGH();

    return result >> 3; // 12-bit to 9-bit conversion
}

/**
 * @brief Read the X coordinate from the touch screen.
 * @return The X coordinate value.
 * 
 */

uint16_t ReadX(void)
{
    return TSC2046_ReadCoord(CMD_X);
}

/**
 * @brief Read the Y coordinate from the touch screen.
 * @return The Y coordinate value.
 * 
 */

uint16_t ReadY(void)
{
    return TSC2046_ReadCoord(CMD_Y);
}