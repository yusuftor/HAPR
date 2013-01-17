#include "lpc17xx_uart.h"
#include "lpc17xx_pinsel.h"
#include "lpc_types.h"
#include <string.h>

void Delay(int i) { //Delay in ms. 1000 = 1,000 * 10,000 = 1s
  int c, n;
  c = i *  10000;
  for(n = 0; n < c; n++);
}

#include "Serial.h"
#include "Movement.h"

int main() {
  Init_Serial();
  
  WriteByte((char) 0xB7);


  int i;
  Forward(0.1f);
  Delay(1000);
  Stop();
  Delay(2000);

  Spin(0.15f);
  Delay(1950);

  Stop();
  Delay(1000);

  Spin(-0.15f);
  Delay(1950);

  Stop();
  Delay(2000);

  SpinAngle(0.25f);
  SpinAngle(-0.25f);
  while(1);
}


