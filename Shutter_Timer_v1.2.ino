long startTime;   // this is the time in microseconds that the shutter opens (the arduino runs a microsecond clock in the background always - it is reasonably accurate for this purpose)
long stopTime;    // this is the time in microseconds that the shutter closes
bool fired = false;  // this is a flag indicating when the shutter has been fired completely.  when fired =1, the shutter has been fired, and the computer needs to display the information related to the exposure time.
bool runningTest = false;
int currentValue = 0;
int lastValue = 1;


void setup() {                                                  //This part of the program is run exactly once on boot

  Serial.begin(9600);                                          //opens a serial connection.
  Serial.println(F("Shutter speed test"));
  attachInterrupt(digitalPinToInterrupt(2), CLOCK, CHANGE);    //run the function CLOCK, every time the voltage on pin 2 changes.
  lastValue = digitalRead(2);
}

void loop() {                                                  // this part of the program is run, in order, over and over again, start to finish, unless INTERRUPTED by our interrupt
  if(runningTest){
    //Serial.println("measuring...");
  }
  if(fired){
    Serial.print("Start: ");
    Serial.println(startTime);
    Serial.print("Stop: ");
    Serial.println(stopTime);
    long elapsedTime = (stopTime - startTime);
    float elapsedSeconds = (float)elapsedTime/1000000;
    Serial.print("Microseconds: ");
    Serial.print(elapsedTime);
    Serial.print(" (");
    Serial.print(elapsedSeconds);
    Serial.println(" s)");
    float shutterSpeed = 1/elapsedSeconds;
    Serial.print("shutter speed: 1/");
    Serial.println(shutterSpeed);
    Serial.println();
    fired = false;
  } 
  //Serial.println(digitalRead(2));
}

void CLOCK(){                     //this is the interrupt function, which is called everytime the voltage on pin 2 changes, no matter where in the main program loop that the computer is currently in
  currentValue = digitalRead(2);

  if(currentValue != lastValue && !fired){
    if(currentValue == LOW){
      // start the measurement
      runningTest = true;
      startTime = micros();
    }
    else {
      //stop when it goes back again
      runningTest = false;
      stopTime = micros();
      fired = true;
    }
    lastValue = currentValue;
  }
}
