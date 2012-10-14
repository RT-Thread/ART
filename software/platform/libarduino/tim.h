#ifndef TIM_H_INCLUDED
#define TIM_H_INCLUDED

#include <stm32f4xx.h>

typedef struct
{
    TIM_TypeDef *tim;
    uint32_t tim_rcc;
    uint16_t tim_channel;
    uint8_t pin_source;
    uint8_t gpio_af;
} pin_to_timer_index_t;

#define NOT_A_TIM RT_NULL

const pin_to_timer_index_t *pin_to_timer(uint8_t pin);
void pwmConfig(uint8_t pin,
               uint8_t duty_cycle,
               unsigned int frequency,
               unsigned int clock);

#endif // TIM_H_INCLUDED
