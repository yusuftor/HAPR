int result[2];

void GetVoltages(Side s) {
  if(s == LEFT) {
    result[0] = Get_ADC_Val("P15");
    result[1] = Get_ADC_Val("P18");
  }
  else {
    result[0] = Get_ADC_Val("P15");
    result[1] = Get_ADC_Val("P18");
  }
}

void FollowWall(float dist, Side s) {
  int wantedV = 2300; //##Based this on dist
  currentlyFollowing = s;
  
  int loopCount;
  //int result[2];
  for(loopCount = 0; loopCount < 10000000; loopCount++) {
    GetVoltages(s);
    //if both greater than, turn away from and go forwards a bit
    if((result[0] > wantedV) && (result[1] > wantedV)) {
      //If to close to (left, turn right)/(right, turn left) and go forwards a bit
      if(s == LEFT) Spin(0.2f);
      else Spin(-0.2f);
      Delay(5);
      //Stop();
      Forward(0.6f);
      Delay(20);
      //Stop();
    }
    //If both less than wanted, turn towards and go forwards a bit
    if((result[0] < wantedV) && (result[1] < wantedV)) {
      //If to far from (left, turn left)/(right, turn right) and go forwards a bit
      if(s == LEFT) Spin(-0.2f);
      else Spin(0.2f);
      Delay(5);
      //Stop();
      Forward(0.6f);
      Delay(20);
      //Stop();
    }
    //else check if we arent parallel
    if(result[0] > (result[1] + 200)) {
      //Turn away from wall, if left wall turn right, if right wall turn left
      if(s == LEFT) Spin(0.2f);
      else Spin(-0.2f);
      Delay(5);
      //Stop();
      Forward(0.6f);
      Delay(20);
      //Stop();
    }
    else if(result[1] > (result[0] + 200)) {
      //Turn towards wall, if left wall turn left, if right wall turn right
      if(s == LEFT) Spin(-0.2f);
      else Spin(0.2f);
      Delay(5);
      //Stop();
      Forward(0.6f);
      Delay(20);
      //Stop();
    }   
    /*int result = GetDigitalSensorStatus();
    char output[20] = "";
    sprintf(output, "%x\r\n", result);
    

    ConsoleWrite("\r\nPre-check: ");
    ConsoleWrite(output);
    ConsoleWrite("\r\n");
    if (result) {
      ConsoleWrite("\r\nObject detected in front");
      Init_RIT(5000);
      Stop();
      ConsoleWrite("\r\nTimer set and stopped");
      //Wait till eiter we can move or need to turn
      while(GetDigitalSensorStatus()) {
        if(fiveSecTimer) {
          //turn 90 degrees
          ConsoleWrite("\r\nTimer triggered turning");
          if(currentlyFollowing = LEFT) SpinAngle(-0.25f);
          else SpinAngle(0.25f);
          fiveSecTimer = false;
        }
      } 
    }*/
  }
  
}
