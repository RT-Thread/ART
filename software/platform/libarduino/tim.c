#include <rtthread.h>

#include "libarduino.h"
#include "tim.h"
#include "pin_table.h"
#include "board.h"

static const pin_to_timer_index_t pin_to_timer_index[] =
{
    /* 0  */ {NOT_A_TIM},
    /* 1  */ {NOT_A_TIM},
    /* 2  */ {TIM3, RCC_APB1Periph_TIM3, TIM3_IRQn,          TIM_Channel_3, TIM_IT_CC3, GPIO_PinSource8,  GPIO_AF_TIM3},
    /* 3  */ {TIM2, RCC_APB1Periph_TIM2, TIM2_IRQn,          TIM_Channel_4, TIM_IT_CC4, GPIO_PinSource11, GPIO_AF_TIM2},
    /* 4  */ {TIM1, RCC_APB2Periph_TIM1, TIM1_CC_IRQn,       TIM_Channel_4, TIM_IT_CC4, GPIO_PinSource14, GPIO_AF_TIM1},

    /* 5  */ {TIM1, RCC_APB2Periph_TIM1, TIM1_CC_IRQn,       TIM_Channel_3, TIM_IT_CC3, GPIO_PinSource13, GPIO_AF_TIM1},
    /* 6  */ {TIM1, RCC_APB2Periph_TIM1, TIM1_CC_IRQn,       TIM_Channel_2, TIM_IT_CC2, GPIO_PinSource11, GPIO_AF_TIM1},
    /* 7  */ {TIM1, RCC_APB2Periph_TIM1, TIM1_CC_IRQn,       TIM_Channel_1, TIM_IT_CC1, GPIO_PinSource9,  GPIO_AF_TIM1},
    /* 8  */ {TIM4, RCC_APB1Periph_TIM4, TIM4_IRQn,          TIM_Channel_1, TIM_IT_CC1, GPIO_PinSource12, GPIO_AF_TIM4},
    /* 9  */ {TIM4, RCC_APB1Periph_TIM4, TIM4_IRQn,          TIM_Channel_2, TIM_IT_CC2, GPIO_PinSource13, GPIO_AF_TIM4},

    /* 10 */ {TIM4, RCC_APB1Periph_TIM4, TIM4_IRQn,          TIM_Channel_3, TIM_IT_CC3, GPIO_PinSource14, GPIO_AF_TIM4},
    /* 11 */ {TIM4, RCC_APB1Periph_TIM4, TIM4_IRQn,          TIM_Channel_4, TIM_IT_CC4, GPIO_PinSource15, GPIO_AF_TIM4},
    /* 12 */ {TIM9, RCC_APB2Periph_TIM9, TIM1_BRK_TIM9_IRQn, TIM_Channel_2, TIM_IT_CC2, GPIO_PinSource6,  GPIO_AF_TIM9},
    /* 13 */ {TIM9, RCC_APB2Periph_TIM9, TIM1_BRK_TIM9_IRQn, TIM_Channel_1, TIM_IT_CC1, GPIO_PinSource5,  GPIO_AF_TIM9},
};

static uint32_t pwm_frequency[14];
static struct pulsein_node pulsein_table[14];
struct rt_event pulsein_event;

#define ITEM_NUM(items) sizeof(items)/sizeof(items[0])
const pin_to_timer_index_t *pin_to_timer(uint8_t pin)
{
    if(pin >= ITEM_NUM(pin_to_timer_index))
        return RT_NULL;
    else
        return pin_to_timer_index + pin;
}

struct pulsein_node *get_pulsein_node(uint8_t pin)
{
    return &pulsein_table[pin];
};

#define PWM_COUNTER_CLOCK       1000000 // choose a value which will result in proper prescalar value
#define PWM_FREQUENCY_DEFAULT   1000    // choose a value which will result in proper period

