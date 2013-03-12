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

void InitLogs();
void EINT3_IRQHandler();
char* MakeTimestamp();
void AddLog(char* log);
void PrintLogs();

int originSeconds;
int originMinutes;
log* head;
log* last;
char timestampstr[8] = "";

//Method: InitLogs()
//Author: Tom Collier
//Use: This method sets up and adds the first log to the onboard logging system,
// the system is implemented in a linked list so it takes no more space than is necessary. 
void InitLogs()
{

	log * temp = (log *)malloc(sizeof(log)); //create initial log and set the start and end of the list to its position
	temp->logtext = "@ 00:00 PROGRAM STARTED\n\r";
	temp->next = NULL;
	last = temp;
	head = temp;
	
	originSeconds = RTC_GetTime(LPC_RTC, RTC_TIMETYPE_SECOND); //get the real-world time the program began execution
	originMinutes = RTC_GetTime(LPC_RTC, RTC_TIMETYPE_MINUTE);

	RTC_ResetClockTickCounter(LPC_RTC);
	RTC_Cmd(LPC_RTC, ENABLE); //start the clock
}

//Method: EINT3_IRQHandler()
//Author: Tom Collier
//Use: This method handles the button press, printing the logs at the end of execution, or  
// starting execution dependent on the go variable 
void EINT3_IRQHandler()
{
	addlog("LOGS PRINTED");
	PrintLogs();
	GPIO_ClearInt(2,(1<<5));  	
}

//Method: MakeTimestamp()
//Author: Tom Collier
//Use: This method creates a timestamp showing how far into execution the robot is
// the timestamp is use to indicate when each succesive method call or state change happened 
char* MakeTimestamp()
{	
	
	int secval = RTC_GetTime(LPC_RTC, RTC_TIMETYPE_SECOND); //get current time
	int minval = RTC_GetTime(LPC_RTC, RTC_TIMETYPE_MINUTE);
	
	if(secval < originSeconds) //get difference in time from origin
	{	
		secval = (secval + 60) - originSeconds;
		minval = minval - 1 - originMinutes;
	}else{
		secval = secval - originSeconds;
		minval = minval - originMinutes;
	}
	
	int a = sprintf(timestampstr,"@ %d:%d ", minval, secval); //create timestamp
	return timestampstr;
}

//Method: AddLog(char* logstring)
//Author: Tom Collier
//Use: This method appends a new log to the end of the list with the text of logstring
void AddLog(char* logstring)
{
	
	log *  newlog = (log *)malloc(sizeof(log)); //allocate memory for new log

	*timestampstr = maketimestamp(); //get timestamp value
	
	char *tempstring = malloc(strlen(timestampstr) + strlen(logstring) + 3); // additional bytes for \n\r\0
	strcpy(tempstring, timestampstr); //fill string with log content
	strcat(tempstring, logstring);
	strcat(tempstring, "\n\r");
	
	newlog->logtext = tempstring; //set last item of list to the new log
	newlog->next = NULL;
					
	last->next = newlog; 

	last = newlog;

}

//Method: PrintLogs()
//Author: Tom Collier
//Use: Prints all logs to screen, called on the button press at the end of execution 
void PrintLogs()
{
	last = head;
	while(last->next) //while another log exists, print it to screen
		{
			ConsoleWrite(last->logtext);
			last = last->next;
		}
	ConsoleWrite(last->logtext); //write the last log out (due to last log->next = null as it's the last one
}
