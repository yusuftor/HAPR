#include <lpc17xx_gpio.h>

#define SENSOR	(1<<12)
#define PORT	0		

void EINT3_IRQHandler();

void Init_DigitalSensor() {
  GPIO_SetDir(0,SENSOR,0); // Initialise
  GPIO_IntCmd(0,(1<<17),1);
  NVIC_EnableIRQ(EINT3_IRQn);
  //GPIO_SetDir(PORT, SENSOR, 0);
}

/*int GetDigitalSensorStatus() {
  int result = GPIO_ReadValue(PORT);
  result -= 0x407d8fbf;
  if(result == 0) return 1; 
  else return 0;
}*/

int GetDigitalSensorStatus() {
  int result = GPIO_ReadValue(0);
  return (result & (1<<17));
}

void EINT3_IRQHandler() {// Interrupt handler
  ConsoleWrite("GPIO IRQ Handler\r\n");
  if(!frontInterruptUp) {
    ConsoleWrite("IRQ Handler Entered\r\n");
    frontInterruptUp = true;
    if (GPIO_GetIntStatus(0, 17, 1)) {
      ConsoleWrite("In the IF\r\n");
      SlowStop();
      Init_RIT(5000);
      ConsoleWrite("Timer set\r\n");
    }	
  } 
  ConsoleWrite("Done, clearing int\r\n");
  GPIO_ClearInt(0,(1<<17)); 
} 

//P12(14)
//1000000011111111000011110111111 none
//1000000011111011000011110111111 obj
//0000000000000X00000000000000000
//0000000000000100000000000000000
