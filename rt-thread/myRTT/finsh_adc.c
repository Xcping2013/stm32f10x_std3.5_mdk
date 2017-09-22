#include <rtthread.h>
#include <finsh.h>
#include <stdio.h>

#include "delay.h"
#include "MCP3421.h"

mcp3421_t adc1=
{
	&PA0,
	&PC3,
	MCP3421_SAMPLE_RATE_15_SPS,
	MCP3421_GAIN_8,
	0
};

u16 currentDat[10];
static u16 getAdcMean(u16 *dat, u8 len)
{
	u8  i, maxIndex=0, minIndex=0;
	u16 maxValue=0, minValue=0;
	u32 sum=0;
	
	maxValue = dat[0];
	minValue = dat[0];
	
	if(len==0)					return 0;
	else if(len==1)			return dat[0];
	else if(len==2)			return ((dat[0]+dat[1])/2);
	
	// 取出最大值和最小值
	for(i=1; i<len; i++)
	{
		if(dat[i]>maxValue)
		{
			maxValue = dat[i];
			maxIndex = i;
		}
		
		if(dat[i]<minValue)
		{
			minValue = dat[i];
			minIndex = i;
		}
	}
	
	if(minIndex==maxIndex)		// 如果最大值和最小值一样
		return dat[0];					// 说明所有的数据的值都一样
	
	// 去掉最大值和最小值
	dat[maxIndex]=0;
	dat[minIndex]=0;
	
	// 求和
	for(i=0; i<len; i++)
		sum += dat[i];
	
	return (sum/(len-2));		// 取平均值
	
}

static u16 Get_Adc_Average(u8 times)
{
	u8 t;
	for(t=0;t<times;t++)
	{
		MCP3421StartConvertion(&adc1);
		currentDat[t]=MCP3421GetValue(&adc1);
		delay_ms(1);
	}
	return(getAdcMean(currentDat,times));
} 	 

static rt_uint8_t MCP3421_inited = 0;

void getUSBCurrent(void)
{
	char chCurrent[10];

	if (!MCP3421_inited)
	{
		SoftI2c.pin_init(adc1.pin);
		MCP3421_inited = 1;
	}
	MCP3421SetConvertionModeOneShot(&adc1);	
	MCP3421StartConvertion(&adc1);
	sprintf(chCurrent,"%5.2f",(float)(Get_Adc_Average(10)*10.0*(MCP3421_LSB_16BITS*1000.0 / MCP3421_GAIN_8_VALUE))-0.16);
	//sprintf(chCurrent,"%5.2f",MCP3421GetValue()*10);
	rt_kprintf("=%smA",chCurrent);	
	
}
FINSH_FUNCTION_EXPORT(getUSBCurrent, get usb current.)

