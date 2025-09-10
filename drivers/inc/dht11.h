#ifndef _DHT11_H_
#define _DHT11_H_

#include "stm32f10x.h"
#include "tim.h"

//高电平及低电平
#define dht11_high() GPIO_SetBits(GPIOA,GPIO_Pin_8)
#define dht11_low()  GPIO_ResetBits(GPIOA,GPIO_Pin_8)

// 读取温湿度的数据
#define dht11_data GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)

extern volatile unsigned int data[4];

void dhtll_init_out(void);
void dhtll_init_in(void);
void DHT11_start(void);
char DHT11_RBYTE(void);
void GET_DATA(void);

#endif
