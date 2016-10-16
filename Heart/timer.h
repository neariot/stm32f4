#ifndef __TIMER_H
#define __TIMER_H

#include "stm32f4xx_conf.h"

#define CODEC_FLAG_TIMEOUT             ((uint32_t)0x1000)
#define CODEC_LONG_TIMEOUT             ((uint32_t)(300 * CODEC_FLAG_TIMEOUT))
void TIM4_Config(void);
void TIM3_Config(void);
//void TIM3_Cinit(void);
void USART3_IRQHandler(void) ;

#endif