void TIM_config(uint8_t pin, uint32_t frequency)
{
    uint32_t period;

    if((frequency == 0) && (pwm_frequency[pin] == 0))
    {
        frequency = PWM_FREQUENCY_DEFAULT;
    }
    else if(frequency == 0)
    {
        frequency = pwm_frequency[pin];
    }

    /* Time base configuration */
    if(pwm_frequency[pin] != frequency)
    {
        TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

        uint16_t APBx_Prescaler;
        uint16_t PrescalerValue;
        const pin_to_timer_index_t *timer_index;

        pwm_frequency[pin] = frequency;
        period = PWM_COUNTER_CLOCK / pwm_frequency[pin] - 1;

        timer_index = pin_to_timer(pin);
        if(!timer_index || !timer_index->tim)
        {
            return;
        }
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

        /* Compute the prescaler value */
        PrescalerValue = (SystemCoreClock * 2 / APBx_Prescaler / PWM_COUNTER_CLOCK)
                         - 1;
        RT_ASSERT(PrescalerValue <= UINT16_MAX);
        PrescalerValue &= 0XFFFF;

        TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
        TIM_TimeBaseStructure.TIM_Period = period;
        TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
        TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

        TIM_TimeBaseInit(timer_index->tim, &TIM_TimeBaseStructure);
    }
}

void TIM_NVIC_Configuration(uint8_t pin)
{
    const pin_to_timer_index_t *timer_index;
    NVIC_InitTypeDef NVIC_InitStructure;

    timer_index = pin_to_timer(pin);
    if(!timer_index || !timer_index->tim)
    {
        return;
    }

    /* Enable the TIM5 global Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = timer_index->IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void pwmConfig(uint8_t pin, uint8_t duty_cycle)
{
    TIM_OCInitTypeDef TIM_OCInitStructure;
    const struct pin_index *pin_index_p;
    const pin_to_timer_index_t *timer_index;
    uint32_t period = PWM_COUNTER_CLOCK / pwm_frequency[pin] - 1;

    pin_index_p = get_pin(pin);
    if(pin_index_p == RT_NULL)
    {
        return;
    }

    timer_index = pin_to_timer(pin);
    if(!timer_index || !timer_index->tim)
    {
        return;
    }

    TIM_config(pin, pwm_frequency[pin]);

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

    //TIM_config(pin, pwm_frequency[pin]);

    /* PWM1 Mode configuration. */
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

    if(timer_index->tim == TIM1)
        TIM_CtrlPWMOutputs(TIM1, ENABLE);

    TIM_ARRPreloadConfig(timer_index->tim, ENABLE);
    TIM_Cmd(timer_index->tim, ENABLE);
}


