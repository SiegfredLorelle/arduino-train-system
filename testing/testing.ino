#include <LiquidCrystal_I2C.h> 
#include <Servo.h> 

Servo myServo;

const int STATIONS_SIZE = 6;
const int TRAINS_SIZE = 3;

LiquidCrystal_I2C lcdStation1(0x27, 16, 2);
LiquidCrystal_I2C lcdStation2(0x26, 16, 2);
LiquidCrystal_I2C lcdStation3(0x25, 16, 2);
LiquidCrystal_I2C lcdStation4(0x24, 16, 2);
LiquidCrystal_I2C lcdStation5(0x23, 16, 2);
LiquidCrystal_I2C lcdStation6(0x22, 16, 2);
LiquidCrystal_I2C LCDS[STATIONS_SIZE] = {
  lcdStation1,
  lcdStation2,
  lcdStation3,
  lcdStation4,
  lcdStation5,
  lcdStation6,
};

int IR1 = 8; 
int IR2 = 9; 
int IR3 = 10; 
int IR4 = 11; 
int IR5 = 12; 
int IR6 = 13; 
const int IRS[STATIONS_SIZE] = {
  IR1,
  IR2,
  IR3,
  IR4,
  IR5,
  IR6,
};

int motor1 = 5;
int motor2 = 6;
int motor3 = 7;
const int MOTORS[TRAINS_SIZE] = {
  motor1,
  motor2,
  motor3,
};
int currentTrain = motor1;

int areStationsOccupied[STATIONS_SIZE] = {
  false,
  false,
  false,
  false,
  false,
  false,
};

int currentIrSignals[STATIONS_SIZE] = {
  false,
  false,
  false,
  false,
  false,
  false,
};

int currentServoPos = 90; // Initial servo position (adjust as needed)
bool servoRaised = false; // Flag to track servo state (raised or lowered)

void setup()
{
  Serial.begin(9600);
  initLcds();
  // initIrs();
  initMotors();

  // findOccupiedStations();
  myServo.attach(3);    // Attach the servo to pin 9
  myServo.write(currentServoPos); // Set initial position

}


void loop()
{
  // readIrSignals();
  // testMotors();
  // findOccupiedStations(); // Prolly sa setup dapat to
  // delay(00);
  // runTrain();
  int currSignal = digitalRead(13);
  if (currSignal == 0) {
    Serial.println("RAISING");
    raiseServo();
  }
  else {
    Serial.println("LOWERING");
    lowerServo();
  }

  delay(100);
}

void initLcds() {
  for (int i = 0; i < STATIONS_SIZE; i++) {
    LCDS[i].init();
    LCDS[i].backlight();
    LCDS[i].print("Station ");
    LCDS[i].print(i + 1);
  }
}

void initIrs() {
  for (int i = 0; i < STATIONS_SIZE; i++) {
    pinMode(IRS[i], INPUT);
  }
}

void initMotors() {
  for (int i = 0; i < TRAINS_SIZE; i++) {
    pinMode(MOTORS[i], OUTPUT);
  }
}

void readIrSignals() {
  for (int i = 0; i < STATIONS_SIZE; i++) {
    int currSignal = digitalRead(IRS[i]);
    currentIrSignals[i] = currSignal;
  }
    printArray("IR SIGNALS: ", currentIrSignals, STATIONS_SIZE);
  
}

void testMotors() {
  for (int i = 0; i < TRAINS_SIZE; i++) {
    digitalWrite(MOTORS[i], HIGH);
    delay(5000);
    digitalWrite(MOTORS[i], LOW);
    delay(1000);
  }
}

void findOccupiedStations() {
  for (int i = 0; i < STATIONS_SIZE; i++) {
    int currentSignal = digitalRead(IRS[i]);
    areStationsOccupied[i] = currentSignal;
    currentIrSignals[i] = currentSignal;
  }
  printArray("STATIONS: ", areStationsOccupied, STATIONS_SIZE);
}

void printArray(char* message, int arr[], int size) {
  Serial.print(message);
  for (int i = 0; i < size; i++) {
    Serial.print(arr[i]);
  }
  Serial.println();
}

void runTrain() {
  digitalWrite(currentTrain, HIGH);
  while (true) {
    readIrSignals();
    // printArray("STATIONS: ", areStationsOccupied, STATIONS_SIZE);
    // printArray("IR SIGNALS: ", currentIrSignals, STATIONS_SIZE);
    // STOP MOTOR WHEN DIFFERENT IR SIGNALS DIFFERENT FROM STATIONS

  }
  // UPDATE LCD ON THE SIGNAL THAT CHANGES

  // UPDATE CURRENT TRAIN

  // REPEAT
}



void raiseServo() {
  // Check if servo is already raised and avoid unnecessary movement
  if (servoRaised) {
    return;
  }

  for (int pos = currentServoPos; pos <= 150; pos += 1) {
    myServo.write(pos);
    delay(15);
  }
  currentServoPos = 150; // Update current position
  servoRaised = true; // Set raised flag
}



void lowerServo() {
  // Check if servo is already lowered and avoid unnecessary movement
  if (!servoRaised) {
    return;
  }

  for (int pos = currentServoPos; pos >= 45; pos -= 1) {
    myServo.write(pos);
    delay(15);
  }
  currentServoPos = 45; // Update current position
  servoRaised = false; // Set lowered flag
}