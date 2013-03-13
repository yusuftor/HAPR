//#### Libraries ####
#include "lpc17xx_uart.h"
#include "lpc17xx_pinsel.h"
#include "lpc_types.h"
#include "lpc17xx_rit.h"
#include "lpc17xx_adc.h"
#include <lpc17xx_gpio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include "KeyboardHost.h"




//#### Definitions and global variables ####
//Mouse
#define INTERVAL	10
#define LENGTH		6.9f
#define CONVERT 	((float) 945 / 2.5f)

//Digital Sensor
#define SENSOR		(1<<12)
#define SENSORBIT	(1<<17)
#define PORT		0	

//Push Button
#define BUTTON		(1<<21)
#define BUTTONBIT	(1<<5)
#define BUTTONPORT	2		

//Sounds
#define BEEP 	"T240O8MSC"
#define ALARM 	"T240O8MSCCC"

//States
enum state {TRAVEL, WALLF, LINEF};
typedef enum state State;
State currentState;

//WallFollowing
enum side {LEFT, RIGHT};
typedef enum side Side;
Side currentlyFollowing;
bool fiveSecTimer;
bool frontInterruptUp;
int result[2];

//Movement
float actSpeed;
int leftS;
int rightS;

//LineFollowing
bool dockNotFound;
int sensor1;
int sensor2;
int sensor3;
int sensor4;
int sensor5;

//Co-ordinates
float currentX;
float currentY;
float currentTHETA;
bool go;
int moving = 0;




//#### Functions ####
//Predefined here for use in headers. 
//Actual implementation should be in driver file
//Details and author in comments above the functions implmentation

//General.h
void Init();
void Delay(int i);

//Sound.h
void Play(char* s);

//RIT.h
void Init_RIT(int time);
void RIT_IRQHandler();

//ConsoleDebug.h
void ConsoleInit_Serial();
void ConsoleWrite(char* str);
void ConsoleWriteByte(char c);
void ConsoleWriteFloat(float n);
void ConsoleWriteInt(int n);

//Serial.h
void Init_Serial();
void Write(char* str, int l);
void WriteByte(char c);
void Read();
char ReadByte();

//Movement.h
int SpeedTranslate(float speed);
void Stop();
void SlowStop();
void Move(float speed);
void MotorSet(float s1, float s2);
void Spin(float speed);
void Pivot(Side w, float speed);
void RoughSpinAngle(float angle);
void RoughMoveDistance(float cm);

//ADC.h
void Init_ADC();
int Get_ADC_Val(char* pin);
float TranslateToDistance(int mv);
void PrintAllSensors();

//DigitalSensor.h
void Init_DigitalSensor();
int GetDigitalSensorStatus();
void EINT3_IRQHandler();

//PushButton.h
void Init_Button();
void EINT0_IRQHandler();

//WallFollowing.h
void GetVoltages(Side s);
void FollowWall(float dist, Side s);
void FindWall();

//LineFollowing.h
void StartLineFollowing();
void FindLine();
void Calibrate();

//Mouse.h
void WaitForFirstPoll();
void Cb(uint8_t buttons, int8_t X, int8_t Y);
void Attach();
void Detach();
void MousePins();
void TurnAngle(float angle);
void MoveDistance(float distance, int xy);




//#### Drivers ####
#include "General.h"
#include "Sound.h"
#include "RIT.h"
#include "ConsoleDebug.h"
#include "Serial.h"
#include "Movement.h"
#include "ADC.h"
#include "DigitalSensor.h"
#include "PushButton.h"
#include "WallFollowing.h"
#include "LineFollowing.h"
#include "Mouse.h"