/**/
void TIM1_CC_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM1, TIM_IT_CC1) == SET)
    {
        uint8_t pin = 7;
        const pin_to_timer_index_t *timer_index;

        timer_index = pin_to_timer(pin);

        if((pulsein_table[pin].step == 0)
                && (digitalRead(pin) == pulsein_table[pin].state))
        {
            pulsein_table[pin].step = 1;
            pulsein_table[pin].value = rt_hw_tick_get_microsecond();
        }
        else if(pulsein_table[pin].step == 1)
        {
            uint32_t tmp = rt_hw_tick_get_microsecond();

            pulsein_table[pin].step = 2;

            if(tmp < pulsein_table[pin].value)
            {
                pulsein_table[pin].value = (0xFFFFFFFF -  pulsein_table[pin].value) + tmp;
            }
            else
            {
                pulsein_table[pin].value = tmp - pulsein_table[pin].value;
            }

            TIM_ITConfig(timer_index->tim, timer_index->TIM_IT, DISABLE);
            rt_event_send(&pulsein_event, (1 << pin));
        }

        /* Clear TIM Capture compare interrupt pending bit */
        TIM_ClearITPendingBit(timer_index->tim, timer_index->TIM_IT);
    } /* TIM_IT_CC1 */

    if(TIM_GetITStatus(TIM1, TIM_IT_CC2) == SET)
    {
        uint8_t pin = 6;
        const pin_to_timer_index_t *timer_index;

        timer_index = pin_to_timer(pin);

        if((pulsein_table[pin].step == 0)
                && (digitalRead(pin) == pulsein_table[pin].state))
        {
            pulsein_table[pin].step = 1;
            pulsein_table[pin].value = rt_hw_tick_get_microsecond();
        }
        else if(pulsein_table[pin].step == 1)
        {
            uint32_t tmp = rt_hw_tick_get_microsecond();

            pulsein_table[pin].step = 2;

            if(tmp < pulsein_table[pin].value)
            {
                pulsein_table[pin].value = (0xFFFFFFFF -  pulsein_table[pin].value) + tmp;
            }
            else
            {
                pulsein_table[pin].value = tmp - pulsein_table[pin].value;
            }

            TIM_ITConfig(timer_index->tim, timer_index->TIM_IT, DISABLE);
            rt_event_send(&pulsein_event, (1 << pin));
        }
        /* Clear TIM Capture compare interrupt pending bit */
        TIM_ClearITPendingBit(TIM1, TIM_IT_CC2);
    }

    if(TIM_GetITStatus(TIM1, TIM_IT_CC3) == SET)
    {
        uint8_t pin = 5;
        const pin_to_timer_index_t *timer_index;

        timer_index = pin_to_timer(pin);

        if((pulsein_table[pin].step == 0)
                && (digitalRead(pin) == pulsein_table[pin].state))
        {
            pulsein_table[pin].step = 1;
            pulsein_table[pin].value = rt_hw_tick_get_microsecond();
        }
        else if(pulsein_table[pin].step == 1)
        {
            uint32_t tmp = rt_hw_tick_get_microsecond();

            pulsein_table[pin].step = 2;

            if(tmp < pulsein_table[pin].value)
            {
                pulsein_table[pin].value = (0xFFFFFFFF -  pulsein_table[pin].value) + tmp;
            }
            else
            {
                pulsein_table[pin].value = tmp - pulsein_table[pin].value;
            }

            TIM_ITConfig(timer_index->tim, timer_index->TIM_IT, DISABLE);
            rt_event_send(&pulsein_event, (1 << pin));
        }
        /* Clear TIM Capture compare interrupt pending bit */
        TIM_ClearITPendingBit(TIM1, TIM_IT_CC3);
    }

    if(TIM_GetITStatus(TIM1, TIM_IT_CC4) == SET)
    {
        uint8_t pin = 4;
        const pin_to_timer_index_t *timer_index;

        timer_index = pin_to_timer(pin);

        if((pulsein_table[pin].step == 0)
                && (digitalRead(pin) == pulsein_table[pin].state))
        {
            pulsein_table[pin].step = 1;
            pulsein_table[pin].value = rt_hw_tick_get_microsecond();
        }
        else if(pulsein_table[pin].step == 1)
        {
            uint32_t tmp = rt_hw_tick_get_microsecond();

            pulsein_table[pin].step = 2;

            if(tmp < pulsein_table[pin].value)
            {
                pulsein_table[pin].value = (0xFFFFFFFF -  pulsein_table[pin].value) + tmp;
            }
            else
            {
                pulsein_table[pin].value = tmp - pulsein_table[pin].value;
            }

            TIM_ITConfig(timer_index->tim, timer_index->TIM_IT, DISABLE);
            rt_event_send(&pulsein_event, (1 << pin));
        }
        /* Clear TIM Capture compare interrupt pending bit */
        TIM_ClearITPendingBit(TIM1, TIM_IT_CC4);
    }
}

