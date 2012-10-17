/*
 * File      : application.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2012-02-28     Bernard      the first version
 */

#include "stm32f4xx.h"
#include <board.h>
#include <rtthread.h>

#include <components.h>

#define LED_SYS_ON()     GPIO_SetBits(GPIOB, GPIO_Pin_2)
#define LED_SYS_OFF()    GPIO_ResetBits(GPIOB, GPIO_Pin_2)
static void led_thread_entry(void* parameter)
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

static void thread_entry(void* parameter)
{
    rt_components_init();

#ifdef RT_USING_DFS
        if (dfs_mount(RT_NULL, "/", "rom", 0, DFS_ROMFS_ROOT) == 0)
        {
            rt_kprintf("ROM File System initialized!\n");
            {
                rt_thread_delay(RT_TICK_PER_SECOND);
            }
        }
        else
            rt_kprintf("ROM File System initialzation failed!\n");
#endif

#ifdef RT_USING_USB_DEVICE
    /* usb device controller driver initilize */
    rt_hw_usbd_init();

    rt_usb_device_init("usbd");

    rt_usb_vcom_init();

#ifdef RT_USING_CONSOLE
    rt_console_set_device("vcom");
#endif
#ifdef RT_USING_FINSH
    finsh_set_device("vcom");
#endif
#endif
    
#ifdef RT_USING_USB_HOST
    /* register stm32 usb host controller driver */
    rt_hw_susb_init();
#endif

    do_init();
}

int rt_application_init(void)
{
    rt_thread_t tid;

    tid = rt_thread_create("sys_led",
                           led_thread_entry, RT_NULL,
                           2048, RT_THREAD_PRIORITY_MAX-2, 2);

    if (tid != RT_NULL)
        rt_thread_startup(tid);

    tid = rt_thread_create("init",
                           thread_entry, RT_NULL,
                           2048, RT_THREAD_PRIORITY_MAX/3, 20);

    if (tid != RT_NULL)
        rt_thread_startup(tid);

    return 0;
}

/*@}*/