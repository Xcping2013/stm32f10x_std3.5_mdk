#include <rtthread.h>
#include <finsh.h>
#include <stdio.h>  
#include "mcu_spi.h"
#include "w25qxx.h" 
//W25Q32 1K(1024)个   sector(4KB)	= 16Pages (256B)  4MB/100 =1024*1024B/100>10000
SPI_CONFIG_TYPE	w25q32_spi_config=
{
	&PD7,
	SPI2,
	SPI_MODE3,
	SPI_CLOCK_DIV2,				//设置为18M时钟,高速模式
	SPI_BITODER_MSB,
};

static uint8_t w25q32_Inited = 0; 
//100个字节为基准
void Flash_write(uint16_t Addr,char *buf)
{
	uint8_t num;
	if(w25q32_Inited==0)
	{
		w25q32_Inited=1;
		w25qxx.init(&w25q32_spi_config);
		//rt_kprintf("id=%d\r\n",w25qxx.id);		
	}	
	num=strlen(buf);
	//rt_kprintf("%s",buf);	
  if(0<Addr && Addr <	10001	&& num <101)
	{
		//rt_kprintf("w25q32 start write");	
		if(num<250) buf[num]='\0';
		w25qxx.write(&w25q32_spi_config,(uint8_t*)buf,(uint32_t)(Addr-1)*100,100);
		rt_kprintf("=ok");	
	}
}
static void Flash_read(uint16_t Addr)
{
	u8 i;
	u8 readData[101];
	memset(readData,'\0',101);
	if(w25q32_Inited==0)
	{
		w25q32_Inited=1;
		spi.init(&w25q32_spi_config);
	}	
  if(0<Addr && Addr <	10001)
	{
		//w25qxx.erase_sector(&w25q32_spi_config,SectorAddr-1);
		w25qxx.read(&w25q32_spi_config,(uint8_t*)readData,(Addr-1)*100,100);
		
		for(i=0;i<101;i++)	
		{
			if(readData[i]>127) readData[i]='\0';
		}
		rt_kprintf("=%s",readData);	
	}
}
FINSH_FUNCTION_EXPORT_ALIAS(Flash_write, save, read info 1~10000 max to 100Bytes.)
FINSH_FUNCTION_EXPORT_ALIAS(Flash_read, read,	save info 1~10000 max to 100Bytes.)

