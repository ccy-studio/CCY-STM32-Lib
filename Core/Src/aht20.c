
#include "aht20.h"


void SensorAHT20_Delay_Us(uint32_t t) {
    AHT20_Delay_Us(t);
}

void Delay_4us(void) {
    AHT20_Delay_Us(4);
}

void Delay_5us(void) {
    AHT20_Delay_Us(5);
}

void Delay_1ms(uint32_t t) {
    AHT20_Delay_Ms(t);
}


void SDA_Pin_Output_High(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = AHT20_I2C_SDA_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(AHT20_I2C_SDA_PIN_G, &GPIO_InitStruct);
    HAL_GPIO_WritePin(AHT20_I2C_SDA_PIN_G, AHT20_I2C_SDA_PIN, GPIO_PIN_SET);
}

void SDA_Pin_Output_Low(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = AHT20_I2C_SDA_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(AHT20_I2C_SDA_PIN_G, &GPIO_InitStruct);
    HAL_GPIO_WritePin(AHT20_I2C_SDA_PIN_G, AHT20_I2C_SDA_PIN, GPIO_PIN_RESET);
}

void SDA_Pin_IN_FLOATING(void) {

    GPIO_InitTypeDef gpioInitTypeDef;
    gpioInitTypeDef.Mode = GPIO_MODE_INPUT;
    gpioInitTypeDef.Speed = GPIO_SPEED_FREQ_MEDIUM;
    gpioInitTypeDef.Pull = GPIO_NOPULL;
    gpioInitTypeDef.Pin = AHT20_I2C_SDA_PIN;
    HAL_GPIO_Init(AHT20_I2C_SDA_PIN_G, &gpioInitTypeDef);
}

void SCL_Pin_Output_High(void) {
    HAL_GPIO_WritePin(AHT20_I2C_SCL_PIN_G, AHT20_I2C_SCL_PIN, GPIO_PIN_SET);
}

void SCL_Pin_Output_Low(void) {
    HAL_GPIO_WritePin(AHT20_I2C_SCL_PIN_G, AHT20_I2C_SCL_PIN, GPIO_PIN_RESET);
}


void I2C_Start1(void) {
    SDA_Pin_Output_High();
    SensorAHT20_Delay_Us(8);
    SCL_Pin_Output_High();
    SensorAHT20_Delay_Us(8);
    SDA_Pin_Output_Low();
    SensorAHT20_Delay_Us(8);
    SCL_Pin_Output_Low();
    SensorAHT20_Delay_Us(8);
}


void AHT20_WR_Byte(uint8_t Byte) {
    uint8_t Data, N, i;
    Data = Byte;
    i = 0x80;
    for (N = 0; N < 8; N++) {
        SCL_Pin_Output_Low();
        Delay_4us();
        if (i & Data) {
            SDA_Pin_Output_High();
        } else {
            SDA_Pin_Output_Low();
        }

        SCL_Pin_Output_High();
        Delay_4us();
        Data <<= 1;

    }
    SCL_Pin_Output_Low();
    SensorAHT20_Delay_Us(8);
    SDA_Pin_IN_FLOATING();
    SensorAHT20_Delay_Us(8);

}


uint8_t AHT20_RD_Byte(void) {
    uint8_t Byte, i, a;
    Byte = 0;
    SCL_Pin_Output_Low();
    SDA_Pin_IN_FLOATING();
    SensorAHT20_Delay_Us(8);
    for (i = 0; i < 8; i++) {
        SCL_Pin_Output_High();
        Delay_5us();
        a = 0;
        if (HAL_GPIO_ReadPin(IIC_SDA_GPIO_Port, IIC_SDA_Pin)) a = 1;
        Byte = (Byte << 1) | a;
        SCL_Pin_Output_Low();
        Delay_5us();
    }
    SDA_Pin_IN_FLOATING();
    SensorAHT20_Delay_Us(8);
    return Byte;
}


uint8_t Receive_ACK(void) {
    uint16_t CNT;
    CNT = 0;
    SCL_Pin_Output_Low();
    SDA_Pin_IN_FLOATING();
    SensorAHT20_Delay_Us(8);
    SCL_Pin_Output_High();
    SensorAHT20_Delay_Us(8);
    while (HAL_GPIO_ReadPin(IIC_SDA_GPIO_Port, IIC_SDA_Pin) && CNT < 100)
        CNT++;
    if (CNT == 100) {
        return 0;
    }
    SCL_Pin_Output_Low();
    SensorAHT20_Delay_Us(8);
    return 1;
}

