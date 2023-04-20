//
// Created by 83503 on 2023/4/20.
//

#ifndef CCY_STM32_LIBRARY_SOFT_I2C_H
#define CCY_STM32_LIBRARY_SOFT_I2C_H

#include "main.h"


#define I2C_SDA_PIN_G IIC_SDA_GPIO_Port
#define I2C_SDA_PIN IIC_SDA_Pin
#define I2C_SCL_PIN_G IIC_SCL_GPIO_Port
#define I2C_SCL_PIN IIC_SCL_Pin

#define I2C_SDA_L HAL_GPIO_WritePin(I2C_SDA_PIN_G,I2C_SDA_PIN,GPIO_PIN_RESET)
#define I2C_SDA_H HAL_GPIO_WritePin(I2C_SDA_PIN_G,I2C_SDA_PIN,GPIO_PIN_SET)
#define I2C_SCL_L HAL_GPIO_WritePin(I2C_SCL_PIN_G,I2C_SCL_PIN,GPIO_PIN_RESET)
#define I2C_SCL_H HAL_GPIO_WritePin(I2C_SCL_PIN_G,I2C_SCL_PIN,GPIO_PIN_SET)

#define I2C_DelayUs(us) HAL_Delay(us)

#define I2C_Read_SDA HAL_GPIO_ReadPin(IIC_SDA_GPIO_Port,IIC_SDA_Pin)

void I2C_Init(void);

void I2C_Start(void);

void I2C_Stop(void);

int I2C_Send(uint8_t data);

uint8_t I2C_Receive(int ack);

#endif //CCY_STM32_LIBRARY_SOFT_I2C_H
