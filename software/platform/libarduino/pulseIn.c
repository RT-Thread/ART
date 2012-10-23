#include <rtm.h>

#include "libarduino.h"
#include "pin_table.h"
#include "tim.h"

unsigned long pulseIn(uint8_t pin,
                      uint8_t state,
                      unsigned long timeout)
{
    const pin_to_timer_index_t *timer_index;
    const struct pin_index *pin_index_p;
    struct pulsein_node * node;

    static int int_flag = 0;

    if(int_flag == 0)
    {
        int_flag = 1;
        rt_event_init(&pulsein_event, "pulsein", RT_IPC_FLAG_FIFO);
    }

    pin_index_p = get_pin(pin);
    if(pin_index_p == RT_NULL)
    {
        return 0;
    }

    timer_index = pin_to_timer(pin);
    if(!timer_index || !timer_index->tim)
    {
        return 0;
    }

    node = get_pulsein_node(pin);
    if(node == RT_NULL)
    {
        return 0;
    }

    TIM_config(pin, 0);

    node->state = state;
    node->step = 0;

    /* setting */
    {
        TIM_ICInitTypeDef TIM_ICInitStructure;

        TIM_ICStructInit(&TIM_ICInitStructure);

        TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
        TIM_ICInitStructure.TIM_ICFilter    = 3;
        TIM_ICInitStructure.TIM_Channel     = timer_index->tim_channel;
        TIM_ICInitStructure.TIM_ICPolarity  = TIM_ICPolarity_BothEdge;
        TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
        TIM_ICInit(timer_index->tim, &TIM_ICInitStructure);
    }

    /* GPIO clock enable */
    RCC_AHB1PeriphClockCmd(pin_index_p->rcc, ENABLE);

    /* GPIO configuration */
    {
        GPIO_InitTypeDef GPIO_InitStructure;

        GPIO_InitStructure.GPIO_Pin   = pin_index_p->pin;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

        GPIO_Init(pin_index_p->gpio, &GPIO_InitStructure);
    }

    /* Connect TIM pins to AF */
    GPIO_PinAFConfig(pin_index_p->gpio,
                     timer_index->pin_source,
                     timer_index->gpio_af);

    TIM_NVIC_Configuration(pin);
    TIM_ClearITPendingBit(timer_index->tim, timer_index->TIM_IT);
    TIM_ITConfig(timer_index->tim, timer_index->TIM_IT, ENABLE);
    TIM_Cmd(timer_index->tim, ENABLE);

    /* wait */
    {
        rt_uint32_t dummy;
        rt_tick_t tick;

        tick = rt_tick_from_millisecond(timeout);

        if (rt_event_recv(&pulsein_event,
                          (1 << pin),
                          RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
                          tick,
                          &dummy)
                == RT_EOK)
        {
            return node->value;
        }
        else
        {
            TIM_ITConfig(timer_index->tim, timer_index->TIM_IT, DISABLE);
        }
    } /* wait */

    return 0;
}
RTM_EXPORT(pulseIn);
