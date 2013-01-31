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

void ConsoleWrite(char* str) {
  int l = strlen(str);
  UART_Send((LPC_UART_TypeDef*) LPC_UART0, (uint8_t*) str, l, BLOCKING);
}

void ConsoleWriteByte(char c) {
  UART_SendByte((LPC_UART_TypeDef*) LPC_UART0, (uint8_t) c);
}
