#ifndef _BSP_ADC_H
#define _BSP_ADC_H
#include "stm32f4xx_adc.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "delay.h"
void mx_adc_init(void);
u16 mx_get_adc(u8 ch);
u16 get_adc_average(u8 ch,u8 times);
float get_real_adc_average(u8 ch,u8 times);
#endif
