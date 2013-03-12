//Method: SpeedTranslate(float speed)
//Author: Ross Court
//Use: This is a function called at the start of most
//     movement functions which checks the speed value
//     given and makes sure it isn't out of bounds. It
//     then converts this to a value usable by the 3pi
//     and then reduces the right motor speed due to 
//     differences in the weight and power on the Pololu.
int SpeedTranslate(float speed) {
	if(speed > 1.0f) speed = 1.0f;
	else if(speed < -1.0f) speed = -1.0f;
	speed = 127.0f * speed;
	leftS = (int) speed;
	rightS = (int) (0.94f * speed);
}

//Method: Stop()
//Author: Ross Court
//Use: This sets the motors speed to zero to be sure
//     that the Pololu is completely stopped.
void Stop() { 
	char stop[4] = {0xC1, 0x00, 0xC5, 0x00};
	Write(stop, 4);
	actSpeed = 0;
}

//Method: SlowStop()
//Author: Yusuf Tor
//Use: This gradually reduces the stored speed to zero
//     so that the the Pololu slows down as it stops.
void SlowStop() {
	if (actSpeed != 0) {
		float i;
		for (i = ((0.5f*actSpeed) -1); i >= 1; i = (i * 0.5f)) {
			char stop[4] = {0xC1, (char) i, 0xC5, (char) i};
			Write(stop, 4);
			Delay(50);
		}
		Stop();
	}
}

//Method: Move(float speed)
//Author: Ross Court
//Use: This either moves the Pololu straight forwards
//     or backwards.
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

//Method: MotorSet(float s1, float s2)
//Author: Ross Court / Yusuf Tor
//Use: This allows you to specify the speeds and
//     directions of each of the motors individually.
void MotorSet(float s1, float s2) {
	if(s1 > 1.0f) s1 = 1.0f;
	else if(s1 < -1.0f) s1 = -1.0f; 
	if(s2 > 1.0f) s2 = 1.0f;
	else if(s2 < -1.0f) s2 = -1.0f;
	int a1 = (int) 127.0f * s1;
	int a2 = (int) 127.0f * s2;

	char forward[4] = {0xC1, 0x00, 0xC5, 0x00};
	if (a1 < 0) { // If a1 < 0, make the left wheel go backwards
		forward[0] = 0xC2;
		a1 *= -1;
	}

	if (a2 < 0) { // If a2 < 0, make the right wheel go backwards
		forward[2] = 0xC6;
		a2 *= -1;
	}

	forward[1] = (char) a1;
	forward[3] = (char) a2;
	Write(forward, 4);
}

//Method: Spin(float speed)
//Author: Ross Court
//Use: This sets the Pololu to spin either clockwise
//     or anti-clockwise on the spot.
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

//Method: Pivot(Side w, float speed) 
//Author: Ross Court
//Use: This makes the Pololu pivot one a specified wheel,
//     the wheel given as input will be stationary. The
//     moving wheel can be going either forwards or backwards.
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

//Method: RoughSpinAngle(float angle)
//Author: Ross Court
//Use: This spins the Pololu a certain angle and direction.
//     Negative values will spin the Pololu anti-clockwise.
//     The input is a float representation of the percentage
//     of a full spin to do (eg 0.5f = 50% of a turn so 180
//     degrees). This isn't very acurate as it is based on
//     observation.
void RoughSpinAngle(float angle) {
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

//Method: RoughMoveDistance(float cm)
//Author: Ross Court
//Use: This moves the Pololu forwards a rough distance
//     forwards or backwards. This isn't very acurate
//     as it is based on observation.
void RoughMoveDistance(float cm) {
	int time;
	if(cm < 0) {
		cm *= -1;
		Move(-0.15f);
	}
	else Move(0.15f);
	time = (int) 131 * cm;
	Delay(time);
	Stop();
	currentX = currentX + (cm * cosf(currentTHETA));
	currentY = currentY + (cm * sinf(currentTHETA));
}
