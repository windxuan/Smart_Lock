#ifndef _systick_timer_h
#define _systick_timer_h

#include "stm32f10x.h"

void systick_config(u8 sysclk);
void systick_Nms(int Nms);
void SysTicks_NusDelay(uint32_t Nus);

#endif
