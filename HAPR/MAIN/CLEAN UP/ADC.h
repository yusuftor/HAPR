//Method: Init_ADC()
//Author: Ross Court
//Use: Initialises each of the four pins which have analogue sensors
//     and intialises the ADC part of the MBED.
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

//Method: Get_ADC_Val(char* pin)
//Author: Ross Court
//Use: Gets the value of a specific ADC input and returns this as
//     an int.
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

//Method: TranslateToDistance(int mv)
//Author: Ross Court
//Use: This is the mathematical formula for converting input voltage
//     to a returned value in centimeters. This is the formula I 
//     found to be most accurate for our analogue sensors.
float TranslateToDistance(int mv) {
	float result = 37.8f * (pow((((float) mv)/1000.0f), (-1.15f)));
	return result;
}

//Method: PrintAllSensors()
//Author: Ross Court
//Use: This fuction prints out all of the analogue sensor values to
//     the debug terminal. This function is for use in debugging to
//     observe all input values.
void PrintAllSensors() {
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
