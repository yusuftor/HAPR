#define GPIO_INT        (1<<12)

void Init_DigitalSensor() {
	GPIO_SetDir(0,GPIO_INT,0); // Initialise
	GPIO_IntCmd(0,(1<<17),1);
	NVIC_EnableIRQ(EINT3_IRQn);
}

int GetDigitalSensorStatus() {
  int result = GPIO_ReadValue(0);
  return (result & (1<<17));
}
