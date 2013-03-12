//Method: GetVoltages(Side s)
//Author: Ross Court
//Use: This function updates a global variable used by wall following with the values
//     of a specified side of the Pololu. This function can also be set to use an 
//     average of a given amount of checks to do.
void GetVoltages(Side s) {
	int i; 
	float checks = 1.0f;
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

//Method: FollowWall(float dist, Side s)
//Author: Ross Court
//Use: This function follows a wall on an input side at an input distance (in cm).
//     The algorithm will exit when the wall is no longer sensed by the front 
//     analogue sensor.
void FollowWall(float dist, Side s) {
	//First the distance is changed to be from the center of the Pololu
	dist -= 7.0f;
	//Then converted to the voltage the analogue sensors should aim for
	int wantedV = (int) (1000.0f * pow((dist / 37.8f), (1.0f/-1.15f)));
	bool changedValues;
	currentlyFollowing = s;
	GetVoltages(s);
	//Each loop the algorithm checks four things: to close to, to far away from, angled towards wall, angled away from wall
	//There are breakout points at each movement command incase we get the front sensor interrupt during the body of the loop
	while(result[0] > 1000) {
		if(!frontInterruptUp) {
				changedValues = true;
				GetVoltages(s);
				if(result[0] <= 1000) continue;
				Move(0.5f);

				if((result[0] > (wantedV + 200)) && (result[1] > (wantedV + 200))) {
					//If to close to (left, turn right)/(right, turn left) and go forwards a bit
					if(frontInterruptUp) continue;
					if(s == LEFT) Pivot(RIGHT, 0.6f);
					else Pivot(LEFT, 0.6f);
					Delay(10);
					if(frontInterruptUp) continue;
					Move(0.5f);
					Delay(15);
					changedValues = true;
				}

				if(changedValues) {
					GetVoltages(s);
					if(result[0] <= 1000) continue;
					changedValues = false;
				}
				if((result[0] < (wantedV - 200)) && (result[1] < (wantedV - 200))) {
					//If to far from (left, turn left)/(right, turn right) and go forwards a bit
					if(frontInterruptUp) continue;
					if(s == LEFT) Pivot(LEFT, 0.6f);
					else Pivot(RIGHT, 0.6f);
					Delay(10);
					if(frontInterruptUp) continue;
					Move(0.5f);
					Delay(15);
					changedValues = true;
				}


				if(changedValues) {
					GetVoltages(s);
					if(result[0] <= 1000) continue;
					changedValues = false;
				}
				//check if we arent parallel
				int diff = result[0] - result[1];
				if(diff > 100) {
					//Turn away from wall, if left wall turn right, if right wall turn left
					if(frontInterruptUp) continue;
					if(s == LEFT) Pivot(RIGHT, 0.6f);
					else Pivot(LEFT, 0.6f);
					Delay(10);
					changedValues = true;
				}

				if(changedValues) {
					GetVoltages(s);
					if(result[0] <= 1000) continue;
					changedValues = false;
					diff = result[0] - result[1];
				}
				if(diff < -100) {
					//Turn towards wall, if left wall turn left, if right wall turn right
					if(frontInterruptUp) continue;
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
					//turn 90 degrees as many times as required until front of robot is clear
					if(currentlyFollowing == LEFT) RoughSpinAngle(0.25f);
					else RoughSpinAngle(-0.25f);
				}
			}

			ConsoleWrite("\r\nInterrupt area exited.");
			frontInterruptUp = false;
			RIT_Cmd(LPC_RIT, DISABLE);
		}
	}
	ConsoleWrite("\r\nWall following ended.");
}

//Method: FindWall()
//Author: Ross Court
//Use: This is a general function that could be used to search fo a wall and start
//     following it when found. It will exit when a wall has been found, followed
//     and the wall following algorithm finishes
void FindWall() {
	Move(0.3f);
	bool foundWall = false;
	while(!foundWall) {
		GetVoltages(RIGHT);
		if((result[0] > 1000) || (result[1] > 1000)) {
			FollowWall(20.0f, RIGHT);
			foundWall = true;
		}
		else {
			GetVoltages(LEFT);
			if((result[0] > 1000) || (result[1] > 1000)) {
				FollowWall(20.0f, LEFT);
				foundWall = true;
			}
		} 
	}
}
