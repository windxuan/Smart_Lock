#ifndef __KEY_H_
#define __KEY_H_

#include "stm32f10x.h"

#define WiFi_Key   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)

void WiFi_Key_Init(void);
void WiFi_Key_Switch(void);

#endif
