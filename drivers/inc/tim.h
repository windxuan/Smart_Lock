#ifndef __TIM_H_
#define __TIM_H_

#include "stm32f10x.h"

void tim6_init(uint16_t prescaler);
void tim6_delay_ms(uint32_t ms);
void tim6_delay_us(uint32_t us);

void tim4_ch3_init(uint32_t prescaler, uint32_t period);
void TIM3_Int_Init(void); //TIM3≥ı ºªØ
void TIM7_Int_Init(u16 arr,u16 psc);

void turn_pwm_3(void);
void turn_pwm_1(void);
#endif  // __TIM_H_
