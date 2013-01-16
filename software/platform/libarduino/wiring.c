#include <rtm.h>
#include "libarduino.h"
#include "tim.h"

#define DELAY_TIM       TIM6
#define TIM_PSC         ((84/4) - 1)
void delayMicroseconds(unsigned int us)
{
    uint32_t arr = us * 4;

    DELAY_TIM->CR1 = 0; /* stop TIM. */
    DELAY_TIM->DIER = TIM_IT_Update; /* Update interrupt enable */
    DELAY_TIM->PSC = TIM_PSC;
    DELAY_TIM->CNT = 0;

    if(arr > 6)
    {
        DELAY_TIM->ARR = arr - 6;
    }
    else
    {
        DELAY_TIM->ARR = 1;
    }

    DELAY_TIM->CR1 = TIM_CR1_CEN; /* enable TIM. */

    while((DELAY_TIM->SR & TIM_IT_Update) == 0); /* wait.. */

    DELAY_TIM->SR  = 0; /* clean interrupt. */
    DELAY_TIM->CR1 = 0; /* stop TIM. */
}
RTM_EXPORT(delayMicroseconds);
