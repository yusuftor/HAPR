float actSpeed;
//enum wheel {LEFT, RIGHT};
//typedef enum wheel Wheel;


void Delay(int i) { //Delay in ms. 1000 = 1,000 * 10,000 = 1s
  int c, n;
  c = i *  10000;
  for(n = 0; n < c; n++);
}


void Forward(float speed) {
  if(speed > 1.0f) speed = 1.0f;
  else if(speed < -1.0f) speed = -1.0f;
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

