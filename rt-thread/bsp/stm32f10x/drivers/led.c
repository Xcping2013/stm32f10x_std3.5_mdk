#include <rtthread.h>
#include <stm32f10x.h>

#include "74HC595.h"

#define led_rcc                    RCC_APB2Periph_GPIOE
#define led_gpio                   GPIOE
//#define led_pin                    (GPIO_Pin_9)  //EARPHONE
#define led_pin                    (GPIO_Pin_6)//INC
//#define led_pin                    (GPIO_Pin_2)//Mini

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

/* Includes ------------------------------------------------------------------*/
//#include "MB1616007.h"
///*
//static LCD_DrvTypeDef  *lcd_drv;
//lcd_drv = &hx8347d_drv;
//LCD_DrvTypeDef   hx8347d_drv ={};

//typedef struct 
//{
//  uint32_t   ChunkID;      
//}TypeDef;

//static void function(TypeDef * format);
//TypeDef *waveformat =  NULL;
//waveformat = (TypeDef*) FILE_ADDRESS;


//typedef struct
//{
//  uint8_t Mode;           
//}TSENSOR_InitTypeDef;

//TSENSOR_InitTypeDef tsensor_initstruct = {0};
//void    function(TSENSOR_InitTypeDef *pInitStruct);
//*/
//SPI_CONFIG_TYPE	dev_spi_config[4]=
//{
//	&PA13,
//	SPI1,
//	SPI_MODE3,
//	SPI_CLOCK_DIV2,				//DEV1 SPI1
//	SPI_BITODER_MSB,

//	&PA14,
//	SPI1,
//	SPI_MODE3,
//	SPI_CLOCK_DIV2,				//DEV2 SPI1
//	SPI_BITODER_MSB,
//	
//	&PB7,
//	SPI3,
//	SPI_MODE3,
//	SPI_CLOCK_DIV2,				//DEV3 SPI3
//	SPI_BITODER_MSB,
//	
//	&PB12,
//	SPI2,
//	SPI_MODE3,
//	SPI_CLOCK_DIV2,				//DEV4 SPI2
//	SPI_BITODER_MSB,
//};


//void device_init(uint8_t dev, uint8_t type)
//{
//	switch(type)
//	{
//		case DEV_SPI: 
//									spi.init(&dev_spi_config[dev] );
//									if(dev==DEV1)
//									{
//										pinMode(&PD6,GPIO_Mode_Out_PP);	pinSet(&PD6);
//										pinMode(&PB8,GPIO_Mode_Out_PP);	pinSet(&PB8);
//										pinMode(&PB9,GPIO_Mode_Out_PP);	pinSet(&PB9);	
//									}
//									else if(dev==DEV2)
//									{
//										pinMode(&PD3,GPIO_Mode_Out_PP);	pinSet(&PD3);
//										pinMode(&PD4,GPIO_Mode_Out_PP);	pinSet(&PD4);
//										pinMode(&PD5,GPIO_Mode_Out_PP);	pinSet(&PD5);	
//									}
//			break;
//		case DEV_UART: 											
//			break;	
//		case DEV_SOFT_I2C: 											
//			break;	
//		default:
//			break;	
//	}
//}


//#ifndef __MB1616007_H
//#define __MB1616007_H

//#include "sys.h"
//#include "mcu_spi.h"

////PA2	PA1	PA0
//#define DEV1			1
//#define DEV2			2
//#define DEV3			3
//#define DEV4			4

//#define DEV_GPIO			0
//#define DEV_UART			1
//#define DEV_SPI				2
//#define DEV_SOFT_I2C	3
//#define DEV_SOFT_SPI	4


//#define DEV2_UART			1
//#define DEV2_SOFT_I2C	2
//#define DEV2_SOFT_SPI	3

//#define DEV3_UART			1
//#define DEV3_SOFT_I2C	2
//#define DEV3_SOFT_SPI	3

//extern SPI_CONFIG_TYPE	dev_spi_config[4];
//void device_init(uint8_t dev, uint8_t type);
/*DEV1_pin:

	PD6,		//DEV1MISO_SEL
	PB8,		//DEV1MOSI_SEL
	PB9,		//DEV1SCK_SEL		SPI1
	
	PC9,		//or DEV12MISO	PA06
	PA8,		//or DEV12MOSI	PA07
	PA3,		//or DEV1RX			USART2
	PA2,		//or DEV1TX	
	PC7,		//or DEV1IRQ
	PA13,		//or DEV1CS
	PC8,		//or DEV12SCK	PA05
*/

/*DEV2_pin:

	PD3,		//DEV2MISO_SEL	SPI1
	PD4,		//DEV2MOSI_SEL
	PD5,		//DEV2SCK_SEL
	
	PD1,		//or DEV12MISO	PA06
	PD0,		//or DEV12MOSI	PA07
	PC11,		//or DEV2RX		  USART4
	PC10,		//or DEV2TX	
	PC6,		//or DEV2IRQ
	PA14,		//or DEV2CS
	PA15,		//or DEV12SCK	PA05
*/

/*DEV3_pin

	PB4,		//or DEV3MISO	 SPI3
	PB5,		//or DEV3MOSI	
	PD2,		//or DEV3RX		 USART5
	PC12,		//or DEV3TX	
	PB6,		//or DEV3IRQ
	PB7,		//or DEV3CS
	PB3,		//or DEV3SCK	
*/
/*DEV4_pin

	PD13,		
	PD12,		
	//SPI2 If used w25q32, w25q32_spi_config
	PB14,		//DEV4MISO		SPI2
	PB15,		//DEV4MOSI	
	PD9,		//or DEV4RX		
	PD8,		//or DEV4TX	 remap usart3
	PD14,		//or DEV4IRQ
	PB12,		//or DEV4CS
	PB13,		//or DEV4SCK	
	PD11,	//DEV4PC1
	PD10,	//DEV4PC0
*/

/*DEV5_PIN:

	PE10,								
	PE13,
	PE11,			//TIM
*/
/*DEV6_PIN

	PE09,		//TIM
	PE08,
	PB01,		//ADC
*/

/*DEV7_PIN

	PE12,
	PE14,
	PE15,		//TIM
*/


