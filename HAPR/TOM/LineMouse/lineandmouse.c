#include "lpc17xx_uart.h"
#include "lpc_types.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_rit.h"
#include "Serial.h"
#include "Movement.h"
#include "KeyboardHost.h"
#include "ConsoleDebug.h"
#include "lpc17xx_gpio.h"
#include "sound.h"

#define GPIO_INT        (1<<12)
#define INTERVAL 10
#define LENGTH 6.9f
#define CONVERT ((float) 800 / 2.5)

float currentTHETA;

void RIT_IRQHandler();
void cb(uint8_t buttons, int8_t X, int8_t Y);
void attach();
void detach();
void mousepins();
void Init_IRQ();
int GetDigitalSensorStatus();
void EINT3_IRQHandler();

int main() {
	currentTHETA = 0.0f;
	Init_Serial();
	Init_IRQ();
	ConsoleInit_Serial();
	WriteByte((char) 0xB7);
	
	RIT_Init(LPC_RIT);
	RIT_TimerConfig(LPC_RIT, INTERVAL);
	NVIC_SetPriority(RIT_IRQn, ((0x01<<3)|0x01));	
	NVIC_EnableIRQ(RIT_IRQn);

	mousepins();
	mouse_init(cb, attach, detach);

	Calibrate();
	WriteByte((char) 0xBB); // Start PID
	WriteByte((char)((int)127.0f*0.4f)); // Max motor speed
	char parameters[4] = {0x01, 0x14, 0x03, 0x02};
	Write(parameters, 4); // Parameters to adjust motor speed differences
	while (1);
}

void Init_IRQ() {
  GPIO_SetDir(0,GPIO_INT,0); // Initialise
  GPIO_IntCmd(0,(1<<17),1);
  NVIC_SetPriority(EINT3_IRQn, 0x10);
  NVIC_EnableIRQ(EINT3_IRQn);
}


int GetDigitalSensorStatus() {
  int result = GPIO_ReadValue(0);
  return (result & (1<<17));
}

void EINT3_IRQHandler() // Interrupt handler
{
  if (GPIO_GetIntStatus(0, 17, 1)) {
	WriteByte((char) 0xBC);
	StopImmediately();
  }	
  while(!GetDigitalSensorStatus());
  GPIO_ClearInt(0,(1<<17));  
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

	//adjustedX = ((float) X) / CONVERT;
	adjustedY = ((float) Y) / CONVERT;

	//float newX, newY;
	float newTHETA;

	newTHETA = currentTHETA + ((float)( adjustedY / (float) LENGTH));
	//newX = oldX(

	ConsoleWrite("THETA: ");
	ConsoleWriteFloat(newTHETA);
	//ConsoleWrite("  y: ");
	//ConsoleWriteInt((int) totY);
	ConsoleWrite("\n\r");
	//play("T240O8V10MSC");
	if(newTHETA > 6.28f)
	{
	currentTHETA = newTHETA - 6.28f;
	play(_BEEP);
	}else if(newTHETA < -6.28f){
	currentTHETA = newTHETA + 6.28f;
	}else{
	currentTHETA = newTHETA;
	}
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
