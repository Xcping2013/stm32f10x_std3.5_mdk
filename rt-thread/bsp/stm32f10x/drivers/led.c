#include <rtthread.h>
#include <stm32f10x.h>

#include "74HC595.h"

#define led_rcc                    RCC_APB2Periph_GPIOE
#define led_gpio                   GPIOE
//#define led_pin                    (GPIO_Pin_9)  //EARPHONE
//#define led_pin                    (GPIO_Pin_6)//INC
#define led_pin                    (GPIO_Pin_2)//Mini

#define MB1616007	1	
void rt_hw_led_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(led_rcc,ENABLE);

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_InitStructure.GPIO_Pin   = led_pin;
    GPIO_Init(led_gpio, &GPIO_InitStructure);

#if (MB1616007==1)
		hc595.init();
		hc595.write(&hc595.out[0],2,MSB_FIRST);
		hc595.updata();
#endif	
}

void rt_hw_led_on(void)
{
  GPIO_SetBits(led_gpio, led_pin);
}

void rt_hw_led_off(void)
{
  GPIO_ResetBits(led_gpio, led_pin);
}

#ifdef RT_USING_FINSH
#include <finsh.h>
static rt_uint8_t led_inited = 0;
void set_led(rt_uint32_t value)
{
    /* init led configuration if it's not inited. */
    if (!led_inited)
    {
        rt_hw_led_init();
        led_inited = 1;
    }
		switch (value)
		{
			case 0:
					rt_hw_led_off();
					break;
			case 1:
					rt_hw_led_on();
					break;
			default:
					break;
    }
}

FINSH_FUNCTION_EXPORT(set_led, set led on[1] or off[0].)
#endif

