/**
 * 
 * Created on: Apr 21, 2025
 * Author: vrushabh
 * Reference:
 * This code is inspired by / based on the work of Aleksander Alekseev
 * GitHub Repo : https://github.com/afiskon/stm32-ili9341
 */


#include "ili9341.h"
#include "stm32f0xx_hal.h"
#include "fonts.h"

extern SPI_HandleTypeDef hspi1;

#define CS_LOW() HAL_GPIO_WritePin(ILI9341_CS_GPIO_Port, ILI9341_CS_Pin, GPIO_PIN_RESET)
#define CS_HIGH() HAL_GPIO_WritePin(ILI9341_CS_GPIO_Port, ILI9341_CS_Pin, GPIO_PIN_SET)
#define DC_LOW() HAL_GPIO_WritePin(ILI9341_DC_GPIO_Port, ILI9341_DC_Pin, GPIO_PIN_RESET)
#define DC_HIGH() HAL_GPIO_WritePin(ILI9341_DC_GPIO_Port, ILI9341_DC_Pin, GPIO_PIN_SET)
#define RST_LOW() HAL_GPIO_WritePin(ILI9341_RST_GPIO_Port, ILI9341_RST_Pin, GPIO_PIN_RESET)
#define RST_HIGH() HAL_GPIO_WritePin(ILI9341_RST_GPIO_Port, ILI9341_RST_Pin, GPIO_PIN_SET)


/**
 * @brief Set the display oriental.
 * @param rotation The rotation value (0-3).
 * 
 */
void ILI9341_SetRotation(uint8_t rotation)
{
    uint8_t madctl = 0;

    switch (rotation)
    {
        case 0:
            madctl = 0x48; // Portrait
            break;
        case 1:
            madctl = 0x28; // Landscape
            break;
        case 2:
            madctl = 0x88; // Portrait flipped
            break;
        case 3:
            madctl = 0xE8; // Landscape flipped
            break;
        default:
            return; // Invalid rotation
    }

    ILI9341_SendCommand(0x36); // Memory Access Control
    ILI9341_SendData(&madctl, 1);
}

/**
 * @brief Send a command to the ILI9341 display controller.
 * @param cmd The command byte to send.
 * 
 */
void ILI9341_SendCommand(uint8_t cmd)
{
    DC_LOW();
    CS_LOW();
    HAL_SPI_Transmit(&hspi1, &cmd, 1, HAL_MAX_DELAY);
    CS_HIGH();
}


/**
 * @brief Send data to the ILI9341 display controller.
 * @param data Pointer to the data buffer.
 * @param size Size of the data in bytes.
 * 
 */
void ILI9341_SendData(uint8_t *data, uint16_t size)
{
    DC_HIGH();
    CS_LOW();
    HAL_SPI_Transmit(&hspi1, data, size, HAL_MAX_DELAY);
    CS_HIGH();
}


/**
 * @brief Reset the ILI9341 display controller.
 * 
 */
void ILI9341_Reset(void)
{
    RST_LOW();
    HAL_Delay(20);
    RST_HIGH();
    HAL_Delay(150);
}


/**
 * @brief Initialize the ILI9341 display controller.
 * 
 */
void ILI9341_Init(void)
{
    ILI9341_Reset();

    ILI9341_SendCommand(0x01); // Software Reset
    HAL_Delay(100);

    ILI9341_SendCommand(0x28); // Display OFF

    uint8_t data[] = {0x39, 0x2C, 0x00, 0x34, 0x02};
    ILI9341_SendCommand(0xCB);
    ILI9341_SendData(data, 5);

    data[0] = 0x00;
    data[1] = 0xC1;
    data[2] = 0x30;
    ILI9341_SendCommand(0xCF);
    ILI9341_SendData(data, 3);

    data[0] = 0x85;
    data[1] = 0x00;
    data[2] = 0x78;
    ILI9341_SendCommand(0xE8);
    ILI9341_SendData(data, 3);

    data[0] = 0x00;
    data[1] = 0x00;
    ILI9341_SendCommand(0xEA);
    ILI9341_SendData(data, 2);

    data[0] = 0x20;
    ILI9341_SendCommand(0xF7);
    ILI9341_SendData(data, 1);

    data[0] = 0x23;
    ILI9341_SendCommand(0xC0);
    ILI9341_SendData(data, 1);

    data[0] = 0x10;
    ILI9341_SendCommand(0xC1);
    ILI9341_SendData(data, 1);

    data[0] = 0x3e;
    data[1] = 0x28;
    ILI9341_SendCommand(0xC5);
    ILI9341_SendData(data, 2);

    data[0] = 0x86;
    ILI9341_SendCommand(0xC7);
    ILI9341_SendData(data, 1);

    data[0] = 0x48;
    ILI9341_SendCommand(0x36);
    ILI9341_SendData(data, 1);

    data[0] = 0x55;
    ILI9341_SendCommand(0x3A);
    ILI9341_SendData(data, 1);

    ILI9341_SendCommand(0x11);
    HAL_Delay(120);
    ILI9341_SendCommand(0x29); // Display ON
}

