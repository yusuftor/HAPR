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

char* Read() {
  char received[10];
  int l = UART_Receive((LPC_UART_TypeDef*) LPC_UART3, received, 10, BLOCKING);
  char* data = malloc(l * sizeof(char));
  int i;
  ConsoleWrite("\r\nData Length: ");
  ConsoleWriteInt(l);
  ConsoleWrite(", Data: ");
  for(i = 0; i < l; i++) {
    ConsoleWriteByte(received[i]);
    data[l-(i+1)] = received[i];
  }
  return data;
}

char ReadByte() {
  return UART_ReceiveByte((LPC_UART_TypeDef*) LPC_UART3);
}
