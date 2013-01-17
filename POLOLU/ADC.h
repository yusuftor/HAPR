void Init_ADC() {
  PinCfg.Funcnum = 1;
  PinCfg.OpenDrain = 0;
  PinCfg.Pinmode = 0;
  PinCfg.Portnum = 0;
  PinCfg.Pinnum = 23;
  PINSEL_ConfigPin(&PinCfg);
  PinCfg.Pinnum = 24;
  PINSEL_ConfigPin(&PinCfg);
  PinCfg.Pinnum = 25;
  PINSEL_ConfigPin(&PinCfg);

  ADC_Init(LPC_ADC, 200000);
  ADC_IntConfig(LPC_ADC, _ADC_INT, DISABLE);
  ADC_ChannelCmd(LPC_ADC, _ADC_CHANNEL, ENABLE);
}