/**
 * @brief Set the address window for drawing.
 * @param x0 Start X coordinate.
 * @param y0 Start Y coordinate.
 * @param x1 End X coordinate.
 * @param y1 End Y coordinate.
 * 
 */
void ILI9341_SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    uint8_t data[4];
    ILI9341_SendCommand(0x2A);
    data[0] = x0 >> 8;
    data[1] = x0 & 0xFF;
    data[2] = x1 >> 8;
    data[3] = x1 & 0xFF;
    ILI9341_SendData(data, 4);

    ILI9341_SendCommand(0x2B);
    data[0] = y0 >> 8;
    data[1] = y0 & 0xFF;
    data[2] = y1 >> 8;
    data[3] = y1 & 0xFF;
    ILI9341_SendData(data, 4);

    ILI9341_SendCommand(0x2C); // Memory Write
}


/**
 * @brief Fill the entire screen with a color.
 * @param color The color to fill the screen with.
 * 
 */
void ILI9341_FillScreen(uint16_t color)
{
    ILI9341_SetAddressWindow(0, 0, 239, 319); // 240x320

    uint8_t data[2] = {color >> 8, color & 0xFF};
    CS_LOW();
    DC_HIGH();

    for (uint32_t i = 0; i < 240 * 320; i++)
    {
        HAL_SPI_Transmit(&hspi1, data, 2, HAL_MAX_DELAY);
    }

    CS_HIGH();
}


/**
 * @brief Fill a rectangle with a color.
 * @param x X coordinate of the top-left corner.
 * @param y Y coordinate of the top-left corner.
 * @param w Width of the rectangle.
 * @param h Height of the rectangle.
 * @param color The color to fill the rectangle with.
 * 
 */
void ILI9341_FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
    if ((x >= 240) || (y >= 320))
        return;

    // Clip width/height if drawing outside bounds
    if ((x + w - 1) >= 240)
        w = 240 - x;
    if ((y + h - 1) >= 320)
        h = 320 - y;

    ILI9341_SetAddressWindow(x, y, x + w - 1, y + h - 1);

    uint8_t data[2] = {color >> 8, color & 0xFF};

    DC_HIGH();
    CS_LOW();
    for (uint32_t i = 0; i < w * h; i++)
    {
        HAL_SPI_Transmit(&hspi1, data, 2, HAL_MAX_DELAY);
    }
    CS_HIGH();
}


/**
 * @brief Read the display ID.
 * @return The display ID.
 * 
 */
uint32_t ILI9341_ReadID(void)
{
    uint8_t cmd = 0xD3;
    uint8_t dummy[4] = {0x00, 0x00, 0x00, 0x00};
    uint8_t response[4];

    DC_LOW();
    CS_LOW();
    HAL_SPI_Transmit(&hspi1, &cmd, 1, HAL_MAX_DELAY);

    DC_HIGH();
    HAL_SPI_TransmitReceive(&hspi1, dummy, response, 4, HAL_MAX_DELAY);
    CS_HIGH();

    // response[1] = 0x93, response[2] = 0x41
    uint32_t id = (response[1] << 16) | (response[2] << 8) | response[3];
    return id;
}


/**
 * @brief Draw a character on the display.
 * @param x X coordinate of the top-left corner.
 * @param y Y coordinate of the top-left corner.
 * @param ch The character to draw.
 * @param font The font to use.
 * @param color The color of the character.
 * 
 */
void ILI9341_DrawChar(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t color)
{
    const uint16_t *charData = &font.data[(ch - 32) * font.height]; // ASCII offset

    for (uint8_t row = 0; row < font.height; row++)
    {
        uint16_t line = charData[row];
        for (uint8_t col = 0; col < font.width; col++)
        {
            if (line & (1 << (15 - col)))
            {
                ILI9341_DrawPixel(x + col, y + row, color);
            }
        }
    }
}


/**
 * @brief Write a string on the display.
 * @param x X coordinate of the top-left corner.
 * @param y Y coordinate of the top-left corner.
 * @param str The string to write.
 * @param font The font to use.
 * @param color The color of the text.
 * 
 */
void ILI9341_WriteString(uint16_t x, uint16_t y, const char *str, FontDef font, uint16_t color)
{
    while (*str)
    {
        ILI9341_DrawChar(x, y, *str, font, color);
        x += font.width;
        str++;
    }
}

