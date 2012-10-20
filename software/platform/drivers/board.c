/*
 * File      : board.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2009 RT-Thread Develop Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      first implementation
 */

#include <rthw.h>
#include <rtthread.h>

#include "stm32f4xx.h"
#include "board.h"

/**
 * @addtogroup STM32
 */

/*@{*/

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures Vector Table base location.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NVIC_Configuration(void)
{
#ifdef  VECT_TAB_RAM
    /* Set the Vector Table base location at 0x20000000 */
    NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  /* VECT_TAB_FLASH  */
    /* Set the Vector Table base location at 0x08000000 */
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
#endif

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
}

/**
 * This is the timer interrupt service routine.
 *
 */
void SysTick_Handler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    rt_tick_increase();

    /* leave interrupt */
    rt_interrupt_leave();
}

static void gpio_init(void)
{
    /* USB_PWR_EN : PC14 PC15. */
    {
        GPIO_InitTypeDef  GPIO_InitStructure;

        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
        /* output setting */
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
        GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
        GPIO_Init(GPIOC, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
        /* output setting */
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
        GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
        GPIO_Init(GPIOC, &GPIO_InitStructure);
        GPIO_ResetBits(GPIOC, GPIO_Pin_15);
    }
}

static void idle_hook(void)
{
    __WFI();
}

/**
 * This function will initial STM32 board.
 */
void rt_hw_board_init()
{
    rt_thread_idle_sethook(idle_hook);

    gpio_init();

    /* SYSTEM config. */
    {
        /* Enable SYSCFG clock */
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    }

    /* NVIC Configuration */
    NVIC_Configuration();

    /* Configure the SysTick */
    SysTick_Config(SystemCoreClock / RT_TICK_PER_SECOND);

    rt_hw_usart_init();
#ifdef RT_USING_CONSOLE
    rt_console_set_device(CONSOLE_DEVICE);
#endif
}

#define TICK_MS (1000ULL/RT_TICK_PER_SECOND)
#define TICK_US	(1000000ULL/RT_TICK_PER_SECOND)

#include <rtm.h>

/**
 * get millisecond since system passed
 */
rt_uint32_t rt_hw_tick_get_millisecond(void)
{
    rt_tick_t tick;
    rt_uint32_t value;

    tick = rt_tick_get();
    value = tick * TICK_MS + (SysTick->LOAD - SysTick->VAL) * TICK_MS / SysTick->LOAD;

    return value;
}

/**
 * get microsecond since system passed
 */
rt_uint32_t rt_hw_tick_get_microsecond(void)
{
    rt_tick_t tick;
    rt_uint32_t value;

    tick = rt_tick_get();
    value = tick * TICK_US + (SysTick->LOAD - SysTick->VAL) * TICK_US / SysTick->LOAD;

    return value;
}

RTM_EXPORT(rt_hw_tick_get_millisecond);
RTM_EXPORT(rt_hw_tick_get_microsecond);

/*@}*/
