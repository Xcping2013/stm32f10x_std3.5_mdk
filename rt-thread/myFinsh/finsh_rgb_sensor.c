#include "finsh_cmd.h"

#ifdef ISL29125_USED

#define IN3_EARPHONE_V12 0
isl29125_t rgbSensor1=
{
#if (IN3_EARPHONE_V12)
	&PB6,
	&PB7,
	&PE3,
#else
	&PE15,
	&PE14,
	&PE12,
#endif

	ISL29125_MODE_RGB,
	ISL29125_RANGE_10K,
	ISL29125_RESOLUTION_16,
	
	ISL29125_INTERRUPT_STATUS_RED,
	ISL29125_INTERRUPT_PERSIST_1,
	ISL29125_INTERRUPT_CONV_DIS,
	0,
	8000,
};

isl29125_rgb_t data;
color_rgb_t data8bit;	

void rgbSensor_init(void)
{
	memset(&data, 0x00, sizeof(data));
	rt_kprintf("ISL29125 init ... ");	

	if (isl29125_init(&rgbSensor1)== 0)
	{
			rt_kprintf("[OK]\n");
	}
	else rt_kprintf("[ERR]\n");
	isl29125_init_int(&rgbSensor1);
	
//	static const isl29125_mode_t modes[] = 
//	{
//		ISL29125_MODE_DOWN, ISL29125_MODE_STANDBY, ISL29125_MODE_RGB,
//		ISL29125_MODE_R, ISL29125_MODE_G, ISL29125_MODE_B,
//		ISL29125_MODE_RG, ISL29125_MODE_GB
//	};
//	static const char* mode_names[] =
//	{
//		"ISL29125_MODE_DOWN", "ISL29125_MODE_STANDBY", "ISL29125_MODE_RGB",
//		"ISL29125_MODE_R", "ISL29125_MODE_G", "ISL29125_MODE_B",
//		"ISL29125_MODE_RG", "ISL29125_MODE_GB"
//	};
//	for (int i = 0; i < sizeof(modes); i++)
//	{
//		rt_kprintf("Setting mode %s\n", mode_names[i]);
//		isl29125_set_mode(&rgbSensor1, modes[i]);
//		delay_ms(200);
//		isl29125_read_rgb_color(&rgbSensor1, &data8bit);
//		rt_kprintf("RGB value: (%3i / %3i / %3i) 8 bit\n",
//						data8bit.r, data8bit.g, data8bit.b);
//	}

    
}
void rgbSensor_loop(u8 time)
{
	rt_kprintf("Resetting mode to RGB and reading continuously");
    isl29125_set_mode(&rgbSensor1, ISL29125_MODE_RGB);
    delay_ms(200);
    for (int i = 0; i < time; i++)
		{
//			isl29125_read_rgb_lux(&rgbSensor1, &data);
//			rt_kprintf("RGB value: (%5i / %5i / %5i) lux\n",
//									(int)data.red, (int)data.green, (int)data.blue);
			delay_ms(200);
//			rt_kprintf("IRQ-Status: %i \n", isl29125_read_irq_status(&rgbSensor1));
			isl29125_read_rgb_color(&rgbSensor1, &data8bit);
			rt_kprintf("RGB value: (%3i / %3i / %3i) 8 bit\n",
						data8bit.r, data8bit.g, data8bit.b);
		}
}
//RGB value: ( 25 /  14 /   2) 8 bit
//RGB value: (  2 /   5 /   3) 8 bit

//RGB value: ( 12 /   5 /   0) 8 bit
//RGB value: (  2 /   5 /   3) 8 bit

//RGB value: ( 15 /   4 /   0) 8 bit
//RGB value: (  2 /   5 /   3) 8 bit

//RGB value: ( 11 /   6 /   1) 8 bit
//RGB value: (  2 /   5 /   3) 8 bit

FINSH_FUNCTION_EXPORT(rgbSensor_init,.)
FINSH_FUNCTION_EXPORT(rgbSensor_loop,.)
#endif
