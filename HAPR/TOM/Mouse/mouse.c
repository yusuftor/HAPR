#include "lpc17xx_uart.h"
#include "lpc17xx_pinsel.h"
#include "lpc_types.h"
#include <string.h>
#include "serial.h"
#include "sound.h"
#include "KeyboardHost.h"
#include "ConsoleDebug.h"

#define INTERVAL 15

void RIT_IRQHandler();
void cb(uint8_t buttons, int8_t X, int8_t Y);
void attach();
void detach();
void mousepins();

int main() 
{

	Init_Serial();
	ConsoleInit_Serial();
	WriteByte((char) 0xB7);

	RIT_Init(LPC_RIT);
	RIT_TimerConfig(LPC_RIT, INTERVAL);
	NVIC_SetPriority(RIT_IRQn, ((0x01<<3)|0x01));	
	NVIC_EnableIRQ(RIT_IRQn);
	

	mousepins();
	mouse_init(cb, attach, detach);

	while(1);

}

void mousepins()
{

	PINSEL_CFG_Type PinCfg;

	PinCfg.Funcnum = 1;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Portnum = 1;
	PinCfg.Pinnum = 18;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 9;
	PinCfg.Portnum = 2;
	PINSEL_ConfigPin(&PinCfg);

}

void cb(uint8_t buttons, int8_t X, int8_t Y)
{
	ConsoleWrite("x: ");
	ConsoleWriteInt(X);
	ConsoleWrite("  y: ");
	ConsoleWriteInt(Y);
	ConsoleWrite("\n\r");
}

void attach()
{
	ConsoleWrite("attached");
}

void detach()
{
	ConsoleWrite("detached");
}

void RIT_IRQHandler()
{
	RIT_GetIntStatus(LPC_RIT);	
	mouse_poll();	
	//ConsoleWrite("poll");
	
}
