#ifndef __ADC_H_
#define __ADC_H_

#include "stm32f10x.h"

#define AD1 (adc1_ch11_get_adkey_value()<=300)
#define AD2 ((adc1_ch11_get_adkey_value()>=400)&&( adc1_ch11_get_adkey_value()<=650))
#define AD3 ((adc1_ch11_get_adkey_value()>=760)&&( adc1_ch11_get_adkey_value()<=1100))
#define AD4 ((adc1_ch11_get_adkey_value()>=1150)&&(adc1_ch11_get_adkey_value()<=1300))
#define AD5 ((adc1_ch11_get_adkey_value()>=1350)&&(adc1_ch11_get_adkey_value()<=1700))

void adc1_ch11_init(void);
uint16_t adc1_ch11_get_adkey_value(void);

#endif  // __ADC_H_
