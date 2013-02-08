#include "lpc17xx_rit.h"

void RIT_IRQHandler();

void Init_RIT(int time) {
  RIT_Init(LPC_RIT);
  RIT_TimerConfig(LPC_RIT, time);
  NVIC_EnableIRQ(RIT_IRQn);
}

void RIT_IRQHandler() {
  RIT_GetIntStatus(LPC_RIT);
  ConsoleWrite("\r\nTimer int");
  fiveSecTimer = true;
  RIT_DeInit(LPC_RIT);
}


/* On GPIO INT
*  
*  - Set RIT to 5sec
*  - If object still there 
*     - then wait 
*  - else 
*     - exit irq (will return to following)
*  - If timer has triggered
*     - reset timer flag to false
*     - turn 90 degrees
*     - then exit irq (return to wall following
*/

/*
void EINT3_IRQHandler() {
  if (GPIO_GetIntStatus(0, 17, 1)) {
    Init_RIT(5000);
    Stop();
    //Wait till eiter we can move or need to turn
    while(GetDigitalSensorStatus()) {
      if(fiveSecTimer) {
        //turn 90 degrees
        if(currentlyFollowing = LEFT) SpinAngle(-0.25f);
        else SpinAngle(0.25f);
        fiveSecTimer = false;
      }
    } 
  }
  GPIO_ClearInt(0,(1<<17));
}

*/
