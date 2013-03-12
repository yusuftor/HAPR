#include "lpc17xx_uart.h"
#include "lpc_types.h"
#include "lpc17xx_pinsel.h"
#include "Serial.h"
#include "Movement.h"
#include "ConsoleDebug.h"
#include "lpc17xx_gpio.h"
#include "DigitalSensor.h"

void EINT3_IRQHandler() // Interrupt handler
{
  if (GPIO_GetIntStatus(0, 17, 1)) {
	WriteByte((char) 0xBC);
	Stop();
  }	
  while(!GetDigitalSensorStatus());
  GPIO_ClearInt(0,(1<<17));  
}

void FindLine() {
	while(1) {
		WriteByte((char) 0x86); // Read calibrated sensor values
		Read(10);
		Delay(10);
		if (sensor1 > 1600 && sensor2 <= 1600) { //Line under sensor 1 
            Spin(0.3f);
            Delay(50);
            Move(0.2f);
            Delay(50);
            Stop();
            continue;
        }
        else if (sensor4 > 1600 && sensor3 <= 1600) {//Line under sensor 4
            Spin(-0.3f);
            Delay(50);
            Move(0.2f);
            Delay(50);
            Stop();
            continue;
        }
        else if (sensor2 > 1600 && sensor3 <= 1600) {//Line under sensor 2
            Spin(0.3f);
            Delay(50);
            Move(0.2f);
            Delay(50);
            Stop();
            continue;
        }
        else if (sensor5 > 1600 && sensor4 <= 1600) { // Line is under sensor 5
            Spin(-0.3f);
            Delay(50);
            Move(0.2f);
            Delay(50);
            Stop();
            continue;
        }
        else if (sensor2 > 1600 && sensor3 > 1600 && sensor4 > 1600) {//Line under sensor 3, move forwards a bit then calibrate
            Move(0.2f);
            Delay(100);
            break;
        }
		else { // Line not under any sensor, just keep moving forwards
			Move(0.2f);
			Delay(100);
			Stop();
		}
	}
}

void LineFollow() {
	Init_Serial();
	Init_DigitalSensor();
	ConsoleInit_Serial();
	FindLine();
	Calibrate();
	WriteByte((char) 0xBB); // Start PID
	WriteByte((char)((int)127.0f*0.4f)); // Max motor speed
	char parameters[4] = {0x01, 0x14, 0x03, 0x02};
	Write(parameters, 4); // Parameters to adjust motor speed differences
	while (1);
}
