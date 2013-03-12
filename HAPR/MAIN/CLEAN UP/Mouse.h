#include "KeyboardHost.h"

void WaitForFirstPoll() {
	noFirstCB = true;
	go = false;
	while(noFirstCB) {
		Move(0.3f);
		Delay(50);
		Stop();
		Move(-0.3f);
		Delay(50);
		Stop();
	}
	Play(ALARM);
}

//Method: MousePins()
//Author: Tom Collier
//Use: This method sets up 2 pins which were shown (in practicals)
//     to aid the use of the mouse. 
void MousePins() {
	PINSEL_CFG_Type PinCfg;
	PinCfg.Funcnum = 1;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Portnum = 1;
	PinCfg.Pinnum = 18;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 9;
	PinCfg.Portnum = 2;
	PINSEL_ConfigPin(&PinCfg);
}

//Method: Cb(uint8_t buttons, int8_t X, int8_t Y)
//Author: Tom Collier
//Use: This is the method called to retrieve and process information given by the mouse.
//     the method is applied differently if the robot is currently spinning or moving in a direction.
//     when the mouse is moving in a direction this method also attempts to keep movements straight by 
//     controlling the right motor and increasing it's speed or decreasing it based on the change in the angle of
//     the robot.
void Cb(uint8_t buttons, int8_t X, int8_t Y) {
if(noFirstCB) noFirstCB = false;

	float adjustedX, adjustedY;

	adjustedX = ((float) X) / CONVERT; //converting the "dots" values to cm
	adjustedY = ((float) Y) / CONVERT;

	float newX, newY;
	float newTHETA;

	if(moving)	{ //decision of whether the robot is spinning or moving straight
		newX = currentX + (((float) adjustedX) * cosf(currentTHETA));
		newY = currentY + (((float) adjustedX) * sinf(currentTHETA));
		newTHETA = currentTHETA + ((float)( adjustedY / (float) LENGTH));

		if(newTHETA > currentTHETA) { //straightening path during movement based on increasing or decresing theta values
			rightS--;
			MotorSet((leftS / 127.0f), (rightS / 127.0f));
		}
		else if(moving && newTHETA < currentTHETA) {
			rightS++;
			MotorSet((leftS / 127.0f), (rightS / 127.0f));
		}

		currentX = newX;
		currentY = newY;

	}
	else {

		newTHETA = currentTHETA + ((float)( adjustedY / (float) LENGTH));

		currentTHETA = newTHETA;
		ConsoleWrite("\n\rCallback");

	}
}

//Method: Attach()
//Author: Tom Collier
//Use: The method called when the mouse is attached, this is used (in our case)
//     only for an indication of whether the mouse is attached.
void Attach() {
	ConsoleWrite("\n\rAttached");
}

//Method: Detached()
//Author: Tom Collier
//Use: The method called when the mouse is detached, this is used (in our case)
//     only for an indication of whether the mouse is attached.
void Detach() {
	ConsoleWrite("\n\rDetached");
}

//Method: TurnAngle(angle)
//Author: Tom Collier
//Use: This function is designed to turn the robot by a set amount of radians (clockwise = positive).
void TurnAngle(float angle) {
	angle = -(angle); //clockwise is actually negative as measured by the mouse, simple fix.

	float finalTHETA = currentTHETA + angle; //value of theta after the turn is completed

	if(angle > 0.0f) { //spin anticlockwise
		moving = 0;
		Spin(-0.17f);
		while(currentTHETA < finalTHETA);
		Stop();

	}
	else if(angle < 0.0f) { //spin clockwise
		moving = 0;
		Spin(0.17f);
		while(currentTHETA > finalTHETA);
		Stop();
	}

}

//Method: MoveDistance(float distance, int xy)
//Author: Tom Collier
//Use: This function is designed to move the robot a certain distance in either a positive or negative 
//     amount in either of the X or Y axis.
//     This is achieved by first zeroing the angle of the robot, and turning either 90 degrees (in either direction) or 180.
void MoveDistance(float distance, int xy) {

	if(!xy) {// 1 = move in the y direction, 0 in the x
		float diff = currentTHETA;
		if((diff > 6.28f) || (diff < -6.28f)) { //determining how far to turn to zero the angle
			diff = fmodf(diff,6.28f); 
		}

		if(distance > 0.0f) {

			TurnAngle(diff); //positive x -> travel at theta = 0

		} 
		else {

			TurnAngle(diff); //negative x -> travel at theta = 3.14
			Delay(1000);
			TurnAngle(3.14f);

		}

		float finalX = currentX + distance;
		float distance10 = distance * 0.1f;
		float prelimX = finalX - distance10;

		moving = 1; //enable straightness correction

		if(distance > 0.0f) {
			Move(0.2f);

			while(currentX < prelimX);
			Move(0.15f);

			while(currentX < finalX);
			Stop();

		}
		else if(distance < 0.0f) {

			Move(0.2f);
			while(currentX > prelimX);
			Move(0.15f);
			while(currentX > finalX);
			Stop();
		}

	} 
	else {

		float diff = currentTHETA;
		if((diff > 6.28f) || (diff < -6.28f)) {

			diff = fmodf(diff,6.28f);

		}

		if(distance > 0.0f) {

			TurnAngle(diff); //positive y -> travel at theta = 1.57f
			Delay(1000);
			TurnAngle(-1.57f);

		} 
		else {

			TurnAngle(diff); //negative y -> travel at theta = -1.57f
			Delay(1000);
			TurnAngle(1.57f);

		}

		float finalY = currentY + distance; //robot slows down when 10% of the movement remain for added preciseness
		float distance10 = distance * 0.1f;
		float prelimY = finalY - distance10;

		moving = 1; //enable straightness correction
		if(distance > 0.0f) {

			Move(0.2f);

			while(currentY < prelimY);
			Move(0.15f);

			while(currentY < finalY);
			Stop();


		}
		else if(distance < 0.0f) {
			Move(0.2f);
			while(currentY > prelimY);
			Move(0.15f);
			while(currentY > finalY);
			Stop();

		}

	}

	moving = 0;
}