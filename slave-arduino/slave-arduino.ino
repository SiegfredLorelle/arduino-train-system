#include <Wire.h>
#include <Servo.h> 

const int STATIONS_SIZE = 6;


Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;
Servo servo6;
Servo SERVOS[STATIONS_SIZE] {
  servo1,
  servo2,
  servo3,
  servo4,
  servo5,
  servo6,
};

int servosPins[STATIONS_SIZE] = {
  3,
  5,
  6,
  9,
  10,
  11,
};

int currentServoPos = 45; // Initial servo position (adjust as needed)
bool servoRaised = false; // Flag to track servo state (raised or lowered)

void setup()
{
  Wire.begin(4);                // join i2c bus with address #4
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);           // start serial for output

  initServos();
}

void loop()
{
  delay(100);
  testServos();
}

void initServos() {
  for (int i; i < STATIONS_SIZE; i++) {
    SERVOS[i].attach(servosPins[i]);    
    SERVOS[i].write(45);
  }
}


// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
  while(1 < Wire.available()) // loop through all but the last
  {
    char c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
  }
  int x = Wire.read();    // receive byte as an integer
  Serial.println(x);         // print the integer
}


void raiseServo(Servo servo) {
  // Check if servo is already raised and avoid unnecessary movement
  if (servoRaised) {
    return;
  }

  for (int pos = currentServoPos; pos <= 150; pos += 1) {
    servo.write(pos);
    delay(15);
  }
  currentServoPos = 150; // Update current position
  servoRaised = true; // Set raised flag
}



void lowerServo(Servo servo) {
  // Check if servo is already lowered and avoid unnecessary movement
  if (!servoRaised) {
    return;
  }

  for (int pos = currentServoPos; pos >= 45; pos -= 1) {
    servo.write(pos);
    delay(15);
  }
  currentServoPos = 45; // Update current position
  servoRaised = false; // Set lowered flag
}

void testServos() {
  for (int i = 0; i < 6; i++) {
    Serial.print("RAISING SERVO ");
    Serial.println(i);
    raiseServo(SERVOS[i]);
    delay(100);
    Serial.print("LOWERING SERVO ");
    Serial.println(i);
    lowerServo(SERVOS[i]);
  }
}