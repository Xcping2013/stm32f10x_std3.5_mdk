#include "finsh_cmd.h"

#ifdef DS18B20_USED

void get_temp(void)
{
	char temp[10];
	sprintf(temp,"%5.2f",Ds18b20_get_temp(&PB3));
	rt_kprintf("=%s'C",temp);	
	
}
FINSH_FUNCTION_EXPORT(get_temp,.)

#endif
