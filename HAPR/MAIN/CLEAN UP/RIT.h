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
//Author: Ross Court
//Use: This sets a flag to true so that the wall
//     following method knows to move around the
//     object
void RIT_IRQHandler() {
	RIT_GetIntStatus(LPC_RIT);
	ConsoleWrite("\r\nRIT Timer interrupt.");
	fiveSecTimer = true;
}