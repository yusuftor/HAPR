#include <math.h>
#include "Movement.h"
#include "lpc17xx_adc.h"
#include "lpc17xx_uart.h"
#include "lpc17xx_pinsel.h"
#include "lpc_types.h"
#include "Serial.h"
#include "ConsoleDebug.h"

void Delay(int i) { //Delay in ms. 1000 = 1,000 * 10,000 = 1s
  int c, n;
  c = i *  10000;
  for(n = 0; n < c; n++);
}

void Init_ADC() {
  PINSEL_CFG_Type PinCfg;
  
  PinCfg.Funcnum = 1;
  PinCfg.OpenDrain = 0;
  PinCfg.Pinmode = 0;
  PinCfg.Portnum = 0;
  PinCfg.Pinnum = 23; //P15 - Front - left - Channel 0
  PINSEL_ConfigPin(&PinCfg);
  PinCfg.Pinnum = 24; //P16 - Front - right - Channel 1
  PINSEL_ConfigPin(&PinCfg);
  PinCfg.Pinnum = 25; //P17 - Back - right - Channel 2
  PINSEL_ConfigPin(&PinCfg);
  PinCfg.Pinnum = 26; //P18 - Back - left - Channel 3
  PINSEL_ConfigPin(&PinCfg);

  ADC_Init(LPC_ADC, 200000); //200KHz
}

int Get_ADC_Val(char* pin) {
  int channel, intChannel;
  if(pin == "P15") {
    channel = ADC_CHANNEL_0;
    intChannel = ADC_ADINTEN0;
  }
  else if(pin == "P16") {
    channel = ADC_CHANNEL_1;
    intChannel = ADC_ADINTEN1;
  }
  else if(pin == "P17") {
    channel = ADC_CHANNEL_2;
    intChannel = ADC_ADINTEN2;
  }
  else if(pin == "P18") {
    channel = ADC_CHANNEL_3;
    intChannel = ADC_ADINTEN3;
  }
  ADC_IntConfig(LPC_ADC, intChannel, DISABLE);
  ADC_ChannelCmd(LPC_ADC, channel, ENABLE);
  ADC_StartCmd(LPC_ADC, ADC_START_NOW);
  
  while(!(ADC_ChannelGetStatus(LPC_ADC, channel, ADC_DATA_DONE)));
  uint32_t adcVal= ADC_ChannelGetData(LPC_ADC, channel);

  ADC_ChannelCmd(LPC_ADC, channel, DISABLE);
  return (int) adcVal;
}

float TranslateToDistance(int mv) {
  float result = 28.7f *(pow((((float) mv)/1000.0f), (-1.15f)));
  return result;
}

int main() {
	Init_ADC();
	ConsoleInit_Serial();
	WriteByte((char) 0xB7);
	int i;
	while (1) {
	int frontleft = Get_ADC_Val("P15");
	int backleft = Get_ADC_Val("P18");
	ConsoleWrite((char)frontleft);
	ConsoleWrite((char)backleft);
	if (frontleft > (backleft + 200)) {
		Spin(-0.5f);
	}
	if (backleft > (frontleft + 200)) {
		Spin(0.5f);
	}
	Forward(0.1f);
}
}