void TIM4_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM4, TIM_IT_CC1) == SET)
    {
        uint8_t pin = 8;
        const pin_to_timer_index_t *timer_index;

        timer_index = pin_to_timer(pin);

        if((pulsein_table[pin].step == 0)
                && (digitalRead(pin) == pulsein_table[pin].state))
        {
            pulsein_table[pin].step = 1;
            pulsein_table[pin].value = rt_hw_tick_get_microsecond();
        }
        else if(pulsein_table[pin].step == 1)
        {
            uint32_t tmp = rt_hw_tick_get_microsecond();

            pulsein_table[pin].step = 2;

            if(tmp < pulsein_table[pin].value)
            {
                pulsein_table[pin].value = (0xFFFFFFFF -  pulsein_table[pin].value) + tmp;
            }
            pulsein_table[pin].value = tmp - pulsein_table[pin].value;

            TIM_ITConfig(timer_index->tim, timer_index->TIM_IT, DISABLE);
            rt_event_send(&pulsein_event, (1 << pin));
        }

        /* Clear TIM Capture compare interrupt pending bit */
        TIM_ClearITPendingBit(timer_index->tim, timer_index->TIM_IT);
    } /* TIM_IT_CC1 */

    if(TIM_GetITStatus(TIM4, TIM_IT_CC2) == SET)
    {
        uint8_t pin = 9;
        const pin_to_timer_index_t *timer_index;

        timer_index = pin_to_timer(pin);

        if((pulsein_table[pin].step == 0)
                && (digitalRead(pin) == pulsein_table[pin].state))
        {
            pulsein_table[pin].step = 1;
            pulsein_table[pin].value = rt_hw_tick_get_microsecond();
        }
        else if(pulsein_table[pin].step == 1)
        {
            uint32_t tmp = rt_hw_tick_get_microsecond();

            pulsein_table[pin].step = 2;

            if(tmp < pulsein_table[pin].value)
            {
                pulsein_table[pin].value = (0xFFFFFFFF -  pulsein_table[pin].value) + tmp;
            }
            pulsein_table[pin].value = tmp - pulsein_table[pin].value;

            TIM_ITConfig(timer_index->tim, timer_index->TIM_IT, DISABLE);
            rt_event_send(&pulsein_event, (1 << pin));
        }

        /* Clear TIM Capture compare interrupt pending bit */
        TIM_ClearITPendingBit(timer_index->tim, timer_index->TIM_IT);
    } /* TIM_IT_CC2 */

    if(TIM_GetITStatus(TIM4, TIM_IT_CC3) == SET)
    {
        uint8_t pin = 10;
        const pin_to_timer_index_t *timer_index;

        timer_index = pin_to_timer(pin);

        if((pulsein_table[pin].step == 0)
                && (digitalRead(pin) == pulsein_table[pin].state))
        {
            pulsein_table[pin].step = 1;
            pulsein_table[pin].value = rt_hw_tick_get_microsecond();
        }
        else if(pulsein_table[pin].step == 1)
        {
            uint32_t tmp = rt_hw_tick_get_microsecond();

            pulsein_table[pin].step = 2;

            if(tmp < pulsein_table[pin].value)
            {
                pulsein_table[pin].value = (0xFFFFFFFF -  pulsein_table[pin].value) + tmp;
            }
            pulsein_table[pin].value = tmp - pulsein_table[pin].value;

            TIM_ITConfig(timer_index->tim, timer_index->TIM_IT, DISABLE);
            rt_event_send(&pulsein_event, (1 << pin));
        }

        /* Clear TIM Capture compare interrupt pending bit */
        TIM_ClearITPendingBit(timer_index->tim, timer_index->TIM_IT);
    } /* TIM_IT_CC3 */

    if(TIM_GetITStatus(TIM4, TIM_IT_CC4) == SET)
    {
        uint8_t pin = 11;
        const pin_to_timer_index_t *timer_index;

        timer_index = pin_to_timer(pin);

        if((pulsein_table[pin].step == 0)
                && (digitalRead(pin) == pulsein_table[pin].state))
        {
            pulsein_table[pin].step = 1;
            pulsein_table[pin].value = rt_hw_tick_get_microsecond();
        }
        else if(pulsein_table[pin].step == 1)
        {
            uint32_t tmp = rt_hw_tick_get_microsecond();

            pulsein_table[pin].step = 2;

            if(tmp < pulsein_table[pin].value)
            {
                pulsein_table[pin].value = (0xFFFFFFFF -  pulsein_table[pin].value) + tmp;
            }
            pulsein_table[pin].value = tmp - pulsein_table[pin].value;

            TIM_ITConfig(timer_index->tim, timer_index->TIM_IT, DISABLE);
            rt_event_send(&pulsein_event, (1 << pin));
        }

        /* Clear TIM Capture compare interrupt pending bit */
        TIM_ClearITPendingBit(timer_index->tim, timer_index->TIM_IT);
    } /* TIM_IT_CC4 */

}

void TIM3_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM3, TIM_IT_CC3) == SET)
    {
        uint8_t pin = 2;
        const pin_to_timer_index_t *timer_index;

        timer_index = pin_to_timer(pin);

        if((pulsein_table[pin].step == 0)
                && (digitalRead(pin) == pulsein_table[pin].state))
        {
            pulsein_table[pin].step = 1;
            pulsein_table[pin].value = rt_hw_tick_get_microsecond();
        }
        else if(pulsein_table[pin].step == 1)
        {
            uint32_t tmp = rt_hw_tick_get_microsecond();

            pulsein_table[pin].step = 2;

            if(tmp < pulsein_table[pin].value)
            {
                pulsein_table[pin].value = (0xFFFFFFFF -  pulsein_table[pin].value) + tmp;
            }
            pulsein_table[pin].value = tmp - pulsein_table[pin].value;

            TIM_ITConfig(timer_index->tim, timer_index->TIM_IT, DISABLE);
            rt_event_send(&pulsein_event, (1 << pin));
        }

        /* Clear TIM Capture compare interrupt pending bit */
        TIM_ClearITPendingBit(timer_index->tim, timer_index->TIM_IT);
    } /* TIM_IT_CC3 */
}

