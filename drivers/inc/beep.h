#ifndef __BEEP_H_
#define __BEEP_H_

#include "stm32f10x.h"

#define BEEP_ON()  GPIO_ResetBits(GPIOC, GPIO_Pin_9)
#define BEEP_OFF() GPIO_SetBits(GPIOC, GPIO_Pin_9)

void beep_init(void);

#endif  // __BEEP_H_
