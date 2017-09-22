#include <rtthread.h>
#include <finsh.h>
#include <stdio.h>

#include "at24cxx.h"
#include "mcu_softI2c.h"
#include "delay.h"

IIC_GPIO at2404_pin=
{
	&PA0,
	&PC3
};

//一个字符占用一个字节	太浪费空间  
//小空间可用于保全 浮点数据，大空间用于字符串保存
static uint8_t at24lc04_Inited = 0;
static uint8_t at24lc04_reads(uint16_t address)
{
	uint8_t	i;
	uint8_t data[17];
	for(i=0;i<17;i++)	data[i]='\0';
	
	if(at24lc04_Inited==0)
	{
		at24lc04_Inited=1;
		SoftI2c.pin_init(at2404_pin);
	}	
	if(0<address && address <17)	
	{
		if(SoftI2c.reads(at2404_pin,1,0xa4,(address-1)*16,data,16)) return 1;		
	}
	else if(address<33 && 0<address)	
	{
		if(SoftI2c.reads(at2404_pin,1,0xa6,(address-1)*16,data,16)) return 1;
	}
	else return 1;	
	for(i=0;i<16;i++)	
	{
		if(data[i]>127) data[i]='\0';
	}
	data[16]='\0';
	rt_kprintf("=%s",data);	return 0;	
}

static uint8_t at24lc04_writes(uint8_t address,char *data)
{
	uint8_t len;
	if(at24lc04_Inited==0)
	{
		at24lc04_Inited=1;
		SoftI2c.pin_init(at2404_pin);
	}	
	len=strlen(data);
	if(len<17 && len>0)
	{
		if(len==16) {;}
		else	data[len]='\0';

		if(0<address && address <17)		
		{
			if(SoftI2c.writes(at2404_pin,1,0xa4,(uint16_t)(address-1)*16,(uint8_t*)data,16)) return 1;
			delay_ms(5);	
		}
		else if(address<33 && 0<address )	
		{
			if(SoftI2c.writes(at2404_pin,1,0xa6,(uint16_t)(address-1)*16,(uint8_t*)data,16)) return 1;
		}
		else return 1;	
	}
	else return 1;	
	rt_kprintf("=ok");	return 0;	
}

//FINSH_FUNCTION_EXPORT_ALIAS(at24lc04_reads, readInfo, read info [1~32].)
//FINSH_FUNCTION_EXPORT_ALIAS(at24lc04_writes,saveInfo,	save info [1~32] max 16Bytes)

/**			AT24CXXX_APP
 */
at24cxx_t at24c256=
{
	{&PA0,&PC3},
	0xA2,
};

void eeprom_init(void)
{
	at24c256.eeprom=C24LC04;//C24LC256;
	at24cxx.init(at24c256);
}
//
void at24c256_read(u16 address, u16 len)
{
	uint8_t	i;
	uint8_t dataRead[100]="";
	rt_kprintf("\r\nbefore dataRead=%.100s\r\n",dataRead);
	at24cxx.read(at24c256, address, dataRead, len);
	for(i=0;i<100;i++)	
	{
		if(dataRead[i]>127) { dataRead[i]='\0';break;}
	}
	rt_kprintf("after  dataRead=%.100s",dataRead);
}
//
void at24c256_write(u16 address, uint8_t *data, u16 len)
{
	at24cxx.write(at24c256, address, data, len);
	rt_kprintf("ok");
}
//
FINSH_FUNCTION_EXPORT(eeprom_init, 		eeprom init)
FINSH_FUNCTION_EXPORT(at24c256_read,	at24c256 read)
FINSH_FUNCTION_EXPORT(at24c256_write,	at24c256 write)
//