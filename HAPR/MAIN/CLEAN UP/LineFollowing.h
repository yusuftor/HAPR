//Method: StartLineFollowing()
//Author: Yusuf Tor
//Use: This function is used to transistion into line 
//     following by moving forwards until the line is 
//     detected,calibrating and then following it until
//     the dock has been found.
void StartLineFollowing() {
    dockNotFound = true;
    FindLine();
    Calibrate();
    WriteByte((char) 0xBB); // Start PID
    WriteByte((char)((int)127.0f*0.4f)); // Max motor speed
    char parameters[4] = {0x02, 0x01, 0x03, 0x02};
    Write(parameters, 4); // Parameters to adjust motor speed differences
    while (dockNotFound);
}

//Method: FindLine()
//Author: Yusuf Tor
//Use: This will move fowards until a line is detected
//     under the center of the Pololu. If a line is found
//     to either side of the front it will move on to the
//     line more until it is centered.
void FindLine() {
    while(1) {
        WriteByte((char) 0x86); // Read calibrated sensor values
        Read();
        Delay(10);
        if (sensor3 > 1600) {//Line under sensor 3, move forwards a bit then calibrate
            moving = 1;
            Move(0.2f);
            Delay(100);
            break;
        }
        else if (sensor1 > 1600) { //Line under sensor 1 
            moving = 0;
            Spin(0.2f);
            Delay(50);
            moving = 1;
            Move(0.2f);
            Delay(50);
            continue;
        }
        else if (sensor5 > 1600) {//Line under sensor 
            moving = 0;
            Spin(-0.2f);
            Delay(50);
            moving = 1;
            Move(0.2f);
            Delay(50);
            continue;
        }
        else { // Line not under any sensor, just keep moving forwards
            moving = 1;
            Move(0.2f);
            Delay(100);
            Stop();
        }
    }
    moving = 0;
}

//Method: Calibrate()
//Author: Yusuf Tor
//Use: This function activates the 3pi calibrate function
//     and then in order to make the calibrate command work
//     it spins clockwise an angle, then anti-clockwise for
//     double that angle and then clockwise the amount of
//     the original angle so that it finishes in the same
//     position as it started.
void Calibrate() {
    Spin(0.25f); //Spin clockwise
    int i;
    for (i = 0; i < 35; i++) {
        WriteByte((char) 0xB4);
        Delay(10);
    }
    Stop();
    Spin(-0.25f); //Spin anti-clockwise
    for (i = 0; i < 70; i++) {
        WriteByte((char) 0xB4);
        Delay(10);
    }
    Stop();
    Spin(0.25f); //Spin clockwise
    for (i = 0; i < 35; i++) {
        WriteByte((char) 0xB4);
        Delay(10);
    }
    Stop();
}