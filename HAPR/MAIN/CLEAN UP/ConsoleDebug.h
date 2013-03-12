//Method: ConsoleInit_Serial()
//Author: Ross Court
//Use: Initialises UART0 for sending data to a terminal
//     on a connected computer. This is for use in
//     debugging and requires the Pololu to be connected
//     via cable.
void ConsoleInit_Serial() {
	UART_CFG_Type UARTConfigStruct;
	UART_FIFO_CFG_Type UARTFIFOConfigStruct;
	PINSEL_CFG_Type PinCfg;

	PinCfg.Funcnum = 1;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Portnum = 0;
	PinCfg.Pinnum = 2;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 3;
	PINSEL_ConfigPin(&PinCfg);

	UART_ConfigStructInit(&UARTConfigStruct);
	UART_FIFOConfigStructInit(&UARTFIFOConfigStruct);

	UART_Init((LPC_UART_TypeDef*) LPC_UART0, &UARTConfigStruct);
	UART_FIFOConfig((LPC_UART_TypeDef*) LPC_UART0, &UARTFIFOConfigStruct);
	UART_TxCmd((LPC_UART_TypeDef*) LPC_UART0, ENABLE);
}

//Method: ConsoleWrite(char* str)
//Author: Ross Court
//Use: This sends a string of characters down the UART0
//     line to the terminal.
void ConsoleWrite(char* str) {
	int l = strlen(str);
	UART_Send((LPC_UART_TypeDef*) LPC_UART0, (uint8_t*) str, l, BLOCKING);
}

//Method: ConsoleWriteByte(char c)
//Author: Ross Court
//Use: This sends a single character to the terminal.
void ConsoleWriteByte(char c) {
	UART_SendByte((LPC_UART_TypeDef*) LPC_UART0, (uint8_t) c);
}

//Method: ConsoleWriteFloat(float n)
//Author: Ross Court
//Use: This converts a float to a string format and sends it.
void ConsoleWriteFloat(float n) {
	char output[20] = "";
	sprintf(output, "%2.2f", n);
	ConsoleWrite(output);
}

//Method: ConsoleWriteInt(int n)
//Author: Ross Court
//Use: This converts a int to a string format and sends it.
void ConsoleWriteInt(int n) {
	char output[20] = "";
	sprintf(output, "%d ", n);
	ConsoleWrite(output);
}
