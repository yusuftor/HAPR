float actSpeed;
//enum wheel {LEFT, RIGHT};
//typedef enum wheel Wheel;

void Stop() { 
  char stop[4] = {0xC1, 0x00, 0xC5, 0x00};
  Write(stop, 4);
  actSpeed = 0;
  //ConsoleWrite("Stopped.\r\n");
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

void Forward(float speed) {
  if(speed > 1.0f) speed = 1.0f;
  else if(speed < -1.0f) speed = -1.0f;
  actSpeed = (int) 127.0f * speed;
  ConsoleWrite("\r\nFW: ActSpeed: ");
		ConsoleWriteFloat(actSpeed);
                ConsoleWrite("\r\n");
  char forward[4] = {0xC1, (char) actSpeed, 0xC5, (char) actSpeed};
  Write(forward, 4);
}

void Backwards(float speed) {
  if(speed > 1.0f) speed = 1.0f;
  actSpeed = (int) 127.0f * speed;

  char backwards[4] = {0xC2, (char) actSpeed, 0xC6, (char) actSpeed};
  Write(backwards, 4);
}

void Spin(float speed) {
  char spin[4] = {0x00, 0x00, 0x00, 0x00};
  if(speed < 0.0f) { //speed < 0 = spin anti-clockwise (to left)
    spin[0] = 0xC2;
    spin[2] = 0xC5;
    if(speed < -1.0f) speed = -1.0f;
    actSpeed = (-1 * speed) * 127;
    spin[1] = (char) actSpeed;
    spin[3] = (char) actSpeed;
  }
  else { // speed > 0 = spin clockwise (to right)
    spin[0] = 0xC1;
    spin[2] = 0xC6;
    if(speed > 1.0f) speed = 1.0f;
    actSpeed = speed * 127;
    spin[1] = (char) actSpeed;
    spin[3] = (char) actSpeed;
  }
  
  Write(spin, 4);
}

void Pivot(Side w, float speed) {
  if(speed > 1.0f) speed = 1.0f;
  else if(speed < -1.0f) speed = -1.0f;
  char pivot[4] = {0x00, 0x00, 0x00, 0x00};
  if(w == LEFT) {
    //Keep left wheel stationary
    pivot[2] = 0xC1;
    pivot[3] = 0x00;
    if(speed < 0.0f) { //speed < 0 = move right wheel backwards
      pivot[0] = 0xC6;
      actSpeed = (-1 * speed) * 127;
      pivot[1] = (char) actSpeed;
    }
    else { // speed > 0 = move right wheel forwards
      pivot[0] = 0xC5;
      actSpeed = speed * 127;
      pivot[1] = (char) actSpeed;
    }
  }
  else {
    //Keep right wheel stationary
    pivot[2] = 0xC5;
    pivot[3] = 0x00;
    if(speed < 0.0f) { //speed < 0 = move left wheel backwards
      pivot[0] = 0xC2;
      actSpeed = (-1 * speed) * 127;
      pivot[1] = (char) actSpeed;

    }
    else { // speed > 0 = move left wheel forwards
      pivot[0] = 0xC1;
      actSpeed = speed * 127;
      pivot[1] = (char) actSpeed;
    }
  }
  Write(pivot, 4);
}

void SpinAngle(float angle) {
  int time;
  if(angle < 0) {
    angle = angle * -1;
    Spin(-0.3f);
  }
  else Spin(0.3f);
  
  time = (int) 1950.0f * angle;
  Delay(time);
  Stop();
}
