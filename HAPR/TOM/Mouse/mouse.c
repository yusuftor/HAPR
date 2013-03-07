#include "lpc17xx_uart.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_rit.h"
#include "lpc_types.h"
#include <string.h>
#include "serial.h"
#include "sound.h"
#include "KeyboardHost.h"
#include "ConsoleDebug.h"
#include "Movement.h"
#include <math.h>

#define INTERVAL 10
#define LENGTH 6.9f
#define CONVERT ((float) 800 / 2.5)

void RIT_IRQHandler();
void cb(uint8_t buttons, int8_t X, int8_t Y);
void attach();
void detach();
void mousepins();

float currentX;
float currentY;
float currentTHETA;

int main() 
{
	//currentX = 0.0f;
	//currentY = 0.0f;
	currentTHETA = 0.0f;
	Init_Serial();
	ConsoleInit_Serial();
	WriteByte((char) 0xB7);

	RIT_Init(LPC_RIT);
	RIT_TimerConfig(LPC_RIT, INTERVAL);
	NVIC_SetPriority(RIT_IRQn, ((0x01<<3)|0x01));	
	NVIC_EnableIRQ(RIT_IRQn);
	

	mousepins();
	mouse_init(cb, attach, detach);
	
	int tryit = 0;
	while(!tryit)
	{
	if(currentX > 10.0f){
		tryit++;
		play(_BEEP);
	}
}
	while(1);
	return(1);
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
	float adjustedX, adjustedY;

	adjustedX = ((float) X) / CONVERT;
	adjustedY = ((float) Y) / CONVERT;

	float newX, newY;
	float newTHETA;

	newTHETA = currentTHETA + ((float)( adjustedY / (float) LENGTH));
	newX = currentX + (((float) adjustedX) * cosf(currentTHETA));
	newY = currentY + (((float) adjustedY) * sinf(currentTHETA));

	ConsoleWrite("THETA: ");
	ConsoleWriteFloat(newTHETA);
	ConsoleWrite("\n\r");
	ConsoleWrite("X: ");
	ConsoleWriteFloat(newX);
	ConsoleWrite("\n\r");
	ConsoleWrite("Y: ");
	ConsoleWriteFloat(newY);
	ConsoleWrite("\n\r");
	ConsoleWrite("\n\r");
	//play("T240O8V10MSC");
	

	if(newTHETA > 6.28f)
	{
	currentTHETA = newTHETA - 6.28f;
	play(_BEEP);
	}else if(newTHETA < -6.28f){
	currentTHETA = newTHETA + 6.28f;
	play(_BEEP);
	}else{
	currentTHETA = newTHETA;
	}

	currentX = newX;
	currentY = newY;
}

void attach()
{
	ConsoleWrite("attached");
	//play(_ALARM);

}

void detach()
{
	ConsoleWrite("detached");
	//play("CCCCC");
}

void RIT_IRQHandler()
{
	RIT_GetIntStatus(LPC_RIT);	
	mouse_poll();	
	//ConsoleWrite("poll");
	
}
