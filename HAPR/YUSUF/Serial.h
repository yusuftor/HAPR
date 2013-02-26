

void Init_Serial() {
  UART_CFG_Type UARTConfigStruct;
  UART_FIFO_CFG_Type UARTFIFOConfigStruct;
  PINSEL_CFG_Type PinCfg;

  PinCfg.Funcnum = 2;
  PinCfg.OpenDrain = 0;
  PinCfg.Pinmode = 0;
  PinCfg.Portnum = 0;
  PinCfg.Pinnum = 0;
  PINSEL_ConfigPin(&PinCfg);
  PinCfg.Pinnum = 1;
  PINSEL_ConfigPin(&PinCfg);

  UART_ConfigStructInit(&UARTConfigStruct);
  UARTConfigStruct.Baud_rate = 115200;

  UART_FIFOConfigStructInit(&UARTFIFOConfigStruct);
  UART_Init((LPC_UART_TypeDef*) LPC_UART3, &UARTConfigStruct);
  UART_FIFOConfig((LPC_UART_TypeDef*) LPC_UART3, &UARTFIFOConfigStruct);
  UART_TxCmd((LPC_UART_TypeDef*) LPC_UART3, ENABLE);
}

void Write(char* str, int l) {
  UART_Send((LPC_UART_TypeDef*) LPC_UART3, (uint8_t*) str, l, BLOCKING);
}

void WriteByte(char c) {
  UART_SendByte((LPC_UART_TypeDef*) LPC_UART3, (uint8_t) c); 	
}


int Read(int length) {
	char received[10];
	int l = UART_Receive((LPC_UART_TypeDef*) LPC_UART3, received, length, BLOCKING);
	int i;
	int value = 0;
	for (i = 0; i < l; i++) {
		value += (received[i] << (8*i)); //Left shift last byte and add first byte received because the bytes are sent in reverse order.
		ConsoleWriteInt(value);
		
	}
	ConsoleWriteInt(value);
	return value;
	
}

char ReadByte() {
	return UART_ReceiveByte((LPC_UART_TypeDef*) LPC_UART3);
}

void Calibrate() {
	Spin(0.3f); //Spin clockwise
	int i;
	for (i = 0; i < 35; i++) {
		WriteByte((char) 0xB4);
		Delay(10);
	}
	StopImmediately();
	Spin(-0.3f); //Spin anti-clockwise
	for (i = 0; i < 70; i++) {
		WriteByte((char) 0xB4);
		Delay(10);
	}
  	StopImmediately();
	Spin(0.3f); //Spin clockwise
	for (i = 0; i < 35; i++) {
		WriteByte((char) 0xB4);
		Delay(10);
	}
  	StopImmediately();
	
}