void Send_ACK(void) {
    SCL_Pin_Output_Low();
    SensorAHT20_Delay_Us(8);
    SDA_Pin_Output_Low();
    SensorAHT20_Delay_Us(8);
    SCL_Pin_Output_High();
    SensorAHT20_Delay_Us(8);
    SCL_Pin_Output_Low();
    SensorAHT20_Delay_Us(8);
    SDA_Pin_IN_FLOATING();
    SensorAHT20_Delay_Us(8);
}

void Send_NOT_ACK(void) {
    SCL_Pin_Output_Low();
    SensorAHT20_Delay_Us(8);
    SDA_Pin_Output_High();
    SensorAHT20_Delay_Us(8);
    SCL_Pin_Output_High();
    SensorAHT20_Delay_Us(8);
    SCL_Pin_Output_Low();
    SensorAHT20_Delay_Us(8);
    SDA_Pin_Output_Low();
    SensorAHT20_Delay_Us(8);
}

void Stop_I2C(void) {
    SDA_Pin_Output_Low();
    SensorAHT20_Delay_Us(8);
    SCL_Pin_Output_High();
    SensorAHT20_Delay_Us(8);
    SDA_Pin_Output_High();
    SensorAHT20_Delay_Us(8);
}

uint8_t AHT20_Read_Status(void) {

    uint8_t Byte_first;
    I2C_Start1();
    AHT20_WR_Byte(0x71);
    Receive_ACK();
    Byte_first = AHT20_RD_Byte();
    Send_NOT_ACK();
    Stop_I2C();
    return Byte_first;
}

void AHT20_SendAC(void) {

    I2C_Start1();
    AHT20_WR_Byte(0x70);
    Receive_ACK();
    AHT20_WR_Byte(0xac);
    Receive_ACK();
    AHT20_WR_Byte(0x33);
    Receive_ACK();
    AHT20_WR_Byte(0x00);
    Receive_ACK();
    Stop_I2C();

}

uint8_t Calc_CRC8(uint8_t *message, uint8_t Num) {
    uint8_t i;
    uint8_t byte;
    uint8_t crc = 0xFF;
    for (byte = 0; byte < Num; byte++) {
        crc ^= (message[byte]);
        for (i = 8; i > 0; --i) {
            if (crc & 0x80) crc = (crc << 1) ^ 0x31;
            else crc = (crc << 1);
        }
    }
    return crc;
}

void AHT20_Read_CTdata(uint32_t *ct) {
    volatile uint8_t Byte_1th = 0;
    volatile uint8_t Byte_2th = 0;
    volatile uint8_t Byte_3th = 0;
    volatile uint8_t Byte_4th = 0;
    volatile uint8_t Byte_5th = 0;
    volatile uint8_t Byte_6th = 0;
    uint32_t RetuData = 0;
    uint16_t cnt = 0;
    AHT20_SendAC();
    Delay_1ms(80);
    cnt = 0;
    while (((AHT20_Read_Status() & 0x80) == 0x80)) {
        SensorAHT20_Delay_Us(1508);
        if (cnt++ >= 100) {
            break;
        }
    }
    I2C_Start1();
    AHT20_WR_Byte(0x71);
    Receive_ACK();
    Byte_1th = AHT20_RD_Byte();
    Send_ACK();
    Byte_2th = AHT20_RD_Byte();
    Send_ACK();
    Byte_3th = AHT20_RD_Byte();
    Send_ACK();
    Byte_4th = AHT20_RD_Byte();
    Send_ACK();
    Byte_5th = AHT20_RD_Byte();
    Send_ACK();
    Byte_6th = AHT20_RD_Byte();
    Send_NOT_ACK();
    Stop_I2C();

    RetuData = (RetuData | Byte_2th) << 8;
    RetuData = (RetuData | Byte_3th) << 8;
    RetuData = (RetuData | Byte_4th);
    RetuData = RetuData >> 4;
    ct[0] = RetuData;
    RetuData = 0;
    RetuData = (RetuData | Byte_4th) << 8;
    RetuData = (RetuData | Byte_5th) << 8;
    RetuData = (RetuData | Byte_6th);
    RetuData = RetuData & 0xfffff;
    ct[1] = RetuData;

}


