#include "lpc17xx_uart.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"
#include "lpc_types.h"
#include <string.h>

#define GPIO_INT        (1<<12)
#define _ALARM "V15T240O8MSCCC"

int ok = 1;

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


  	Forward(0.15f);

	
	
	while(1){
	if(!ok) {
	Forward(0.15f);
	ok = 1;
	}
	}
}

void play(char* s) {	
	int count = 0;
	int i;	
	for(i=0;s[i]!='\0';i++){
	count++;
	}	
	
	WriteByte((char) 0xB3);
	WriteByte((char) count);	
	Write(s, count);
}

int GetDigitalSensorStatus() {
  int result = GPIO_ReadValue(0);
  return (result & (1<<17));
}


void EINT3_IRQHandler() // Interrupt handler
{
 
//ConsoleWrite("Handler\r\n");
  if (GPIO_GetIntStatus(0, 17, 1)) {
//ConsoleWrite("In the IF\r\n");
	Stop();
	//play("T120>>C2");
	play(_ALARM);
  }	
while(!GetDigitalSensorStatus());
	ok = 0;
 GPIO_ClearInt(0,(1<<17));  
} 
