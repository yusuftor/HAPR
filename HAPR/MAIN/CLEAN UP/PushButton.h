//Method: Button_Init()
//Author: Tom Collier
//Use: This enables the use of the A button on the robot.
void Button_Init() {
	GPIO_SetDir(BUTTONPORT, BUTTON, 0);
	GPIO_IntCmd(BUTTONPORT, BUTTONBIT, 0);
}