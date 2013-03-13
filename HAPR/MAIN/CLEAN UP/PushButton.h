//Method: Button_Init()
//Author: Tom Collier
//Use: This enables the use of the A button on the robot.
void Button_Init() {
	GPIO_SetDir(BUTTONPORT, BUTTON, 0);
	GPIO_IntCmd(BUTTONPORT, BUTTONBIT, 0);
	NVIC_EnableIRQ(EINT0_IRQn);
}

//Method: EINT0_IRQHandler()
//Author: Tom Collier
//Use: This is the function called when the A button on the robot is pressed,
//     here it is used as a zero-ing function for the position variables and also to tell the robot
//     to begin execution of it's program.
void EINT0_IRQHandler() {
	NVIC_DisableIRQ(EINT0_IRQn);
	go = true; //begin execution
	currentX = 0.0f; //zero position
	currentY = 0.0f;
	currentTHETA = 0.0f;
	GPIO_ClearInt(BUTTONPORT, BUTTONBIT);
}