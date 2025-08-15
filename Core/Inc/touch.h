/*
 * touch.h
 *
 *  Created on: Apr 18, 2025
 *      Author: vrushabh
 */

#ifndef INC_TOUCH_H_
#define INC_TOUCH_H_


#include "stdint.h"

#define ILI9341_TOUCH_IRQ_Pin        GPIO_PIN_4  // Arduino D5
#define ILI9341_TOUCH_IRQ_GPIO_Port  GPIOB

#define ILI9341_TOUCH_CS_Pin         GPIO_PIN_10 // Arduino D2
#define ILI9341_TOUCH_CS_GPIO_Port   GPIOB


#define CMD_X 0xD0  // 1101 0000
#define CMD_Y 0x90  // 1001 0000



uint16_t ReadY(void);
uint16_t ReadX(void);

#endif /* INC_TOUCH_H_ */
