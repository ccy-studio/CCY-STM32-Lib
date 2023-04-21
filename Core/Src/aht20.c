//
// Created by 83503 on 2023/4/21.
//
#include "aht20.h"
#include "delay.h"

uint32_t t, h;

/**
 * 读取状态位
 * @return
 */
uint8_t aht20_GetStatus() {
    uint8_t data;
    I2C_Start();
    if (!I2C_Send(AHT20_READ)) {
        return 0;
    }
    data = I2C_Receive(0);
    I2C_Stop();
    return data;
}

/**
 * 校准初始化
 */
void aht20_calibration() {
    I2C_Start();
    if (!I2C_Send(AHT20_WRITE)) {
        Error_Handler();
        return;
    }
    if (!I2C_Send(0xBE)) {
        return;
    }
    if (!I2C_Send(0x80)) {
        return;
    }
    if (!I2C_Send(0x00)) {
        return;
    }
    I2C_Stop();
    delay_ms(5);
}

void aht20_init(void) {
    delay_us(40);
    I2C_Init();
    //获取状态位
    uint8_t status = aht20_GetStatus();
    if (!status) {
        Error_Handler();
        return;
    }
    if (!(status & (1 << 2))) {
        //进行初始化校准
        aht20_calibration();
    }
}


void aht20_read(double *temp, double *humidity) {
    //发数测量命令
    I2C_Start();
    if (!I2C_Send(AHT20_WRITE)) {
        Error_Handler();
        return;
    }
    if (!I2C_Send(0xAC)) {
        Error_Handler();
        return;
    }
    if (!I2C_Send(0x33)) {
        Error_Handler();
        return;
    }
    if (!I2C_Send(0x00)) {
        Error_Handler();
        return;
    }
    I2C_Stop();
    delay_ms(80);

    //判断当前工作状态是否是忙
    if ((aht20_GetStatus() & (1 << 6))) {
        //是忙状态就退出不更新数据
        delay_ms(10);
        return;
    }
    //读取数据
    uint8_t ctd[6] = {0};
    I2C_Start();
    if (!I2C_Send(AHT20_READ)) {
        Error_Handler();
        return;
    }
    ctd[0] = I2C_Receive(1);    //状态
    ctd[1] = I2C_Receive(1);    //湿度0
    ctd[2] = I2C_Receive(1);    //湿度1
    ctd[3] = I2C_Receive(1);    //湿度/温度
    ctd[4] = I2C_Receive(1);    //温度
    ctd[5] = I2C_Receive(0);    //温度
    I2C_Stop();

    if (!(ctd[0] & (1 << 6))) {
        //不忙的状态下
        //计算湿度
        h = ctd[1];
        h = (h << 8) | ctd[2];
        h = (h << 8) | ctd[3];
        h >>= 4;
        h = (h / 1048576) * 100;

        //计算温度
        t = ctd[3];
        //T1 = T1 & 0x0000000F;
        t = (t << 8) | ctd[4];
        t = (t << 8) | ctd[5];
        t <<= 4;
        t = (t / 1048576) * 200 - 50;

        *humidity = (double) h;
        *temp = (double) t;
    }
}