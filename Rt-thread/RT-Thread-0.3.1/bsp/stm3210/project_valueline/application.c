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
 * 2009-01-05     Bernard      the first version
 */

/**
 * @addtogroup STM32
 */
/*@{*/

#include <rtthread.h>
#include "led.h"

ALIGN(RT_ALIGN_SIZE)
char thread_led_stack[512];
struct rt_thread thread_led;

void rt_thread_entry_led(void* parameter)
{
    rt_uint32_t count=0;

	/* init led hardware */
	rt_hw_led_init();

    while (1)
    {
        /* led on */
        rt_kprintf("led on,count : %d\n", count ++);
        rt_hw_led_on(0);
        rt_thread_delay(RT_TICK_PER_SECOND);

        /* led off */
        rt_kprintf("led off\n");
        rt_hw_led_off(0);
        rt_thread_delay(RT_TICK_PER_SECOND);
    }
}

int rt_application_init()
{
	/* init led thread */
    rt_thread_init(&thread_led, "led",
                   rt_thread_entry_led, RT_NULL,
                   &thread_led_stack[0], sizeof(thread_led_stack),
				   5, 10);
    rt_thread_startup(&thread_led);

    return 0;
}

/*@}*/
