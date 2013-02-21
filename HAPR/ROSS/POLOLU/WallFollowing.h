int result[2];

void GetVoltages(Side s) {
  if(s == LEFT) {
    result[0] = Get_ADC_Val("P15");
    result[1] = Get_ADC_Val("P18");
  }
  else {
    result[0] = Get_ADC_Val("P16");
    result[1] = Get_ADC_Val("P17");
  }
}

void FollowWall(float dist, Side s) {
  int wantedV = 2300; //##Based on dist
  currentlyFollowing = s;
  bool toFarOrClose;
  int loopCount;

  for(loopCount = 0; loopCount < 10000000; loopCount++) {
    toFarOrClose = false;
    if(!frontInterruptUp) {
        GetVoltages(s);
        if((result[0] > wantedV) && (result[1] > wantedV)) {
          //If to close to (left, turn right)/(right, turn left) and go forwards a bit
          toFarOrClose = true;
          if(s == LEFT) Pivot(RIGHT, 0.2f);
          else Pivot(LEFT, 0.2f);
          Delay(15);
          if(frontInterruptUp) continue;
          Forward(0.6f);
          if(frontInterruptUp) continue;
          Delay(5);
        }
        if(frontInterruptUp) continue;
        if((result[0] < wantedV) && (result[1] < wantedV)) {
          //If to far from (left, turn left)/(right, turn right) and go forwards a bit
          toFarOrClose = true;
          if(s == LEFT) Pivot(LEFT, 0.2f);
          else Pivot(RIGHT, 0.2f);
          Delay(10);
          if(frontInterruptUp) continue;
          Forward(0.6f);
          if(frontInterruptUp) continue;
          Delay(5);
        }
        if(frontInterruptUp) continue;
        //check if we arent parallel
        //if((!toFarOrClose) && (result[0] > (result[1] + 200))) {
        if(result[0] > (result[1] + 200)) {
          //Turn away from wall, if left wall turn right, if right wall turn left
          if(s == LEFT) Pivot(RIGHT, 0.3f);
          else Pivot(LEFT, 0.3f);
          Delay(10);
          //while(result[0] > (result[1] + 200));
          if(frontInterruptUp) continue;
          Forward(0.6f);
          if(frontInterruptUp) continue;
          Delay(5);
        }
        if(frontInterruptUp) continue;
        //if((!toFarOrClose) && (result[1] > (result[0] + 200))) {
        if(result[1] > (result[0] + 200)) {
          //Turn towards wall, if left wall turn left, if right wall turn right
          if(s == LEFT) Pivot(LEFT, 0.2f);
          else Pivot(RIGHT, 0.2f);
          Delay(10);
          //while(result[1] > (result[0] + 200));
          if(frontInterruptUp) continue;
          Forward(0.6f);
          if(frontInterruptUp) continue;
          Delay(5);
        }
    }
    else {
      while(!GetDigitalSensorStatus()) {
        if(fiveSecTimer) {
          //turn 90 degrees
          if(currentlyFollowing == LEFT) SpinAngle(0.25f);
          else SpinAngle(-0.25f);
          //fiveSecTimer = false;
        }
      }

      ConsoleWrite("\r\nInterrupt area exited.");
      frontInterruptUp = false;
      RIT_Cmd(LPC_RIT, DISABLE);
      ConsoleWrite("\r\nA\r\n");
    }
  }
}

void FindWall() {
  Forward(0.2);
  while(1) {
    GetVoltages(LEFT);
    if((result[0] > 1500) || (result[1] > 1500)) {
      FollowWall(0.0f, LEFT);
    }
    else {
      GetVoltages(RIGHT);
      if((result[0] > 1500) || (result[1] > 1500)) {
        FollowWall(0.0f, RIGHT);
      }
    } 
  }
}
