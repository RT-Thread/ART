#ifndef TIM_H_INCLUDED
#define TIM_H_INCLUDED

#include <stm32f4xx.h>

typedef struct
{
    TIM_TypeDef *tim;
    uint32_t tim_rcc;

    IRQn_Type IRQn;
    uint16_t tim_channel;
    uint16_t TIM_IT;

    uint8_t pin_source;
    uint8_t gpio_af;
} pin_to_timer_index_t;

struct pulsein_node
{
    uint8_t state;
    uint8_t step;
    uint32_t value;
};

extern struct rt_event pulsein_event;

#define NOT_A_TIM RT_NULL

const pin_to_timer_index_t *pin_to_timer(uint8_t pin);

void TIM_config(uint8_t pin, uint32_t frequency);
void TIM_NVIC_Configuration(uint8_t pin);

void pwmConfig(uint8_t pin, uint8_t duty_cycle);
struct pulsein_node * get_pulsein_node(uint8_t pin);

#endif // TIM_H_INCLUDED
