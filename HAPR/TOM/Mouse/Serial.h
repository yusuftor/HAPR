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
