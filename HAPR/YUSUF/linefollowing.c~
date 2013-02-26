#include "lpc17xx_uart.h"
#include "lpc_types.h"
#include "lpc17xx_pinsel.h"
#include "Movement.h"
#include "Serial.h"
#include "ConsoleDebug.h"
#include "lpc17xx_gpio.h"

#define GPIO_INT        (1<<12)

void Init_IRQ() {
  GPIO_SetDir(0,GPIO_INT,0); // Initialise
  GPIO_IntCmd(0,(1<<17),1);
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

int main() {
	Init_Serial();
	Init_IRQ();
	ConsoleInit_Serial();
	WriteByte((char) 0xB7);
	Calibrate();
	WriteByte((char) 0xBB); // Start PID
	WriteByte((char)((int)127.0f*0.4f)); // Max motor speed
	char parameters[4] = {0x01, 0x14, 0x03, 0x02};
	Write(parameters, 4); // Parameters to adjust motor speed differences
	while (1);
}
