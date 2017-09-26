#include "finsh_cmd.h"

#ifdef L6480_USED

L6480_RegsStruct_TypeDef MotorConfig[N_O_MOTORS];

void L6480s_init(void)
{
	uint8_t i;
	pinMode(&L6480_CS1_PIN,GPIO_Mode_Out_PP);
	pinMode(&L6480_CS2_PIN,GPIO_Mode_Out_PP);
	pinMode(&L6480_CS2_PIN,GPIO_Mode_Out_PP);
	
	pinMode(&L6480_RESET1_PIN,GPIO_Mode_Out_PP);
	pinMode(&L6480_RESET2_PIN,GPIO_Mode_Out_PP);
	pinMode(&L6480_RESET3_PIN,GPIO_Mode_Out_PP);
	
	pinMode(&L6480_BUSY1_PIN,GPIO_Mode_IPU);
	pinMode(&L6480_BUSY2_PIN,GPIO_Mode_IPU);
	pinMode(&L6480_BUSY3_PIN,GPIO_Mode_IPU);
	pinMode(&L6480_FLAG1_PIN,GPIO_Mode_IPU);
	pinMode(&L6480_FLAG2_PIN,GPIO_Mode_IPU);
	pinMode(&L6480_FLAG3_PIN,GPIO_Mode_IPU);
	
	pinSet(&L6480_CS1_PIN);			pinSet(&L6480_CS2_PIN);			pinSet(&L6480_CS3_PIN);
	pinSet(&L6480_RESET1_PIN);	pinSet(&L6480_RESET2_PIN);	pinSet(&L6480_RESET2_PIN);
	
	spi.init(&L6480s.spiConfig);
	
	for(i=1;i<N_O_MOTORS+1;i++)
	{
		L6480s.motor=i;
		L6480_Reset_And_Standby();
		L6480_Regs_Struct_Reset(&MotorConfig[i-1]);
		
		MotorConfig[i-1].ACC 		= AccDec_Steps_to_Par(L6480_CONF_PARAM_ACC);
		MotorConfig[i-1].DEC 		= AccDec_Steps_to_Par(L6480_CONF_PARAM_DEC); 
		MotorConfig[i-1].MAX_SPEED 	= MaxSpd_Steps_to_Par(L6480_CONF_PARAM_MAX_SPEED);
		MotorConfig[i-1].FS_SPD 	= FSSpd_Steps_to_Par(L6480_CONF_PARAM_FS_SPD);
		MotorConfig[i-1].MIN_SPEED	= L6480_CONF_PARAM_LSPD_BIT|MinSpd_Steps_to_Par(L6480_CONF_PARAM_MIN_SPEED);
		MotorConfig[i-1].KVAL_ACC 	= Kval_Perc_to_Par(L6480_CONF_PARAM_KVAL_ACC);
					/* Deceleration duty cycle (torque) settings to L6480_CONF_PARAM_KVAL_DEC in %, range 0 to 99.6% */
		MotorConfig[i-1].KVAL_DEC 	= Kval_Perc_to_Par(L6480_CONF_PARAM_KVAL_DEC);		
					/* Run duty cycle (torque) settings to L6480_CONF_PARAM_KVAL_RUN in %, range 0 to 99.6% */
		MotorConfig[i-1].KVAL_RUN 	= Kval_Perc_to_Par(L6480_CONF_PARAM_KVAL_RUN);
					/* Hold duty cycle (torque) settings to L6480_CONF_PARAM_KVAL_HOLD in %, range 0 to 99.6% */
		MotorConfig[i-1].KVAL_HOLD 	= Kval_Perc_to_Par(L6480_CONF_PARAM_KVAL_HOLD);
						/* Thermal compensation param settings to L6480_CONF_PARAM_K_THERM, range 1 to 1.46875 */
		MotorConfig[i-1].K_THERM 	= KTherm_to_Par(L6480_CONF_PARAM_K_THERM);
		/* Intersect speed settings for BEMF compensation to L6480_CONF_PARAM_INT_SPD in steps/s, range 0 to 3906 steps/s */
		MotorConfig[i-1].INT_SPD 	= IntSpd_Steps_to_Par(L6480_CONF_PARAM_INT_SPD);
		/* BEMF start slope settings for BEMF compensation to L6480_CONF_PARAM_ST_SLP in % step/s, range 0 to 0.4% s/step */
		MotorConfig[i-1].ST_SLP 	= BEMF_Slope_Perc_to_Par(L6480_CONF_PARAM_ST_SLP);
		/* BEMF final acc slope settings for BEMF compensation to L6480_CONF_PARAM_FN_SLP_ACC in% step/s, range 0 to 0.4% s/step */
		MotorConfig[i-1].FN_SLP_ACC = BEMF_Slope_Perc_to_Par(L6480_CONF_PARAM_FN_SLP_ACC);
		/* BEMF final dec slope settings for BEMF compensation to L6480_CONF_PARAM_FN_SLP_DEC in% step/s, range 0 to 0.4% s/step */
		MotorConfig[i-1].FN_SLP_DEC = BEMF_Slope_Perc_to_Par(L6480_CONF_PARAM_FN_SLP_DEC);
		/* Stall threshold settings to L6480_CONF_PARAM_STALL_TH in mV, range 31.25 to 1000mV */
		MotorConfig[i-1].STALL_TH 	= StallTh_to_Par(L6480_CONF_PARAM_STALL_TH);
					/* Set Config register according to config parameters */
					/* clock setting, switch hard stop interrupt mode, */
					/*  supply voltage compensation, overcurrent shutdown */
					/* UVLO threshold, VCC reg output voltage , PWM frequency */
		MotorConfig[i-1].CONFIG 	= (uint16_t)L6480_CONF_PARAM_CLOCK_SETTING | \
																						(uint16_t)L6480_CONF_PARAM_SW_MODE	   | \
																						(uint16_t)L6480_CONF_PARAM_VS_COMP       | \
																						(uint16_t)L6480_CONF_PARAM_OC_SD         | \
																						(uint16_t)L6480_CONF_PARAM_UVLOVAL       | \
																						(uint16_t)L6480_CONF_PARAM_VCCVAL	   | \
																						(uint16_t)L6480_CONF_PARAM_PWM_DIV       | \
																						(uint16_t)L6480_CONF_PARAM_PWM_MUL;
			
		/* Overcurrent threshold settings to L6480_CONF_PARAM_OCD_TH, range 31.25 to 1000mV */
		MotorConfig[i-1].OCD_TH 	= L6480_CONF_PARAM_OCD_TH;        
					/* Alarm settings to L6480_CONF_PARAM_ALARM_EN */
		MotorConfig[i-1].ALARM_EN 	= L6480_CONF_PARAM_ALARM_EN;
					/* Step mode and sycn mode settings via L6480_CONF_PARAM_SYNC_MODE and L6480_CONF_PARAM_STEP_MODE */
		MotorConfig[i-1].STEP_MODE 	= (uint8_t)L6480_CONF_PARAM_SYNC_MODE | \
																						(uint8_t)L6480_CONF_PARAM_STEP_MODE;
		/* Sink/source current, duration of constant current phases, duration of overboost phase settings */
		MotorConfig[i-1].GATECFG1       = (uint16_t)L6480_CONF_PARAM_IGATE | \
																			(uint16_t)L6480_CONF_PARAM_TCC   | \
																			(uint16_t)L6480_CONF_PARAM_TBOOST;
		/* Blank time, Dead time stiings */
		MotorConfig[i-1].GATECFG2       = (uint16_t)L6480_CONF_PARAM_TBLANK | \
																			 (uint16_t)L6480_CONF_PARAM_TDT;
		/* Program all cSPIN registers */		
		L6480_Registers_Set(&MotorConfig[i-1]);	
		
		/* Send cSPIN command change hold duty cycle to 0.5% */
		L6480_Set_Param(L6480_KVAL_HOLD, Kval_Perc_to_Par(0.5));	
		/* Send L6480 command change run duty cycle to 5% */
		L6480_Set_Param(L6480_KVAL_RUN, Kval_Perc_to_Par(5));	
		L6480_Set_Param(L6480_KVAL_HOLD, Kval_Perc_to_Par(L6480_CONF_PARAM_KVAL_HOLD));	
		/* RESET KVAL_RUN to initial value */
		L6480_Set_Param(L6480_KVAL_RUN, Kval_Perc_to_Par(L6480_CONF_PARAM_KVAL_RUN));
	}
}

