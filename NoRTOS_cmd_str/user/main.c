#include <stdlib.h>

#include "delay.h"
#include "usart.h"

#include "dido.h"
#include "at24cxx.h"

#include "Commands.h"

#define SYS_LED	PB2
		
int main(void)
{	 
	u32 BlinkDelay;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	delay_init();	 
	uart_init(115200);	 	
 	 		
	pinMode(&SYS_LED, GPIO_Mode_Out_PP);
	
	Int.init();
	Out.init();
	printf("mcu init ok ; ");	
	
	eeprom_init();
	
	if(at24cxx.readU8(at24c256, 16383)==0)
	{
		printf("eeprom is inited ; fixture for B188 init ok\r\n>>");
	}
	else if(at24cxx.readU8(at24c256, 16383)==1)
	{
		Out.write(16, 1);															//HUBGND1-USBGND2	DUT供VGND闭合
		Out.write(15, 0);															//按键气缸进去	
		Out.write(1, 0);	Out.write(2, 1);						//P2左边爪子闭合
		Out.write(3, 0);	Out.write(4, 1);						//P2右边爪子闭合
		Out.write(5, 0);	Out.write(6, 1);						//P3左边爪子闭合
		Out.write(7, 0);	Out.write(8, 1);	
		printf("eeprom is inited ; fixture for B235 init ok\r\n>>");
	}
	else 
	{
		at24cxx.writeU8(at24c256, 16383, 0);	
		printf("eeprom init...\r\n>>");
	}
	BlinkDelay=GetSysTimer();
	
	while(1)
	{
		CommandCheckAndExe();
  	if(abs((int)(GetSysTimer()-BlinkDelay))>200)
  	{
  		pinToggle(&SYS_LED);
  		BlinkDelay=GetSysTimer();
		} 	
	}
}
