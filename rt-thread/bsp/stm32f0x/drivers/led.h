/*
 * File      : led.h
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2009, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2013-13-05     bright       the first version
 */

#ifndef __LED_H__
#define __LED_H__

#include <rthw.h>
#include <rtthread.h>
#include <stm32f0xx.h>

#include <bsp.h>

#define rt_hw_led_on()   		 GPIO_SetBits(GPIOD, GPIO_Pin_2)
#define rt_hw_led_off()  		 GPIO_ResetBits(GPIOD, GPIO_Pin_2)
#define rt_hw_led_toggle()	 GPIOD->ODR^=BIT2

void rt_hw_led_init(void);

#endif