//
void L6480s_Rotate(uint8_t motor, uint8_t dir, uint32_t speed)
{
	if(0<motor&&motor<N_O_MOTORS+1)
	{
		L6480s.motor = motor;
		L6480_Run(dir, speed);
		rt_kprintf("=ok");
	}
	
}
//
void L6480s_Move(uint8_t motor, uint8_t dir, uint32_t n_step)
{
	if(0<motor&&motor<N_O_MOTORS+1)
	{
		L6480s.motor = motor;
		L6480_Move(dir, n_step);
		rt_kprintf("=ok");
	}
}
//
void L6480s_Move_To(uint8_t motor, uint8_t dir, uint32_t abs_pos)
{
	if(0<motor&&motor<N_O_MOTORS+1)
	{
		L6480s.motor = motor;
		L6480_Go_To_Dir(dir, abs_pos);
		rt_kprintf("=ok");
	}
}
//
void L6480s_Stop(uint8_t motor)
{
	if(0<motor&&motor<N_O_MOTORS+1)
	{
		L6480s.motor = motor;
		//L6480_Soft_Stop();
		L6480_Hard_Stop();
		rt_kprintf("=ok");
	}
}

void L6480s_Set(uint8_t motor,uint8_t reg, uint32_t value)
{
	if(0<motor&&motor<N_O_MOTORS+1)
	{
		L6480s.motor = motor;
		//L6480_Soft_Stop();
		L6480_Set_Param(reg, value);
		rt_kprintf("=ok");
	}
}

void L6480s_Get(uint8_t motor,uint8_t reg, uint32_t value)
{
	if(0<motor&&motor<N_O_MOTORS+1)
	{
		L6480s.motor = motor;

		value=L6480_Get_Param(reg);
		rt_kprintf("=%d",value);
	}
}

FINSH_FUNCTION_EXPORT_ALIAS(L6480s_init,init, .)


FINSH_FUNCTION_EXPORT_ALIAS(L6480s_Rotate,rotate, .)
FINSH_FUNCTION_EXPORT_ALIAS(L6480s_Move,move, .)
FINSH_FUNCTION_EXPORT_ALIAS(L6480s_Move_To,go, .)
FINSH_FUNCTION_EXPORT_ALIAS(L6480s_Stop,stop, .)

FINSH_FUNCTION_EXPORT_ALIAS(L6480s_Set,set, .)
FINSH_FUNCTION_EXPORT_ALIAS(L6480s_Get,get, .)

#endif
