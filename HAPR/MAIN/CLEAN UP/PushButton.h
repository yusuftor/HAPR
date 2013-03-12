void Button_Init() {
	GPIO_SetDir(BUTTONPORT, BUTTON, 0);
	GPIO_IntCmd(BUTTONPORT, BUTTONBIT, 0);
	NVIC_EnableIRQ(EINT0_IRQn);
}

void EINT0_IRQHandler() {
	//Code here
	GPIO_ClearInt(BUTTONPORT, BUTTONBIT);   	
}