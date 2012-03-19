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

#ifdef RT_USING_DFS
#include <dfs_fs.h>
#include <dfs_elm.h>
#include <dfs_romfs.h>
#endif

static void thread_entry(void* parameter)
{
    /* File System Initialization */
#ifdef RT_USING_DFS
    {
        /* initialize the device file system */
        dfs_init();

#ifdef RT_USING_DFS_ELMFAT
        /* initialize the elm FAT file system*/
        elm_init();
#endif
#ifdef RT_USING_DFS_ROMFS
        /* initialize romfs */
        dfs_romfs_init();
#endif

    	if (dfs_mount(RT_NULL, "/", "rom", 0, DFS_ROMFS_ROOT) == 0)
    	{
    		rt_kprintf("ROM File System initialized!\n");
    	}
    	else
    		rt_kprintf("ROM File System initialzation failed!\n");
    }
#endif
}

int rt_application_init()
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
