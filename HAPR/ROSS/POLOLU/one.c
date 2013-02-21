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

#include "RIT.h"
#include "ConsoleDebug.h"
#include "Serial.h"
#include "Movement.h"
#include "ADC.h"
#include "DigitalSensor.h"
#include "WallFollowing.h"


int main() {
  Init();

  if(1) {
    ConsoleWrite("Testing Find Wall.\r\n");
    Delay(500);
    FindWall();
  }
  
  if(0) {
    ConsoleWrite("Testing Wall Following.\r\n");
    Delay(500);
    FollowWall(0.0f, LEFT);
  }
  
  //Test front digital sensor
  if(0) {
    ConsoleWrite("Testing Digital Sensor.\r\n");
    Delay(1000);
    char output[20] = "";
    int result;
    while(1) {
      result = GetDigitalSensorStatus();
      if(result) ConsoleWrite("Object detected: ");
      else ConsoleWrite("No object detected: ");
      sprintf(output, "%x\r\n", result);
      ConsoleWrite(output);
      ConsoleWrite("\r\n");
      Delay(1000);
    }
  }


  //Test Analgue IR Sensors
  if(0) {
    ConsoleWrite("Testing Analogue Sensors.\r\n");
    Delay(1000);
    while(1) {
      PrintAllSensors();
      Delay(1000);
    } 
  }

  //Test Movement
  if(0) {
    ConsoleWrite("Testing movement.\r\n");
    Delay(1000);
    while(1) {
      Forward(0.1f);
      Delay(1000);
      Stop();
      Delay(100);
      Backwards(0.1f);
      Delay(1000);
      Stop();
      Delay(100);
      Spin(0.1f);
      Delay(1000);
      Stop();
      Delay(100);
      Spin(-0.1f);
      Delay(1000);
      Stop();
      Delay(100);
    }
  }

  while(1);
}

void Init() {
  Init_Serial();
  Init_ADC();
  Init_DigitalSensor();
  ConsoleInit_Serial(); 
  
  WriteByte((char) 0xB7);
  ConsoleWrite("Starting Main.\r\n");
  ConsoleWrite("Version 12.\r\n");
}

void Delay(int i) { //Delay in ms. 1000 = 1,000 * 10,000 = 1s
  int c, n;
  c = i *  10000;
  for(n = 0; n < c; n++);
}