void TIM2_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM2, TIM_IT_CC4) == SET)
    {
        uint8_t pin = 3;
        const pin_to_timer_index_t *timer_index;

        timer_index = pin_to_timer(pin);

        if((pulsein_table[pin].step == 0)
                && (digitalRead(pin) == pulsein_table[pin].state))
        {
            pulsein_table[pin].step = 1;
            pulsein_table[pin].value = rt_hw_tick_get_microsecond();
        }
        else if(pulsein_table[pin].step == 1)
        {
            uint32_t tmp = rt_hw_tick_get_microsecond();

            pulsein_table[pin].step = 2;

            if(tmp < pulsein_table[pin].value)
            {
                pulsein_table[pin].value = (0xFFFFFFFF -  pulsein_table[pin].value) + tmp;
            }
            pulsein_table[pin].value = tmp - pulsein_table[pin].value;

            TIM_ITConfig(timer_index->tim, timer_index->TIM_IT, DISABLE);
            rt_event_send(&pulsein_event, (1 << pin));
        }

        /* Clear TIM Capture compare interrupt pending bit */
        TIM_ClearITPendingBit(timer_index->tim, timer_index->TIM_IT);
    } /* TIM_IT_CC4 */
}

void TIM1_BRK_TIM9_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM9, TIM_IT_CC1) == SET)
    {
        uint8_t pin = 13;
        const pin_to_timer_index_t *timer_index;

        timer_index = pin_to_timer(pin);

        if((pulsein_table[pin].step == 0)
                && (digitalRead(pin) == pulsein_table[pin].state))
        {
            pulsein_table[pin].step = 1;
            pulsein_table[pin].value = rt_hw_tick_get_microsecond();
        }
        else if(pulsein_table[pin].step == 1)
        {
            uint32_t tmp = rt_hw_tick_get_microsecond();

            pulsein_table[pin].step = 2;

            if(tmp < pulsein_table[pin].value)
            {
                pulsein_table[pin].value = (0xFFFFFFFF -  pulsein_table[pin].value) + tmp;
            }
            else
            {
                pulsein_table[pin].value = tmp - pulsein_table[pin].value;
            }

            TIM_ITConfig(timer_index->tim, timer_index->TIM_IT, DISABLE);
            rt_event_send(&pulsein_event, (1 << pin));
        }

        /* Clear TIM Capture compare interrupt pending bit */
        TIM_ClearITPendingBit(timer_index->tim, timer_index->TIM_IT);
    } /* TIM_IT_CC1 */

    if(TIM_GetITStatus(TIM9, TIM_IT_CC2) == SET)
    {
        uint8_t pin = 12;
        const pin_to_timer_index_t *timer_index;

        timer_index = pin_to_timer(pin);

        if((pulsein_table[pin].step == 0)
                && (digitalRead(pin) == pulsein_table[pin].state))
        {
            pulsein_table[pin].step = 1;
            pulsein_table[pin].value = rt_hw_tick_get_microsecond();
        }
        else if(pulsein_table[pin].step == 1)
        {
            uint32_t tmp = rt_hw_tick_get_microsecond();

            pulsein_table[pin].step = 2;

            if(tmp < pulsein_table[pin].value)
            {
                pulsein_table[pin].value = (0xFFFFFFFF -  pulsein_table[pin].value) + tmp;
            }
            else
            {
                pulsein_table[pin].value = tmp - pulsein_table[pin].value;
            }

            TIM_ITConfig(timer_index->tim, timer_index->TIM_IT, DISABLE);
            rt_event_send(&pulsein_event, (1 << pin));
        }

        /* Clear TIM Capture compare interrupt pending bit */
        TIM_ClearITPendingBit(timer_index->tim, timer_index->TIM_IT);
    } /* TIM_IT_CC2 */

}
