#include "finsh_cmd.h"

#ifdef EXIT_USED

void cylinder_set(char* cylinder);
void cylinder_reset(char* cylinder);
	
void	sendMessage(void);
	
exit_t SensorIt1=
{
	0x03,
	&PE8,
	EXTI_Trigger_Falling,
	ENABLE,
	sendMessage,	
};
void	sendMessage(void)
{
	rt_kprintf("PE8 is press\r\n");	
	pinToggle(&PE2);
}

void exit_start(void)
{
	exit_init(&SensorIt1);	
	pinMode(&PE2,GPIO_Mode_Out_PP);
	pinSet(&PE2);
	pinMode(&PE4,GPIO_Mode_Out_PP);
	pinSet(&PE4);
	pinMode(&PE6,GPIO_Mode_Out_PP);
	pinSet(&PE6);
}

FINSH_FUNCTION_EXPORT(exit_start,.)

#endif
