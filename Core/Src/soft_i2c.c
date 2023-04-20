//
// Created by 83503 on 2023/4/20.
//
#include "soft_i2c.h"


void I2C_SDA_Out() {
    GPIO_InitTypeDef gpioInitTypeDef;
    gpioInitTypeDef.Mode = GPIO_MODE_OUTPUT_OD;
    gpioInitTypeDef.Speed = GPIO_SPEED_FREQ_HIGH;
    gpioInitTypeDef.Pull = GPIO_NOPULL;
    gpioInitTypeDef.Pin = I2C_SDA_PIN;
    HAL_GPIO_Init(I2C_SDA_PIN_G, &gpioInitTypeDef);
}

void I2C_SDA_In() {
    GPIO_InitTypeDef gpioInitTypeDef;
    gpioInitTypeDef.Mode = GPIO_MODE_INPUT;
    gpioInitTypeDef.Speed = GPIO_SPEED_FREQ_HIGH;
    gpioInitTypeDef.Pull = GPIO_NOPULL;
    gpioInitTypeDef.Pin = I2C_SDA_PIN;
    HAL_GPIO_Init(I2C_SDA_PIN_G, &gpioInitTypeDef);
}

/**
 * 初始化
 */
void I2C_Init(void) {
    I2C_SDA_Out();
    I2C_SDA_H;
    I2C_SCL_H;
}

/**
 * 开始位（Start）：当SCL=1时，SDA由1向0跳变
 */
void I2C_Start(void) {
    I2C_SDA_Out();
    I2C_SDA_H;
    I2C_SCL_H;
    I2C_DelayUs(2);
    I2C_SDA_L;
    I2C_DelayUs(2);
    //最后拉低SCL数据线准备发送数据进行做准备
    I2C_SCL_L;
}

/**
 * 停止位（Stop）：当SCL=1时，SDA由0向1跳变；
 */
void I2C_Stop(void) {
    I2C_SDA_Out();
    I2C_SDA_L;
    I2C_DelayUs(2);
    I2C_SCL_H;
    I2C_DelayUs(2);
    I2C_SDA_H;
    I2C_DelayUs(2);
}

/**
 *否应答位（NACK）：当发送方传送完8位时，发送方释放SDA，由接收方控制SDA，且SDA=1。
 */
void I2C_NAck() {
    I2C_SCL_L;
    I2C_DelayUs(2);
    I2C_SDA_H;
    I2C_DelayUs(2);
    I2C_SCL_H;
    I2C_DelayUs(2);
    I2C_SCL_L;
}

/**
 *应答位（ACK）：当发送方传送完8位时，发送方释放SDA(高电平释放)，由接收方控制SDA，且SDA=0；
 */
void I2C_Ack() {
    I2C_SCL_L;
    I2C_DelayUs(2);
    I2C_SDA_L;
    I2C_DelayUs(2);
    I2C_SCL_H;
    I2C_DelayUs(2);
    I2C_SCL_L;
    I2C_SDA_H; //释放SDA总线
}


/**
 * 检测应答信号 发送方释放SDA(高电平释放)，由接收方控制SDA
 * @return  1成功,0失败
 */
uint8_t I2C_CheckAck() {
    uint8_t ack = 0, errorRetry = 3;
    I2C_SDA_H;
    I2C_DelayUs(1);
    I2C_SDA_In();
    I2C_DelayUs(1);
    I2C_SCL_H;
    I2C_DelayUs(1);
    while (I2C_Read_SDA) {
        if (errorRetry <= 0) {
            break;
        }
        delay_us(1);
        errorRetry--;
    }
    if (errorRetry) {
        ack = 1;
    }
    I2C_SCL_L;
    return ack;
}

/**
 * 当SCL由0向1跳变时，由发送方控制SDA，此时SDA为有效数据，不可随意改变SDA；当SCL保持为0时，SDA上的数据可随意改变；
 * @param data  数据
 * @param ackState 发完数据ACK未的状态
 */
int I2C_Send(uint8_t data) {
    I2C_SDA_Out();
    I2C_SCL_L;
    I2C_DelayUs(2);
    for (int i = 0; i < 8; i++) {
        if (data & 0x80) {
            I2C_SDA_H;
        } else {
            I2C_SDA_L;
        }
        I2C_DelayUs(2);
        I2C_SCL_H;
        I2C_DelayUs(2);
        I2C_SCL_L;
        data <<= 1;
    }
    return I2C_CheckAck();
}

/**
 * 接受数据
 * @param ack
 * @return
 */
uint8_t I2C_Receive(int ack) {
    uint8_t receiveData = 0;
    I2C_SDA_In();
    for (int i = 0; i < 8; i++) {
        I2C_SCL_L;
        I2C_DelayUs(2);
        I2C_SCL_H;
        receiveData <<= 1;
        if (I2C_Read_SDA == GPIO_PIN_SET) {
            receiveData |= 0x01;
        }
        I2C_DelayUs(1);
    }
    ack ? I2C_Ack() : I2C_NAck();
    return receiveData;
}