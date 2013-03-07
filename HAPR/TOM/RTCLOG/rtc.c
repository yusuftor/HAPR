#include "lpc17xx_uart.h"
#include "lpc17xx_pinsel.h"
#include "ConsoleDebug.h"
#include "lpc17xx_rtc.h"
#include "lpc17xx_clkpwr.h"
#include "lpc17xx_gpio.h"

void initButton();

int main()
{

	ConsoleInit_Serial();
	RTC_Init(LPC_RTC);

	int secval = RTC_GetTime(LPC_RTC, RTC_TIMETYPE_SECOND);
	int minval = RTC_GetTime(LPC_RTC, RTC_TIMETYPE_MINUTE);

	RTC_ResetClockTickCounter(LPC_RTC);
	RTC_Cmd(LPC_RTC, ENABLE);

	ConsoleWriteInt(minval);
	ConsoleWrite(":");
	ConsoleWriteInt(secval);
	ConsoleWrite("\n\r");

	initButton();
	while(1);
}

void initButton()
{
	GPIO_SetDir(2,(1<<21),0);
	GPIO_IntCmd(2,(1<<5),0);
	NVIC_EnableIRQ(EINT3_IRQn);
}

void EINT3_IRQHandler() {// Interrupt handler
  	
  	int secval = RTC_GetTime(LPC_RTC, RTC_TIMETYPE_SECOND);
	int minval = RTC_GetTime(LPC_RTC, RTC_TIMETYPE_MINUTE);


  	ConsoleWrite("Time : ");
 	ConsoleWriteInt(minval);
	ConsoleWrite(":");
	ConsoleWriteInt(secval);
	ConsoleWrite("\n\r");
  	GPIO_ClearInt(2,(1<<5)); 
}