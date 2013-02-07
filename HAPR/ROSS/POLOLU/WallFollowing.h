enum side {LEFT, RIGHT};
typedef enum side Side;
Side currentlyFollowing;
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
      if(s == LEFT) Spin(0.1f);
      else Spin(-0.1f);
      Delay(10);
      Stop();
      Forward(0.3f);
      Delay(10);
      Stop();
    }
    //If both less than wanted, turn towards and go forwards a bit
    if((result[0] < wantedV) && (result[1] < wantedV)) {
      //If to far from (left, turn left)/(right, turn right) and go forwards a bit
      if(s == LEFT) Spin(-0.1f);
      else Spin(0.1f);
      Delay(10);
      Stop();
      Forward(0.3f);
      Delay(10);
      Stop();
    }
    //else check if we arent parallel
    if(result[0] > (result[1] + 200)) {
      //Turn away from wall, if left wall turn right, if right wall turn left
      if(s == LEFT) Spin(0.1f);
      else Spin(-0.1f);
      Delay(10);
      Stop();
      Forward(0.3f);
      Delay(10);
      Stop();
    }
    else if(result[1] > (result[0] + 200)) {
      //Turn towards wall, if left wall turn left, if right wall turn right
      if(s == LEFT) Spin(-0.1f);
      else Spin(0.1f);
      Delay(10);
      Stop();
      Forward(0.3f);
      Delay(10);
      Stop();
    }   

    if (GetDigitalSensorStatus()) {
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
  }
  
}
