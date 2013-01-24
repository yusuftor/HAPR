float actSpeed;

void Stop() {
  if (actSpeed == 0x00) {}
  else {
  	int i;
  	for (i = (actSpeed-1); i >= 0x00; i--) {
  		char stop[4] = {0xC1, (char) i, 0xC5, (char) i};
  		Write(stop, 4);
		Delay(100);
  	}
  	actSpeed = 0x00;
  }
}

void Forward(float speed) {
  if(speed > 1.0f) speed = 1.0f;
  actSpeed = (int) 127.0f * speed;
  
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
  if(speed < 0.0f) { //speed < 0 = spin anti-clockwise
    spin[0] = 0xC2;
    spin[2] = 0xC5;
    if(speed < -1.0f) speed = -1.0f;
    actSpeed = (-1 * speed) * 127;
    spin[1] = (char) actSpeed;
    spin[3] = (char) actSpeed;
  }
  else { // speed > 0 = spin clockwise
    spin[0] = 0xC1;
    spin[2] = 0xC6;
    if(speed > 1.0f) speed = 1.0f;
    actSpeed = speed * 127;
    spin[1] = (char) actSpeed;
    spin[3] = (char) actSpeed;
  }
  
  Write(spin, 4);
}

void SpinAngle(float angle) {
  int time;
  if(angle < 0) {
    angle = angle * -1;
    Spin(-0.15f);
  }
  else Spin(0.15f);
  
  time = (int) 1950.0f * angle;
  Delay(time);
  Stop();
}
