/*
 * File      : sysled.h
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

#ifndef __SYS_LED_H__
#define __SYS_LED_H__

#include <rtthread.h>

#ifdef __cplusplus
extern "C" {
#endif

void sys_led_init(void);

#ifdef __cplusplus
}
#endif

#endif
