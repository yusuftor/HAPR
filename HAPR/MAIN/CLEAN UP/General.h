void Init() {
	ConsoleInit_Serial(); 
	Init_Serial();
	
	WriteByte((char) 0xB7);
	ConsoleWrite("Starting Main.\r\n");
	ConsoleWrite("Version X.\r\n");
}

void Delay(int i) { //Delay in ms. 1000 = 1,000 * 10,000 = 1s
	int c, n;
	c = i *  10000;
	for(n = 0; n < c; n++);
}