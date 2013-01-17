#include "lpc17xx_pinsel.h"
#include "lpc_types.h"
#include "lpc17xx_uart.h"
#include "debug_frmwrk.h"
#include <string.h>

PINSEL_CFG_Type PinCfg;

void Init_Serial();
void test();

int main() {
	debug_frmwrk_init();
	Init_Serial();
	test();
}

void Init_Serial() {
	UART_CFG_Type UARTConfigStruct;
	UART_FIFO_CFG_Type UARTFIFOConfigStruct;
	PINSEL_CFG_Type PinCfg;

	PinCfg.Funcnum = 1;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Portnum = 0;
	PinCfg.Pinnum = 9;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 10;
	PINSEL_ConfigPin(&PinCfg);

	UART_ConfigStructInit(&UARTConfigStruct);
	UART_FIFOConfigStructInit(&UARTFIFOConfigStruct);

	UART_Init((LPC_UART_TypeDef*) LPC_UART3, &UARTConfigStruct);
	UART_FIFOConfig((LPC_UART_TypeDef*) LPC_UART3, &UARTFIFOConfigStruct);
	UART_TxCmd((LPC_UART_TypeDef*) LPC_UART3, ENABLE);
}

void test() {
	char motorforward[2] = {0xC1,0x7f};
	int l = strlen(motorforward);
	UART_Send((LPC_UART_TypeDef*) LPC_UART3, (uint8_t *)motorforward, l, BLOCKING);
}
