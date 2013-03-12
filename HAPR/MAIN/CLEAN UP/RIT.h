//Method: Init_RIT()
//Author: Ross Court
//Use: This method sets up the RIT for use in the 
//     Wall Following section to detect when an 
//     obstical hasn't moved for five seconds. At
//     this point the Pololu is to assume it either
//     isn't going to move or is a wall and will
//     turn away from the wall to continue
void Init_RIT(int time) {
	RIT_Init(LPC_RIT);
	RIT_TimerConfig(LPC_RIT, time);
	NVIC_EnableIRQ(RIT_IRQn);
	fiveSecTimer = false;
}

//Method: RIT_IRQHandler()
//Author: Ross Court / Tom Collier
//Use: The use of this depends on if the Pololu is
//     wall following or travelling using the mouse.
//     If wall following sets a flag to true so that
//     the wall following method knows to move around
//     the object.
//     If the Pololu is travelling the RIT prompts a 
//     update from the mouse.
void RIT_IRQHandler() {
	if(currentState == TRAVEL) {
		RIT_GetIntStatus(LPC_RIT);
		mouse_poll();
	}
	else if(currentState == WALLF) {
		RIT_GetIntStatus(LPC_RIT);
		ConsoleWrite("\r\nRIT Timer interrupt.");
		fiveSecTimer = true;
	}
}