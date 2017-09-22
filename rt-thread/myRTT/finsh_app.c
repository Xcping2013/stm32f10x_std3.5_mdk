#define ENABLE_DEBUG (0)
#if ENABLE_DEBUG
#include <rtthread.h>
#define DEBUG rt_kprintf
#else
#define DEBUG(...)
#endif

#include "string.h"

void cylinder_set(char* cylinder);
void cylinder_reset(char* cylinder);

void cylinder_set(char* cylinder)
{
	if(strcmp(cylinder,"door")==0)  ;
	
}
void cylinder_reset(char* cylinder)
{
	
	
}

#if ENABLE_DEBUG
#include <finsh.h>
FINSH_FUNCTION_EXPORT(cylinder_set,----------)
FINSH_FUNCTION_EXPORT(cylinder_reset,----------)
#endif
