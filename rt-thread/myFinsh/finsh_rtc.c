#include "finsh_cmd.h"

#ifdef CALENDAR_USED

void rtc_sec_sendMessage1(void);
void rtc_alarm_sendMessage1(void);
void rtc_overflow_sendMessage1(void);

Rtc_DrvTypeDef 	rtc=
{
	0,0,0,0,
	Rtc_init,
	Rtc_set_counter,
	Rtc_get_counter,
	Rtc_set_alarm,
	
	rtc_sec_sendMessage1,
	rtc_alarm_sendMessage1,
	rtc_overflow_sendMessage1,
	
	Rtc_interrupt_en,
	
};

void rtc_sec_sendMessage1(void)
{
	rt_kprintf("count = %x\r\n", rtc.get_counter());	
}

void rtc_alarm_sendMessage1(void)
{
	rt_kprintf("alarm event\r\n");	
}

void rtc_overflow_sendMessage1(void)
{
	rt_kprintf("overflow event\r\n");	
}

void rtc_basic(void)
{
	rtc.init(0);
	rtc.set_counter(0xfffffffa);
	rtc.set_alarm(5);
	rtc.it_en(1, ENABLE);
	rtc.it_en(2, ENABLE);
	rtc.it_en(3, ENABLE);
}

void rtcsecit(void)
{
   sec_event();
   rt_kprintf("time=%02d:%02d:%02d\r\n", rtc.hour, rtc.min, rtc.sec);
}

void rtc_clock(void)
{
	rtc.sec_it=rtcsecit;
	set_clock(23,59,55);
	rtc.init(0);
	rtc.it_en(1, ENABLE);
}
//
void rtc_sec_event()
{
  Calendar_sec_process();
	rt_kprintf("date:%04d-%02d-%02d\r\nweek:%02d\r\n",calendar.dt.year+2000,calendar.dt.month,calendar.dt.date,calendar.dt.week);
	rt_kprintf("time:%02d:%02d:%02d\r\n",calendar.dt.hour,calendar.dt.min,calendar.dt.sec);
	rt_kprintf("date string:%s\r\n",calendar.date_string);
	rt_kprintf("time string:%s\r\n",calendar.time_string);
	rt_kprintf("cutdown dhms string:%s\r\n",calendar.cutdown_dddhhmmss_string);
	rt_kprintf("cutdown  hms string:%s\r\n\r\n",calendar.cutdown_hhmmss_string);
}

void calendar_app(void)
{
	Calendar_init();
	rtc.sec_it=rtc_sec_event;
	rtc.init(0);
	rtc.it_en(1, ENABLE);
	Calendar_set(16,12,31,23,00,00);
	alarm_set(17,1,2,0,0,0);
}

FINSH_FUNCTION_EXPORT(rtc_basic,.)
FINSH_FUNCTION_EXPORT(rtc_clock,.)
FINSH_FUNCTION_EXPORT(calendar_app,.)
#endif
