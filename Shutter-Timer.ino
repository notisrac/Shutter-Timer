// this is the time in microseconds that the shutter opens (the arduino runs a microsecond clock in the background always - it is reasonably accurate for this purpose)
long startTime;
// this is the time in microseconds that the shutter closes
long stopTime;
// this is a flag indicating when the shutter has been fired completely.  when fired =1, the shutter has been fired, and the computer needs to display the information related to the exposure time.
bool fired = false;
// the current status of the code - true: running, false: waiting
bool currentStatus = false;
// stores the last status - this is needed to have the code display the accompanying message only once
bool lastStatus = false;
// the current value read from the digital port
int currentValue = 0;
// same as above, to eliminate double measurements
int lastValue = 1;

//This part of the program is run exactly once on boot
void setup() {
  //opens a serial connection.
  Serial.begin(9600);
  Serial.println(F("Shutter speed test"));
  //run the function CLOCK, every time the voltage on pin 2 changes.
  attachInterrupt(digitalPinToInterrupt(2), CLOCK, CHANGE);
  lastValue = digitalRead(2);
}

// this part of the program is run, in order, over and over again, start to finish, unless INTERRUPTED by our interrupt
void loop() {
  if(currentStatus != lastStatus) {
    if(currentStatus){
      Serial.println("measuring...");
    }
    else {
      Serial.println("waiting...");
      Serial.println("");
    }
    lastStatus = currentStatus;
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
}

//this is the interrupt function, which is called everytime the voltage on pin 2 changes, no matter where in the main program loop that the computer is currently in
void CLOCK(){
  currentValue = digitalRead(2);

  if(currentValue != lastValue && !fired){
    if(currentValue == LOW){
      // start the measurement
      currentStatus = true;
      startTime = micros();
    }
    else {
      //stop when it goes back again
      currentStatus = false;
      stopTime = micros();
      fired = true;
    }
    lastValue = currentValue;
  }
}
