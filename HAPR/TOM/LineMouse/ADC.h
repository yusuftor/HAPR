#include "lpc17xx_adc.h"
#include <math.h>

#define TOLERANCE 1.0f

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

int areLeftSensorsEqual() {
	float frontleft = TranslateToDistance(Get_ADC_Val("P15"));
	float backleft = TranslateToDistance(Get_ADC_Val("P18"));
	fprintf(output, "%2.2f", frontleft);
	if(frontleft > (backleft + TOLERANCE)) {
		return 0; //Needs to rotate anticlockwise
	}
	else if(frontleft < (backleft + TOLERANCE)) {
		return 1; //Needs to rotate clockwise
	}
	return 2; // No need to rotate
}

int areRightSensorsEqual() {
	float frontright = TranslateToDistance(Get_ADC_Val("P16"));
	float backright = TranslateToDistance(Get_ADC_Val("P17"));
	if(frontright > (backright + TOLERANCE)) {
		return 0; //Needs to rotate anticlockwise
	}
	else if(frontright < (backright + TOLERANCE)) {
		return 1; //Needs to rotate clockwise
	}
	return 2; // No need to rotate
}

void PrintAllSensors(){
  char output[20] = "";
  int result;

  ConsoleWrite("\r\nADC Channels:\r\n");
  ConsoleWrite("P15: ");
  result = Get_ADC_Val("P15");
  sprintf(output, "%d, ", result);
  ConsoleWrite(output);
  sprintf(output, "%2.2f cm\r\n", TranslateToDistance(result));
  ConsoleWrite(output);

  ConsoleWrite("P16: ");
  result = Get_ADC_Val("P16");
  sprintf(output, "%d, ", result);
  ConsoleWrite(output);
  sprintf(output, "%2.2f cm\r\n", TranslateToDistance(result));
  ConsoleWrite(output);
    
  ConsoleWrite("P17: ");
  result = Get_ADC_Val("P17");
  sprintf(output, "%d, ", result);
  ConsoleWrite(output);
  sprintf(output, "%2.2f cm\r\n", TranslateToDistance(result));
  ConsoleWrite(output);
    
  ConsoleWrite("P18: ");
  result = Get_ADC_Val("P18");
  sprintf(output, "%d, ", result);
  ConsoleWrite(output);
  sprintf(output, "%2.2f cm\r\n", TranslateToDistance(result));
  ConsoleWrite(output);
}
