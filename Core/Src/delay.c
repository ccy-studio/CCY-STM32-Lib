//
// Created by Chenzedeng on 2023/4/20.
//
#include "delay.h"

static int fac_us = 0;

/**
 *  配置时钟源 --> 72MHz / 8 = 9MHz，
	滴答定时器每计数一次所需时间为 T = 1/(9MHz) s，即 1us = 10^(-6)s ,
	T = (1/9) * 10^(-6) s = 1/9 us , 即每计数一次的时间为1/9微秒
	换言之，系统时钟频率为72MHz时，1us需要计数9次

 * @param sysMhz  输入系统的配置的频率
 */
void delay_init(int sysMhz) {
    SysTick->CTRL = 0; //复位此（控制及状态）寄存的所有的状态 0为复位
    //进行8分频
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK_DIV8);
    //这里记录下分频后的频率
    fac_us = sysMhz / 8;
}

/**
 * 微秒的延迟
 * @param us
 */
void delay_us(uint32_t us) {
    uint32_t ctrlState;
    SysTick->VAL = 0; //计数器重置0
    // 设置重载的值：需要计数的值。1Mhz是1微秒所以拿分频的系数乘以要延时的数字凑够1us
    SysTick->LOAD = us * fac_us;
    //使能tick开始倒数计数
    SysTick->CTRL |= 0x01;
    do {
        ctrlState = SysTick->CTRL;
    } while ((ctrlState & 0x01) && !(ctrlState & (1 << 16)));
    //第一个条件是寄存器0位enable状态是使能的，第二个1左移16位判断是否是数到了0
    SysTick->CTRL &= 0x00; //关闭定时器
}

/**
 * 毫秒的延迟
 * 这种的写法是应对超频需要计算超频适配
 * @param ms
 */
void delay_ms(uint32_t ms) {
    //这里因为在F1中SysTick是24位可以存储 2^24次方个数为：16777216
    //- 按照分频72/8 =9Mhz 单次时间为1/9Mhz(单位微秒) 计数满是：16777216*1/9Mhz(单位微秒)
    //- 计算结果是：1.864135111111111 秒约算做 1.864秒 9M可以累计计数这么多
    //假设超频F103，15倍频的话，即主频为120Mhz。8分频后=15
    //计满时间为： 16777216*1/120Mhz(单位微秒) = 1.118481066666667秒
    uint32_t one_second_ms = 1864; //这里取值就是实际计算后的值
    //拿到有几个秒
    int s_ms = ms / one_second_ms;
    //那到有几个微秒就是小数点无法整除的
    int s_us = ms % one_second_ms;
    while (s_ms) {
        delay_us(one_second_ms * 1000);
        s_ms--;
    }
    if (s_us) {
        delay_us(s_us * 1000);
    }
}

/**
 * 秒的延迟
 * @param s
 */
void delay_s(uint16_t s) {
    while (s) {
        delay_ms(1000);
        s--;
    }
}