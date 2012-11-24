/*
 * File      : sysled.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2012, RT-Thread Develop Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2012-11-16     Bernard      Move sysled into a standalone file.
 */

#include "stm32f4xx.h"
#include <rtthread.h>

#define LED_SYS_ON()     GPIO_SetBits(GPIOB, GPIO_Pin_2)
#define LED_SYS_OFF()    GPIO_ResetBits(GPIOB, GPIO_Pin_2)
static void sys_led_thread_entry(void* parameter)
{
    /* LED : PB2 BOOT1 config. */
    {
        GPIO_InitTypeDef  GPIO_InitStructure;

        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
        /* output setting */
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
        GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
        GPIO_Init(GPIOB, &GPIO_InitStructure);
    }

    /* sys led blink. */
    while(1)
    {
        /* turn on led 20ms. */
        LED_SYS_ON();
        rt_thread_delay(RT_TICK_PER_SECOND/50);

        /* turn off led 1s. */
        LED_SYS_OFF();
        rt_thread_delay(RT_TICK_PER_SECOND);
    }
}

void sys_led_init(void)
{
    rt_thread_t tid;

    tid = rt_thread_create("sysled",
                           sys_led_thread_entry, RT_NULL,
                           512, RT_THREAD_PRIORITY_MAX-2, 2);

    if (tid != RT_NULL)
        rt_thread_startup(tid);
}
