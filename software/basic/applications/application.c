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
static void thread_entry(void* parameter)
{
    rt_components_init();

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
}

int rt_application_init(void)
{
    rt_thread_t tid;

    tid = rt_thread_create("init",
    		thread_entry, RT_NULL,
    		2048, RT_THREAD_PRIORITY_MAX/3, 20);

    if (tid != RT_NULL)
        rt_thread_startup(tid);

    return 0;
}

/*@}*/
