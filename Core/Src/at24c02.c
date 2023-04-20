//
// Created by 83503 on 2023/4/20.
//
#include "at24c02.h"

void C02_Init(void) {
    I2C_Init();
}

int C02_Write(uint8_t address, uint8_t data) {
    I2C_Start();
    //发送起始地址
    if (!I2C_Send(0xA0)) {
        I2C_Stop();
        return 0;
    }
    //发内存地址
    if (!I2C_Send(address)) {
        I2C_Stop();
        return 0;
    }
    //发数据
    if (!I2C_Send(data)) {
        I2C_Stop();
        return 0;
    }
    I2C_Stop();
    //等待写入完成
    C02_Delay(10);
    return 1;
}

uint8_t C02_Read(uint8_t address) {
    uint8_t data;
    I2C_Start();
    //发送起始地址
    if (!I2C_Send(0xA0)) {
        I2C_Stop();
        return 0;
    }
    //发内存地址
    if (!I2C_Send(address)) {
        I2C_Stop();
        return 0;
    }
    //再次发送开始信号
    I2C_Start();
    if (!I2C_Send(0xA1)) {
        I2C_Stop();
        return 0;
    }
    //接受数据然后发数非应答
    data = I2C_Receive(0);
    I2C_Stop();
    return data;
}