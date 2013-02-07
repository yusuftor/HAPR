#include <lpc17xx_gpio.h>

#define SENSOR	(1<<12)
#define PORT	0		

void Init_DigitalSensor() {
  GPIO_SetDir(PORT, SENSOR, 0);
}

int GetDigitalSensorStatus() {
  int result = GPIO_ReadValue(PORT);
  result -= 0x407d8fbf;
  if(result == 0) return 1; 
  else return 0;
}

//P12(14)
//1000000011111111000011110111111 none
//1000000011111011000011110111111 obj
//0000000000000X00000000000000000
//0000000000000100000000000000000
