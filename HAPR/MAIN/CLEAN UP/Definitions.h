//Digital Sensor
#define SENSOR	(1<<12)
#define PORT	0	

//Sounds
#define _BEEP "T240O8MSC"
#define _ALARM "T240O8MSCCC"
#define _AMERICANIDIOT "T176L8MSG#G#G#>C#4>C#>C#F#4F#>C#.G#.F#."
#define _CRAZYFROG "T240MSFG#FFA#FD#F>CFF>C#>CG#F>C>FFD#D#GF"

//WallFollowing
enum side {LEFT, RIGHT};
typedef enum side Side;
Side currentlyFollowing;
bool frontInterruptUp;
int result[2];

//Movement
float actSpeed;
int leftS;
int rightS;

//LineFollowingSensors
int sensor1;
int sensor2;
int sensor3;
int sensor4;
int sensor5;

//Co-ordinates
float globalX;
float globalY;
float globalR;

//Functions
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
//Calibrate##
int SpeedTranslate(float speed);
void Stop();
void SlowStop();
void Move(float speed);
void MotorSet(float s1, float s2);
void Spin(float speed);
void Pivot(Side w, float speed);
void SpinAngle(float angle);
void MoveDistance(float cm);

//ADC.h
void Init_ADC();
int Get_ADC_Val(char* pin);
float TranslateToDistance(int mv);
void PrintAllSensors();

//DigitalSensor.h //WALL + LINE##
void Init_DigitalSensor();
int GetDigitalSensorStatus();
void EINT3_IRQHandler();

//WallFollowing.h
void GetVoltages(Side s);
void FollowWall(float dist, Side s);
void FindWall();

//LineFollowing.h
//void Calibrate();
//FindLine##
//Main##

//Mouse.h
//##