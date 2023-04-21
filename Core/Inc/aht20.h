//
// Created by 83503 on 2023/4/21.
//

#ifndef CCY_STM32_LIBRARY_AHT20_H
#define CCY_STM32_LIBRARY_AHT20_H

#include "delay.h"
#include "main.h"


#define AHT20_I2C_SDA_PIN_G IIC_SDA_GPIO_Port
#define AHT20_I2C_SDA_PIN IIC_SDA_Pin
#define AHT20_I2C_SCL_PIN_G IIC_SCL_GPIO_Port
#define AHT20_I2C_SCL_PIN IIC_SCL_Pin

#define AHT20_Delay_Us(us) delay_us(us)
#define AHT20_Delay_Ms(ms) delay_ms(ms)

void aht20_init();

void aht20_read(float *humidity, float *temp);

#endif //CCY_STM32_LIBRARY_AHT20_H
