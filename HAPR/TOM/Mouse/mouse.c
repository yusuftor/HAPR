#include "lpc17xx_uart.h"
#include "lpc17xx_pinsel.h"
#include "lpc_types.h"
#include <string.h>
#include "serial.h"
#include "sound.h"
#include "KeyboardHost.h"

#define INTERVAL 500

void RIT_IRQHandler();
void cb(uint8_t buttons, int8_t X, int8_t Y);
void attach();
void detach();

int main() 
{

Init_Serial();
WriteByte((char) 0xB7);

RIT_Init(LPC_RIT);
RIT_TimerConfig(LPC_RIT, INTERVAL);
NVIC_EnableIRQ(RIT_IRQn);

mouse_init(cb, attach, detach);
play("T240O8MSE");

while(1);

}

void cb(uint8_t buttons, int8_t X, int8_t Y)
{

    if(X != 0)
    {
        
    }

}

void attach()
{
}

void detach()
{
}

void RIT_IRQHandler()
{
	RIT_GetIntStatus(LPC_RIT);	
	mouse_poll();	
	play("T240O8MSC");
	
}
