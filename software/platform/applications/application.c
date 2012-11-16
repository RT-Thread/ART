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

#include <board.h>
#include <rtthread.h>

#include <components.h>

static void thread_entry(void* parameter)
{
    sys_led_init();

    rt_platform_init();
    rt_components_init();

#ifdef RT_USING_DFS
    if (dfs_mount(RT_NULL, "/", "rom", 0, DFS_ROMFS_ROOT) == 0)
    {
        rt_kprintf("ROM File System initialized!\n");

#ifdef RT_USING_DFS_ELMFAT
		/* mount FAT file system on SD card */
        msd_init("sd0", "spi30");
        if (dfs_mount("sd0", "/sd", "elm", 0, 0) == 0)
        {
            rt_kprintf("SDCard File System initialized!\n");
        }
		else
		{
			rt_kprintf("SDCard File System initialzation failed!\n");
		}
#endif
    }
    else
        rt_kprintf("ROM File System initialzation failed!\n");
#endif

#ifdef RT_USING_USB_DEVICE
    /* usb device controller driver initilize */
    rt_hw_usbd_init();

    rt_usb_device_init("usbd");

    rt_usb_vcom_init();
#endif
    
#ifdef RT_USING_USB_HOST
    /* register stm32 usb host controller driver */
    rt_hw_susb_init();
#endif

	/* re-set console and finsh device */
#ifdef RT_USING_CONSOLE
		rt_console_set_device(RT_CONSOLE_DEVICE_NAME);
#endif
#ifdef RT_USING_FINSH
		finsh_set_device(RT_CONSOLE_DEVICE_NAME);
#endif

	/* do initialization script file */
    do_init();
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
