float actSpeed;
int leftS;
int rightS;

int SpeedTranslate(float speed) {
  if(speed > 1.0f) speed = 1.0f;
    else if(speed < -1.0f) speed = -1.0f;
  speed = 127.0f * speed;
  leftS = (int) speed;
  rightS = (int) (0.94f * speed);
}

void Stop() { 
  char stop[4] = {0xC1, 0x00, 0xC5, 0x00};
  Write(stop, 4);
  actSpeed = 0;
}

void SlowStop() {
  if (actSpeed == 0x00) {}
  else {
    float i;
    for (i = ((0.5f*actSpeed) -1); i >= 1; i = (i * 0.5f)) {
      char stop[4] = {0xC1, (char) i, 0xC5, (char) i};
      Write(stop, 4);
    Delay(50);
                ConsoleWrite("\r\nActSpeed: ");
    ConsoleWriteFloat(i);
                ConsoleWrite("\r\n");
    }
    Stop();
  }
}

void Move(float speed) {
  SpeedTranslate(speed);
  actSpeed = rightS;
  if(speed < 0.0f) {
    char move[4] = {0xC2, (char) (-1 * leftS), 0xC6, (char) (-1 * rightS)};
    Write(move, 4);
  }
  else {
    char move[4] = {0xC1, (char) leftS, 0xC5, (char) rightS};
    Write(move, 4);
  }
}

void MoveDistance(float cm) {

}

void MotorSet(float s1, float s2) {
  if(s1 > 1.0f) s1 = 1.0f;
  else if(s1 < -1.0f) s1 = -1.0f; 
  if(s2 > 1.0f) s2 = 1.0f;
  else if(s2 < -1.0f) s2 = -1.0f;
  int a1 = (int) 127.0f * s1;
  int a2 = (int) 127.0f * s2;
  char forward[4] = {0xC1, (char) a1, 0xC5, (char) a2};
  Write(forward, 4);
}

void Spin(float speed) {
  SpeedTranslate(speed);
  char spin[4] = {0x00, 0x00, 0x00, 0x00};
  if(speed < 0.0f) { //speed < 0 = spin anti-clockwise (to left)
    spin[0] = 0xC2;
    spin[2] = 0xC5;
    spin[1] = (char) (-1 * leftS);
    spin[3] = (char) (-1 * rightS);
  }
  else { // speed > 0 = spin clockwise (to right)
    spin[0] = 0xC1;
    spin[2] = 0xC6;
    spin[1] = (char) leftS;
    spin[3] = (char) rightS;
  }
  Write(spin, 4);
}

void Pivot(Side w, float speed) {
  SpeedTranslate(speed);
  char pivot[4] = {0x00, 0x00, 0x00, 0x00};
  if(w == LEFT) {//Keep left wheel stationary
    pivot[2] = 0xC1;
    pivot[3] = 0x00;
    if(speed < 0.0f) { //speed < 0 = move right wheel backwards
      pivot[0] = 0xC6;
      pivot[1] = (char) (-1 * rightS);
    }
    else { // speed > 0 = move right wheel forwards
      pivot[0] = 0xC5;
      pivot[1] = (char) rightS;
    }
  }
  else {//Keep right wheel stationary
    pivot[2] = 0xC5;
    pivot[3] = 0x00;
    if(speed < 0.0f) { //speed < 0 = move left wheel backwards
      pivot[0] = 0xC2;
      pivot[1] = (char) (-1 * leftS);
    }
    else { // speed > 0 = move left wheel forwards
      pivot[0] = 0xC1;
      pivot[1] = (char) leftS;
    }
  }
  Write(pivot, 4);
}

void SpinAngle(float angle) {
  int time;
  if(angle < 0) {
    angle = angle * -1;
    Spin(-0.15f);
  }
  else Spin(0.15f);
  time = (int) 3500.0f * angle;
  Delay(time);
  Stop();
}