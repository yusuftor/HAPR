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
#define CONVERT ((float) 945 / 2.5)

void RIT_IRQHandler();
void Cb(uint8_t buttons, int8_t X, int8_t Y);
void Attach();
void Detach();
void MousePins();
void TurnAngle(float angle);
void MoveDistance(float distance, int xy);
void InitButton();
void EINT3_IRQHandler();

float currentX;
float currentY;
float currentTHETA;

int go;
int moving = 0;

void MousePins()
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

void Cb(uint8_t buttons, int8_t X, int8_t Y)
{
	float adjustedX, adjustedY;

	adjustedX = ((float) X) / CONVERT;
	adjustedY = ((float) Y) / CONVERT;

	float newX, newY;
	float newTHETA;

	if(moving)
	{

		newX = currentX + (((float) adjustedX) * cosf(currentTHETA));
		newY = currentY + (((float) adjustedX) * sinf(currentTHETA));
		newTHETA = currentTHETA + ((float)( adjustedY / (float) LENGTH));

		if(newTHETA > currentTHETA)
		{
		rightS--;
		MotorSet((leftS / 127.0f), (rightS / 127.0f));
		}else if(moving && newTHETA < currentTHETA)
		{
		rightS++;
		MotorSet((leftS / 127.0f), (rightS / 127.0f));
		}

		currentX = newX;
		currentY = newY;

	}else{

		newTHETA = currentTHETA + ((float)( adjustedY / (float) LENGTH));

		currentTHETA = newTHETA;
		ConsoleWrite("Callback\n\r");

	}
}

void Attach()
{
	ConsoleWrite("Attached\n\r");
}

void Detach()
{
	ConsoleWrite("Detached\n\r");
}

void RIT_IRQHandler()
{
	RIT_GetIntStatus(LPC_RIT);
	mouse_poll();
}

void TurnAngle(float angle)
{

	angle = -(angle);

	float finalTHETA = currentTHETA + angle;

	if(angle > 0.0f)
	{
		moving = 0;
		Spin(-0.17f);
		while(currentTHETA < finalTHETA);
		Stop();

	}else if(angle < 0.0f){
		moving = 0;
		Spin(0.17f);
		while(currentTHETA > finalTHETA);
		Stop();
	}

}

void MoveDistance(float distance, int xy)
{

	if(!xy)
	{

		float diff = currentTHETA;
		if((diff > 6.28f) || (diff < -6.28f))
		{

			diff = fmodf(diff,6.28f);

		}

		if(distance > 0.0f)
		{

			TurnAngle(diff);

		} else {

			TurnAngle(diff);
			Delay(1000);
			TurnAngle(3.14f);

		}

		float finalX = currentX + distance;
		float distance10 = distance * 0.1f;
		float prelimX = finalX - distance10;

		moving = 1;

		if(distance > 0.0f)
		{
			Move(0.2f);

			while(currentX < prelimX);
			Move(0.15f);

			while(currentX < finalX);
			Stop();

		}else if(distance < 0.0f){

			Move(0.2f);
			while(currentX > prelimX);
			Move(0.15f);
			while(currentX > finalX);
			Stop();
		}

	} else {

		float diff = currentTHETA;
		if((diff > 6.28f) || (diff < -6.28f))
		{

			diff = fmodf(diff,6.28f);

		}

		if(distance > 0.0f)

		{

			TurnAngle(diff);
			Delay(1000);
			TurnAngle(-1.57f);

		} else {

			TurnAngle(diff);
			Delay(1000);
			TurnAngle(1.57f);

		}

		float finalY = currentY + distance;
		float distance10 = distance * 0.1f;
		float prelimY = finalY - distance10;

		moving = 1;
		if(distance > 0.0f)
		{

			Move(0.2f);

			while(currentY < prelimY);
			Move(0.15f);

			while(currentY < finalY);
			Stop();


		}else if(distance < 0.0f){
			Move(0.2f);
			while(currentY > prelimY);
			Move(0.15f);
			while(currentY > finalY);
			Stop();

		}

	}

	moving = 0;
}

void InitButton()
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
