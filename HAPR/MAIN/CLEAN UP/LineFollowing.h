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

void FindLine() {
    while(1) {
        WriteByte((char) 0x86); // Read calibrated sensor values
        Read();
        Delay(10);
        if (sensor3 > 1600) {//Line under sensor 3, move forwards a bit then calibrate
            Move(0.2f);
            Delay(100);
            break;
        }
        else if (sensor1 > 1600) { //Line under sensor 1 
            Spin(0.2f);
            Delay(50);
            Move(0.2f);
            Delay(50);
            continue;
        }
        else if (sensor5 > 1600) {//Line under sensor 5
            Spin(-0.2f);
            Delay(50);
            Move(0.2f);
            Delay(50);
            continue;
        }
        else { // Line not under any sensor, just keep moving forwards
            Move(0.2f);
            Delay(100);
            Stop();
        }
    }
}

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