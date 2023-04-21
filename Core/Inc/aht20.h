//
// Created by 83503 on 2023/4/21.
//

#ifndef CCY_STM32_LIBRARY_AHT20_H
#define CCY_STM32_LIBRARY_AHT20_H

#include "soft_i2c.h"

#define AHT20_READ 0x71
#define AHT20_WRITE 0x70

void aht20_init(void);

void aht20_read(double *temp, double *humidity);

#endif //CCY_STM32_LIBRARY_AHT20_H
