#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "Commands.h"
#include "user_cmd_fun.h"
#include "usart.h"	  
#include "delay.h"	  

static uint8_t  ProcessCommand(char  *Commands);
static void 		MakeLowercase( char *str);
static void 		PrintCommandlist(void);

static uint8_t  CMD_help(char *Commands);

static uint8_t  CMD_sw(char *Commands);
static uint8_t  CMD_hw(char *Commands);

static u8 CMD_GetSensorStatus(char *str);
static u8 CMD_AirControl(char *str);
static u8 CMD_X2B_TEST(char *str);

static void MakeLowercase( char *str)		
{
	while(*str!='\0')
	{
			if( ((*str)>='A')&&((*str)<='Z') )
			{
					*str=*str+32;
			}
			str++;
	}
}

//
void CommandCheckAndExe(void)
{
	u8 len;
	if(USART_RX_STA&0x8000)									//接收到\r\n						
	{		
		len=USART_RX_STA&0x3fff;							//得到此次接收到的数据长度
		USART_RX_BUF[len]='\0';								//在末尾加入结束符. 
		printf((char*)USART_RX_BUF);
		if(USART_RX_BUF[0]=='\0')									;
		else ProcessCommand((char*)USART_RX_BUF)	;				
		printf("\r\n>>");	
		USART_RX_STA=0;
	}
}
//
uint8_t  ProcessCommand(char  *Commands)
{
	MakeLowercase(Commands);	
	if			(strcmp("switch to b188",	Commands )==0)		{at24cxx.writeU8(at24c256, 16383, 0);	 return 1;}
	else 	if(strcmp("switch to b235", Commands )==0)		{at24cxx.writeU8(at24c256, 16383, 1);	 return 1;}
	
	else  if(CMD_help(Commands)==1)									return 1;
	else  if(CMD_rom(Commands)==1)									return 1;	
	else	if(CMD_sw(Commands)==1)										return 1;
	else	if(CMD_hw(Commands)==1)										return 1;
	else	if(CMD_fsn(Commands)==1)									return 1;	
	else	if(CMD_int(Commands)==1)									return 1;
	else	if(CMD_out(Commands)==1)									return 1;
		
	else	if(CMD_RomPage(Commands)==1)							return 1;		
	/*******QT1_X2B命令封装：传感器采集，气缸动作，继电器切换，返回值需要不同，加以区别*/
	else if(CMD_GetSensorStatus(Commands)==1) 																				return 1;			//传感器输入检测	
	else if(CMD_AirControl(Commands)==1) 			{return 1;	}		//气缸输出控制
	else if(CMD_X2B_TEST(Commands)==1) 				{ 																		return 1;	}		//继电器小板切换测试项目
		
	else
	{
		printf("<NA>");	return 0;
	}
}
/**				List the command list and discription
**
**/
static void PrintCommandlist(void)
{
	printf("\r\n\r\n");
	printf("sw-------------------get software version\r\n");
	printf("hw-------------------get hardware version\r\n");
	printf("fsn------------------get fixture serial number\r\n");	
	printf("fsn[-----------------set fixture serial number\r\n");	
	printf("rom[]----------------read roms\r\n");
	printf("rom[][]--------------write roms\r\n");
	printf("save info xx yyyyy---write datas\r\n");
	printf("read info xx---------read datas\r\n");
	printf("\r\n");
	printf("int[]----------------get the input status\r\n");
	printf("out[][]--------------set the output status\r\n");
	printf("\r\n");
	printf("switch to b188\r\n");
	printf("switch to b235\r\n\r\n");

	if(at24cxx.readU8(at24c256, 16383)==1)
	{
		printf("sn:xx----------------set the fixture SN number\n");		
		
		printf("input xx-------------get the input pin status\r\n");
		printf("output xx y----------set the output pin to y\r\n");	
		
		printf("air in---------------cylinder in\r\n");
		printf("air out--------------cylinder out\r\n");
		printf("p2 open--------------p2 claw open\r\n");
		printf("p2 close  -----------p2 claw close\r\n");
		printf("p3 open  ------------p3 claw open\r\n");
		printf("p3 close  -----------p3 claw close\r\n");

		printf("i dut  --------------switch to measure dut charging current\r\n");	
		printf("r ea  ---------------switch to measure earphoneA resistance\r\n");
		printf("r eb  ---------------switch to measure earphoneB resistance\r\n");
		printf("i ea  ---------------switch to measure earphoneA current\r\n");
		printf("i eb  ---------------switch to measure earphoneB current\r\n");
		printf("p2 done  ------------dut connect to power supply,out of measure mode\r\n");
		printf("power on  -----------dut connect to power supply\r\n");
		printf("power off  ----------dut disconnect to power supply\r\n");
		
		printf("left status  --------\r\n");
		printf("right status  -------\r\n");
		printf("light status  -------\r\n");
		printf("p0/4 status  --------\r\n");
		
		printf("inside status--------\r\n");
		printf("outside status  -----\r\n");	

		printf("p2 status  ----------\r\n");
		printf("p2 upside status-----\r\n");
		
		printf("p3 status------------\r\n");
		printf("p3 upside status-----\r\n");
	}
	printf("\r\n");
//	printf("set 5v-----set 5v<ok>-----set the port to 5v\n");
//	printf("set 0v-----set 0v<ok>-----set the port to 0v\n");
	printf("Please send command as above end with \\r\\n\n");
}
//
static uint8_t  CMD_help(char *Commands)
{
	if(strcmp("help",	Commands)==0)				
	{		
		PrintCommandlist();								
		return 1;		
	}
	else return 0;
}
//
static uint8_t  CMD_sw(char *Commands)
{
  if(strcmp("sw",		Commands)==0)			
	{		
		printf("<B188-B235-01>");
		return 1;		
	}
	else return 0;
}
//
static uint8_t  CMD_hw(char *Commands)
{
	if(strcmp("hw",		Commands)==0)			
	{		
		printf("<INC-MB1616007-04>");
		return 1;	
	}
	else return 0;
}
//
/***************************QT1 对   应  项  目  控	制	命	令	封	装****************************/
//输入传感器检测
static u8 CMD_GetSensorStatus(char *str)
{
	if( strcmp(str,"left status")==0 ) 						//左边光栅状态检测，光栅无触碰时为0，触碰后卫1.
	{
		if( Int.read(3)==0 || Int.read(4)==0 )
	  {
			printf("\r\nleft true@");return 1;
		}		
		else printf("\r\nleft false@");return 1;
	}	
	else if( strcmp(str,"light status")==0 ) 			//左右光栅一起检测
	{			
		if( Int.read(3)==0 || Int.read(4)==0 || Int.read(5)==0 || Int.read(6)==0)
		{
			printf("\r\nlight touched true@");return 1;
		}		
		else printf("\r\nlight touched false@");return 1;
	}
	
	else if( strcmp(str,"right status")==0 ) 			//右光栅状态检测
	{
		if( Int.read(5)==0 || Int.read(6)==0 )
		{
			printf("\r\nright true@");return 1;
		}	
		else printf("\r\nright false@");return 1;
	}
	
	else if( strcmp(str,"p0 status")==0 ) 				//P0位置有无产品检测，无产品时为0，可设置传感器
	{
		if( Int.read(1)==0)
		{	
			printf("\r\np0 true@");return 1;	
		}
		else printf("\r\np0 false@");return 1;
	}
	else if( strcmp(str,"p4 status")==0 ) 				//P4位置有无产品检测
	{
		if( Int.read(2)==0 )
		{
			printf("\r\np4 true@");return 1;
		}		
		else printf("\r\np4 false@");return 1;
	}
	else if( strcmp(str,"inside status")==0 ) 		//按键气缸是否处于缩回状态	 	
	{
		if( Int.read(7)==1 && Int.read(8)==0)
		{
			printf("\r\ninside true@");return 1;
		}		
		else printf("\r\ninside false@");return 1;
	}
	
	else if( strcmp(str,"outside status")==0 ) 		//按键气缸是否处于弹出状态	
	{
		if( Int.read(7)==0 && Int.read(8)==1)
		{
			printf("\r\noutside true@");return 1;
		}		
		else printf("\r\noutside false@");return 1;
	}
	else if( strcmp(str,"p2 upside status")==0 ) 	//P2垂直气缸所处位置查询	
	{
		if( Int.read(13)==1 && Int.read(14)==0 )
		{				
			printf("\r\np2 downside true@");return 1;
		}		
		else if( Int.read(13)==0 && Int.read(14)==1 )
		{	
			printf("\r\np2 upside true@");return 1;
		}		
		else	printf("p2 status undefine@");return 1;
	}
	else if( strcmp(str,"p3 upside status")==0 ) 	//P3垂直气缸所处位置查询		
	{
		if( Int.read(15)==1 && Int.read(16)==0 )
		{
			printf("\r\np3 downside true@");return 1;
		}		
		else if( Int.read(15)==0 && Int.read(16)==1 )
		{	
			printf("\r\np3 upside true@");return 1;
		}		
		else	printf("\r\np3 status undefine@");return 1;
	}

	else if( strcmp(str,"p2 status")==0 ) 				//P2爪子状态，左右一起	
	{
		if( Int.read(9)==0 && Int.read(10)==0 )
		{	
			printf("\r\np2 open@");return 1;	
		}
		else if( Int.read(9)==1 && Int.read(10)==1 )
		{	
			printf("\r\np2 close@");return 1;
		}				
		else	printf("\r\np2 undefine@");return 1;
	}	
	else if( strcmp(str,"p3 status")==0 ) 				//P3爪子状态	
	{
		if( Int.read(11)==1 && Int.read(12)==1 )
		{	
			printf("\r\np3 close@");return 1;	
		}
		else if( Int.read(11)==0 && Int.read(12)==0 )
		{	
			printf("\r\np3 open@");return 1;
		}		
		else printf("\r\np3 undefine@");return 1;
	}	
	return 0;
}
//输出气缸控制
static u8 CMD_AirControl(char *str)
{
	if( strcmp(str,"air in")==0 ) 						//按键气缸缩进
	{
		Out.write(15, 0);
		printf("\r\nok@AIR IN");
		return 1;	
	}
	else if( strcmp(str,"air out")==0 ) 			//按键气缸弹出
	{
		Out.write(15, 1);
		printf("\r\nok@AIR OUT");
		return 1;
	}
	else if( strcmp(str,"p2 open")==0 ) 			//P2左右爪子都打松开
	{
		Out.write(1, 1);Out.write(2, 0);
		Out.write(3, 1);Out.write(4, 0);
		printf("\r\nok@P2 OPEN");
		return 1;
	}
	else if( strcmp(str,"p2 close")==0 ) 			//P2左右爪子都闭合
	{
		Out.write(1, 0);Out.write(2, 1);
		Out.write(3, 0);Out.write(4, 1);
		printf("\r\nok@P2 CLOSE");
		return 1;
	}
	else if( strcmp(str,"p3 open")==0 ) 			//P3左右爪子松开
	{
		Out.write(5, 1);Out.write(6, 0);
		Out.write(7, 1);Out.write(8, 0);
		printf("\r\nok@P3 OPEN");
		return 1;
	}
	else if( strcmp(str,"p3 close")==0 ) 			//P3左右爪子闭合
	{
		Out.write(5, 0);Out.write(6, 1);
		Out.write(7, 0);Out.write(8, 1);
		printf("\r\nok@P3 CLOSE");
		return 1;
	}
	else return 0;
}
//测试项电路切换
static u8 CMD_X2B_TEST(char *str)
{
	if( strcmp(str,"i dut")==0 ) 																												//测试DUT充电电流
	{
		Out.write(12, 0);	Out.write(11, 0); Out.write(10, 0);	Out.write(9, 0);								//断开其它线路
		Out.write(13, 1);	Out.write(16, 0);																								//地线闭合，电源线断开
		Out.write(14, 1);																																  //串入电流表
		printf("\r\nok@I DUT");return 1;
	}
	else if( strcmp(str,"r ea")==0 ) 														//测试DUT左边电阻
	{
		Out.write(16, 0);																								//断开地线
		Out.write(13, 0);Out.write(14, 1);Out.write(12, 0);Out.write(11, 0);Out.write(10, 0);		//断开电源线
		Out.write(9, 1);																									//接入电阻表	
		printf("\r\nok@R EA");return 1;
	}
	else if( strcmp(str,"r eb")==0 ) 														//测试DUT右边电阻
	{
		Out.write(16, 0);																								//断开地线
		Out.write(13, 0);Out.write(14, 1);Out.write(12, 0);Out.write(11, 0);Out.write(9, 0);		//断开电源线
		Out.write(10, 1);																								//接入电阻表
		printf("\r\nok@R EB");return 1;
	}
	else if( strcmp(str,"i eb")==0 ) 														//测试DUT左耳电流
	{
		Out.write(16, 1);																								//地线闭合
		Out.write(13, 0);Out.write(14, 0);Out.write(11, 0);Out.write(10, 0);Out.write(9, 0);		//电源线闭合
		Out.write(12, 1);																								//电流表串入耳机线路中
		printf("\r\nok@I EB");return 1;	
	}	
	else if( strcmp(str,"i ea")==0 ) 														//测试DUT右耳电流
	{
		Out.write(16, 1);																								//DUT地线闭合
		Out.write(13, 0);Out.write(14, 0);Out.write(12, 0);Out.write(10, 0);Out.write(9, 0);		//其它继电器断开
		Out.write(11, 1);																									//电流表串入耳机线路中
		printf("\r\nok@I EA");return 1;
	}	
	else if( strcmp(str,"power on")==0 ) 												//DUT：GND与VCC闭合导通
	{
		Out.write(16, 1);	Out.write(14, 0);	Out.write(13, 0);																  //闭合地线、电源线																		
		Out.write(9, 0);Out.write(10, 0);Out.write(11, 0);Out.write(12, 0);	  //断开其他
		printf("\r\nok@POWER ON");return 1;
	}	
	else if( strcmp(str,"power off")==0 ) 											//DUT：GND与VCC断开
	{
		Out.write(16, 1);	Out.write(14, 1);	Out.write(13, 0);												//电源线   保留地线
		Out.write(9, 0);Out.write(10, 0);Out.write(11, 0);Out.write(12, 0);							  //断开其他	
		printf("\r\nok@POWER OFF");return 1;
	}
	else if( strcmp(str,"p2 done")==0 ) 												//P2测试完成，线路切换为充电状态，不接如万用表
	{	
		Out.write(16, 1);		
		Out.write(13, 0);Out.write(14, 0);Out.write(12, 0);Out.write(11, 0);Out.write(10, 0);Out.write(9, 0);
		printf("\r\nok@P2 DONE");return 1;
	}
	return 0;
}
//





