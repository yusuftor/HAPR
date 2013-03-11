#include "lpc17xx_uart.h"
#include "lpc17xx_pinsel.h"
#include "ConsoleDebug.h"
#include "lpc17xx_rtc.h"
#include "lpc17xx_gpio.h"
#include "Serial.h"

/*struct loglist {
	char* log;
	struct loglist* next;
};
typedef struct loglist log;*/



void initButton();
void EINT3_IRQHandler();
//char* maketimestamp();
//void addlog(char* log);
//void printlogs();

//int originSeconds;
//int originMinutes;
//log* head;
//log* last;

int main()
{
	WriteByte((char) 0xB7);
	/*head = (log *)malloc(sizeof(log));
	head->log = "@ 00:00 PROGRAM STARTED";
	head->next = NULL;
	last = head;*/

	ConsoleInit_Serial();
	RTC_Init(LPC_RTC);
	ConsoleWrite("Go");
	int originSeconds = RTC_GetTime(LPC_RTC, RTC_TIMETYPE_SECOND);
	int originMinutes = RTC_GetTime(LPC_RTC, RTC_TIMETYPE_MINUTE);

	RTC_ResetClockTickCounter(LPC_RTC);
	RTC_Cmd(LPC_RTC, ENABLE);

	initButton();
	while(1);
}

void initButton()
{
	GPIO_SetDir(2,(1<<21),0);
	GPIO_IntCmd(2,(1<<5),0);
	NVIC_EnableIRQ(EINT3_IRQn);
}

void EINT3_IRQHandler()
{
	//printlogs();  	
}
/*
char* maketimestamp()
{
	char* str;

	int secval = RTC_GetTime(LPC_RTC, RTC_TIMETYPE_SECOND);
	int minval = RTC_GetTime(LPC_RTC, RTC_TIMETYPE_MINUTE);

	secval = secval - originSeconds;
	minval = minval - originMinutes;

	if(secval < 0)
	{
		minval--;
		secval = 60 - secval;
	}

	int a = sprintf(str,"@ %d:%d ", minval, secval);
	return str;

}

void addlog(char* logstring)
{

	log *  newlog = (log *)malloc(sizeof(log));
	int a =  sprintf(newlog->log,"%s%s\n\r", maketimestamp,  logstring);
	newlog->next = NULL;
	last->next = &newlog;
	last = newlog;
}

void printlogs()
{
	log* curr;
	addlog("LOGS PRINTED");
	ConsoleWrite(head->log);
	curr = head;
	while(curr->next)
	{
		curr = curr->next;
		ConsoleWrite(curr->log);
	}
}
*/
