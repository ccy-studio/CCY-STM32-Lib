//
// Created by 83503 on 2023/4/20.
//

#ifndef CCY_STM32_LIBRARY_AT24C02_H
#define CCY_STM32_LIBRARY_AT24C02_H

#include "main.h"

#define C02_Delay(us) HAL_Delay(us)

void C02_Init(void);
int C02_Write(uint8_t address,uint8_t data);
uint8_t C02_Read(uint8_t address);

#endif //CCY_STM32_LIBRARY_AT24C02_H
