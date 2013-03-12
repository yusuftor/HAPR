
void GetVoltages(Side s) {
	int i; 
	float checks = 10.0f;
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
	GetVoltages(s);
	ConsoleWrite("/r/nCalculated V: ");
	ConsoleWriteInt(wantedV);
	while(result[0] > 800) {
		if(!frontInterruptUp) {
				changedValues = true;
				GetVoltages(s);
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
					changedValues = false;
				}
				ConsoleWrite("\r\nFound3 V: ");
				ConsoleWriteInt(result[0]);
				ConsoleWrite(" - ");
				ConsoleWriteInt(result[1]);
				//check if we arent parallel
				int diff = result[0] - result[1];
				if(diff > 100) {
					ConsoleWrite("\r\nC");
					//Turn away from wall, if left wall turn right, if right wall turn left
					if(s == LEFT) Pivot(RIGHT, 0.6f);
					else Pivot(LEFT, 0.6f);
					Delay(10);
					changedValues = true;
				}
				if(frontInterruptUp) continue;
				if(changedValues) {
					GetVoltages(s);
					changedValues = false;
					diff = result[0] - result[1];
				}
				ConsoleWrite("\r\nFound4 V: ");
				ConsoleWriteInt(result[0]);
				ConsoleWrite(" - ");
				ConsoleWriteInt(result[1]);
				if(diff < -100) {
					ConsoleWrite("\r\nD");
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
					//turn 90 degrees as many times as required until front of robot is clear
					if(currentlyFollowing == LEFT) SpinAngle(0.25f);
					else SpinAngle(-0.25f);
				}
			}

			ConsoleWrite("\r\nInterrupt area exited.");
			frontInterruptUp = false;
			RIT_Cmd(LPC_RIT, DISABLE);
		}
	}
	ConsoleWrite("\r\nWall following ended.");
}

void FindWall() {
	Move(0.2f);
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
