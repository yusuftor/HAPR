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
#include "lpc17xx_gpio.h"
#include <math.h>

#define INTERVAL 10
#define LENGTH 6.9f
#define CONVERT ((float) 940 / 2.5)

void RIT_IRQHandler();
void cb(uint8_t buttons, int8_t X, int8_t Y);
void attach();
void detach();
void mousepins();
void turnangle(float angle);
void movedistance(float distance);
void initButton();
void EINT3_IRQHandler();

float currentX;
float currentY;
float currentTHETA;

int go;
int moving = 0;

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
	
	go = 0;

	mousepins();
	mouse_init(cb, attach, detach);

	initButton();

	while(!go);

	movedistance(200.0f);
	Delay(1000);
	turnangle(-3.14);
	Delay(1000);
	movedistance(200.0f);


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


	if(moving && newTHETA > currentTHETA)
	{
		rightS--;
		MotorSet((leftS / 127.0f), (rightS / 127.0f));
	}else if(moving && newTHETA < currentTHETA)
	{
		rightS++;
		MotorSet((leftS / 127.0f), (rightS / 127.0f));
	}


	
	currentTHETA = newTHETA;
	ConsoleWriteFloat(currentTHETA);
	ConsoleWrite("\n\r");
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

void turnangle(float angle)
{

	angle = -(angle);


if(angle > 3.14f)
{
angle = -(angle - 3.14f);
}else if(angle < -3.28f)
{
angle = -(angle + 3.14f);
}

float finalTHETA = currentTHETA + angle;
ConsoleWriteFloat(currentTHETA);
ConsoleWriteFloat(finalTHETA);
float angle10 = angle * 0.1f;
ConsoleWriteFloat(angle10);
float prelimTHETA = finalTHETA - angle10;
ConsoleWriteFloat(prelimTHETA);

if(angle > 0.0f)
{
Spin(-0.2f);
while(currentTHETA < prelimTHETA);
Spin(-0.15f);
while(currentTHETA < finalTHETA);
Stop();

}else if(angle < 0.0f){
Spin(0.2f);
while(currentTHETA > prelimTHETA);
Spin(0.15f);
while(currentTHETA > finalTHETA);
Stop();
}

}

void movedistance(float distance)
{

float finalX = currentX + distance;
float distance10 = distance * 0.1f;
float prelimX = finalX - distance10;


if(distance > 0.0f)
{
Move(0.2f);
moving = 1;
while(currentX < prelimX);
Move(0.15f);

while(currentX < finalX);
Stop();
moving = 0;

}else if(distance < 0.0f){
Move(-0.2f);
moving = 1;
while(currentX > prelimX);
Move(-0.15f);
while(currentX > finalX);
Stop();
moving = 0;
}

}

void initButton()
{
	GPIO_SetDir(2,(1<<21),0);
	GPIO_IntCmd(2,(1<<5),0);
	NVIC_EnableIRQ(EINT3_IRQn);
}

void EINT3_IRQHandler()
{
	NVIC_DisableIRQ(EINT3_IRQn);
	go = 1;
	currentX = 0.0f;
	currentY = 0.0f;
	currentTHETA = 0.0f;
	GPIO_ClearInt(2,(1<<5));   	
}