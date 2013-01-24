#include <lpc17xx_gpio.h>

#define SENSOR	(1<<12)
#define PORT	0		

void Init_DigitalSensor() {
  GPIO_SetDir(PORT, SENSOR, 0);
}

int GetDigitalSensorStatus() {
  int result = GPIO_ReadValue(PORT);
  return result;//(result & (1<<17));
}

//P12(14)
//1000000011111111000011110111111 none
//1000000011111011000011110111111 obj
//0000000000000X00000000000000000
//0000000000000100000000000000000
