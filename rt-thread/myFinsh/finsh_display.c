#include "finsh_cmd.h"

#ifdef TM1638_USED

static uint8_t tm1638_Inited = 0; 
//100个字节为基准
void led_show(char *num)
{
	if(tm1638_Inited==0)
	{
		tm1638.init(tm1638_8bit);
		tm1638_Inited=1;
	}	
	tm1638.show(tm1638_8bit,num);
}

FINSH_FUNCTION_EXPORT(led_show,  .)

#endif