void AHT20_Read_CTdata_crc(uint32_t *ct) {
    volatile uint8_t Byte_1th = 0;
    volatile uint8_t Byte_2th = 0;
    volatile uint8_t Byte_3th = 0;
    volatile uint8_t Byte_4th = 0;
    volatile uint8_t Byte_5th = 0;
    volatile uint8_t Byte_6th = 0;
    volatile uint8_t Byte_7th = 0;
    uint32_t RetuData = 0;
    uint16_t cnt = 0;
    // uint8_t  CRCDATA=0;
    uint8_t CTDATA[6] = {0};

    AHT20_SendAC();
    Delay_1ms(80);
    cnt = 0;
    while (((AHT20_Read_Status() & 0x80) == 0x80)) {
        SensorAHT20_Delay_Us(1508);
        if (cnt++ >= 100) {
            break;
        }
    }

    I2C_Start1();

    AHT20_WR_Byte(0x71);
    Receive_ACK();
    CTDATA[0] = Byte_1th = AHT20_RD_Byte();
    Send_ACK();
    CTDATA[1] = Byte_2th = AHT20_RD_Byte();//???
    Send_ACK();
    CTDATA[2] = Byte_3th = AHT20_RD_Byte();//???
    Send_ACK();
    CTDATA[3] = Byte_4th = AHT20_RD_Byte();//???/???
    Send_ACK();
    CTDATA[4] = Byte_5th = AHT20_RD_Byte();//???
    Send_ACK();
    CTDATA[5] = Byte_6th = AHT20_RD_Byte();//???
    Send_ACK();
    Byte_7th = AHT20_RD_Byte();//CRC????
    Send_NOT_ACK();                           //???: ????????NAK
    Stop_I2C();

    if (Calc_CRC8(CTDATA, 6) == Byte_7th) {
        RetuData = (RetuData | Byte_2th) << 8;
        RetuData = (RetuData | Byte_3th) << 8;
        RetuData = (RetuData | Byte_4th);
        RetuData = RetuData >> 4;
        ct[0] = RetuData;//???
        RetuData = 0;
        RetuData = (RetuData | Byte_4th) << 8;
        RetuData = (RetuData | Byte_5th) << 8;
        RetuData = (RetuData | Byte_6th);
        RetuData = RetuData & 0xfffff;
        ct[1] = RetuData; //???

    } else {
        ct[0] = 0x00;
        ct[1] = 0x00;//У?????????????????????????????
    }//CRC????
}


void JH_Reset_REG(uint8_t addr) {

    uint8_t Byte_first, Byte_second, Byte_third, Byte_fourth;
    I2C_Start1();
    AHT20_WR_Byte(0x70);//?????0x70
    Receive_ACK();
    AHT20_WR_Byte(addr);
    Receive_ACK();
    AHT20_WR_Byte(0x00);
    Receive_ACK();
    AHT20_WR_Byte(0x00);
    Receive_ACK();
    Stop_I2C();

    Delay_1ms(5);//???5ms????
    I2C_Start1();
    AHT20_WR_Byte(0x71);//
    Receive_ACK();
    Byte_first = AHT20_RD_Byte();
    Send_ACK();
    Byte_second = AHT20_RD_Byte();
    Send_ACK();
    Byte_third = AHT20_RD_Byte();
    Send_NOT_ACK();
    Stop_I2C();

    Delay_1ms(10);//???10ms????
    I2C_Start1();
    AHT20_WR_Byte(0x70);///
    Receive_ACK();
    AHT20_WR_Byte(0xB0 | addr);////?????????
    Receive_ACK();
    AHT20_WR_Byte(Byte_second);
    Receive_ACK();
    AHT20_WR_Byte(Byte_third);
    Receive_ACK();
    Stop_I2C();

    Byte_second = 0x00;
    Byte_third = 0x00;
}

void AHT20_Start_Init(void) {
    JH_Reset_REG(0x1b);
    JH_Reset_REG(0x1c);
    JH_Reset_REG(0x1e);
}

static uint32_t CT_data[2];


void aht20_init() {
    if ((AHT20_Read_Status() & 0x18) != 0x18) {
        AHT20_Start_Init(); //重新初始化寄存器，一般不需要此初始化，只有当读回的状态字节不正确时才初始化AHT20
        Delay_1ms(10);
    }
}

void aht20_read(float *humidity, float *temp) {
    AHT20_Read_CTdata(CT_data);  //读取温度和湿度 ， 可间隔1.5S读一次
    *humidity = ((float) CT_data[0] * 100) / 0x100000;
    *temp = ((float) CT_data[1] * 200 / 0x100000) - 50;
}