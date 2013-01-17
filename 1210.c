#include <lpc17xx_gpio.h>
#include <debug_frmwrk.h>


#define LED1 (1<<18) 	//LED lines
#define LED2 (1<<20)
#define LED3 (1<<21)
#define LED4 (1<<23)
#define prompt "enter number from 0-9\r\n"
#define error "please enter valid number"

void ledsetup();

int main()
{
	int temp;
	char c;
	ledsetup();
	debug_frmwrk_init();
	while(1)
		{
		_DBG(prompt);
		c = UARTGetChar(DEBUG_UART_PORT);
		if(c >='0' && c <= '9')
			{
			temp = atoi(&c);
			if(temp/8)
				{
				GPIO_SetValue(1,LED4);
				temp -= 8;
				} else {
				GPIO_ClearValue(1,LED4);			
				}
			if(temp/4)
				{
				GPIO_SetValue(1,LED3);
				temp -= 4;
				} else {
				GPIO_ClearValue(1,LED3);			
				}
			if(temp/2)
				{
				GPIO_SetValue(1,LED2);
				temp -= 2;
				} else {
				GPIO_ClearValue(1,LED2);			
				}
			if(temp/1)
				{
				GPIO_SetValue(1,LED1);
				temp -= 1;
				} else {
				GPIO_ClearValue(1,LED1);
				}
			} else {
			_DBG(error);
			}
		}
}




void ledsetup()
{
GPIO_SetDir(1,LED1,1); //set LED lines out
GPIO_SetDir(1,LED2,1);
GPIO_SetDir(1,LED3,1);
GPIO_SetDir(1,LED4,1);

GPIO_ClearValue(1, LED1); // clears LED ports
GPIO_ClearValue(1, LED2);
GPIO_ClearValue(1, LED3);
GPIO_ClearValue(1, LED4);
}

