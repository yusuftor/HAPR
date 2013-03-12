#include "Definitions.h"

int main() {
	//Main demonstration execution order
	Init();

	//Short course:
	// - FW 2 meters (using mouse)
	// - Follow wall for 2 meters (wall following algorithm)
	// - FW until line reached
	// - Follow line until dock detected (line following algorithm)
	if(1) {

		currentState = TRAVEL;
		Play(BEEP);
		Init_RIT(INTERVAL);
		WaitForFirstPoll();
		while(!go);
		MoveDistance(200.0f,0);
		RIT_Cmd(LPC_RIT, DISABLE);

		currentState = WALLF;
		Play(BEEP);
		FollowWall(30.0f, LEFT);

		currentState = LINEF;
		Play(BEEP);
		StartLineFollowing();
	}

	//Long course:
	// - FW 2 meters (using mouse)
	// - Follow wall for 2 meters (wall following algorithm)
	// - Turn 90 degrees right (using mouse)
	// - FW 2 meters (using mouse)
	// - Turn 90 degrees left (using mouse)
	// - FW 2 meters (using mouse)
	// - Follow wall for 2 meters (wall following algorithm)
	// - FW until line reached
	// - Follow line until dock detected (line following algorithm)
	if(0) {
		currentState = TRAVEL;
		Play(BEEP);
		Init_RIT(INTERVAL);
		WaitForFirstPoll();
		while(!go);
		MoveDistance(200.0f,0);
		RIT_Cmd(LPC_RIT, DISABLE);

		currentState = WALLF;
		Play(BEEP);
		FollowWall(30.0f, LEFT);

		currentState = TRAVEL;
		Play(BEEP);
		Init_RIT(INTERVAL);
		TurnAngle(90.0f);
		RoughMoveDistance(120.0f);
		TurnAngle(-90.0f);
		RoughMoveDistance(200.0f);
		RIT_Cmd(LPC_RIT, DISABLE);

		currentState = WALLF;
		Play(BEEP);
		FollowWall(28.0f, RIGHT);

		currentState = LINEF;
		Play(BEEP);
		StartLineFollowing();
	}

	if(0) {
		Calibrate();
		dockNotFound = true;
		WriteByte((char) 0xBB); // Start PID
		WriteByte((char)((int)127.0f*0.4f)); // Max motor speed
		char parameters[4] = {0x02, 0x01, 0x03, 0x02};
		Write(parameters, 4); // Parameters to adjust motor speed differences
		while (dockNotFound);
	}

	//After the run the Pololu will halt here until it is reset.
	Stop();
	while(1);
}