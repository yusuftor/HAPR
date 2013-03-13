//Method: Init_DigitalSensor()
//Author: Yusuf Tor
//Use: Initialises the direction of the GPIO pin
//     that the sensor is connected to so that we
//     can read the value. Also connects changes
//     on this in put to a interrupt handler.
void Init_DigitalSensor() {
	GPIO_SetDir(PORT, SENSOR, 0); // Initialise
	GPIO_IntCmd(PORT, SENSORBIT, 1);
	NVIC_EnableIRQ(EINT3_IRQn);
}

//Method: GetDigitalSensorStatus()
//Author: Yusuf Tor
//Use: This get the value of the port that the
//     digital sensor is on.Singles out the bit
//     that represents the digital sensors value
//     and returns it as an int.
int GetDigitalSensorStatus() {
	int result = GPIO_ReadValue(PORT);
	return (result & SENSORBIT);
}

//Method: EINT3_IRQHandler()
//Author: Yusuf Tor / Ross Court
//Use: This handles interrupts on the GPIO port
//     giving different responses depending on
//     the state the Pololu is currently in.
//     -If wall following the method set at timer
//      to be used to determine when to try and
//      move around an object.
//     -If line following it signals that the
//      dock has been found which will end the 
//      execution of the demo run
void EINT3_IRQHandler() {
	ConsoleWrite("\r\nGPIO IRQ Handler");
	if(currentState == TRAVEL) {
		if (GPIO_GetIntStatus(BUTTONPORT, BUTTONBIT, 0)) {
			ConsoleWrite("\r\nBUTTON");
			go = true; //begin execution
			currentX = 0.0f; //zero position
			currentY = 0.0f;
			currentTHETA = 0.0f;
			GPIO_ClearInt(BUTTONPORT, BUTTONBIT);
		}
	}
	else if (currentState == WALLF) {
		if(!frontInterruptUp) {
			ConsoleWrite("\r\nIRQ Handler Entered");
			frontInterruptUp = true;
			if (GPIO_GetIntStatus(PORT, 17, 1)) {
				SlowStop();
				Init_RIT(5000);
				ConsoleWrite("\r\nTimer set");
			}	
			GPIO_ClearInt(PORT, SENSORBIT); 
		} 
	}
    else if(currentState == LINEF) {
        if (GPIO_GetIntStatus(PORT, 17, 1)) {
            WriteByte((char) 0xBC);
            Stop();
            dockNotFound = false;
        }
        GPIO_ClearInt(PORT, SENSORBIT); 
    }
	ConsoleWrite("\r\nDone, clearing int");
	
} 