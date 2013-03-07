void Init_DigitalSensor() {
  GPIO_SetDir(0,SENSOR,0); // Initialise
  GPIO_IntCmd(0,(1<<17),1);
  NVIC_EnableIRQ(EINT3_IRQn);
}

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