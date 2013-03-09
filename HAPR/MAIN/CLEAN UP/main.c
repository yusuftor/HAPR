#include "Definitions.h"
#include "Headers.h"

int main() {
	//Demo program
	Init();

	//Short course:
	//FW 2 meters (using mouse)
	//Follow wall for 2 meters (wall following algorithm)
	//FW until line reached
	//Follow line until dock detected (line following algorithm)
	if(0) {
		MoveDistance(200.0f);
		FollowWall(30.0f, LEFT);
		StartLineFollowing();
	}

	//Long course:
	//FW 2 meters (using mouse)
	//Follow wall for 2 meters (wall following algorithm)
	//Turn 90 degrees right (using mouse)
	//FW 2 meters (using mouse)
	//Turn 90 degrees left (using mouse)
	//FW 2 meters (using mouse)
	//Follow wall for 2 meters (wall following algorithm)
	//FW until line reached
	//Follow line until dock detected (line following algorithm)
	if(0) {
		MoveDistance(200.0f);
		FollowWall(30.0f, LEFT);
		TurnAngle(90.0f);
		MoveDistance(120.0f);
		TurnAngle(-90.0f);
		MoveDistance(200.0f);
		FollowWall(28.0f, RIGHT);
		StartLineFollowing();
	}

	//After run the Pololu will halt here until it is reset.
	Stop();
	while(1);
}