//Method: Init()
//Author: Ross Court
//Use: Initialises different drivers we will use during the demo.
//     Also writes a byte to the slave program of the 3pi to stop
//     it from booting into the example program.
//     Also prints a string including the current version number
//     to the terminal to make sure that the program is up to date
//     and the program is initialising correctly.
void Init() {
	Init_Serial();
	WriteByte((char) 0xB7);

	ConsoleInit_Serial();
	Init_ADC();
	Init_DigitalSensor();
	
	currentX = 0.0f;
	currentY = 0.0f;
	currentTHETA = 0.0f;

	ConsoleWrite("Starting Main.\r\n");
	ConsoleWrite("Version 3.\r\n");
}

//Method: Delay(int ms)
//Author: Ross Court
//Use: This is a simple thread locking delay that stalls the
//     processor for an estimated amount of time in milliseconds.
void Delay(int ms) { //Delay in ms. 1000 = 1,000 * 10,000 = 1s
	int t, n;
	t = ms *  10000;
	for(n = 0; n < t; n++);
}