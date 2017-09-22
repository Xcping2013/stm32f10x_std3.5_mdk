#include <rtthread.h>
#include <finsh.h>
#include <stdio.h>

#include "74HC165.h"
#include "74HC595.h"

char data_in[16] = {0};
u8 hc165_in	=0;

static rt_uint8_t input_Inited = 0;

static uint8_t readPin(uint8_t pin)
{
	if(input_Inited == 0)
  {
    input_Inited = 1;	
		
		pinMode(&PE0,GPIO_Mode_IPU);
		pinMode(&PE1,GPIO_Mode_IPU);
		pinMode(&PE2,GPIO_Mode_IPU);
		pinMode(&PE3,GPIO_Mode_IPU);
		pinMode(&PE4,GPIO_Mode_IPU);
		pinMode(&PE5,GPIO_Mode_IPU);
		pinMode(&PE6,GPIO_Mode_IPU);
		pinMode(&PE7,GPIO_Mode_IPU);
		
		hc165.init();
	}
	switch(pin)
	{
		case 1:			data_in[pin-1]=pinRead(&PE0);
			break;	
		case 2:			data_in[pin-1]=pinRead(&PE1);
			break;	
		case 3:			data_in[pin-1]=pinRead(&PE2);
			break;	
		case 4:			data_in[pin-1]=pinRead(&PE3);
			break;				
		case 5:			data_in[pin-1]=pinRead(&PE4);
			break;	
		case 6:			data_in[pin-1]=pinRead(&PE5);
			break;		
		case 7:			data_in[pin-1]=pinRead(&PE6);
			break;	
		case 8:			data_in[pin-1]=pinRead(&PE7);
			break;	
		
		case 9:			
		case 10:		
		case 11:		
		case 12:						
		case 13:			
		case 14:				
		case 15:			
		case 16:		hc165.read(&hc165_in,1,LSB_FIRST);
			break;	
		default:
			return 0;		
	}
	if(0<pin&&pin<9)	rt_kprintf("=%d",data_in[pin-1]);			
	else							rt_kprintf("=%d",(hc165_in>>(16-pin))&0x01);	//rt_kprintf("=%d",hc165_in);	
	return 1;
}

void  readPins(void)
{
	u8 i;
	if(input_Inited == 0)
  {
    input_Inited = 1;	
		
		pinMode(&PE0,GPIO_Mode_IPU);
		pinMode(&PE1,GPIO_Mode_IPU);
		pinMode(&PE2,GPIO_Mode_IPU);
		pinMode(&PE3,GPIO_Mode_IPU);
		pinMode(&PE4,GPIO_Mode_IPU);
		pinMode(&PE5,GPIO_Mode_IPU);
		pinMode(&PE6,GPIO_Mode_IPU);
		pinMode(&PE7,GPIO_Mode_IPU);
		
		hc165.init();
	}
	data_in[0]=pinRead(&PE0)? '1':'0';
	data_in[1]=pinRead(&PE1)? '1':'0';
	data_in[2]=pinRead(&PE2)? '1':'0';
	data_in[3]=pinRead(&PE3)? '1':'0';
	data_in[4]=pinRead(&PE4)? '1':'0';
	data_in[5]=pinRead(&PE5)? '1':'0';
	data_in[6]=pinRead(&PE6)? '1':'0';
	data_in[7]=pinRead(&PE7)? '1':'0';
	hc165.read(&hc165_in,1,LSB_FIRST);
	for(i=0;i<8;i++)
	data_in[i+8]=(hc165_in>>(7-i))&0x01 ? '1':'0';	
	rt_kprintf("=%.16s",data_in);	
}

