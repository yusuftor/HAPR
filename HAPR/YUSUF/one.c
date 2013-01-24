#include "lpc17xx_uart.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"
#include "lpc_types.h"
#include <string.h>

#define GPIO_INT        (1<<12)

void Delay(int i) { //Delay in ms. 1000 = 1,000 * 10,000 = 1s
  int c, n;
  c = i *  10000;
  for(n = 0; n < c; n++);
}

void EINT3_IRQHandler();


#include "Serial.h"
#include "Movement.h"
#include "ConsoleDebug.h"

void Init_IRQ() {
  GPIO_SetDir(0,GPIO_INT,0); // Initialise
  GPIO_IntCmd(0,(1<<17),1);
  GPIO_IntCmd(0,(1<<17),0);
  NVIC_EnableIRQ(EINT3_IRQn);
}
int main() {
	ConsoleInit_Serial();
  	Init_Serial();
  	Init_IRQ();
  	WriteByte((char) 0xB7);


  	Forward(0.2f);

  	while(1);
}

void EINT3_IRQHandler() // Interrupt handler
{
  ConsoleWrite("Handler\r\n");
  if (GPIO_GetIntStatus(0, 17, 1)) {
ConsoleWrite("In the IF\r\n");
	Stop();
  }
  else {
ConsoleWrite("In else\r\n");
	Forward(0.2f);
  }
  GPIO_ClearInt(0,(1<<17));
}
