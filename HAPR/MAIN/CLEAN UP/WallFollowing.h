//Method: GetVoltages(Side s)
//Author: Ross Court
//Use: This function updates a global variable used by wall following with the values
//     of a specified side of the Pololu. This function can also be set to use an 
//     average of a given amount of checks to do.
void GetVoltages(Side s, float checks) {
	int i;
	result[0] = 0;
	result[1] = 0;
	for(i = 0; i < checks; i++) {
		if(s == LEFT) {
			result[0] += Get_ADC_Val("P15");
			result[1] += Get_ADC_Val("P18");
		}
		else {
			result[0] += Get_ADC_Val("P16");
			result[1] += Get_ADC_Val("P17");
		}
	}
	result[0] = (int) (((float) result[0]) / checks);
	result[1] = (int) (((float) result[1]) / checks);
}

void FollowWall(float dist, Side s) {
	dist -= 7.0f;
	int wantedV = (int) (1000.0f * pow((dist / 37.8f), (1.0f/-1.15f)));
	bool changedValues;
	currentlyFollowing = s;
<<<<<<< HEAD
	GetVoltages(s);
	ConsoleWrite("/r/nCalculated V: ");
	ConsoleWriteInt(wantedV);
	while(result[0] > 1000) {
=======
	bool toFarOrClose;
	int loopCount;

	for(loopCount = 0; loopCount < 10000000; loopCount++) {
		toFarOrClose = false;
>>>>>>> parent of beac94a... Wall following can exit
		if(!frontInterruptUp) {
				changedValues = true;
				GetVoltages(s);
				if(result[0] <= 1000) continue;
				ConsoleWrite("\r\nFound1 V: ");
				ConsoleWriteInt(result[0]);
				ConsoleWrite(" - ");
				ConsoleWriteInt(result[1]);
				Move(0.5f);
				if((result[0] > (wantedV + 200)) && (result[1] > (wantedV + 200))) {
					ConsoleWrite("\r\nA");
					//If to close to (left, turn right)/(right, turn left) and go forwards a bit
					if(s == LEFT) Pivot(RIGHT, 0.6f);
					else Pivot(LEFT, 0.6f);
					Delay(10);
					if(frontInterruptUp) continue;
					Move(0.5f);
					if(frontInterruptUp) continue;
					Delay(15);
					changedValues = true;
				}
				if(frontInterruptUp) continue;
				if(changedValues) {
					GetVoltages(s);
					if(result[0] <= 1000) continue;
					changedValues = false;
				}
				ConsoleWrite("\r\nFound2 V: ");
				ConsoleWriteInt(result[0]);
				ConsoleWrite(" - ");
				ConsoleWriteInt(result[1]);
				if((result[0] < (wantedV - 200)) && (result[1] < (wantedV - 200))) {
					ConsoleWrite("\r\nB");
					//If to far from (left, turn left)/(right, turn right) and go forwards a bit
					if(s == LEFT) Pivot(LEFT, 0.6f);
					else Pivot(RIGHT, 0.6f);
					Delay(10);
					if(frontInterruptUp) continue;
					Move(0.5f);
					if(frontInterruptUp) continue;
					Delay(15);
					changedValues = true;
				}
				if(frontInterruptUp) continue;
				if(changedValues) {
					GetVoltages(s);
					if(result[0] <= 1000) continue;
					changedValues = false;
				}
				ConsoleWrite("\r\nFound3 V: ");
				ConsoleWriteInt(result[0]);
				ConsoleWrite(" - ");
				ConsoleWriteInt(result[1]);
				//check if we arent parallel
<<<<<<< HEAD
				int diff = result[0] - result[1];
				if(diff > 100) {
					ConsoleWrite("\r\nC");
=======
				//if((!toFarOrClose) && (result[0] > (result[1] + 200))) {
				if(result[0] > (result[1] + 200)) {
>>>>>>> parent of beac94a... Wall following can exit
					//Turn away from wall, if left wall turn right, if right wall turn left
					if(s == LEFT) Pivot(RIGHT, 0.6f);
					else Pivot(LEFT, 0.6f);
					Delay(10);
					changedValues = true;
				}
				if(frontInterruptUp) continue;
<<<<<<< HEAD
				if(changedValues) {
					GetVoltages(s);
					if(result[0] <= 1000) continue;
					changedValues = false;
					diff = result[0] - result[1];
				}
				ConsoleWrite("\r\nFound4 V: ");
				ConsoleWriteInt(result[0]);
				ConsoleWrite(" - ");
				ConsoleWriteInt(result[1]);
				if(diff < -100) {
					ConsoleWrite("\r\nD");
=======
				//if((!toFarOrClose) && (result[1] > (result[0] + 200))) {
				if(result[1] > (result[0] + 200)) {
>>>>>>> parent of beac94a... Wall following can exit
					//Turn towards wall, if left wall turn left, if right wall turn right
					if(s == LEFT) Pivot(LEFT, 0.6f);
					else Pivot(RIGHT, 0.6f);
					Delay(20);
					changedValues = true;

					
				}
				Stop();
		}
		else {
			while(!GetDigitalSensorStatus()) {
				if(fiveSecTimer) {
<<<<<<< HEAD
					//turn 90 degrees as many times as required until front of robot is clear
					if(currentlyFollowing == LEFT) RoughSpinAngle(0.25f);
					else RoughSpinAngle(-0.25f);
=======
					//turn 90 degrees
					if(currentlyFollowing == LEFT) SpinAngle(0.25f);
					else SpinAngle(-0.25f);
					//fiveSecTimer = false;
>>>>>>> parent of beac94a... Wall following can exit
				}
			}

			ConsoleWrite("\r\nInterrupt area exited.");
			frontInterruptUp = false;
			RIT_Cmd(LPC_RIT, DISABLE);
			ConsoleWrite("\r\nA\r\n");
		}
	}
<<<<<<< HEAD
	ConsoleWrite("\r\nWall following ended.");
=======
>>>>>>> parent of beac94a... Wall following can exit
}

void FindWall() {
	Move(0.2f);
	bool foundWall = false;
	while(!foundWall) {
		GetVoltages(RIGHT);
<<<<<<< HEAD
		if((result[0] > 1000) || (result[1] > 1000)) {
			FollowWall(20.0f, RIGHT);
			foundWall = true;
		}
		else {
			GetVoltages(LEFT);
			if((result[0] > 1000) || (result[1] > 1000)) {
				FollowWall(20.0f, LEFT);
				foundWall = true;
=======
		if((result[0] > 2000) || (result[1] > 2000)) {
			//float diff = 
			FollowWall(0.0f, RIGHT);
		}
		else {
			GetVoltages(LEFT);
			if((result[0] > 2000) || (result[1] > 2000)) {
				FollowWall(0.0f, LEFT);
>>>>>>> parent of beac94a... Wall following can exit
			}
		} 
	}
}
