#include "lpc17xx_uart.h"
#include "lpc17xx_pinsel.h"
#include "lpc_types.h"
#include <string.h>
#include <stdbool.h>

enum side {LEFT, RIGHT};
typedef enum side Side;
Side currentlyFollowing;
bool frontInterruptUp;

void Init();
void Delay();
void Init_RIT(int time);
bool fiveSecTimer = false;

#include "sound.h"
#include "RIT.h"
#include "ConsoleDebug.h"
#include "Serial.h"
#include "Movement.h"
#include "ADC.h"
#include "DigitalSensor.h"
#include "WallFollowing.h"


int main() {
	Init();

	while(1) {
		PrintAllSensors();
		Delay(1000);
	}
  
  	while(1);
}

void Init() {
	ConsoleInit_Serial(); 
	Init_Serial();
	Init_ADC();
	Init_DigitalSensor();
	

	WriteByte((char) 0xB7);
	ConsoleWrite("Starting Main.\r\n");
	ConsoleWrite("Version 12.\r\n");
}

void Delay(int i) { //Delay in ms. 1000 = 1,000 * 10,000 = 1s
	int c, n;
	c = i *  10000;
	for(n = 0; n < c; n++);
}
