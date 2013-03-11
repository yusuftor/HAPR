#include "lpc17xx_uart.h"
#include "lpc17xx_pinsel.h"
#include "ConsoleDebug.h"
#include "lpc17xx_rtc.h"
#include "lpc17xx_gpio.h"
#include "Serial.h"
#include "Movement.h"
#include <stdlib.h>

struct loglist {
	char* logtext;
	struct loglist* next;
};
typedef struct loglist log;

void initButton();
void EINT3_IRQHandler();
char* maketimestamp();
void addlog(char* log);

int originSeconds;
int originMinutes;
log* head;
log* last;
char timestampstr[8] = "";

int main()
{
	Init_Serial();
	WriteByte((char) 0xB7);

	initButton();
	ConsoleInit_Serial();
	RTC_Init(LPC_RTC);

	originSeconds = RTC_GetTime(LPC_RTC, RTC_TIMETYPE_SECOND);
	ConsoleWriteInt(originSeconds);
	originMinutes = RTC_GetTime(LPC_RTC, RTC_TIMETYPE_MINUTE);
	ConsoleWriteInt(originMinutes);

	log * temp = (log *)malloc(sizeof(log));
	temp->logtext = "@ 00:00 PROGRAM STARTED\n\r";
	temp->next = NULL;
	last = temp;
	head = temp;

	RTC_ResetClockTickCounter(LPC_RTC);
	RTC_Cmd(LPC_RTC, ENABLE);

	while(1);
	return(1);
}

void initButton()
{
	GPIO_SetDir(2,(1<<21),0);
	GPIO_IntCmd(2,(1<<5),0);
	NVIC_EnableIRQ(EINT3_IRQn);
}

void EINT3_IRQHandler()
{
	addlog("LOGS PRINTED");
	printlogs();
	GPIO_ClearInt(2,(1<<5));  	
}

char* maketimestamp()
{	
	
	int secval = RTC_GetTime(LPC_RTC, RTC_TIMETYPE_SECOND);
	int minval = RTC_GetTime(LPC_RTC, RTC_TIMETYPE_MINUTE);

	//secval = secval - originSeconds;
	//minval = minval - originMinutes;
	
	if(secval < originSeconds)
	{	
		secval = (secval + 60) - originSeconds;
		minval = minval - 1 - originMinutes;
	}else{
		secval = secval - originSeconds;
		minval = minval - originMinutes;
	}
	
	int a = sprintf(timestampstr,"@ %d:%d ", minval, secval);
	ConsoleWrite(timestampstr);
	return timestampstr;
}

void addlog(char* logstring)
{
	
	log *  newlog = (log *)malloc(sizeof(log));

	*timestampstr = maketimestamp();
	
	char *tempstring = malloc(strlen(timestampstr) + strlen(logstring) + 3); // additional bytes for \n\r\0
	strcpy(tempstring, timestampstr);
	strcat(tempstring, logstring);
	strcat(tempstring, "\n\r");
	
	newlog->logtext = tempstring;
	newlog->next = NULL;
					
	last->next = newlog;

	last = newlog;

}

void printlogs()
{
	last = head;
	while(last->next)
		{
			ConsoleWrite(last->logtext);
			last = last->next;
		}
	ConsoleWrite(last->logtext);
}