/**
 * @brief Draw a single pixel on the display.
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param color 16-bit RGB565 color value.
 * 
 */
void ILI9341_DrawPixel(uint16_t x, uint16_t y, uint16_t color)
{
    if (x >= 240 || y >= 320)
        return; // Bounds check

    uint8_t data[2] = {color >> 8, color & 0xFF};

    ILI9341_SetAddressWindow(x, y, x, y);

    DC_HIGH();
    CS_LOW();
    HAL_SPI_Transmit(&hspi1, data, 2, HAL_MAX_DELAY);
    CS_HIGH();
}


/**
 * @brief Fill a circle with a color.
 * @param x0 X coordinate of the center.
 * @param y0 Y coordinate of the center.
 * @param r Radius of the circle.
 * @param delta Delta value.
 * @param color The color to fill the circle with.
 * 
 */
void ILI9341_FillCircleHelper(uint16_t x0, uint16_t y0, uint16_t r, uint8_t corner, uint16_t delta, uint16_t color)
{
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    while (x < y)
    {
        if (f >= 0)
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        if (corner & 0x1)
        {
            ILI9341_FillRect(x0 - x, y0 - y, x + delta, 1, color); // top-left
            ILI9341_FillRect(x0 - y, y0 - x, y + delta, 1, color);
        }
        if (corner & 0x2)
        {
            ILI9341_FillRect(x0, y0 - y, x + delta, 1, color); // top-right
            ILI9341_FillRect(x0, y0 - x, y + delta, 1, color);
        }
        if (corner & 0x4)
        {
            ILI9341_FillRect(x0, y0 + y, x + delta, 1, color); // bottom-right
            ILI9341_FillRect(x0, y0 + x, y + delta, 1, color);
        }
        if (corner & 0x8)
        {
            ILI9341_FillRect(x0 - x, y0 + y, x + delta, 1, color); // bottom-left
            ILI9341_FillRect(x0 - y, y0 + x, y + delta, 1, color);
        }
    }
}


/**
 * @brief Draw a filled rounded rectangle.
 * @param x X coordinate of the top-left corner.
 * @param y Y coordinate of the top-left corner.
 * @param w Width of the rectangle.
 * @param h Height of the rectangle.
 * @param r Corner radius.
 * @param color 16-bit RGB565 color value.
 * 
 */
void ILI9341_FillRoundRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, uint16_t color)
{
    // Center rectangle
    ILI9341_FillRect(x + r, y, w - 2 * r, h, color);

    // Side bars
    ILI9341_FillRect(x, y + r, r, h - 2 * r, color);             // Left
    ILI9341_FillRect(x + w - r, y + r, r, h - 2 * r, color);     // Right

    // Filled corners
    ILI9341_FillCircleHelper(x + r, y + r, r, 0x1, 0, color); // Top-left
    ILI9341_FillCircleHelper(x + w - r - 1, y + r, r, 0x2, 0, color); // Top-right
    ILI9341_FillCircleHelper(x + w - r - 1, y + h - r - 1, r, 0x4, 0, color); // Bottom-right
    ILI9341_FillCircleHelper(x + r, y + h - r - 1, r, 0x8, 0, color); // Bottom-left
}


/**
 * @brief Convert RGB values to 16-bit RGB565 color format.
 * @param r Red component (0-255).
 * @param g Green component (0-255).
 * @param b Blue component (0-255).
 * @return 16-bit RGB565 color value.
 * 
 */

uint16_t ILI9341_Color565(uint8_t r, uint8_t g, uint8_t b)
{
    return ((r & 0xF8) << 8) |   // 5 bits red (bits 11-15)
           ((g & 0xFC) << 3) |   // 6 bits green (bits 5-10)
           (b >> 3);            // 5 bits blue (bits 0-4)
}



/**
 * @brief Convert a hexadecimal color string to 16-bit RGB565 color format.
 * @param hex Hexadecimal color string (e.g., "#FF0000" or "FF0000").
 * @return 16-bit RGB565 color value.
 * 
 */
uint16_t ILI9341_ColorHex(const char *hex)
{
    if (hex[0] == '#') hex++; // Skip the '#' if it's there

    // Convert hex string to 8-bit RGB
    uint8_t r, g, b;
    char buf[3] = {0};

    buf[0] = hex[0]; buf[1] = hex[1];
    r = (uint8_t)strtol(buf, NULL, 16);

    buf[0] = hex[2]; buf[1] = hex[3];
    g = (uint8_t)strtol(buf, NULL, 16);

    buf[0] = hex[4]; buf[1] = hex[5];
    b = (uint8_t)strtol(buf, NULL, 16);

    // Convert to RGB565
    return ((r & 0xF8) << 8) |
           ((g & 0xFC) << 3) |
           (b >> 3);
}
