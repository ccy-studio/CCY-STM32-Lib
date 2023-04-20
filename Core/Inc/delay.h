//
// Created by Chenzedeng on 2023/4/20.
//

#ifndef CCY_STM32_LIBRARY_DELAY_H
#define CCY_STM32_LIBRARY_DELAY_H

#include "stm32f1xx_hal.h"

void delay_init(int sysMhz);

void delay_us(uint32_t us);

void delay_ms(uint32_t ms);

void delay_s(uint16_t s);

#endif //CCY_STM32_LIBRARY_DELAY_H
