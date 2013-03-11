#include "lpc17xx_uart.h"
#include "lpc17xx_pinsel.h"
#include "lpc_types.h"
#include "lpc17xx_rit.h"
#include "lpc17xx_adc.h"
#include <lpc17xx_gpio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

#include "General.h"
#include "Sound.h"
#include "RIT.h"
#include "ConsoleDebug.h"
#include "Serial.h"
#include "Movement.h"
#include "ADC.h"
#include "DigitalSensor.h"
#include "WallFollowing.h"