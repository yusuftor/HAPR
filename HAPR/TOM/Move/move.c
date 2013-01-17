#include "lpc17xx_uart.h"
#include "lpc17xx_pinsel.h"
#include "lpc_types.h"
#include <string.h>

void Init_Serial();
void Write(char* str);
void WriteByte(char c);

void Forward();
void Backwards();
void Stop();
void Spin();

int i;

int main() {
  Init_Serial();
  
  WriteByte((char) 0xB7);

  Forward();
  for(i = 0; i < 20000000; i++);
  Backwards();
  for(i = 0; i < 20000000; i++);
  Stop();
  for(i = 0; i < 20000000; i++);
  Spin();
  for(i = 0; i < 20000000; i++);
  Stop();
  while(1);
}

//Fill array with motor movement (e.g. forwards) and it's speed
//Iterate so each byte is written in turn
void Forward() {
	char forward[4] = {0xC1, 0x0F, 0xC5, 0x0F};
	for(i=0; i<4; i++){WriteByte((char) forward[i]);}
}

void Backwards() {
	char backwards[4] = {0xC2, 0x0F, 0xC6, 0x0F};
	for(i=0; i<4; i++){WriteByte((char) backwards[i]);}
}

void Stop() { 
	char stop[4] = {0xC1, 0x00, 0xC5, 0x00};
	for(i=0; i<4; i++){WriteByte((char) stop[i]);}
}

void Spin() {
  	char spin[4] = {0xC1, 0x0F, 0xC6, 0x0F};
	for(i=0; i<4; i++){WriteByte((char) spin[i]);}
}

void Init_Serial() {
  UART_CFG_Type UARTConfigStruct;
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

  UART_Init((LPC_UART_TypeDef*) LPC_UART3, &UARTConfigStruct);
  UART_TxCmd((LPC_UART_TypeDef*) LPC_UART3, ENABLE);
}

void Write(char* str) {
  int l = strlen(str);
  UART_Send((LPC_UART_TypeDef*) LPC_UART3, (uint8_t*) str, l, BLOCKING);
}

void WriteByte(char c) {
  UART_SendByte((LPC_UART_TypeDef*) LPC_UART3, (uint8_t) c);
}