//output
//reset the buffer and shift out
static uint8_t output_Inited = 0;
static uint8_t writePin(uint8_t pin, uint8_t state)
{
	if(output_Inited==0)
	{
		output_Inited=1;
		hc595.init();
		pinMode(&PB11,GPIO_Mode_Out_PP);
		pinMode(&PB10,GPIO_Mode_Out_PP);
		
		pinMode(&PC13,GPIO_Mode_Out_PP);
		pinMode(&PC14,GPIO_Mode_Out_PP);
		pinMode(&PC15,GPIO_Mode_Out_PP);
		
		pinMode(&PC0,GPIO_Mode_Out_PP);
		pinMode(&PC1,GPIO_Mode_Out_PP);
		pinMode(&PC2,GPIO_Mode_Out_PP);
	}	
	switch(pin)
	{
		case 1:			pinWrite(&PB11,state);
			break;	
		case 2:			pinWrite(&PB10,state);
			break;	
		case 3:			pinWrite(&PC13,state);
			break;	
		case 4:			pinWrite(&PC14,state);
			break;				
		case 5:			pinWrite(&PC15,state);
			break;	
		case 6:			pinWrite(&PC0,state);
			break;		
		case 7:			pinWrite(&PC1,state);
			break;
		case 8:			pinWrite(&PC2,state);
			break;	
		
		case 9:			
		case 10:		
		case 11:		
		case 12:						
		case 13:			
		case 14:				
		case 15:			
		case 16:		if(state) hc595.out[1] |= (0x80>>(16-pin));
								else   		hc595.out[1] &= ((0x7f >> (16-pin)) | (0x7f << (8 -(16-pin))));//Ñ­»·ÓÒÒÆ
								hc595.write(&hc595.out[0],2,MSB_FIRST);
								hc595.updata();
			break;
		
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		case 22:
		case 23:
		case 24:
							if(state==0)   hc595.out[0] |= (0x80>>(24-pin));
								else   		hc595.out[0] &= ((0x7f >> (24-pin)) | (0x7f << (8 -(24-pin))));//Ñ­»·ÓÒÒÆ
								hc595.write(&hc595.out[0],2,MSB_FIRST);
								hc595.updata();
			break;
	
		default:
			return 0;		
	}
	rt_kprintf("=ok");	return 1;		
}

static uint8_t writePins(char *data)
{
	u8 i;
	if(output_Inited==0)
	{
		output_Inited=1;
		hc595.init();
		pinMode(&PB11,GPIO_Mode_Out_PP);
		pinMode(&PB10,GPIO_Mode_Out_PP);
		
		pinMode(&PC13,GPIO_Mode_Out_PP);
		pinMode(&PC14,GPIO_Mode_Out_PP);
		pinMode(&PC15,GPIO_Mode_Out_PP);
		
		pinMode(&PC0,GPIO_Mode_Out_PP);
		pinMode(&PC1,GPIO_Mode_Out_PP);
		pinMode(&PC2,GPIO_Mode_Out_PP);
	}	
	if( strlen(data)==16)
	{		
		pinWrite(&PB11,data[0]-'0');
		pinWrite(&PB10,data[1]-'0');
		pinWrite(&PC13,data[2]-'0');
		pinWrite(&PC14,data[3]-'0');
		pinWrite(&PC15,data[4]-'0');
		pinWrite(&PC0,data[5]-'0');
		pinWrite(&PC1,data[6]-'0');
		pinWrite(&PC2,data[7]-'0');
		for(i=0;i<8;i++)
		{
			if(data[8+i]=='1') 	hc595.out[0] |= (0x80>>(7-i));
			else   							hc595.out[0] &= (0x7f>>(7-i));
			hc595.write(&hc595.out[0],1,MSB_FIRST);	
			hc595.updata();
		}
		rt_kprintf("=ok");	return 1;	
	}
	else return 0;
}


FINSH_FUNCTION_EXPORT(readPin, read input[1 - 16] on[1] or off[0].)
FINSH_FUNCTION_EXPORT(readPins, read inputs[16] on[1] or off[0].)
FINSH_FUNCTION_EXPORT(writePin, write output[1 - 16] on[1] or off[0].)
FINSH_FUNCTION_EXPORT(writePins, write outputs on["1"] or off["0"].)

