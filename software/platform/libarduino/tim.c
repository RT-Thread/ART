#include <rtthread.h>
#include "tim.h"
#include "pin_table.h"

static const pin_to_timer_index_t pin_to_timer_index[] =
{
    /* 0  */ {NOT_A_TIM},
    /* 1  */ {NOT_A_TIM},
    /* 2  */ {TIM3, RCC_APB1Periph_TIM3, TIM_Channel_3, GPIO_PinSource8, GPIO_AF_TIM3},
    /* 3  */ {TIM2, RCC_APB1Periph_TIM2, TIM_Channel_4, GPIO_PinSource11, GPIO_AF_TIM2},
    /* 4  */ {TIM1, RCC_APB2Periph_TIM1, TIM_Channel_4, GPIO_PinSource14, GPIO_AF_TIM1},

    /* 5  */ {TIM1, RCC_APB2Periph_TIM1, TIM_Channel_3, GPIO_PinSource13, GPIO_AF_TIM1},
    /* 6  */ {TIM1, RCC_APB2Periph_TIM1, TIM_Channel_2, GPIO_PinSource11, GPIO_AF_TIM1},
    /* 7  */ {TIM1, RCC_APB2Periph_TIM1, TIM_Channel_1, GPIO_PinSource9, GPIO_AF_TIM1},
    /* 8  */ {TIM4, RCC_APB1Periph_TIM4, TIM_Channel_1, GPIO_PinSource12, GPIO_AF_TIM4},
    /* 9  */ {TIM4, RCC_APB1Periph_TIM4, TIM_Channel_2, GPIO_PinSource13, GPIO_AF_TIM4},

    /* 10 */ {TIM4, RCC_APB1Periph_TIM4, TIM_Channel_3, GPIO_PinSource14, GPIO_AF_TIM4},
    /* 11 */ {TIM4, RCC_APB1Periph_TIM4, TIM_Channel_4, GPIO_PinSource15, GPIO_AF_TIM4},
    /* 12 */ {TIM9, RCC_APB2Periph_TIM9, TIM_Channel_2, GPIO_PinSource6, GPIO_AF_TIM9},
    /* 13 */ {TIM9, RCC_APB2Periph_TIM9, TIM_Channel_1, GPIO_PinSource5, GPIO_AF_TIM9},
};

static uint32_t pwm_frequency[14];

#define ITEM_NUM(items)	sizeof(items)/sizeof(items[0])
const pin_to_timer_index_t *pin_to_timer(uint8_t pin)
{
    if (pin >= ITEM_NUM(pin_to_timer_index))
        return RT_NULL;
    else
        return pin_to_timer_index + pin;
}

void pwmConfig(uint8_t pin, uint8_t duty_cycle, unsigned int frequency, unsigned int clock)
{
    TIM_OCInitTypeDef TIM_OCInitStructure;
    uint16_t APBx_Prescaler;
    uint16_t PrescalerValue;
    const pin_to_timer_index_t *timer_index;
    const struct pin_index *pin_index_p;
    uint32_t period = clock / frequency - 1;

    RT_ASSERT(period <= UINT16_MAX);

    pin_index_p = get_pin(pin);
    if(pin_index_p == RT_NULL)
    {
        return;
    }

    timer_index = pin_to_timer(pin);
    if(!timer_index || !timer_index->tim)
        return;

    /* TIM clock enable */
    if( (timer_index->tim == TIM2) || (timer_index->tim == TIM3)
            || (timer_index->tim == TIM4) )
    {
        APBx_Prescaler = 4;
        RCC_APB1PeriphClockCmd(timer_index->tim_rcc, ENABLE);
    }
    else
    {
        APBx_Prescaler = 2;
        RCC_APB2PeriphClockCmd(timer_index->tim_rcc, ENABLE);
    }

    /* GPIO clock enable */
    RCC_AHB1PeriphClockCmd(pin_index_p->rcc, ENABLE);

    /* GPIO configuration */
    {
        GPIO_InitTypeDef GPIO_InitStructure;
        GPIO_InitStructure.GPIO_Pin = pin_index_p->pin;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_Init(pin_index_p->gpio, &GPIO_InitStructure);
    }
    /* Connect TIM3 pins to AF */
    GPIO_PinAFConfig(pin_index_p->gpio, timer_index->pin_source, timer_index->gpio_af);

    /* Compute the prescaler value */
    PrescalerValue = (SystemCoreClock * 2 / APBx_Prescaler / clock) - 1;
    RT_ASSERT(PrescalerValue <= UINT16_MAX);
    PrescalerValue &= 0XFFFF;

    /* Time base configuration */
    if(pwm_frequency[pin] != frequency)
    {
        TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
        TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
        TIM_TimeBaseStructure.TIM_Period = period;
        TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
        TIM_TimeBaseStructure.TIM_ClockDivision = 0;
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
        TIM_TimeBaseInit(timer_index->tim, &TIM_TimeBaseStructure);
        pwm_frequency[pin] = frequency;
    }

    /* PWM1 Mode configuration: Channel1 */
    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = period * duty_cycle / UINT8_MAX;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    switch (timer_index->tim_channel)
    {
    case TIM_Channel_1:
        TIM_OC1Init(timer_index->tim, &TIM_OCInitStructure);
        TIM_OC1PreloadConfig(timer_index->tim, TIM_OCPreload_Enable);
        break;
    case TIM_Channel_2:
        TIM_OC2Init(timer_index->tim, &TIM_OCInitStructure);
        TIM_OC2PreloadConfig(timer_index->tim, TIM_OCPreload_Enable);
        break;
    case TIM_Channel_3:
        TIM_OC3Init(timer_index->tim, &TIM_OCInitStructure);
        TIM_OC3PreloadConfig(timer_index->tim, TIM_OCPreload_Enable);
        break;
    case TIM_Channel_4:
        TIM_OC4Init(timer_index->tim, &TIM_OCInitStructure);
        TIM_OC4PreloadConfig(timer_index->tim, TIM_OCPreload_Enable);
        break;
    default:
        RT_ASSERT(0);
        return;
    }

    TIM_ARRPreloadConfig(timer_index->tim, ENABLE);
    TIM_Cmd(timer_index->tim, ENABLE);
}
