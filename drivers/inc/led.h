#ifndef __LED_H_
#define __LED_H_

#include "stm32f10x.h"
#include "bitband.h"

#define LED PCout(8)

#define LED_ON()  do {LED = 0;} while(0)
#define LED_OFF() do {LED = 1;} while(0)

void LED_INIT(void);

#endif
